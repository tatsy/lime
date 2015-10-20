/******************************************************************************
Copyright 2015 Tatsuya Yatagawa (tatsy)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

#ifndef SRC_NPR_POISSONDISK_DETAIL_H_
#define SRC_NPR_POISSONDISK_DETAIL_H_

#include <vector>
#include <algorithm>

#include <opencv2/opencv.hpp>

#include "../core/common.hpp"
#include "../core/grid.hpp"
#include "../core/random_queue.h"

namespace lime {

namespace npr {

namespace {  // NOLINT

cv::Point2f coord2Grid(const cv::Point2f& v, double cellSize) {
    cv::Point2f ret;
    ret.x = static_cast<float>(v.x / cellSize);
    ret.y = static_cast<float>(v.y / cellSize);
    return ret;
}

cv::Point2f generateRandomPointAround(const cv::Point2f& v, double min_dist) {
    double radius = (1.0 + genrand_real1()) * min_dist;
    double angle = 2.0 * PI * genrand_real1();
    cv::Point2f ret;
    ret.x = v.x + static_cast<int>(radius * cos(angle));
    ret.y = v.y + static_cast<int>(radius * sin(angle));
    return ret;
}

bool inNeighborhoodForList(const Grid<cv::Point2f>& grid, cv::Point2f point, double min_dist, double cellSize) {
    cv::Point2f gridCoord = coord2Grid(point, cellSize);
    int gridW = grid.cols();
    int gridH = grid.rows();
    for (int dy = -2; dy <= 2; dy++) {
        for (int dx = -2; dx <= 2; dx++) {
            int xx = static_cast<int>(gridCoord.x + dx);
            int yy = static_cast<int>(gridCoord.y + dy);
            if (grid.hasCell(yy, xx)) {
                const std::vector<cv::Point2f>& v = grid(yy, xx);
                if (!v.empty()) {
                    for (int i = 0; i < v.size(); i++) {
                        double deltaX = point.x - v[i].x;
                        double deltaY = point.y - v[i].y;
                        double dist = sqrt(deltaX * deltaX + deltaY * deltaY);
                        if (dist < min_dist) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

double minDistFromIntensity(cv::Point2f p, const cv::Mat& gray, double min_radius, double max_radius) {
    const int px = static_cast<int>(p.x);
    const int py = static_cast<int>(p.y);
    return min_radius + (max_radius - min_radius) * gray.at<float>(py, px);
}

void checkInputMat(const cv::Mat& grayImage) {
    Assertion(grayImage.depth() == CV_32F, "Image depth must be CV_32F");
    Assertion(grayImage.channels() == 1, "Image must be single channel");
}

void pdsRandomQueue(std::vector<cv::Point2f>* points, const cv::Mat& grayImage, double min_radius, double max_radius) {
    checkInputMat(grayImage);

    const int width  = grayImage.cols;
    const int height = grayImage.rows;
    const int dim    = grayImage.channels();
    double cellSize  = std::max(width, height) / 15.0;

    Random rand = Random::getRNG();

    cv::Point2f gridCoord;
    int new_point_count = 8;
    int gridW = static_cast<int>(ceil(width / cellSize));
    int gridH = static_cast<int>(ceil(height / cellSize));
    Grid<cv::Point2f> grid(gridH, gridW);
    lime::random_queue<cv::Point2f> process;

    if (points->empty()) {
        cv::Point2f firstPoint = cv::Point2f(rand.randInt(width), rand.randInt(height));
        process.push(firstPoint);
        points->push_back(firstPoint);
        gridCoord = coord2Grid(firstPoint, cellSize);

        const int gx = static_cast<int>(gridCoord.x);
        const int gy = static_cast<int>(gridCoord.y);
        grid.pushAt(gy, gx, firstPoint);
    } else {
        for (int i = 0; i < points->size(); i++) {
            cv::Point2f p = points->at(i);
            double min_dist = minDistFromIntensity(p, grayImage, min_radius, max_radius);
            if (p.x >= 0 && p.y >= 0 && p.x < width && p.y < height) {
                if (!inNeighborhoodForList(grid, p, min_dist, cellSize)) {
                    process.push(p);
                    gridCoord = coord2Grid(p, cellSize);

                    const int gx = static_cast<int>(gridCoord.x);
                    const int gy = static_cast<int>(gridCoord.y);
                    grid.pushAt(gy, gx, p);
                } else {
                    points->erase(points->begin() + i);
                    i--;
                }
            }
        }
    }

    while (!process.empty()) {
        cv::Point2f p = process.pop();
        for (int i = 0; i < new_point_count; i++) {
            double min_dist = minDistFromIntensity(p, grayImage, min_radius, max_radius);
            cv::Point2f q = generateRandomPointAround(p, min_dist);
            if (q.x >= 0 && q.y >= 0 && q.x < width && q.y < height) {
                if (!inNeighborhoodForList(grid, q, min_dist, cellSize)) {
                    process.push(q);
                    points->push_back(q);
                    gridCoord = coord2Grid(q, cellSize);

                    const int gx = static_cast<int>(gridCoord.x);
                    const int gy = static_cast<int>(gridCoord.y);
                    grid.pushAt(gy, gx, q);
                }
            }
        }
    }
}  // function pdsRandomQueue

double minDistByIntensity(const cv::Point2f p, const cv::Mat& gray, double min_radius, double max_radius) {
    const int px = static_cast<int>(p.x);
    const int py = static_cast<int>(p.y);
    return min_radius + (max_radius - min_radius) * gray.at<float>(py, px);
}

bool isConflict(const cv::Mat& gray, const cv::Mat& noise, cv::Point2f p, double min_radius, double max_radius) {
    const int ksize  = static_cast<int>(max_radius * 2.0);
    const int width  = gray.cols;
    const int height = gray.rows;

    double r1 = minDistByIntensity(p, gray, min_radius, max_radius);
    for (int dy = -ksize; dy <= ksize; dy++) {
        for (int dx = -ksize; dx <= ksize; dx++) {
            int xx = static_cast<int>(p.x + dx);
            int yy = static_cast<int>(p.y + dy);
            if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                if (noise.at<float>(yy, xx) == 1.0f) {
                    double r2 = minDistByIntensity(cv::Point2f(xx, yy), gray, min_radius, max_radius);
                    double rmax = std::max(r1, r2);
                    if (dx * dx + dy * dy < rmax * rmax) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool throwSample(const cv::Mat& gray, const cv::Mat& noise, cv::Point2f* newPoint,
                 int nTrial, const cv::Rect& region, double min_radius, double max_radius) {
    const int width  = gray.cols;
    const int height = gray.rows;

    for (int t = 0; t < nTrial; t++) {
        int rx = region.x + genrand_int31() % region.width;
        int ry = region.y + genrand_int31() % region.height;
        if (rx >= 0 && ry >= 0 && rx < width && ry < height) {
            cv::Point2f p = cv::Point2f(rx, ry);
            if (!isConflict(gray, noise, p, min_radius, max_radius)) {
                *newPoint = p;
                return true;
            }
        }
    }
    return false;
}

bool containPoint(const cv::Mat& noise, const cv::Rect& region) {
    int width = noise.cols;
    int height = noise.rows;

    for (int y = 0; y < region.height; y++) {
        for (int x = 0; x < region.width; x++) {
            int xx = region.x + x;
            int yy = region.y + y;
            if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                if (noise.at<float>(yy, xx) == 1.0f) return true;
            }
        }
    }
    return false;
}

void pdsParallel(std::vector<cv::Point2f>* points, const cv::Mat& grayImage, double min_radius, double max_radius) {
    checkInputMat(grayImage);

    const int width  = grayImage.cols;
    const int height = grayImage.rows;
    const int dim    = grayImage.channels();

    // initialze noises
    cv::Mat noise = cv::Mat::zeros(height, width, CV_32FC1);
    if (!points->empty()) {
        lime::random_queue<cv::Point2f> que;
        const int np = static_cast<int>(points->size());
        for (int i = 0; i < np; i++) {
            que.push(points->at(i));
        }

        while (!que.empty()) {
            cv::Point2f p = que.pop();
            int px = static_cast<int>(p.x);
            int py = static_cast<int>(p.y);

            int rx = static_cast<int>(px - min_radius);
            int ry = static_cast<int>(py - min_radius);
            int rw = static_cast<int>(min_radius * 2.0);
            int rh = static_cast<int>(min_radius * 2.0);

            cv::Rect region(rx, ry, rw, rh);
            if (!containPoint(noise, region)) {
                noise.at<float>(py, px) = 1.0f;
            }
        }
    }

    // precompute
    int nPhaseGroup = static_cast<int>(2.0 * sqrt(2.0) + 1.0);
    int nPhaseGroup2 = nPhaseGroup * nPhaseGroup;
    std::vector<int> order(nPhaseGroup2);
    for (int i = 0; i < nPhaseGroup2; i++) {
        order[i] = i;
    }

    // determine traverse order for phase groups
    for (int i = 0; i < 100; i++) {
        int p = genrand_int31() % nPhaseGroup2;
        int q = genrand_int31() % nPhaseGroup2;
        int t = order[p];
        order[p] = order[q];
        order[q] = t;
    }

    double limit = 2.0 * sqrt(2.0) * min_radius;
    int gridW = width;
    int gridH = height;
    while (std::min(gridW, gridH) > limit) {
        int nGridX = width / gridW;
        int nGridY = height / gridH;
        int cellW = gridW / nPhaseGroup + 1;
        int cellH = gridH / nPhaseGroup + 1;

        ompfor (int gy = 0; gy < nGridY; gy++) {  // NOLINT
            for (int gx = 0; gx < nGridX; gx++) {
                for (int k = 0; k < nPhaseGroup2; k++) {
                    int i = order[k];
                    int cx = i / nPhaseGroup;
                    int cy = i % nPhaseGroup;
                    int sx = gx * gridW + cx * cellW;
                    int sy = gy * gridH + cy * cellH;
                    cv::Rect omega = cv::Rect(sx, sy, cellW, cellH);
                    if (!containPoint(noise, omega)) {
                        cv::Point2f p;
                        if (throwSample(grayImage, noise, &p, 10, omega, min_radius, max_radius)) {
                            int ipx = static_cast<int>(p.x);
                            int ipy = static_cast<int>(p.y);
                            noise.at<float>(ipy, ipx) = 1.0f;
                        }
                    }
                }
            }
        }
        gridW /= 2;
        gridH /= 2;
    }

    // count number of points
    points->clear();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (noise.at<float>(y, x) == 1.0f) {
                points->push_back(cv::Point2f(x, y));
            }
        }
    }
}  // function pdsParallel

}  // anonymous namespace

void poissonDisk(cv::InputArray grayImage, std::vector<cv::Point2f>* samplePoints,
                 PdsMethod pdsMethod, double minRadius, double maxRadius) {
    switch (pdsMethod) {
    case PDS_RAND_QUEUE:
        pdsRandomQueue(samplePoints, grayImage.getMat(), minRadius, maxRadius);
        break;

    case PDS_FAST_PARALLEL:
        pdsParallel(samplePoints, grayImage.getMat(), minRadius, maxRadius);
        break;

    default:
        Assertion(false, "Unknown method specified to Poisson disk sampling.");
    }
}

}  // namespace npr

}  // namespace lime

#endif  // SRC_NPR_POISSONDISK_DETAIL_H_

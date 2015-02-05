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

#ifndef SRC_NPR_VECTORFIELD_DETAIL_H_
#define SRC_NPR_VECTORFIELD_DETAIL_H_

#include <vector>

#include "PoissonDisk.h"

namespace lime {

namespace npr {

namespace {  // NOLINT

void calcETF(cv::InputArray input, cv::OutputArray output,
             int ksize = 5, int maxiter = 3, EdgeDetector edgeDetector = EDGE_SOBEL) {
    cv::Mat  image = input.getMat();
    cv::Mat& etf   = output.getMatRef();

    // check input arguments
    int width = image.cols;
    int height = image.rows;
    int dim = image.channels();
    static const double eps = 1.0e-8;

    cv::Mat gray;
    if (image.depth() != CV_32F) {
        image.convertTo(gray, CV_MAKETYPE(CV_32F, dim), 1.0 / 255.0);
    } else {
        image.convertTo(gray, CV_MAKETYPE(CV_32F, dim));
    }

    if (dim != 1) {
        cv::cvtColor(gray, gray, cv::COLOR_BGR2GRAY);
    }

    // detect standard edge
    cv::Mat gx, gy;
    if (edgeDetector == EDGE_SOBEL) {
        cv::Sobel(gray, gx, CV_32FC1, 1, 0);
        cv::Sobel(gray, gy, CV_32FC1, 0, 1);
    } else if (edgeDetector == EDGE_ROTATIONAL) {
        double p1 = 0.183;
        double Dx[3][3] = { { 0.5 * p1, 0.0, -0.5 * p1 },
        { 0.5 - p1, 0.0, p1 - 0.5 },
        { 0.5 * p1, 0.0, -0.5 * p1 } };

        gx = cv::Mat(height, width, CV_32FC1);
        gy = cv::Mat(height, width, CV_32FC1);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double sumx = 0.0;
                double sumy = 0.0;
                double weightx = 0.0;
                double weighty = 0.0;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        int xx = x + dx;
                        int yy = y + dy;
                        if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                            double wx = Dx[dy + 1][dx + 1];
                            double wy = Dx[dx + 1][dy + 1];
                            sumx += wx * gray.at<float>(yy, xx);
                            sumy += wy * gray.at<float>(yy, xx);
                        }
                    }
                }
                gx.at<float>(y, x) = static_cast<float>(sumx);
                gy.at<float>(y, x) = static_cast<float>(sumy);
            }
        }
    } else {
        msg_assert(false, "Unknown edge detector is specified");
    }

    // compute tangent field
    cv::Mat tangent = cv::Mat::zeros(height, width, CV_32FC2);
    cv::Mat ghat = cv::Mat::zeros(height, width, CV_32FC1);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double vx = gx.at<float>(y, x);
            double vy = gy.at<float>(y, x);
            double mag = sqrt(vx*vx + vy*vy) + eps;
            ghat.at<float>(y, x) = static_cast<float>(mag);

            tangent.at<float>(y, x * 2 + 0) = static_cast<float>(-vy / mag);
            tangent.at<float>(y, x * 2 + 1) = static_cast<float>(vx / mag);
        }
    }

    // compute ETF
    cv::Mat temp = cv::Mat(height, width, CV_32FC2);
    while (maxiter--) {
        ompfor(int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Point2d sum = Point2d(0.0, 0.0);
                double weight = 0.0;
                for (int dy = -ksize; dy <= ksize; dy++) {
                    for (int dx = -ksize; dx <= ksize; dx++) {
                        int xx = x + dx;
                        int yy = y + dy;
                        if (xx < 0 || yy < 0 || xx >= width || yy >= height) continue;
                        if (dx * dx + dy * dy > ksize * ksize) continue;

                        double vx = tangent.at<float>(y, x * 2 + 0);
                        double vy = tangent.at<float>(y, x * 2 + 1);
                        Point2d tx = Point2d(vx, vy);

                        double ux = tangent.at<float>(yy, xx * 2 + 0);
                        double uy = tangent.at<float>(yy, xx * 2 + 1);
                        Point2d ty = Point2d(ux, uy);
                        double wd = tx.dot(ty);

                        double gx = ghat.at<float>(y, x);
                        double gy = ghat.at<float>(yy, xx);
                        double wm = (gy - gx + 1.0) / 2.0;

                        sum = sum + ty * (wm * wd);
                        weight += wm * wd;
                    }
                }

                Point2d v = weight != 0.0 ? sum / weight : Point2d(0.0, 0.0);
                temp.at<float>(y, x * 2 + 0) = static_cast<float>(v.x);
                temp.at<float>(y, x * 2 + 1) = static_cast<float>(v.y);
            }
        }

        temp.convertTo(tangent, CV_32FC2);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double vx = tangent.at<float>(y, x * 2 + 0);
                double vy = tangent.at<float>(y, x * 2 + 1);
                double mag = sqrt(vx * vx + vy * vy) + eps;
                tangent.at<float>(y, x * 2 + 0) = static_cast<float>(vx / mag);
                tangent.at<float>(y, x * 2 + 1) = static_cast<float>(vy / mag);
            }
        }
    }

    // compute tangent direction with angle
    temp.convertTo(etf, CV_32F);
}

void calcSST(cv::InputArray input, cv::OutputArray output, int ksize = 5, EdgeDetector edgeDetector = EDGE_SOBEL) {
    cv::Mat  image = input.getMat();
    cv::Mat& sst   = output.getMatRef();

    int width = image.cols;
    int height = image.rows;
    int dim = image.channels();

    const int offset[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };

    cv::Mat gray;
    if (image.depth() != CV_32F) {
        image.convertTo(gray, CV_MAKETYPE(CV_32F, dim), 1.0 / 255.0);
    } else {
        image.convertTo(gray, CV_MAKETYPE(CV_32F, dim));
    }

    if (dim != 1) {
        cv::cvtColor(gray, gray, cv::COLOR_BGR2GRAY);
    }

    // detect standard edge
    cv::Mat gx, gy;
    if (edgeDetector == EDGE_SOBEL) {
        cv::Sobel(gray, gx, CV_32FC1, 1, 0);
        cv::Sobel(gray, gy, CV_32FC1, 0, 1);
    } else if (edgeDetector == EDGE_ROTATIONAL) {
        double p1 = 0.183;
        double Dx[3][3] = { { 0.5 * p1, 0.0, -0.5 * p1 },
        { 0.5 - p1, 0.0, p1 - 0.5 },
        { 0.5 * p1, 0.0, -0.5 * p1 } };

        gx = cv::Mat(height, width, CV_32FC1);
        gy = cv::Mat(height, width, CV_32FC1);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double sumx = 0.0;
                double sumy = 0.0;
                double weightx = 0.0;
                double weighty = 0.0;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        int xx = x + dx;
                        int yy = y + dy;
                        if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                            double wx = Dx[dy + 1][dx + 1];
                            double wy = Dx[dx + 1][dy + 1];
                            sumx += wx * gray.at<float>(yy, xx);
                            sumy += wy * gray.at<float>(yy, xx);
                        }
                    }
                }
                gx.at<float>(y, x) = static_cast<float>(sumx);
                gy.at<float>(y, x) = static_cast<float>(sumy);
            }
        }
    } else {
        msg_assert(false, "Unknown edge detector is specified.");
    }

    // tensor relaxation step
    cv::Mat efg = cv::Mat(height, width, CV_64FC3);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double fx = gx.at<float>(y, x);
            double fy = gy.at<float>(y, x);
            efg.at<double>(y, x * 3 + 0) = fx * fx;
            efg.at<double>(y, x * 3 + 1) = fx * fy;
            efg.at<double>(y, x * 3 + 2) = fy * fy;
        }
    }

    cv::Mat tmp;
    efg.convertTo(tmp, CV_64FC3);
    const double tau = 0.002;
    for (int it = 0; it < 5; it++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double E = efg.at<double>(y, x * 3 + 0);
                double F = efg.at<double>(y, x * 3 + 1);
                double G = efg.at<double>(y, x * 3 + 2);

                double eng = sqrt(E * E + 2.0 * F * F + G * G);
                if (eng > tau) {
                    double sumE = 0.0;
                    double sumF = 0.0;
                    double sumG = 0.0;
                    double cnt = 0.0;
                    for (int k = 0; k < 4; k++) {
                        int xx = x + offset[k][0];
                        int yy = y + offset[k][1];
                        if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                            sumE += tmp.at<double>(yy, xx * 3 + 0);
                            sumF += tmp.at<double>(yy, xx * 3 + 1);
                            sumG += tmp.at<double>(yy, xx * 3 + 2);
                            cnt += 1.0;
                        }
                    }
                    efg.at<double>(y, x * 3 + 0) = sumE / cnt;
                    efg.at<double>(y, x * 3 + 1) = sumF / cnt;
                    efg.at<double>(y, x * 3 + 2) = sumG / cnt;
                }
            }
        }
        efg.convertTo(tmp, CV_64FC3);
    }

    efg.convertTo(efg, CV_32F);
    double sigma = 2.0 * ksize * ksize;
    cv::GaussianBlur(efg, sst, cv::Size(ksize, ksize), sigma);
}

}  // unnamed namespace

void calcVectorField(cv::InputArray input, cv::OutputArray angles,
                     int ksize, VFieldType vfieldType, EdgeDetector edgeDetector) {
    cv::Mat image = input.getMat();

    const int width = image.cols;
    const int height = image.rows;

    cv::Mat& vfield = angles.getMatRef();
    vfield = cv::Mat(height, width, CV_32FC1);
    if (vfieldType == VECTOR_SST) {
        cv::Mat sst;
        npr::calcSST(image, sst, ksize, edgeDetector);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double E = sst.at<float>(y, x * 3 + 0);
                double F = sst.at<float>(y, x * 3 + 1);
                double G = sst.at<float>(y, x * 3 + 2);
                double D = sqrt((E - G) * (E - G) + 4.0 * F * F);
                double lambda1 = (E + G + D) / 2.0;
                double lambda2 = (E + G - D) / 2.0;
                double lambda = abs(lambda1) > abs(lambda2) ? lambda1 : lambda2;
                vfield.at<float>(y, x) = static_cast<float>(atan2(-F, lambda1 - E));
            }
        }
    } else if (vfieldType == VECTOR_ETF) {
        cv::Mat etf;
        npr::calcETF(image, etf, ksize, edgeDetector);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double tx = etf.at<double>(y, x * 2 + 0);
                double ty = etf.at<double>(y, x * 2 + 1);
                vfield.at<float>(y, x) = static_cast<float>(atan2(ty, tx));
            }
        }
    } else {
        msg_assert(false, "Unknown vector field type is specified.");
    }
}

void detectSingular(const Array2d<Tensor>& sst, std::vector<SingularPoint>* points,
                    std::vector<cv::Point2f>* delauneyNodes) {
    const int width = sst.cols();
    const int height = sst.rows();

    if (delauneyNodes->empty()) {
        cv::Mat white = cv::Mat::ones(height, width, CV_32FC1);
        lime::npr::poissonDisk(white, delauneyNodes, lime::npr::PDS_FAST_PARALLEL, 3.0, 5.0);
    }

    cv::Rect(0, 0, width, height);
    cv::Subdiv2D subdiv(cv::Rect(0, 0, width, height));
    for (int i = 0; i < delauneyNodes->size(); i++) {
        subdiv.insert(delauneyNodes[i]);
    }

    std::vector<cv::Vec6f> tri;
    subdiv.getTriangleList(tri);
    for (int i = 0; i < tri.size(); i++) {
        cv::Mat A = cv::Mat::ones(3, 3, CV_64FC1);
        int x1 = static_cast<int>(tri[i][0]);
        int y1 = static_cast<int>(tri[i][1]);
        if (x1 < 0 || y1 < 0 || x1 >= width || y1 >= height) continue;
        A.at<double>(0, 0) = sst(y1, x1).elem(0, 0) - sst(y1, x1).elem(1, 1);
        A.at<double>(1, 0) = sst(y1, x1).elem(0, 1);

        int x2 = static_cast<int>(tri[i][2]);
        int y2 = static_cast<int>(tri[i][3]);
        if (x2 < 0 || y2 < 0 || x2 >= width || y2 >= height) continue;
        A.at<double>(0, 1) = sst(y2, x2).elem(0, 0) - sst(y2, x2).elem(1, 1);
        A.at<double>(1, 1) = sst(y2, x2).elem(0, 1);

        int x3 = static_cast<int>(tri[i][4]);
        int y3 = static_cast<int>(tri[i][5]);
        if (x3 < 0 || y3 < 0 || x3 >= width || y3 >= height) continue;
        A.at<double>(0, 2) = sst(y3, x3).elem(0, 0) - sst(y3, x3).elem(1, 1);
        A.at<double>(1, 2) = sst(y3, x3).elem(0, 1);

        cv::Mat b = cv::Mat::zeros(3, 1, CV_64FC1);
        b.at<double>(2, 0) = 1.0;

        cv::Mat p = cv::Mat(b.size(), CV_64FC1);
        if (cv::determinant(A) != 0) {
            cv::solve(A, b, p);
            double p1 = p.at<double>(0, 0);
            double p2 = p.at<double>(1, 0);
            double p3 = p.at<double>(2, 0);
            if (p1 >= 0.0 && p2 >= 0.0 && p3 >= 0.0) {
                int px = static_cast<int>(p1 * x1 + p2 * x2 + p3 * x3);
                int py = static_cast<int>(p1 * y1 + p2 * y2 + p3 * y3);
                if (px >= 1 && py >= 1 && px < width - 1 && py < height - 1) {
                    // determine type of singularity
                    Tensor Tx = (sst(py, px + 1) - sst(py, px - 1)) * 0.5;
                    Tensor Ty = (sst(py + 1, px) - sst(py - 1, px)) * 0.5;
                    double h11 = 0.25 * (Tx.elem(0, 0) - Tx.elem(1, 1));
                    double h21 = 0.5 * Tx.elem(0, 1);
                    double h12 = 0.25 * (Ty.elem(0, 0) - Ty.elem(1, 1));
                    double h22 = 0.5 * Ty.elem(0, 1);
                    double delta = h11 * h22 - h12 * h21;
                    double idx = 1 - 0.5 * (2.0 - sign(delta));
                    if (sign(delta) > 0) {
                        points->push_back(SingularPoint(px, py, 0.0, SINGULAR_WEDGE));
                    } else if (sign(delta) < 0) {
                        points->push_back(SingularPoint(px, py, 0.0, SINGULAR_TRISECTOR));
                    } else {
                        points->push_back(SingularPoint(px, py, 0.0, SINGULAR_HIGHER_ORDER));
                    }
                }
            }
        }
    }
}

}  // namespace npr

}  // namespace lime

#endif  // SRC_NPR_VECTORFIELD_DETAIL_H_

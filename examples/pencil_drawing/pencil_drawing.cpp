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

#include <vector>
#include <algorithm>
#include <cassert>

#include "../pencil_drawing/pencil_drawing.h"
#include "../pencil_drawing/math_utils.h"

const double tau = PI / 6.0;
const int t_disc = 24;
const int s_disc = 24;
const double sigma_1 = 4.0;
const double sigma_2 = 2.0;
const double S = 7.0;
const int n = 2;
const int ksize = 11;

void showProgress(int current, int total) {
    double percent = static_cast<double>(current) / static_cast<double>(total);
    int n_prog = static_cast<int>((30.0 * percent));
    printf(" %5.1f [", 100.0 * percent);
    for (int i = 1; i <= 30; i++) {
        if (i == n_prog) {
            printf(">");
        } else {
            printf("%c", i < n_prog ? '=' : ' ');
        }
    }
    printf("]\r");

    if (current == total) printf("\n");
}

void quantizeOrientation(cv::InputOutputArray vfield_, cv::InputArray edge_) {
    cv::Mat& vfield = vfield_.getMatRef();
    cv::Mat  edge   = edge_.getMat();

    int width = vfield.cols;
    int height = vfield.rows;
    double threshold = std::max(width, height) / 50.0;

    double q_s = PI / 4.0;
    double q_t = PI;
    cv::Mat uedge;
    edge.convertTo(uedge, CV_8UC1, 255.0);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (uedge.at<uchar>(y, x) < 192) {
                uedge.at<uchar>(y, x) = 0;
            } else {
                uedge.at<uchar>(y, x) = 1;
            }
        }
    }

    cv::Mat distmap = cv::Mat(height, width, CV_32FC1);
    cv::distanceTransform(uedge, distmap, cv::DIST_L2, 3);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (distmap.at<float>(y, x) > threshold) {
                float theta = vfield.at<float>(y, x);
                vfield.at<float>(y, x) = static_cast<float>(ceil(theta / q_t) * q_t + q_s);
            }
        }
    }
}

void LIConv(cv::OutputArray lic_, cv::InputArray img_, cv::InputArray etf_, cv::InputArray noise_, double ratio) {
    cv::Mat& lic   = lic_.getMatRef();
    cv::Mat  img   = img_.getMat();
    cv::Mat  etf   = etf_.getMat();
    cv::Mat  noise = noise_.getMat();

    int width = img.cols;
    int height = img.rows;
    int dim = img.channels();
    assert(width == etf.cols && height == etf.rows && etf.channels() == 1 && etf.depth() == CV_32F);
    assert(width == noise.cols && height == noise.rows && noise.channels() == 1 && noise.depth() == CV_32F);

    cv::Mat bilateral;
    cv::bilateralFilter(img, bilateral, 19, 0.5, 15.0);

    lic = cv::Mat(height, width, img.type());
    int totalStep = dim * height * width;
    int progress = 0;

    lime::Random rand = lime::Random::getRNG();

    for (int y = 0; y < height; y++) {
        ompfor (int x = 0; x < width; x++) {  // NOLINT
            std::vector<double> s_sum(dim, 0.0);
            std::vector<double> w_sum(dim, 0.0);
            std::vector<double> sum(dim, 0.0);
            std::vector<double> weight(dim, 0.0);

            double t_i = tau / n * (rand.randInt(2*n+1) - n);
            for (int t = -t_disc; t <= t_disc; t++) {
                double theta = tau / t_disc * t +  etf.at<float>(y, x);
                fill(s_sum.begin(), s_sum.end(), 0.0);
                fill(w_sum.begin(), w_sum.end(), 0.0);
                for (int s = -s_disc; s <= s_disc; s++) {
                    double scale = S / s_disc * s;
                    int xx = static_cast<int>(x + scale * cos(theta));
                    int yy = static_cast<int>(y + scale * sin(theta));
                    if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                        for (int c = 0; c < dim; c++) {
                            double dI = bilateral.at<float>(yy, xx*dim+c) - bilateral.at<float>(y, x*dim+c);
                            double G2 = gauss(dI, sigma_2);
                            s_sum[c] += G2 * noise.at<float>(yy, xx) * (1.0 - bilateral.at<float>(y, x*dim+c));
                            w_sum[c] += G2;
                        }
                    }
                }
                double G1 = gauss(t_i - theta, sigma_1);
                for (int c = 0; c < dim; c++) {
                    sum[c] += G1 * s_sum[c];
                    weight[c] += G1 * w_sum[c];
                }
            }

            for (int c = 0; c < dim; c++) {
                lic.at<float>(y, x*dim+c) = static_cast<float>(1.0 - sum[c] / (ratio * weight[c]));
            }
        }

        // Show progress bar
        progress += width * dim;
        showProgress(progress, totalStep);
    }
}

void pencilDrawing(cv::InputArray input, cv::OutputArray output, const std::vector<cv::Point2f>& points) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    const int width  = img.cols;
    const int height = img.rows;
    const int dim    = img.channels();

    if (img.depth() == CV_8U) {
        img.convertTo(img, CV_MAKETYPE(CV_32F, dim), 1.0 / 255.0);
    } else {
        img.convertTo(img, CV_MAKETYPE(CV_32F, dim));
    }

    // * Detect edge DoF
    cv::Mat gray, edge;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    lime::npr::edgeDoG(gray, edge);

    // Compute ETF
    cv::Mat vfield;
    lime::npr::calcVectorField(gray, vfield, ksize);

    // Quantize direction using distance field
    quantizeOrientation(vfield, edge);

    // * Generate noise
    cv::Mat noise;
    double ratio = 1.0;
    if (points.empty()) {
        int nNoise = static_cast<int>(0.2 * width * height);
        noise = cv::Mat(height, width, CV_32FC1);
        lime::npr::uniformNoise(noise, gray, nNoise);
        ratio = 1.5 * nNoise / (width * height);
    } else {
        int nNoise = static_cast<int>(points.size());
        noise = cv::Mat::zeros(height, width, CV_32FC1);
        for (int i = 0; i < nNoise; i++) {
            int px = static_cast<int>(points[i].x);
            int py = static_cast<int>(points[i].y);
            cv::rectangle(noise, cv::Rect(px, py, 1, 1), cv::Scalar(1.0, 1.0, 1.0), -1);
            ratio = 1.2 * nNoise / (width * height);
        }
    }

    // * Convolution
    LIConv(out, img, vfield, noise, ratio);
}

void pencilDrawingLOD(cv::InputArray input, cv::OutputArray output, const std::vector<cv::Point2f>& points, int level) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    int width = img.cols;
    int height = img.rows;
    int dim = img.channels();

    cv::Mat I, O;
    std::vector<cv::Point2f> P;
    out = cv::Mat::zeros(height, width, CV_MAKETYPE(CV_32F, dim));
    for (int l = level; l >= 1; l--) {
        double scale = 1.0 / pow(2.0, l-1);

        cv::resize(img, I, cv::Size(), scale, scale, cv::INTER_CUBIC);

        P.clear();
        for (int i = 0; i < points.size(); i++) {
            cv::Point2f q;
            q.x = static_cast<float>(points[i].x * scale);
            q.y = static_cast<float>(points[i].y * scale);
            P.push_back(q);
        }

        // * Pencil style rendering
        pencilDrawing(I, O, P);
        cv::resize(O, O, cv::Size(width, height), 0.0, 0.0, cv::INTER_CUBIC);
        out  = out + O;
    }
    out = out / static_cast<float>(level);
}


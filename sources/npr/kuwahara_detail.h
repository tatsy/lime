#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_KUWAHARA_DETAIL_H_
#define _NPR_KUWAHARA_DETAIL_H_

#include <vector>
#include <utility>
#include <algorithm>

#include "../core/common.h"
#include "npr_edges.h"
#include "vector_field.h"

namespace lime {

namespace {

void kuwaharaClassical(cv::InputArray input, cv::OutputArray output, int ksize) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    const int width = img.cols;
    const int height = img.rows;
    const int dim = img.channels();

    out = cv::Mat(height, width, CV_MAKETYPE(CV_32F, dim));
    cv::Mat temp;
    img.convertTo(temp, CV_32FC3);
    for (int c = 0; c < dim; c++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                std::vector<double> sum(4, 0.0);
                std::vector<double> var(4, 0.0);
                std::vector<int>    cnt(4, 0);
                for (int dy = -ksize; dy <= ksize; dy++) {
                    for (int dx = -ksize; dx <= ksize; dx++) {
                        int xx = x + dx;
                        int yy = y + dy;
                        if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                            double v = temp.at<float>(yy, xx*dim + c);
                            if (dx <= 0 && dy <= 0) {
                                sum[0] += v;
                                var[0] += v * v;
                                cnt[0]++;
                            }

                            if (dx >= 0 && dy <= 0) {
                                sum[1] += v;
                                var[1] += v * v;
                                cnt[1]++;
                            }

                            if (dx <= 0 && dy >= 0) {
                                sum[2] += v;
                                var[2] += v * v;
                                cnt[2]++;
                            }

                            if (dx >= 0 && dy >= 0) {
                                sum[3] += v;
                                var[3] += v * v;
                                cnt[3]++;
                            }
                        }
                    }
                }

                std::vector<std::pair<double, int> > vec;
                for (int i = 0; i < 4; i++) {
                    sum[i] = cnt[i] != 0 ? sum[i] / cnt[i] : 0.0f;
                    var[i] = cnt[i] != 0 ? var[i] / cnt[i] : 0.0f;
                    var[i] = sqrt(var[i] - sum[i] * sum[i]);
                    vec.push_back(std::make_pair(var[i], i));
                }

                sort(vec.begin(), vec.end());
                out.at<float>(y, x*dim + c) = static_cast<float>(sum[vec[0].second]);
            }
        }
    }
    out.convertTo(temp, CV_32FC3);
}

void kuwaharaGeneralized(cv::InputArray input, cv::OutputArray output, int ksize, int n_div) {
    Assertion(n_div > 4, "Kernel size must be more than 4");

    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    const int width = img.cols;
    const int height = img.rows;
    const int dim = img.channels();
    double angle = 2.0 * PI / n_div;
    double q = 3.0;

    std::vector<std::vector<double> > gauss(ksize + 1, std::vector<double>(ksize + 1));
    for (int i = 0; i <= ksize; i++) {
        for (int j = 0; j <= ksize; j++) {
            double dist = i * i + j * j;
            gauss[i][j] = exp(-dist / (2.0 * ksize));
        }
    }

    out = cv::Mat::ones(height, width, CV_MAKETYPE(CV_32F, dim));
    for (int c = 0; c < dim; c++) {
        ompfor(int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                std::vector<double> sum(n_div, 0.0);
                std::vector<double> var(n_div, 0.0);
                std::vector<double> weight(n_div, 0.0);
                for (int dy = -ksize; dy <= ksize; dy++) {
                    for (int dx = -ksize; dx <= ksize; dx++) {
                        if (dx == 0 && dy == 0) continue;
                        int xx = x + dx;
                        int yy = y + dy;
                        int ax = std::abs(dx);
                        int ay = std::abs(dy);
                        double g = gauss[ay][ax];
                        if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                            double ddx = static_cast<double>(dx);
                            double ddy = static_cast<double>(dy);
                            double theta = atan2(ddy, ddx) + PI;
                            int t = static_cast<int>(floor(theta / angle)) % n_div;
                            if (t >= n_div) {
                                printf("%f %f %d\n", theta, angle, t);
                                exit(0);
                            }
                            double v = img.at<float>(yy, xx*dim + c);
                            sum[t] += g * v;
                            var[t] += g * v * v;
                            weight[t] += g;
                        }
                    }
                }

                double de = 0.0;
                double nu = 0.0;
                for (int i = 0; i < n_div; i++) {
                    sum[i] = weight[i] != 0 ? sum[i] / weight[i] : 0.0;
                    var[i] = weight[i] != 0 ? var[i] / weight[i] : 0.0;
                    var[i] = var[i] - sum[i] * sum[i];
                    var[i] = var[i] > EPS ? sqrt(var[i]) : EPS;
                    double w = pow(var[i], -q);
                    de += sum[i] * w;
                    nu += w;
                }
                double val = nu > 1.0e-5 ? de / nu : 0.0;
                val = val > 1.0 ? 1.0 : val;
                out.at<float>(y, x*dim + c) = static_cast<float>(val);
            }
        }
    }
}

void kuwaharaAnisotropic(cv::InputArray input, cv::OutputArray output, int ksize, int n_div) {
    Assertion(n_div > 4, "Kernel size must be more than 4");

    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    const int width = img.cols;
    const int height = img.rows;
    const int dim = img.channels();
    double angle = 2.0 * PI / n_div;
    double q = 3.0;
    double alpha = 1.0;

    cv::Mat sst;
    calcSST(img, sst);

    cv::Mat A = cv::Mat(height, width, CV_64FC1);
    cv::Mat R = cv::Mat(height, width, CV_64FC1);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double E = sst.at<float>(y, x * 3 + 0);
            double F = sst.at<float>(y, x * 3 + 1);
            double G = sst.at<float>(y, x * 3 + 2);
            double D = sqrt((E - G) * (E - G) + 4.0 * F * F);

            double lambda1 = (E + G + D) / 2.0;
            double lambda2 = (E + G - D) / 2.0;
            A.at<double>(y, x) = (lambda1 - lambda2) / (lambda1 + lambda2);
            R.at<double>(y, x) = atan2(-F, lambda1 - E);
        }
    }

    out = cv::Mat(height, width, CV_MAKETYPE(CV_32F, dim));
    cv::Mat temp;
    img.convertTo(temp, CV_32FC3);
    for (int c = 0; c < dim; c++) {
        ompfor(int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                std::vector<double> sum(n_div, 0.0f);
                std::vector<double> var(n_div, 0.0f);
                std::vector<double> weight(n_div, 0);

                double aniso = A.at<double>(y, x);
                double sx = alpha / (aniso + alpha);
                double sy = (alpha + aniso) / alpha;
                double theta = -R.at<double>(y, x);

                for (int dy = -ksize; dy <= ksize; dy++) {
                    for (int dx = -ksize; dx <= ksize; dx++) {
                        if (dx == 0 && dy == 0) continue;

                        int dx2 = static_cast<int>(sx * (cos(theta) * dx - sin(theta) * dy));
                        int dy2 = static_cast<int>(sy * (sin(theta) * dx + cos(theta) * dy));
                        int xx = x + dx2;
                        int yy = y + dy2;
                        if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                            double ddx2 = static_cast<double>(dx2);
                            double ddy2 = static_cast<double>(dy2);
                            double theta = atan2(ddy2, ddx2) + PI;
                            int t = static_cast<int>(floor(theta / angle)) % n_div;

                            double d2 = dx2 * dx2 + dy2 * dy2;
                            double g = exp(-d2 / (2.0 * ksize));
                            double v = temp.at<float>(yy, xx*dim + c);
                            sum[t] += g * v;
                            var[t] += g * v * v;
                            weight[t] += g;
                        }
                    }
                }

                double de = 0.0;
                double nu = 0.0;
                for (int i = 0; i < n_div; i++) {
                    sum[i] = weight[i] != 0 ? sum[i] / weight[i] : 0.0;
                    var[i] = weight[i] != 0 ? var[i] / weight[i] : 0.0;
                    var[i] = var[i] - sum[i] * sum[i];
                    var[i] = var[i] > EPS ? sqrt(var[i]) : EPS;
                    double w = pow(var[i], -q);
                    de += sum[i] * w;
                    nu += w;
                }
                double val = nu > EPS ? de / nu : 0.0;
                val = val > 1.0 ? 1.0 : val;
                out.at<float>(y, x*dim + c) = static_cast<float>(val);
            }
        }
    }
}

}  // unnamed namespace

void kuwaharaFilter(cv::InputArray input, cv::OutputArray output, int type,
                    int ksize, int nDivide) {
    switch (type) {
    case KUWAHARA_CLASSICAL:
        kuwaharaClassical(input, output, ksize);
        break;

    case KUWAHARA_GENERALIZED:
        kuwaharaGeneralized(input, output, ksize, nDivide);
        break;

    case KUWAHARA_ANISOTROPIC:
        kuwaharaAnisotropic(input, output, ksize, nDivide);
        break;

    default:
        ErrorMsg("Unknown Kuwahara filter type!!");
    }
}

}  // namespace lime

#endif  // _NPR_KUWAHARA_DETAIL_H_

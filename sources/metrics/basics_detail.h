#ifdef _MSC_VER
#pragma once
#endif

#ifndef _METRICS_BASICS_DETAIL_H_
#define _METRICS_BASICS_DETAIL_H_

#include <cmath>

namespace lime {

double MSE(cv::InputArray I_, cv::InputArray J_) {
    cv::Mat I = I_.getMat();
    cv::Mat J = J_.getMat();

    Assertion(I.cols == J.cols && I.rows && J.rows, "Sizes of two images must be the same!");
    Assertion(I.depth() == J.depth(), "Bit depths of two images must be the same!");
    Assertion(I.channels() == 1 && J.channels() == 1, "Two images must be grayscale!");
    Assertion((I.depth() == CV_8U && J.depth() == CV_8U) ||
              (I.depth() == CV_32F && J.depth() == CV_32F),
              "Two images must be with 8-bit unsigned integer or 32-bit floating point number");

    if (I.depth() == CV_32F) {
        I.convertTo(I, CV_8U, 255.0);
    }

    if (J.depth() == CV_32F) {
        J.convertTo(J, CV_8U, 255.0);
    }

    const int width = I.cols;
    const int height = I.rows;
    double score = 0.0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const double diff = (double)I.at<uchar>(y, x) - (double)J.at<uchar>(y, x);
            score += diff * diff;
        }
    }
    return score / (width * height);
}

double PSNR(cv::InputArray I, cv::InputArray J) {
    const int maxI = 255;
    const double mse = MSE(I, J);
    return 10.0 * std::log10(maxI * maxI / mse);
}

double RMSE(cv::InputArray I, cv::InputArray J) {
    const double mse = MSE(I, J);
    return std::sqrt(mse);
}

double SSIM(cv::InputArray I_, cv::InputArray J_) {
    cv::Mat I = I_.getMat();
    cv::Mat J = J_.getMat();

    Assertion(I.cols == J.cols && I.rows && J.rows, "Sizes of two images must be the same!");
    Assertion(I.depth() == J.depth(), "Bit depths of two images must be the same!");
    Assertion(I.channels() == 1 && J.channels() == 1, "Two images must be grayscale!");
    Assertion((I.depth() == CV_8U && J.depth() == CV_8U) ||
              (I.depth() == CV_32F && J.depth() == CV_32F),
              "Two images must be with 8-bit unsigned integer or 32-bit floating point number");

    if (I.depth() == CV_32F) {
        I.convertTo(I, CV_8U, 255.0);
    }

    if (J.depth() == CV_32F) {
        J.convertTo(J, CV_8U, 255.0);
    }

    const int width = I.cols;
    const int height = I.rows;
    const int totalSize = width * height;

    double mu_x = 0.0, mu_y = 0.0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            mu_x += (double)I.at<uchar>(y, x);
            mu_y += (double)J.at<uchar>(y, x);
        }
    }
    mu_x /= totalSize;
    mu_y /= totalSize;

    double sig_x = 0.0, sig_y = 0.0, cov_xy = 0.0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const double diff_x = (double)I.at<uchar>(x, y) - mu_x;
            const double diff_y = (double)J.at<uchar>(x, y) - mu_y;
            sig_x += diff_x * diff_x;
            sig_y += diff_y * diff_y;
            cov_xy += diff_x * diff_y;
        }
    }
    sig_x /= totalSize;
    sig_y /= totalSize;
    cov_xy /= totalSize;

    const double k1 = 0.01;
    const double k2 = 0.03;
    const double L = 255.0;
    const double c1 = (k1 * L) * (k1 * L);
    const double c2 = (k2 * L) * (k2 * L);
    const double c3 = 0.5 * c2;

    const double l = (2.0 * mu_x * mu_y + c1) / (mu_x * mu_x + mu_y * mu_y + c1);
    const double c = (2.0 * sig_x * sig_y + c2) / (sig_x * sig_x + sig_y * sig_y + c2);
    const double s = (cov_xy + c3) / (sig_x * sig_y + c3);

    return l * c * s;
}

}  // namespace lime

#endif  // _METRICS_BASICS_DETAIL_H_

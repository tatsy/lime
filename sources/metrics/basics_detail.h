#ifdef _MSC_VER
#pragma once
#endif

#ifndef _METRICS_BASICS_DETAIL_H_
#define _METRICS_BASICS_DETAIL_H_

#include <cmath>

namespace lime {

namespace {

void calsSSIM_LCS(const cv::Mat& I, const cv::Mat& J, double *l, double *c, double *s) {
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
    sig_x = std::sqrt(sig_x / totalSize);
    sig_y = std::sqrt(sig_y / totalSize);
    cov_xy /= totalSize;

    const double k1 = 0.01;
    const double k2 = 0.03;
    const double L = 255.0;
    const double c1 = (k1 * L) * (k1 * L);
    const double c2 = (k2 * L) * (k2 * L);
    const double c3 = 0.5 * c2;

    *l = (2.0 * mu_x * mu_y + c1) / (mu_x * mu_x + mu_y * mu_y + c1);
    *c = (2.0 * sig_x * sig_y + c2) / (sig_x * sig_x + sig_y * sig_y + c2);
    *s = (cov_xy + c3) / (sig_x * sig_y + c3);
}

}  // anonymous namespace

double MSE(cv::InputArray I_, cv::InputArray J_) {
    cv::Mat I = I_.getMat();
    cv::Mat J = J_.getMat();

    Assertion(I.cols == J.cols && I.rows == J.rows, "Sizes of two images must be the same!");
    Assertion(I.depth() == J.depth(), "Bit depths of two images must be the same!");
    Assertion((I.channels() == 1 && J.channels() == 1) ||
              (I.channels() == 3 && J.channels() == 3),
              "Two images must have 1 or 3 channels!");
    Assertion((I.depth() == CV_8U && J.depth() == CV_8U) ||
              (I.depth() == CV_32F && J.depth() == CV_32F),
              "Two images must be with 8-bit unsigned integer or 32-bit floating point number");

    if (I.depth() == CV_32F) {
        I.convertTo(I, CV_8U, 255.0);
    }

    if (J.depth() == CV_32F) {
        J.convertTo(J, CV_8U, 255.0);
    }

    if (I.channels() == 3) {
        cv::cvtColor(I, I, cv::COLOR_BGR2GRAY);
    }

    if (J.channels() == 3) {
        cv::cvtColor(J, J, cv::COLOR_BGR2GRAY);
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
    if (mse == 0.0) {
        return INFTY;
    }
    return 10.0 * std::log10(maxI * maxI / mse);
}

double RMSE(cv::InputArray I, cv::InputArray J) {
    const double mse = MSE(I, J);
    return std::sqrt(mse);
}

double SSIM(cv::InputArray I_, cv::InputArray J_) {
    cv::Mat I = I_.getMat();
    cv::Mat J = J_.getMat();

    Assertion(I.cols == J.cols && I.rows == J.rows, "Sizes of two images must be the same!");
    Assertion(I.depth() == J.depth(), "Bit depths of two images must be the same!");
    Assertion((I.channels() == 1 && J.channels() == 1) ||
              (I.channels() == 3 && J.channels() == 3),
              "Two images must have 1 or 3 channels!");
    Assertion((I.depth() == CV_8U && J.depth() == CV_8U) ||
              (I.depth() == CV_32F && J.depth() == CV_32F),
              "Two images must be with 8-bit unsigned integer or 32-bit floating point number");

    if (I.depth() == CV_32F) {
        I.convertTo(I, CV_8U, 255.0);
    }

    if (J.depth() == CV_32F) {
        J.convertTo(J, CV_8U, 255.0);
    }

    if (I.channels() == 3) {
        cv::cvtColor(I, I, cv::COLOR_BGR2GRAY);
    }

    if (J.channels() == 3) {
        cv::cvtColor(J, J, cv::COLOR_BGR2GRAY);
    }

    double l, c, s;
    calsSSIM_LCS(I, J, &l, &c, &s);

    return l * c * s;
}

double MSSSIM(cv::InputArray I_, cv::InputArray J_) {
    cv::Mat I = I_.getMat();
    cv::Mat J = J_.getMat();

    Assertion(I.cols == J.cols && I.rows == J.rows, "Sizes of two images must be the same!");
    Assertion(I.depth() == J.depth(), "Bit depths of two images must be the same!");
    Assertion((I.channels() == 1 && J.channels() == 1) ||
              (I.channels() == 3 && J.channels() == 3),
              "Two images must have 1 or 3 channels!");
    Assertion((I.depth() == CV_8U && J.depth() == CV_8U) ||
              (I.depth() == CV_32F && J.depth() == CV_32F),
              "Two images must be with 8-bit unsigned integer or 32-bit floating point number");

    if (I.depth() == CV_32F) {
        I.convertTo(I, CV_8U, 255.0);
    }

    if (J.depth() == CV_32F) {
        J.convertTo(J, CV_8U, 255.0);
    }

    if (I.channels() == 3) {
        cv::cvtColor(I, I, cv::COLOR_BGR2GRAY);
    }

    if (J.channels() == 3) {
        cv::cvtColor(J, J, cv::COLOR_BGR2GRAY);
    }

    const double alpha = 0.1333;
    const double beta[] = { 0.0448, 0.2856, 0.3001, 0.2363, 0.1333 };
    const double gamma[] = { 0.0448, 0.2856, 0.3001, 0.2363, 0.1333 };
    const int M = 5;

    cv::Mat subI, subJ;
    I.copyTo(subI);
    J.copyTo(subJ);

    double score = 1.0;
    for (int i = 0; i < M; i++) {
        double l, c, s;
        calsSSIM_LCS(subI, subJ, &l, &c, &s);

        score *= std::pow(c, beta[i]) * std::pow(s, gamma[i]);
        if (i == M - 1) {
            score *= std::pow(l, alpha);
            break;
        }

        cv::pyrDown(subI, subI);
        cv::pyrDown(subJ, subJ);
    }

    return score;
}

double CMSSIM(cv::InputArray I_, cv::InputArray J_) {
    cv::Mat I = I_.getMat();
    cv::Mat J = J_.getMat();

    Assertion(I.cols == J.cols && I.rows == J.rows, "Sizes of two images must be the same!");
    Assertion(I.depth() == J.depth(), "Bit depths of two images must be the same!");
    Assertion((I.channels() == 1 && J.channels() == 1) ||
              (I.channels() == 3 && J.channels() == 3),
              "Two images must have 1 or 3 channels!");
    Assertion((I.depth() == CV_8U && J.depth() == CV_8U) ||
              (I.depth() == CV_32F && J.depth() == CV_32F),
              "Two images must be with 8-bit unsigned integer or 32-bit floating point number");

    if (I.depth() == CV_32F) {
        I.convertTo(I, CV_8U, 255.0);
    }

    if (J.depth() == CV_32F) {
        J.convertTo(J, CV_8U, 255.0);
    }

    // Calculate standard MS-SSIM, first.
    const double msssim = MSSSIM(I, J);

    // If two images are both grayscale, compute standard MS-SSIM.
    if (I.channels() == 1 && J.channels() == 1) {
        return msssim;
    }

    // Convert images to CIE-Lab to evaluate color similarity.
    cv::Mat labI, labJ;
    I.convertTo(I, CV_32F, 1.0 / 255.0);
    J.convertTo(J, CV_32F, 1.0 / 255.0);
    cv::GaussianBlur(I, I, cv::Size(5, 5), 0.0);
    cv::GaussianBlur(J, J, cv::Size(5, 5), 0.0);
    cv::cvtColor(I, labI, cv::COLOR_BGR2Lab);
    cv::cvtColor(J, labJ, cv::COLOR_BGR2Lab);

    // Calculate color similarity based on Just Noticeable Color Difference (JNCD).
    const double delta = 0.7;
    const double jncd = 2.3;  // 2.3 plus/minus 1.3

    const int width = labI.cols;
    const int height = labI.rows;
    uint64_t count = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const cv::Vec3f cI = labI.at<cv::Vec3f>(y, x);
            const cv::Vec3f cJ = labJ.at<cv::Vec3f>(y, x);
            const double diff = cv::norm(cI - cJ, cv::NORM_L2);
            if (diff <= jncd * 3.0) {
                count += 1;
            }
        }
    }

    const double clr = (double)count / (double)(width * height);
    return std::pow(clr, delta) * msssim;
}

}  // namespace lime

#endif  // _METRICS_BASICS_DETAIL_H_

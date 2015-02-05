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

#ifndef SRC_HVS_COLOR_CONSTANCY_DETAIL_H_
#define SRC_HVS_COLOR_CONSTANCY_DETAIL_H_

#include <vector>
#include <algorithm>

namespace lime {

namespace {  // NOLINT

const int dx[4] = { -1, 1, 0, 0 };
const int dy[4] = { 0, 0, -1, 1 };
const float eps = 0.000001f;

void exponential(cv::InputArray input_, cv::OutputArray output_) {
    cv::Mat  input  = input_.getMat();
    cv::Mat& output = output_.getMatRef();

    const int width = input.cols;
    const int height = input.rows;
    const int channel = input.channels();

    if (input.data != output.data) {
        printf("new expo\n");
        output = cv::Mat(height, width, CV_MAKETYPE(CV_32F, channel));
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channel; c++) {
                output.at<float>(y, x*channel + c) = exp(input.at<float>(y, x*channel + c)) - eps;
            }
        }
    }
}

void logarithm(cv::InputArray input_, cv::OutputArray output_) {
    cv::Mat  input  = input_.getMat();
    cv::Mat& output = output_.getMatRef();

    const int width = input.cols;
    const int height = input.rows;
    const int channel = input.channels();

    if (input.data != output.data) {
        output = cv::Mat(height, width, CV_MAKETYPE(CV_32F, channel));
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channel; c++) {
                output.at<float>(y, x*channel + c) = log(input.at<float>(y, x*channel + c) + eps);
            }
        }
    }
}

void gauss_seidel(cv::InputOutputArray I_, cv::InputArray L_, int maxiter) {
    cv::Mat& I = I_.getMatRef();
    cv::Mat  L = L_.getMat();

    const int width = I.cols;
    const int height = I.rows;
    const int channel = I.channels();
    msg_assert(width == L.cols && height == L.rows && channel == L.channels(),
               "Input and output cv::Mat must be the same size");

    while (maxiter--) {
        for (int c = 0; c < channel; c++) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int count = 0;
                    float sum = 0.0f;
                    for (int i = 0; i < 4; i++) {
                        int xx = x + dx[i];
                        int yy = y + dy[i];
                        if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                            sum += I.at<float>(yy, xx*channel + c);
                            count += 1;
                        }
                    }
                    I.at<float>(y, x*channel + c) = (sum - L.at<float>(y, x*channel + c)) / static_cast<float>(count);
                }
            }
        }
    }
}

void laplacian(cv::InputArray input_, cv::OutputArray output_) {
    cv::Mat  input  = input_.getMat();
    cv::Mat& output = output_.getMatRef();

    const int width = input.cols;
    const int height = input.rows;
    const int channel = input.channels();

    if (input.data != output.data) {
        output = cv::Mat(height, width, CV_MAKETYPE(CV_32F, channel));
    }

    for (int c = 0; c < channel; c++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int count = 0;
                float sum = 0.0f;
                for (int i = 0; i < 4; i++) {
                    int xx = x + dx[i];
                    int yy = y + dy[i];
                    if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                        count += 1;
                        sum += input.at<float>(yy, xx*channel + c);
                    }
                }
                output.at<float>(y, x*channel + c) = sum - count * input.at<float>(y, x*channel + c);
            }
        }
    }
}

void threshold(cv::InputArray input_, cv::OutputArray output_, double threshold) {
    cv::Mat  input  = input_.getMat();
    cv::Mat& output = output_.getMatRef();

    const int width = input.cols;
    const int height = input.rows;
    const int channel = input.channels();

    if (input.data != output.data) {
        output = cv::Mat(height, width, CV_MAKETYPE(CV_32F, channel));
    }

    for (int c = 0; c < channel; c++) {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                if (fabs(input.at<float>(y, x*channel + c)) < threshold) {
                    output.at<float>(y, x*channel + c) = 0.0f;
                } else {
                    output.at<float>(y, x*channel + c) = input.at<float>(y, x*channel + c);
                }
            }
        }
    }
}

void normalizeCC(cv::InputArray input_, cv::OutputArray output_) {
    cv::Mat  input  = input_.getMat();
    cv::Mat& output = output_.getMatRef();

    const int width = input.cols;
    const int height = input.rows;
    const int channel = input.channels();

    if (input.data != output.data) {
        output = cv::Mat(height, width, CV_MAKETYPE(CV_32F, channel));
    }

    for (int c = 0; c < channel; c++) {
        float maxval = -10000.0f;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (maxval < input.at<float>(y, x*channel + c)) {
                    maxval = input.at<float>(y, x*channel + c);
                }
            }
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                output.at<float>(y, x*channel + c) = input.at<float>(y, x*channel + c) - maxval;
            }
        }
    }
}

void normalizeCC(cv::InputArray input_, cv::OutputArray output_, const float lower, const float upper) {
    cv::Mat  input  = input_.getMat();
    cv::Mat& output = output_.getMatRef();

    const int width = input.cols;
    const int height = input.rows;
    const int channel = input.channels();

    if (input.data != output.data) {
        output = cv::Mat(height, width, CV_MAKETYPE(CV_32F, channel));
    }

    float minval = 100000.0f;
    float maxval = -100000.0f;
    for (int c = 0; c < channel; c++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float value = input.at<float>(y, x*channel + c);
                minval = std::min(minval, value);
                maxval = std::max(maxval, value);
            }
        }
    }

    float ratio = (upper - lower) / (maxval - minval);
    for (int c = 0; c < channel; c++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float value = input.at<float>(y, x*channel + c);
                output.at<float>(y, x*channel + c) = (value - minval) * ratio + lower;
            }
        }
    }
}

void highEnhanceFilter(cv::InputArray input_, cv::OutputArray output_) {
    cv::Mat  input  = input_.getMat();
    cv::Mat& output = output_.getMatRef();

    const int width = input.cols;
    const int height = input.rows;
    const int channel = input.channels();

    if (input.data != output.data) {
        output = cv::Mat(height, width, CV_MAKETYPE(CV_32F, channel));
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double r = sqrt(static_cast<double>(x*x + y*y));
            double coeff = 1.5 - exp(-r / 5.0);
            for (int c = 0; c < channel; c++) {
                output.at<float>(y, x*channel + c) = static_cast<float>(coeff * input.at<float>(y, x*channel + c));
            }
        }
    }
}

}  // unnamed namespace

void colorConstancyHorn(cv::InputArray input, cv::OutputArray output, double thre) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();
    cv::Mat laplace;

    logarithm(img, out);
    laplacian(out, laplace);
    threshold(laplace, laplace, thre);
    gauss_seidel(out, laplace, 20);
    normalizeCC(out, out);
    exponential(out, out);
}

void colorConstancyRahman(cv::InputArray input, cv::OutputArray output, double sigma, double scale, int nLevel) {
    msg_assert(input.depth() == CV_32F, "Input cv::Mat must be CV_32 depth");

    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    cv::Mat gauss, tmp;

    double* sigmas = new double[nLevel];
    sigmas[0] = sigma * std::max(img.rows, img.cols);
    for (int i = 1; i < nLevel; i++) sigmas[i] = sigmas[i - 1] * scale;

    out = cv::Mat(img.size(), CV_32FC3);
    double weight = 0.0;
    for (int i = 0; i < nLevel; i++) {
        // Apply Gaussian filter
        cv::GaussianBlur(img, gauss, cv::Size(0, 0), sigmas[i]);

        // Subtraction
        cv::subtract(img, gauss, tmp);

        // Offset reflectance
        tmp.convertTo(tmp, CV_32F, 1.0, -1.0);

        // Normalization
        normalize(tmp, tmp, 0.0f, 1.0f);

        // Accumulate
        cv::scaleAdd(tmp, 1.0 / (i + 1), out, out);
        weight += 1.0 / (i + 1);
    }
    out.convertTo(out, CV_32F, 1.0 / weight);

    delete[] sigmas;
}

void colorConstancyFaugeras(cv::InputArray input, cv::OutputArray output) {
    msg_assert(input.depth() == CV_32F, "Input cv::Mat must be CV_32 depth");

    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    const int width = img.cols;
    const int height = img.rows;
    const int channel = img.channels();

    // Convert color space
    cv::Mat hvs;
    cv::cvtColor(img, hvs, cv::COLOR_BGR2Lab);

    // Homomophic filtering
    std::vector<cv::Mat> chs, spc(channel, cv::Mat(height, width, CV_32FC1));
    cv::split(hvs, chs);

    for (int c = 1; c < channel; c++) {
        cv::dct(chs[c], spc[c]);
        highEnhanceFilter(spc[c], spc[c]);
        cv::idct(spc[c], chs[c]);
    }
    cv::merge(chs, out);
    cv::cvtColor(out, out, cv::COLOR_Lab2BGR);
}

}  // namespace lime

#endif  // SRC_HVS_COLOR_CONSTANCY_DETAIL_H_

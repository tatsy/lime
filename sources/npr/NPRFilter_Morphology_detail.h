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

#ifndef SRC_NPR_NPRFILTER_MORPHOLOGY_DETAIL_H_
#define SRC_NPR_NPRFILTER_MORPHOLOGY_DETAIL_H_

#include <algorithm>

namespace lime {

namespace npr {

namespace filter {

void morphErode(cv::InputArray input, cv::OutputArray output, int ksize) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    const int width = img.cols;
    const int height = img.rows;
    const int dim = img.channels();

    out = cv::Mat(height, width, CV_MAKETYPE(CV_32F, dim));
    for (int c = 0; c < dim; c++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float val = 1.0f;
                for (int dy = -ksize; dy <= ksize; dy++) {
                    for (int dx = -ksize; dx <= ksize; dx++) {
                        if (dx * dx + dy * dy > ksize * ksize) continue;
                        int xx = x + dx;
                        int yy = y + dy;
                        if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                            double dist = dx * dx + dy * dy;
                            float u = img.at<float>(yy, xx*dim + c);
                            val = std::min(val, u);
                        }
                    }
                }
                out.at<float>(y, x*dim + c) = val;
            }
        }
    }
}

void morphDilate(cv::InputArray input, cv::OutputArray output, int ksize) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    int width = img.cols;
    int height = img.rows;
    int dim = img.channels();
    cv::Mat temp = img;

    out = cv::Mat(height, width, CV_MAKETYPE(CV_32F, dim));
    for (int c = 0; c < dim; c++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float val = 0.0f;
                for (int dy = -ksize; dy <= ksize; dy++) {
                    for (int dx = -ksize; dx <= ksize; dx++) {
                        if (dx * dx + dy * dy > ksize * ksize) continue;
                        int xx = x + dx;
                        int yy = y + dy;
                        if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                            double dist = dx * dx + dy * dy;
                            float u = temp.at<float>(yy, xx*dim + c);
                            val = std::max(val, u);
                        }
                    }
                }
                out.at<float>(y, x*dim + c) = val;
            }
        }
    }
}

void morphOpen(cv::InputArray input, cv::OutputArray output, int ksize) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    cv::Mat temp = img;
    morphErode(temp, out, ksize);
    temp = out;
    morphDilate(temp, out, ksize);
}

void morphClose(cv::InputArray input, cv::OutputArray output, int ksize) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    cv::Mat temp = img;
    morphDilate(temp, out, ksize);
    temp = out;
    morphErode(temp, out, ksize);
}

void morphGradient(cv::InputArray input, cv::OutputArray output, int ksize) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    cv::Mat erode, dilate;
    morphErode(img, erode, ksize);
    morphDilate(img, dilate, ksize);
    cv::subtract(dilate, erode, out);
}

void morphTophat(cv::InputArray input, cv::OutputArray output, int ksize) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    cv::Mat opening;
    morphOpen(img, opening, ksize);
    cv::subtract(img, opening, out);
}

void morphBlackhat(cv::InputArray input, cv::OutputArray output, int ksize) {
    cv::Mat  img = input.getMat();
    cv::Mat& out = output.getMatRef();

    cv::Mat closing;
    morphClose(img, closing, ksize);
    cv::subtract(closing, img, out);
}

}  // namespace filter

}  // namespace npr

}  // namespace lime

#endif  // SRC_NPR_NPRFILTER_MORPHOLOGY_DETAIL_H_

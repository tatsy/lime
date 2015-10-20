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

#ifndef SRC_NPR_NOISE_DETAIL_H_
#define SRC_NPR_NOISE_DETAIL_H_

#include "../core/Random.h"

namespace lime {

namespace npr {

namespace noise {

    void random(cv::OutputArray noise, const cv::Size& size) {
        Random rand = Random::getRNG();
        cv::Mat& out = noise.getMatRef();
        out = cv::Mat::zeros(size, CV_32FC1);
        for (int y = 0; y < size.height; y++) {
            for (int x = 0; x < size.width; x++) {
                out.at<float>(y, x) = static_cast<float>(rand.randReal());
            }
        }
    }

    void perlin(cv::OutputArray noise, const cv::Size& size, int level) {
        const int width = size.width;
        const int height = size.height;
        cv::Mat out = noise.getMatRef();
        out = cv::Mat::zeros(height, width, CV_32FC1);

        int scaleW = width >> level;
        int scaleH = height >> level;
        Assertion(scaleW != 0 && scaleH != 0, "Specified level is too large.");

        Random rand = Random::getRNG();
        int count = 0;
        while (scaleW <= width && scaleH <= height) {
            cv::Mat lev = cv::Mat(scaleH, scaleW, CV_32FC1);
            for (int y = 0; y < scaleH; y++) {
                for (int x = 0; x < scaleW; x++) {
                    lev.at<float>(y, x) = static_cast<float>(rand.randReal());
                }
            }

            cv::resize(lev, lev, size);
            cv::GaussianBlur(lev, lev, cv::Size(5, 5), 3.0);
            out = out + lev;

            scaleW <<= 1;
            scaleH <<= 1;
            count++;
        }
        out.convertTo(out, CV_32FC1, 1.0 / count);
    }

}  // namespace noise

}  // namespace npr

}  // namespace lime

#endif  // SRC_NPR_NOISE_DETAIL_H_

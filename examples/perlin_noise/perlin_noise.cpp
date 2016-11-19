/*******************************************************************************
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
*******************************************************************************/

#include <opencv2/opencv.hpp>

#include <iostream>
#include <ctime>

#include "lime.hpp"

int main(int argc, char** argv) {
    const int width  = 256;
    const int height = 256;
    cv::Mat pnoise = cv::Mat::zeros(height, width, CV_32FC1);

    lime::Random rand = lime::Random::getRNG();

    for (int i = 0; i < 100; i++) {
        int scaleW = 8;
        int scaleH = 8;
        int count = 0;
        while (scaleW < width && scaleH < height) {
            cv::Mat noise = cv::Mat(scaleH, scaleW, CV_32FC1);
            for (int y = 0; y < scaleH; y++) {
                for (int x = 0; x < scaleW; x++) {
                    noise.at<float>(y, x) = static_cast<float>(rand.randReal());
                }
            }

            cv::resize(noise, noise, cv::Size(height, width));
            cv::GaussianBlur(noise, noise, cv::Size(9, 9), 5.0);
            pnoise = pnoise + noise;

            scaleW <<= 1;
            scaleH <<= 1;
            count++;
        }

        pnoise.convertTo(pnoise, CV_32FC1, 1.0 / count);

        cv::imshow("pnoise", pnoise);
        cv::waitKey(30);
    }
    cv::destroyAllWindows();
}

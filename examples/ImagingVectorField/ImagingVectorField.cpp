/*
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
*/

#include <opencv2/opencv.hpp>

#define _USE_MATH_DEFINES
#include <cmath>
#include <ctime>
#include <iostream>

#include "../../include/lime.hpp"

void demo_circle() {
    const int width  = 512;
    const int height = 512;
    cv::Mat vfield = cv::Mat(height, width, CV_32FC2);

    const int cx = width / 2;
    const int cy = height / 2;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double dx = x - cx;
            double dy = y - cy;
            double theta = atan2(dx, -dy);
            vfield.at<float>(y, x * 2 + 0) = static_cast<float>(2.0 * cos(theta));
            vfield.at<float>(y, x * 2 + 1) = static_cast<float>(2.0 * sin(theta));
        }
    }

    cv::Mat noise, classic, euler, runge;
    npr::noise::random(noise, cv::Size(width, height));

    printf("[LIC] Classic     -> ");
    npr::lic(classic, noise, vfield, 20, npr::LIC_CLASSIC);
    printf("OK\n");

    printf("[LIC] Eulerian    -> ");
    npr::lic(euler,   noise, vfield, 20, npr::LIC_EULARIAN);
    printf("OK\n");

    printf("[LIC] Runge-Kutta -> ");
    npr::lic(runge,   noise, vfield, 20, npr::LIC_RUNGE_KUTTA);
    printf("OK\n");

    cv::imshow("Noise", noise);
    cv::imshow("Classic", classic);
    cv::imshow("Eular", euler);
    cv::imshow("Runge Kutta", runge);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void demo_img(const cv::Mat& img) {
    const int width  = img.cols;
    const int height = img.rows;
    img.convertTo(img, CV_32FC1, 1.0 / 255.0);

    cv::Mat sst, vfield;
    npr::calcVectorField(img, sst, 11, npr::VECTOR_SST, npr::EDGE_SOBEL);
    vfield = cv::Mat(height, width, CV_32FC2);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float theta = sst.at<float>(y, x);
            vfield.at<float>(y, x * 2 + 0) = static_cast<float>(2.0 * cos(theta));
            vfield.at<float>(y, x * 2 + 1) = static_cast<float>(2.0 * sin(theta));
        }
    }

    cv::Mat noise, flow, out;
    npr::noise::random(noise, cv::Size(width, height));
    npr::lic(noise, flow, vfield, 20, npr::LIC_CLASSIC);
    npr::lic(img, out, vfield, 20, npr::LIC_RUNGE_KUTTA);

    cv::imshow("Input", img);
    cv::imshow("Flow", flow);
    cv::imshow("Output", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

int main(int argc, char** argv) {
    if (argc > 1) {
        cv::Mat img = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
        demo_img(img);
    } else {
        demo_circle();
    }
}

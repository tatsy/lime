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

#include <iostream>

#include "lime.hpp"

int main(int argc, char **argv) {
    if (argc <= 1) {
        std::cerr << "usage: cc_examples.exe [input image]" << std::endl;
        return 1;
    }

    cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cerr << "Failed to load image" << std::endl;
        return 1;
    }

    img.convertTo(img, CV_32F, 1.0 / 255.0);

    cv::Mat outHorn, outRahman, outFaugeras;
    lime::colorConstancyHorn(img, outHorn);
    lime::colorConstancyRahman(img, outRahman);
    lime::colorConstancyFaugeras(img, outFaugeras);

    outHorn.convertTo(outHorn, CV_8U, 255.0);
    outRahman.convertTo(outRahman, CV_8U, 255.0);
    outFaugeras.convertTo(outFaugeras, CV_8U, 255.0);
    cv::imwrite("horn.jpg", outHorn);
    cv::imwrite("rahman.jpg", outRahman);
    cv::imwrite("faugeras.jpg", outFaugeras);

    cv::imshow("Input", img);
    cv::imshow("Horn", outHorn);
    cv::imshow("Rahman", outRahman);
    cv::imshow("Faugeras", outFaugeras);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

/*
Copyright [2015] <tatsuya yatagawa (tatsy)>

permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files(the "software"), to deal in
the software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of
the software, and to permit persons to whom the software is furnished to do so,
subject to the following conditions :

the above copyright notice and this permission notice shall be included in all
copies or substantial portions of the software.
the software is provided "as is", without warranty of any kind, express or
implied, including but not limited to the warranties of merchantability, fitness
for a particular purpose and noninfringement.in no event shall the authors or
copyright holders be liable for any claim, damages or other liability, whether
in an action of contract, tort or otherwise, arising from, out of or in
connection with the software or the use or other dealings in the software.
*/

#include <iostream>

#include "../../include/lime.hpp"

int main(int argc, char **argv) {
    if (argc <= 1) {
        std::cerr << "usage: cc_examples.exe [input image]" << std::endl;
        return 1;
    }

    cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);
    if (img.empty()) {
        cerr << "Failed to load image" << endl;
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

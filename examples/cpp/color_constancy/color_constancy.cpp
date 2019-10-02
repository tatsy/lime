#include <iostream>

#include <lime.hpp>

int main(int argc, char **argv) {
    if (argc <= 1) {
        std::cerr << "usage: demo_color_constancy.exe [input image]" << std::endl;
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

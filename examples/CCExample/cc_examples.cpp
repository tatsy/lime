#include <iostream>
using namespace std;

#undef NDEBUG
#include "../../include/lime.hpp"

int main(int argc, char **argv) {
	if (argc <= 1) {
		cerr << "usage: cc_examples.exe [input image]" << endl;
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

	cv::imshow("Input", img);
    cv::imshow("Horn", outHorn);
    cv::imshow("Rahman", outRahman);
    cv::imshow("Faugeras", outFaugeras);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

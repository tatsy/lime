#include <iostream>
#include <time.h>
using namespace std;

#include <opencv2/opencv.hpp>
#include "../../include/lime.hpp"

int main(int argc, char** argv) {
	const int width = 256;
	const int height = 256;
	cv::Mat pnoise = cv::Mat::zeros(height, width, CV_32FC1);

	lime::Random rand = lime::Random::getRNG();

	for(int i=0; i<100; i++) {
		int scaleW = 8;
		int scaleH = 8;
		int count = 0;
		while(scaleW < width && scaleH < height) {
			cv::Mat noise = cv::Mat(scaleH, scaleW, CV_32FC1);
			for(int y=0; y<scaleH; y++) {
				for(int x=0; x<scaleW; x++) {
					noise.at<float>(y, x) = (float)rand.randReal();
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

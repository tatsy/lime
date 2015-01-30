#define _USE_MATH_DEFINES
#include <cmath>
#include <ctime>
#include <vector>
#include <string>
#include <Windows.h>
using namespace std;

#include <opencv2/opencv.hpp>
#include "PencilDrawing.h"

bool save_result = true;
const int depth = 1;

LARGE_INTEGER t1, t2, freq;
void timer_start() {
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t1);
}

void timer_stop() {
	QueryPerformanceCounter(&t2);
	cout << "  Time: " << (1000.0 * (t2.QuadPart - t1.QuadPart) / freq.QuadPart) << " ms" << endl;
}

string basename(string fname) {
	int i = (int)fname.find_last_of('.');
	return fname.substr(0, i);
}

void run_still_image(string filename) {
	cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);
	if(img.empty()) {
		cout << "Failed to load image \"" << filename << "\"." << endl;
		return;
	}

	int width = img.cols;
	int height = img.rows;
	int dim = img.channels();
	img.convertTo(img, CV_32F, 1.0 / 255.0);

	cv::Mat gray, out;
	vector<cv::Point2f> points;
	cv::Mat noise;

	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
	lime::npr::poissonDisk(gray, points, lime::npr::PDS_RAND_QUEUE, 1.0, 4.0);
	pencilDrawing(img, out, points);

	cv::namedWindow("Input");
	cv::namedWindow("Output");
	cv::imshow("Input", img);
	cv::imshow("Output", out);
	cv::waitKey(0);
	cv::destroyAllWindows();

	if(save_result) {
		out.convertTo(out, CV_MAKETYPE(CV_8U, dim), 255.0);
		noise.convertTo(noise, CV_8UC3, 255.0);

		string base = basename(filename);
		string fileout = base + "_out.png";
		string filenoise = base + "_noise.png";
		cv::imwrite(fileout, out);
		cv::imwrite(filenoise, noise);
		cout << "Results are saved !!" << endl;
	}
}

int main(int argc, char** argv) {
	// * Check input arguments
	if(argc <= 1) {
		cout << "usage: PencilDrawing.exe [input image]" << endl;
		return -1;
	}

	string filename = string(argv[1]);
	run_still_image(filename);
}

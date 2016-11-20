#include <opencv2/opencv.hpp>

#define _USE_MATH_DEFINES
#include <cmath>
#include <ctime>
#include <vector>
#include <string>

#include "../pencil_drawing/pencil_drawing.h"

bool save_result = true;
const int depth = 1;

clock_t start, elapse;
void timer_start() {
    start = clock();
}

void timer_stop() {
    elapse = clock() - start;
    std::cout << "  Time: " << (elapse / 1000.0) << " ms" << std::endl;
}

std::string basename(const std::string& fname) {
    int i = static_cast<int>(fname.find_last_of('.'));
    return fname.substr(0, i);
}

void run_still_image(const std::string& filename) {
    cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cout << "Failed to load image \"" << filename << "\"." << std::endl;
        return;
    }

    int width = img.cols;
    int height = img.rows;
    int dim = img.channels();
    img.convertTo(img, CV_32F, 1.0 / 255.0);

    cv::Mat gray, out;
    std::vector<cv::Point2f> points;

    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    lime::npr::poissonDisk(gray, &points, lime::npr::PDS_RAND_QUEUE, 1.0, 4.0);
    pencilDrawing(img, out, points);

    cv::namedWindow("Input");
    cv::namedWindow("Output");
    cv::imshow("Input", img);
    cv::imshow("Output", out);
    cv::waitKey(0);
    cv::destroyAllWindows();

    if (save_result) {
        out.convertTo(out, CV_MAKETYPE(CV_8U, dim), 255.0);

        std::string base = basename(filename);
        std::string fileout = base + "_out.png";
        std::string filenoise = base + "_noise.png";
        cv::imwrite(fileout, out);
        std::cout << "Results are saved !!" << std::endl;
    }
}

int main(int argc, char** argv) {
    // * Check input arguments
    if (argc <= 1) {
        std::cout << "usage: PencilDrawing.exe [input image]" << std::endl;
        return -1;
    }

    std::string filename(argv[1]);
    run_still_image(filename);
}

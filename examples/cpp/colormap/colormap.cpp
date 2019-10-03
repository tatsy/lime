#include <opencv2/opencv.hpp>
#include <lime.hpp>

int main(int argc, char** argv) {
    const int size = 256;
    const int interval = size / 16;

    cv::Mat intensity(size, size, CV_8UC1);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            const int row = y / interval;
            const int col = x / interval;
            const int val = row * 16 + col;
            intensity.at<uchar>(y, x) = val;
        }
    }

    cv::Mat jet, coolwarm, inferno, magma, plasma, viridis;
    lime::pseudoColors(intensity, jet, lime::CMap::Jet);
    lime::pseudoColors(intensity, coolwarm, lime::CMap::CoolWarm);
    lime::pseudoColors(intensity, inferno, lime::CMap::Inferno);
    lime::pseudoColors(intensity, magma, lime::CMap::Magma);
    lime::pseudoColors(intensity, plasma, lime::CMap::Plasma);
    lime::pseudoColors(intensity, viridis, lime::CMap::Viridis);

    cv::imshow("Intensity", intensity);
    cv::imshow("Jet", jet);
    cv::imshow("CoolWarm", coolwarm);
    cv::imshow("Inferno", inferno);
    cv::imshow("Magma", magma);
    cv::imshow("Plasma", plasma);
    cv::imshow("Viridis", viridis);

    cv::waitKey();
    cv::destroyAllWindows();
}
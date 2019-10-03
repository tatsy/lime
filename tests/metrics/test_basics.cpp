#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "lime_test.h"

#include "lime.hpp"
using namespace lime;

class MetricsBasicsTest : public ::testing::Test {
protected:
    MetricsBasicsTest() {}
    virtual ~MetricsBasicsTest() {}
};

class MetricsBasicsTestWithParams : public MetricsBasicsTest, public ::testing::WithParamInterface<std::string> {
protected:
    MetricsBasicsTestWithParams() {}
    virtual ~MetricsBasicsTestWithParams() {}

    void SetUp() override {
        const std::string filename = DATA_DIR + GetParam();
        image = cv::imread(filename, cv::IMREAD_COLOR);
        if (image.empty()) {
            ErrorMsg("Failed to load image file: %s", filename.c_str());
        }
    }

    void addNoise(cv::Mat &image, int noiseLevel = 16) const {
        Random rand;

        if (image.depth() == CV_8U && image.channels() == 1) {
            image.forEach<uchar>([&](uchar u, const int *p) -> void {
                const int v = (int)u + rand.nextInt(noiseLevel);
                image.at<uchar>(p[0], p[1]) = cv::saturate_cast<uchar>(v);
            });
        }

        if (image.depth() == CV_8U && image.channels() == 3) {
            image.forEach<cv::Vec3b>([&](cv::Vec3b u, const int *p) -> void {
                cv::Vec3b v;
                v[0] = cv::saturate_cast<uchar>((int)u[0] + rand.nextInt(noiseLevel));
                v[1] = cv::saturate_cast<uchar>((int)u[1] + rand.nextInt(noiseLevel));
                v[2] = cv::saturate_cast<uchar>((int)u[2] + rand.nextInt(noiseLevel));
                image.at<cv::Vec3b>(p[0], p[1]) = v;
            });
        }
    }

protected:
    cv::Mat image;
};

TEST_P(MetricsBasicsTestWithParams, MSETest) {
    double score;

    score = lime::MSE(image, image);
    ASSERT_EQ(score, 0.0);

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat noisy;
    gray.copyTo(noisy);
    addNoise(noisy);

    score = lime::MSE(gray, noisy);
    ASSERT_LT(score, 128.0);
}

TEST_P(MetricsBasicsTestWithParams, RMSETest) {
    double score;
    
    score = lime::RMSE(image, image);
    ASSERT_EQ(score, 0.0);

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat noisy;
    gray.copyTo(noisy);
    addNoise(noisy);

    score = lime::RMSE(gray, noisy);
    ASSERT_LT(score, 16.0);
}

TEST_P(MetricsBasicsTestWithParams, PSNRTest) {
    double score;
    
    score = lime::PSNR(image, image);
    ASSERT_EQ(score, INFTY);

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat noisy;
    gray.copyTo(noisy);
    addNoise(noisy);

    score = lime::PSNR(gray, noisy);
    ASSERT_GT(score, 20.0);
}

TEST_P(MetricsBasicsTestWithParams, SSIMTest) {
    double score;

    score = lime::SSIM(image, image);
    ASSERT_EQ(score, 1.0);

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat noisy;
    gray.copyTo(noisy);
    addNoise(noisy);

    score = lime::SSIM(gray, noisy);
    ASSERT_GT(score, 0.8);
}

TEST_P(MetricsBasicsTestWithParams, MSSSIMTest) {
    double score;

    score = lime::MSSSIM(image, image);
    ASSERT_EQ(score, 1.0);

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat noisy;
    gray.copyTo(noisy);
    addNoise(noisy);

    score = lime::MSSSIM(gray, noisy);
    ASSERT_GT(score, 0.8);
}

TEST_P(MetricsBasicsTestWithParams, CMSSIMTest) {
    double score;

    score = lime::CMSSIM(image, image);
    ASSERT_EQ(score, 1.0);

    cv::Mat noisy;
    image.copyTo(noisy);
    addNoise(noisy);

    score = lime::CMSSIM(image, noisy);
    ASSERT_GT(score, 0.8);
}

std::vector<std::string> files = {
    "cameraman.jpg", "barbara.jpg", "lena.jpg"
};

INSTANTIATE_TEST_CASE_P(, MetricsBasicsTestWithParams, ::testing::ValuesIn(files));

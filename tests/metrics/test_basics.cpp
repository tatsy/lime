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

    void addNoise(cv::Mat &image) const {
        Random rand;
        image.forEach<uchar>([&](uchar u, const int *p) -> void {
            const int v = (int)u + rand.nextInt(16);
            image.at<uchar>(p[0], p[1]) = cv::saturate_cast<uchar>(v);
        });
    }

protected:
    cv::Mat image;
};

TEST_P(MetricsBasicsTestWithParams, MSETest) {
    double score;

    score = lime::MSE(image, image);
    ASSERT_EQ(score, 0.0);

    cv::Mat noisy;
    image.copyTo(noisy);
    addNoise(noisy);

    score = lime::MSE(image, noisy);
    ASSERT_LT(score, 32.0);
}

TEST_P(MetricsBasicsTestWithParams, RMSETest) {
    double score;
    
    score = lime::RMSE(image, image);
    ASSERT_EQ(score, 0.0);

    cv::Mat noisy;
    image.copyTo(noisy);
    addNoise(noisy);

    score = lime::RMSE(image, noisy);
    ASSERT_LT(score, 16.0);
}

TEST_P(MetricsBasicsTestWithParams, PSNRTest) {
    double score;
    
    score = lime::PSNR(image, image);
    ASSERT_EQ(score, INFTY);

    cv::Mat noisy;
    image.copyTo(noisy);
    addNoise(noisy);

    score = lime::PSNR(image, noisy);
    ASSERT_GT(score, 30.0);
}

TEST_P(MetricsBasicsTestWithParams, SSIMTest) {
    double score;

    score = lime::SSIM(image, image);
    ASSERT_EQ(score, 1.0);

    cv::Mat noisy;
    image.copyTo(noisy);
    addNoise(noisy);

    score = lime::SSIM(image, noisy);
    ASSERT_GT(score, 0.8);
}

std::vector<std::string> files = {
    "cameraman.jpg", "barbara.jpg"
};

INSTANTIATE_TEST_CASE_P(, MetricsBasicsTestWithParams, ::testing::ValuesIn(files));

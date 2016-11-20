#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_NOISE_DETAIL_H_
#define _NPR_NOISE_DETAIL_H_

#include "../core/random.h"

namespace lime {

void randomNoise(cv::OutputArray noise, const cv::Size& size) {
    Random rand = Random::getRNG();
    cv::Mat& out = noise.getMatRef();
    out = cv::Mat::zeros(size, CV_32FC1);
    for (int y = 0; y < size.height; y++) {
        for (int x = 0; x < size.width; x++) {
            out.at<float>(y, x) = static_cast<float>(rand.randReal());
        }
    }
}

void perlinNoise(cv::OutputArray noise, const cv::Size& size, int level) {
    const int width = size.width;
    const int height = size.height;
    
    cv::Mat &out = noise.getMatRef();
    out = cv::Mat::zeros(height, width, CV_32FC1);

    int scaleW = width >> level;
    int scaleH = height >> level;
    Assertion(scaleW != 0 && scaleH != 0, "Specified level is too large.");

    Random rand = Random::getRNG();
    int count = 0;
    while (scaleW <= width && scaleH <= height) {
        cv::Mat lev = cv::Mat(scaleH, scaleW, CV_32FC1);
        for (int y = 0; y < scaleH; y++) {
            for (int x = 0; x < scaleW; x++) {
                lev.at<float>(y, x) = static_cast<float>(rand.randReal());
            }
        }

        cv::resize(lev, lev, size);
        cv::GaussianBlur(lev, lev, cv::Size(5, 5), 3.0);
        out = out + lev;

        scaleW <<= 1;
        scaleH <<= 1;
        count++;
    }
    out.convertTo(out, CV_32FC1, 1.0 / count);
}

}  // namespace lime

#endif  // _NPR_NOISE_DETAIL_H_

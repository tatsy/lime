#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_NOISE_H_
#define _NPR_NOISE_H_

#include <opencv2/opencv.hpp>

#include <vector>

#include "../core/point.h"

namespace lime {

/**
 * Generate random noise
 * @param[out] noise: generated noise image (CV_32FC1 format)
 * @param[in] size: size of the noise image to be returned
 **/
inline void randomNoise(cv::OutputArray noise, const cv::Size& size);

/**
 * Generate Perlin noise
 * @param[out] noise: generated noise image (CV_32FC1 format)
 * @param[in] size: size of the noise image to be returned
 * @param[in] level: maximum depth level to generate the Perlin noise
 **/
inline void perlinNoise(cv::OutputArray noise, const cv::Size& size, int level);

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "noise_detail.h"
#endif

#endif  // SRC_NPR_NOISE_H_

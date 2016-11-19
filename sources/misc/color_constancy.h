#ifndef _MSC_VER
#pragma once
#endif

#ifndef _MISC_COLOR_CONSTANCY_H_
#define _MISC_COLOR_CONSTANCY_H_

#include <opencv2/opencv.hpp>

namespace lime {

inline void colorConstancyHorn(cv::InputArray input, cv::OutputArray output, double thre = 0.05);

inline void colorConstancyRahman(cv::InputArray input, cv::OutputArray output,
                                 double sigma = 1.0, double scale = 0.16, int nLevel = 3);

inline void colorConstancyFaugeras(cv::InputArray input, cv::OutputArray output);

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "color_constancy_detail.h"
#endif

#endif  // _MISC_COLOR_CONSTANCY_H_

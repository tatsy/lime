#ifndef _MSC_VER
#pragma once
#endif

#ifndef _MISC_COLOR_CONSTANCY_H_
#define _MISC_COLOR_CONSTANCY_H_

#include <opencv2/opencv.hpp>

namespace lime {

/**
 * Color constancy algorithms.
 * @ingroup misc
 **/
enum ConstancyTypes : int {
    CONSTANCY_HORN,         //!< Horm's algorithm.
    CONSTANCY_RAHMAN,       //!< Rahman's algorithm.
    CONSTANCY_FAUGERAS      //!< Faugeras's algorithm.
};

/**
 * @ingroup misc
 * @brief Perform color constancy algorithm for the input image.
 * @param[in] input: Input image.
 * @param[out] output: Output image.
 * @param[in] type: Algorithm type enum.
 *
 * @details
 * @code
 * # Python
 * output = lime.colorConstancy(input, output, type)
 * @endcode
 **/
inline void colorConstancy(cv::InputArray input, cv::OutputArray output, int type = CONSTANCY_RAHMAN);

inline void colorConstancyHorn(cv::InputArray input, cv::OutputArray output, double thre = 0.05);

inline void colorConstancyRahman(cv::InputArray input, cv::OutputArray output,
                                 double sigma = 1.0, double scale = 0.16, int nLevel = 3);

inline void colorConstancyFaugeras(cv::InputArray input, cv::OutputArray output);

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "color_constancy_detail.h"
#endif

#endif  // _MISC_COLOR_CONSTANCY_H_

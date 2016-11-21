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
enum ConstancyType : int {
    CONSTANCY_HORN,         //!< Horm's algorithm.
    CONSTANCY_RAHMAN,       //!< Rahman's algorithm.
    CONSTANCY_FAUGERAS      //!< Faugeras's algorithm.
};

/**
 * Apply color constancy algorithm for the input image.
 * @ingroup misc
 *
 * @details
 * @arg @b src: The input floating-point, 3-channel image.
 * @arg @b dst: The destination image; will have also floating-point, 3-channel pixel values.
 * @arg @b type: Algorithm type (see <a>@c lime::ConstancyType</a>).
 *
 * @b Python
 *
 * @details
 * @code{.py}
 * dst = lime.colorConstancy(src, ftype)
 * @endcode
 *
 * @b Parameters
 * @arg @b src - @c numpy.ndarray : The input floating-point, 3-channel image.
 * @arg @b ftype - @c int : Chosen from <a>@c lime::ConstancyType</a>.
 *
 * @b Returns
 * @arg @b dst - @c numpy.ndarray : The destination image; will have also floating-point, 3-channel pixel values.
 **/
inline void colorConstancy(cv::InputArray src, cv::OutputArray dst, int type);

//! Horn's color constancy algorihtm.
inline void colorConstancyHorn(cv::InputArray input, cv::OutputArray output, double thre = 0.05);

//! Rahman's color constancy algorithm.
inline void colorConstancyRahman(cv::InputArray input, cv::OutputArray output,
                                 double sigma = 1.0, double scale = 0.16, int nLevel = 3);

//! Faugeras's color constancy algorithm.
inline void colorConstancyFaugeras(cv::InputArray input, cv::OutputArray output);

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "color_constancy_detail.h"
#endif

#endif  // _MISC_COLOR_CONSTANCY_H_

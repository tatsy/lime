#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_MORPHOLOGY_H_
#define _NPR_MORPHOLOGY_H_

namespace lime {

/**
 * Morphological filter types.
 * @ingroup npr
 **/
enum MorphTypes : int {
    MORPH_ERODE,        //!< Erosion
    MORPH_DILATE,       //!< Dilation
    MORPH_OPEN,         //!< Opening
    MORPH_CLOSE,        //!< Closing
    MORPH_GRADIENT,     //!< Gradient
    MORPH_TOPHAT,       //!< Top-hat
    MORPH_BLACKHAT      //!< Black-hat
};

/**
 * Image filtering with mathematical morphology.
 * @ingroup npr
 * @param[in] src: Input image.
 * @param[out] dst: Output image.
 * @param[in] type: MorphTypes enum.
 * @param[in] ksize: Kernel size of the filter.
 *
 * @details
 * @code
 * # Python
 * dst = lime.morphFilter(src, ksize)
 * @endcode
 **/
inline void morphFilter(cv::InputArray src, cv::OutputArray dst, int type, int ksize);

//! Compute erosion of mathematical morphology
inline void morphErode(cv::InputArray img, cv::OutputArray out, int ksize);

//! Compute dilation of mathematical morphology
inline void morphDilate(cv::InputArray img, cv::OutputArray out, int ksize);

//! Compute opening of mathematical morphology
inline void morphOpen(cv::InputArray img, cv::OutputArray out, int ksize);

//! Compute closing of mathematical morphology
inline void morphClose(cv::InputArray img, cv::OutputArray out, int ksize);

//! Compute gradient of mathematical morphology
inline void morphGradient(cv::InputArray img, cv::OutputArray out, int ksize);

//! Compute gradient of mathematical morphology
inline void morphTophat(cv::InputArray img, cv::OutputArray out, int ksize);

//! Compute gradient of mathematical morphology
inline void morphBlackhat(cv::InputArray img, cv::OutputArray out, int ksize);

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "morphology_detail.h"
#endif

#endif  // _NPR_MORPHOLOGY_H_

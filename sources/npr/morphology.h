#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_MORPHOLOGY_H_
#define _NPR_MORPHOLOGY_H_

#include "core/common.h"

namespace lime {

/**
 * Morphological filter types.
 * @ingroup npr
 **/
enum MorphType : int {
    MORPH_ERODE,        //!< Erosion.
    MORPH_DILATE,       //!< Dilation.
    MORPH_OPEN,         //!< Opening = \f$ Erosion \rightarrow Dilation \f$.
    MORPH_CLOSE,        //!< Closing = \f$ Dilation \rightarrow Erosion \f$.
    MORPH_GRADIENT,     //!< Gradient = \f$ Dilation - Erosion \f$.
    MORPH_TOPHAT,       //!< Top-hat = \f$ Input - Opening \f$.
    MORPH_BLACKHAT      //!< Black-hat = \f$ Input - Closing \f$.
};

/**
 * Image filtering with mathematical morphology.
 * @ingroup npr
 *
 * @details
 * @b Parameters
 * @arg @b src: The floating-point, 1 or 3-channel image.
 * @arg @b dst: The destination image; will have the same type as <b>src</b>.
 * @arg @b type: The type of morphology operation (see <a>@c lime::MorphType</a>).
 * @arg @b ksize: Kernel size of the filter.
 *
 * @b Python
 *
 * @code
 * dst = lime.morphFilter(src, ftype, ksize)
 * @endcode
 *
 * @b Parameters
 * @arg @b src - @c numpy.ndarray : The floating-point, 1 or 3-channel image.
 * @arg @b ftype - @c int : Chosen from <a>@c lime::MorphType</a>.
 * @arg @b ksize - @c int : Kernel size.
 *
 * @b Returns
 * @arg @b dst - @c numpy.ndarray : The destination image with the same type as <b>src</b>.
 **/
LIME_METHOD_API void morphFilter(cv::InputArray src, cv::OutputArray dst, int type, int ksize);

}  // namespace lime

#ifndef LIME_USE_LIBRARY
#include "morphology_detail.h"
#endif

#endif  // _NPR_MORPHOLOGY_H_

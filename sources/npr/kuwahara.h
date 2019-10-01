#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_KUWAHARA_H_
#define _NPR_KUWAHARA_H_

#include "core/common.h"

namespace lime {

/**
 * Kuwahara filter types.
 * @ingroup npr
 **/
enum KuwaharaType : int {
    KUWAHARA_CLASSICAL,     //!< Classical Kuwahara filter.
    KUWAHARA_GENERALIZED,   //!< Generalized Kuwahara filter.
    KUWAHARA_ANISOTROPIC    //!< Anisotropic Kuwahara filter.
};

/**
 * Apply Kuwahara filter to the image.
 * @ingroup npr
 *
 * @details
 * @b Parameters
 * @arg @b src: Input image.
 * @arg @b dst: Output image.
 * @arg @b type: Kuwahara filter type (see <a>@c lime::KuwaharaType</a>).
 * @arg @b ksize: Kernel size of the filter.
 * @arg @b nDivide: # of orientation divisions (not used for <tt>KUWAHARA_CLASSICAL</tt>).
 *
 * @b Python
 *
 * @code{.py}
 * dst = lime.kuwaharaFilter(src, ftype, ksize, ndivide = 0)
 * @endcode
 *
 * @b Parameters
 * @arg @b src - @c numpy.ndarray : The floating-point, 1 or 3-channel image.
 * @arg @b ftype - @c int : Chosen from <a>@c lime::KuwaharaType</a>.
 * @arg @b ksize - @c int
 * @arg @b ndivide - @c int
 * @b Returns
 * @arg @b dst - @c numpy.ndarray : The destination image; will have the same types as <b>src</b>.
 **/
LIME_METHOD_API void kuwaharaFilter(cv::InputArray src, cv::OutputArray dst, int type, int ksize, int nDivide = 0);

}  // namespace lime

#ifndef LIME_USE_LIBRARY
#include "kuwahara_detail.h"
#endif

#endif

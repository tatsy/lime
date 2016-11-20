#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_KUWAHARA_H_
#define _NPR_KUWAHARA_H_

namespace lime {

/**
 * Kuwahara filter types.
 * @ingroup npr
 **/
enum KuwaharaTypes : int {
    KUWAHARA_CLASSICAL,     //!< Classical Kuwahara filter.
    KUWAHARA_GENERALIZED,   //!< Generalized Kuwahara filter.
    KUWAHARA_ANISOTROPIC    //!< Anisotropic Kuwahara filter.
};

/**
 * Kuwahara filter
 * @ingroup npr
 * @param[in] src: Input image.
 * @param[out] dst: Output image.
 * @param[in] type: KuwaharaTypes enum.
 * @param[in] ksize: Kernel size of the filter.
 * @param[in] nDivide: # of orientation divisions (not used for CLASSICAL).
 *
 * @detail
 * @code
 * # Python
 * dst = lime.kuwaharaFilter(src, type, ksize, nDivide = 0)
 * @endcode
 **/
inline void kuwaharaFilter(cv::InputArray src, cv::OutputArray dst, int type, int ksize, int nDivide = 0);

//! Clasical kuwahara filter
inline void kuwaharaClassical(cv::InputArray src, cv::OutputArray dst, int ksize);

//! Generalized kuwahara filter
inline void kuwaharaGeneralized(cv::InputArray src, cv::OutputArray dst, int ksize, int nDivide);

//! Anisotropic kuwahara filter
inline void kuwaharaAnisotropic(cv::InputArray src, cv::OutputArray dst, int ksize, int nDivide);

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "kuwahara_detail.h"
#endif

#endif

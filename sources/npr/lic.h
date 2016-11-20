#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_LIC_H_
#define _NPR_LIC_H_

#include <cmath>

#include "../core/point.h"

namespace lime {

/**
 * LIC algorithm types.
 * @ingroup npr
 * @details
 * This type represents the algorithm to generate integration path.
 **/
enum LICTypes : int {
    LIC_CLASSICAL,      //!< Classical.
    LIC_EULERIAN,       //!< Eulerian.
    LIC_RUNGE_KUTTA     //!< Runge-Kutta.
};

/**
 * Visualize a vector field using line integral convolusion (LIC).
 * @ingroup npr
 * @param[in] img: input image to be convoluted
 * @param[out] out: output image
 * @param[in] tangent: cv::Mat of CV_32FC2 depth which specifies a tangent direction to each pixel
 * @param[in] L: convolution length
 * @param[in] algo_type: algorithm type used for LIC
 * @details algorighms:
 *      LIC_CLASSICAL: slow but outputs beautiful vector field
 *      LIC_EULARIAN: fast and stable algorithm (default)
 *      LIC_RUNGE_KUTTA: second-order line integration
 **/
inline void LIC(cv::InputArray src, cv::OutputArray dst,
                const cv::Mat& tangent, int L, int licType = LIC_EULERIAN);

inline void angle2vector(cv::InputArray angle, cv::OutputArray vfield, double scale = 1.0);

inline void vector2angle(cv::InputArray vfield, cv::OutputArray angle);

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "../npr/lic_detail.h"
#endif

#endif  // _NPR_LIC_H_

#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_NPREDGES_H_
#define _NPR_NPREDGES_H_

#include <opencv2/opencv.hpp>

namespace lime {

/**
 * NPR edge types.
 * @ingroup npr
 */
enum NPREdgeType {
    NPR_EDGE_XDOG,      //!< Extended DoG (XDoG) filter.
    NPR_EDGE_FDOG       //!< Flow DoG (FDoG) filter.
};

/**
 * DoG parameters.
 * @ingroup npr
 **/
struct DoGParams {
    double kappa;
    double sigma;
    double tau;
    double phi;
    NPREdgeType edgeType;

    //! Constructor.
    explicit DoGParams(double kappa = 4.5, double sigma = 0.5, double tau = 0.95,
                       double phi = 10.0, NPREdgeType edgeType = NPR_EDGE_XDOG);
};  // struct DoGParams

/**
 * Detecting NPR edges with DoG filter.
 * @ingroup npr
 *
 * @details
 * @b Parameters
 * @arg @b image: The input floating-point, 1-channel image.
 * @arg @b edge: The floating-point, 1-channel edge image.
 * @arg @b params: DoG parameters.
 *
 * @details
 * @code{.py}
 * edge = lime.edgeDoG(image, params = {})
 * @endcode
 *
 * @b Parameters
 * @arg @b image - @c numpy.ndarray : The input floating-point, 1-channel image.
 * @arg @b params - @c dict : Stores the pair of parameter names and corresponding values.
 *
 * @b Returns
 * @arg @b edge - @c numpy.ndarray : The floating-point, 1-channel edge image.
 **/
inline void edgeDoG(cv::InputArray image, cv::OutputArray edge, const DoGParams& params = DoGParams());

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "npr_edges_detail.h"
#endif

#endif  // _NPR_NPREDGES_H_

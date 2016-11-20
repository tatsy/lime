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
 **/
enum NPREdgeTypes {
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
    NPREdgeTypes edgeType;

    explicit DoGParams(double kappa = 4.5, double sigma = 0.5, double tau = 0.95,
                      double phi = 10.0, NPREdgeTypes edgeType = NPR_EDGE_XDOG);
};  // struct DoGParams

/**
 * Detecting NPR edges with DoG filter.
 * @ingroup npr
 * @param[in] image: Input image.
 * @param[out] edge: Output edge image.
 * @param[in] params: DoG parameters.
 *
 * @details
 * @code
 * # Python
 * edge = lime.edgeDoG(image, params)
 * @endcode
 * <b>params</b>: Python builtin "dict" including DoG parameters.
 **/
inline void edgeDoG(cv::InputArray image, cv::OutputArray edge, const DoGParams& params = DoGParams());

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "npr_edges_detail.h"
#endif

#endif  // _NPR_NPREDGES_H_

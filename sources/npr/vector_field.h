#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_VECTORFIELD_H_
#define _NPR_VECTORFIELD_H_

#include <opencv2/opencv.hpp>

#include <vector>

#include "../core/point.h"
#include "../core/array2d.h"
#include "tensor.hpp"
#include "singularity.hpp"

namespace lime {

/**
 * Vector field types.
 * @ingroup npr
 **/
enum VecFieldTypes : int {
    VEC_FIELD_SST,      //!< Smoothed structural tensor.
    VEC_FIELD_ETF       //!< Edge tangent filter.
};

/**
 * Edge detector types.
 * @ingroup npe
 **/
enum EdgeDetector : int {
    EDGE_DETECT_SOBEL,         //!< Sobel filter.
    EDGE_DETECT_ROTATIONAL     //!< Rotationally invariant edge detector.
};

/**
 * Compute vector field
 * @ingroup npr
 * @param[in] img: input image from which a vector field is computed
 * @param[out] angles: output array of CV_32FC1 depth which stores tangent direction of vectors
 * @param[in] ksize: kernel size for smoothing the vector field
 * @param[in] vfieldType: algorithm to detect vector field (SST or ETF)
 * @param[in] edgeDetector: edge detection algorithm
 *
 * @details
 * @code
 * # Python
 * angles = lime.calcVectorField(input, ksize = 5, vfieldType = lime.VEC_FIELD_SST, edgeDetector = lime.EDGE_SOBEL)
 * @endcode
 **/
inline void calcVectorField(cv::InputArray input, cv::OutputArray angles, int ksize = 5,
                            int vfieldType = VEC_FIELD_SST, int edgeDetector = EDGE_DETECT_SOBEL);

// * detect singularity of vector field
inline void detectSingular(const Array2D<Tensor>& sst, std::vector<SingularPoint>* points,
                           std::vector<cv::Point2f>* delauneyNodes);

}  // namespace lime

#include "vector_field_detail.h"

#endif  // _NPR_VECTORFIELD_H_

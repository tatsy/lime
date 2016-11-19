#ifdef _MSC_VER
#pragma once
#endif

#ifndef SRC_NPR_VECTORFIELD_H_
#define SRC_NPR_VECTORFIELD_H_

#include <opencv2/opencv.hpp>

#include <vector>

#include "../core/point.h"
#include "../core/array2d.h"
#include "tensor.hpp"
#include "singularity.hpp"

namespace lime {

namespace npr {

enum VFieldType {
    VECTOR_SST,
    VECTOR_ETF
};

enum EdgeDetector {
    EDGE_SOBEL,
    EDGE_ROTATIONAL
};

/* Compute vector field
 * @param[in] img: input image from which a vector field is computed
 * @param[out] angles: output array of CV_32FC1 depth which stores tangent direction of vectors
 * @param[in] ksize: kernel size for smoothing the vector field
 * @param[in] vfieldType: algorithm to detect vector field (SST or ETF)
 * @param[in] edgeDetector: edge detection algorithm
 */
inline void calcVectorField(cv::InputArray input, cv::OutputArray angles, int ksize = 5,
                            VFieldType vfieldType = VECTOR_SST, EdgeDetector edgeDetector = EDGE_SOBEL);

// * detect singularity of vector field
inline void detectSingular(const Array2D<Tensor>& sst, std::vector<SingularPoint>* points,
                           std::vector<cv::Point2f>* delauneyNodes);

}  // namespace npr

}  // namespace lime

#include "vector_field_detail.h"

#endif  // SRC_NPR_VECTORFIELD_H_

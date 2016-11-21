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
 */
enum VecFieldType : int {
    VEC_FIELD_SST,      //!< Smoothed structural tensor.
    VEC_FIELD_ETF       //!< Edge tangent filter.
};

/**
 * Edge detector types.
 * @ingroup npr
 */
enum EdgeDetector : int {
    EDGE_DETECT_SOBEL,         //!< Sobel filter.
    EDGE_DETECT_ROTATIONAL     //!< Rotationally invariant edge detector.
};

/**
 * Compute smooth edge flow field from the input image.
 * @ingroup npr
 *
 * @details
 * @b Parameters
 * @arg @b img: The input 8-bit integer or floating-point, 1 or 3-channel image.
 * @arg @b angles: The destination vector field; will have floating-point, 1-channel orientation angles.
 * @arg @b ksize: Kernel size for smoothing the vector field
 * @arg @b vfieldType: Algorithm to detect vector field (see <a>@c lime::VecFieldType</a>).
 * @arg @b edgeDetector: Edge detection algorithm (see <a>@c lime::EdgeDetector</a>).
 *
 * @b Python
 *
 * @code{.py}
 * angles = lime.calcVectorField(img, ksize = 5, vftype = lime.VEC_FIELD_SST, edtype = lime.EDGE_DETECT_SOBEL)
 * @endcode
 *
 * @b Parameters
 * @arg @b img - @c numpy.ndarray
 * @arg @b ksize - @c int
 * @arg @b vftype - @c int : Chosen from <a>@c lime::VecFieldType</a>.
 * @arg @b edtype - @c int : Chosen from <a>@c lime::EdgeDetector</a>.
 */
inline void calcVectorField(cv::InputArray input, cv::OutputArray angles, int ksize = 5,
                            int vfieldType = VEC_FIELD_SST, int edgeDetector = EDGE_DETECT_SOBEL);

//! Detect singularity of vector field
inline void detectSingular(const Array2D<Tensor>& sst, std::vector<SingularPoint>* points,
                           std::vector<cv::Point2f>* delauneyNodes);

}  // namespace lime

#include "vector_field_detail.h"

#endif  // _NPR_VECTORFIELD_H_

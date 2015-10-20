/******************************************************************************
Copyright 2015 Tatsuya Yatagawa (tatsy)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

#ifndef SRC_NPR_VECTORFIELD_H_
#define SRC_NPR_VECTORFIELD_H_

#include <opencv2/opencv.hpp>

#include <vector>

#include "../core/point.hpp"
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
inline void detectSingular(const Array2d<Tensor>& sst, std::vector<SingularPoint>* points,
                           std::vector<cv::Point2f>* delauneyNodes);

}  // namespace npr

}  // namespace lime

#include "vector_field_detail.h"

#endif  // SRC_NPR_VECTORFIELD_H_

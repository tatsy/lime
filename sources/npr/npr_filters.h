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

#ifndef SRC_NPR_NPRFILTERS_H_
#define SRC_NPR_NPRFILTERS_H_

#include <opencv2/opencv.hpp>

namespace lime {

namespace npr {

namespace filter {

// solve anistropic diffusion
inline void solveAD(cv::InputArray img, cv::OutputArray out, double lambda, int maxiter);

// solve PDE for shock filter
inline void solveSF(cv::InputArray img, cv::OutputArray out, double lambda, int maxiter);

// solve PDE for mean curvature flow
inline void solveMCF(cv::InputArray img, cv::OutputArray out, double lambda, int maxiter);

// compute erosion of mathematical morphology
inline void morphErode(cv::InputArray img, cv::OutputArray out, int ksize);

// compute dilation of mathematical morphology
inline void morphDilate(cv::InputArray img, cv::OutputArray out, int ksize);

// compute opening of mathematical morphology
inline void morphOpen(cv::InputArray img, cv::OutputArray out, int ksize);

// compute closing of mathematical morphology
inline void morphClose(cv::InputArray img, cv::OutputArray out, int ksize);

// compute gradient of mathematical morphology
inline void morphGradient(cv::InputArray img, cv::OutputArray out, int ksize);

// compute gradient of mathematical morphology
inline void morphTophat(cv::InputArray img, cv::OutputArray out, int ksize);

// compute gradient of mathematical morphology
inline void morphBlackhat(cv::InputArray img, cv::OutputArray out, int ksize);

// normal kuwahara filter
inline void kuwaharaFilter(cv::InputArray img, cv::OutputArray out, int ksize);

// generalized kuwahara filter
inline void generalKF(cv::InputArray img, cv::OutputArray out, int n_div, int ksize);

// anisotropic kuwahara filter
inline void anisoKF(cv::InputArray img, cv::OutputArray out, int n_div, int ksize);

// compute tangent field
inline void calcTangent(cv::InputArray img, cv::OutputArray out, int ksize, int maxiter);

}  // namespace filter

}  // namespace npr

}  // namespace lime

#include "npr_filter_kuwahara_detail.h"
#include "npr_filter_morphology_detail.h"
#include "npr_filter_pdebased_detail.h"

#endif  // SRC_NPR_NPRFILTERS_H_

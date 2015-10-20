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

#ifndef SRC_NPR_LIC_H_
#define SRC_NPR_LIC_H_

#include <cmath>

#include "../core/Point.hpp"

namespace lime {

namespace npr {

enum LicAlgo {
    LIC_CLASSIC = 0x01,
    LIC_EULARIAN,
    LIC_RUNGE_KUTTA
};

/* Visualize a vector field using line integral convolusion (LIC)
* @param[out] out: output image
* @param[in] img: input image to be convoluted
* @param[in] tangent: cv::Mat of CV_32FC2 depth which specifies a tangent direction to each pixel
* @param[in] L: convolution length
* @param[in] algo_type: algorithm type used for LIC
*   algorighms:
*      NPR_LIC_CLASSIC: slow but outputs beautiful vector field
*      NPR_LIC_EULARIAN: fast and stable algorithm (default)
*      NPR_LIC_RUNGE_KUTTA: second-order line integration
*/
inline void lic(cv::OutputArray out, cv::InputArray img,
                const cv::Mat& tangent, int L, LicAlgo algo_type = LIC_EULARIAN);

inline void angle2vector(cv::InputArray angle, cv::OutputArray vfield, double scale = 1.0);

inline void vector2angle(cv::InputArray vfield, cv::OutputArray angle);

}  // namespace npr

}  // namespace lime

#include "../npr/lic_detail.h"

#endif  // SRC_NPR_LIC_H_

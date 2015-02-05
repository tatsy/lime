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

#ifndef SRC_HVS_COLOR_CONSTANCY_H_
#define SRC_HVS_COLOR_CONSTANCY_H_

#include <opencv2/opencv.hpp>

namespace lime {

    inline void colorConstancyHorn(cv::InputArray input, cv::OutputArray output, double thre = 0.05);

    inline void colorConstancyRahman(cv::InputArray input, cv::OutputArray output,
                                     double sigma = 1.0, double scale = 0.16, int nLevel = 3);

    inline void colorConstancyFaugeras(cv::InputArray input, cv::OutputArray output);

}  // namespace lime

#include "../hvs/color_constancy_detail.h"

#endif  // SRC_HVS_COLOR_CONSTANCY_H_

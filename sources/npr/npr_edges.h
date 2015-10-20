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

#ifndef SRC_NPR_NPREDGES_H_
#define SRC_NPR_NPREDGES_H_

#include <opencv2/opencv.hpp>

namespace lime {

namespace npr {

enum DoGType {
    EDGE_XDOG,
    EDGE_FDOG
};

struct DoGParam {
    double kappa;
    double sigma;
    double tau;
    double phi;
    DoGType dogType;

    explicit DoGParam(double kappa = 4.5, double sigma = 0.5, double tau = 0.95,
                      double phi = 10.0, DoGType dogType = EDGE_XDOG);
};  // class DoGParam

inline void edgeDoG(cv::InputArray image, cv::OutputArray edge, const DoGParam& param = DoGParam());

}  // namespace npr

}  // namespace lime

#include "npr_edges_detail.h"

#endif  // SRC_NPR_NPREDGES_H_

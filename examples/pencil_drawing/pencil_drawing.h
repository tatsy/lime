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

#ifndef EXAMPLES_PENCIL_DRAWING_PENCIL_DRAWING_H_
#define EXAMPLES_PENCIL_DRAWING_PENCIL_DRAWING_H_

#include <opencv2/opencv.hpp>

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include "../../include/lime.hpp"

/* Line integral convolution */
void LIConv(cv::OutputArray lic, cv::InputArray img, cv::InputArray etf, cv::InputArray noise, double ratio);

/* Generate pencil drawing */
void pencilDrawing(cv::InputArray img, cv::OutputArray out, const std::vector<cv::Point2f>& points);

/* Generate pencil drawing in LOD basis */
void pencilDrawingLOD(cv::InputArray img, cv::OutputArray out, const std::vector<cv::Point2f>& points, int level);

#endif  // EXAMPLES_PENCIL_DRAWING_PENCIL_DRAWING_H_

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

#ifndef SRC_NPR_NOISE_H_
#define SRC_NPR_NOISE_H_

#include <opencv2/opencv.hpp>

#include <vector>

#include "../core/Point.hpp"

namespace lime {

namespace npr {

namespace noise {

    /* Generate random noise
        * @param[out] noise: generated noise image (CV_32FC1 format)
        * @param[in] size: size of the noise image to be returned
        */
    inline void random(cv::OutputArray noise, const cv::Size& size);

    /* Generate Perlin noise
        * @param[out] noise: generated noise image (CV_32FC1 format)
        * @param[in] size: size of the noise image to be returned
        * @param[in] level: maximum depth level to generate the Perlin noise
        */
    inline void perlin(cv::OutputArray noise, const cv::Size& size, int level);

}  // namespace noise

}  // namespace npr

}  // namespace lime

#include "Noise_detail.h"

#endif  // SRC_NPR_NOISE_H_

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

#ifndef SRC_NPR_POISSONDISK_H_
#define SRC_NPR_POISSONDISK_H_

#include <opencv2/opencv.hpp>

#include <vector>

#include "../core/Point.hpp"

namespace lime {

namespace npr {

enum PdsMethod {
    PDS_RAND_QUEUE,
    PDS_FAST_PARALLEL
};

/* Generate sample points using Poisson disk sampling. If 'samplePoints' already has same items, they are used for initial samples.
    * @param[in] grayImage: a grayscale image which specifies the density of sample points
    * @param[in/out] samplePoints: sample points to be returned (pointer)
    * @param[in] pdsMethod: a method for Poisson disk sampling
    * @param[in] minRadius: minimum radius of Poisson disk
    * @param[in] maxRadius: maximum radius of Poisson disk
    */
inline void poissonDisk(cv::InputArray grayImage, std::vector<cv::Point2f>* samplePoints,
                        PdsMethod pdsMethod = PDS_FAST_PARALLEL, double minRadius = 2.0, double maxRadius = 5.0);

}  // namespace npr

}  // namespace lime

#include "poisson_disk_detail.h"

#endif  // SRC_NPR_POISSONDISK_H_

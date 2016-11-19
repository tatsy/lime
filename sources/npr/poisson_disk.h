#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_POISSONDISK_H_
#define _NPR_POISSONDISK_H_

#include <opencv2/opencv.hpp>

#include <vector>

#include "../core/point.h"

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

#ifndef LIME_USE_STATIC_LIB
#include "poisson_disk_detail.h"
#endif

#endif  // _NPR_POISSONDISK_H_

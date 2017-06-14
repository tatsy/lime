#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_POISSONDISK_H_
#define _NPR_POISSONDISK_H_

#include <opencv2/opencv.hpp>

#include <vector>

#include "../core/point.h"

namespace lime {

enum PDSMethod {
    PDS_RAND_QUEUE,
    PDS_FAST_PARALLEL
};

/**
 * Generate sample points using Poisson disk sampling.
 * If <b>samplePoints</b> already has same items, they are used for initial samples.
 * @arg @b grayImage: a grayscale image which specifies the density of sample points
 * @arg @b samplePoints: sample points to be returned (pointer)
 * @arg @b pdsMethod: a method for Poisson disk sampling
 * @arg @b minRadius: minimum radius of Poisson disk
 * @arg @b maxRadius: maximum radius of Poisson disk
 */
inline void poissonDisk(cv::InputArray grayImage, std::vector<cv::Point2f>* samplePoints,
                        PDSMethod pdsMethod = PDS_FAST_PARALLEL,
                        double minRadius = 2.0, double maxRadius = 5.0);

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "poisson_disk_detail.h"
#endif

#endif  // _NPR_POISSONDISK_H_

#ifndef _MSC_VER
#pragma once
#endif

#ifndef _METRICS_BASICS_H_
#define _METRICS_BASICS_H_

#include "core/common.h"

#include <opencv2/opencv.hpp>

namespace lime {

/**
 * Calculate mean squared error (MSE).
 * @ingroup metrics
 * 
 * @details
 * @b Parameters
 * @arg @b I: The first input image.
 * @arg @b J: The second input image.
 *
 * @b Python
 * @code{.py}
 * score = lime.PSNR(I, J)
 * @endcode
 *
 * @b Parameters
 * @arg @b I - @c numpy.ndarray : The first input image.
 * @arg @b J - @c numpy.ndarray : The second input image.
 * @b Returns
 * @arg @b score - @c float : The value of MSE.
 */
LIME_METHOD_API double MSE(cv::InputArray I, cv::InputArray J);

/**
 * Calculate root mean squared error (RMSE).
 * @ingroup metrics
 * 
 * @details
 * @b Parameters
 * @arg @b I: The first input image.
 * @arg @b J: The second input image.
 *
 * @b Python
 * @code{.py}
 * score = lime.PSNR(I, J)
 * @endcode
 *
 * @b Parameters
 * @arg @b I - @c numpy.ndarray : The first input image.
 * @arg @b J - @c numpy.ndarray : The second input image.
 * @b Returns
 * @arg @b score - @c float : The value of RMSE.
 */
LIME_METHOD_API double RMSE(cv::InputArray I, cv::InputArray J);

/**
 * Calculate peak signal to noise ratio (PSNR).
 * @ingroup metrics
 * 
 * @details
 * @b Parameters
 * @arg @b I: The first input image.
 * @arg @b J: The second input image.
 *
 * @b Python
 * @code{.py}
 * score = lime.PSNR(I, J)
 * @endcode
 *
 * @b Parameters
 * @arg @b I - @c numpy.ndarray : The first input image.
 * @arg @b J - @c numpy.ndarray : The second input image.
 * @b Returns
 * @arg @b score - @c float : The value of PSNR.
 */
LIME_METHOD_API double PSNR(cv::InputArray I, cv::InputArray J);

/**
 * Calculate structural similarity (SSIM).
 * @ingroup metrics
 * 
 * @details
 * @b Parameters
 * @arg @b I: The first input image.
 * @arg @b J: The second input image.
 *
 * @b Python
 * @code{.py}
 * score = lime.PSNR(I, J)
 * @endcode
 *
 * @b Parameters
 * @arg @b I - @c numpy.ndarray : The first input image.
 * @arg @b J - @c numpy.ndarray : The second input image.
 * @b Returns
 * @arg @b score - @c float : The value of SSIM.
 */
LIME_METHOD_API double SSIM(cv::InputArray I, cv::InputArray J);

/**
 * Calculate multi-scale structural similarity (MS-SSIM).
 * This method is based on the following paper.
 * Wang et al., 2003, "Multi-scale Structural Similarity for Image Quaility Assessment"
 * @ingroup metrics
 * 
 * @details
 * @b Parameters
 * @arg @b I: The first input image.
 * @arg @b J: The second input image.
 *
 * @b Python
 * @code{.py}
 * score = lime.PSNR(I, J)
 * @endcode
 *
 * @b Parameters
 * @arg @b I - @c numpy.ndarray : The first input image.
 * @arg @b J - @c numpy.ndarray : The second input image.
 * @b Returns
 * @arg @b score - @c float : The value of MS-SSIM.
 */
LIME_METHOD_API double MSSSIM(cv::InputArray I, cv::InputArray J);

/**
 * Calculate color multi-scale structural similarity (CM-SSIM).
 * This method is based on the following paper.
 * Hassan and Bhagvati 2012, "Structural Similarity Measure for Color Images"
 * @ingroup metrics
 * 
 * @details
 * @b Parameters
 * @arg @b I: The first input image.
 * @arg @b J: The second input image.
 *
 * @b Python
 * @code{.py}
 * score = lime.PSNR(I, J)
 * @endcode
 *
 * @b Parameters
 * @arg @b I - @c numpy.ndarray : The first input image.
 * @arg @b J - @c numpy.ndarray : The second input image.
 * @b Returns
 * @arg @b score - @c float : The value of CM-SSIM.
 */
LIME_METHOD_API double CMSSIM(cv::InputArray I, cv::InputArray J);

}  // namespace lime

#ifndef LIME_USE_LIBRARY
#include "basics_detail.h"
#endif

#endif  // _METRICS_BASICS_H_

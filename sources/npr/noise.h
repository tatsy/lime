#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_NOISE_H_
#define _NPR_NOISE_H_

#include <opencv2/opencv.hpp>

#include <vector>

#include "core/common.h"
#include "core/point.h"

namespace lime {

/**
 * Generate random noise. Each pixel value takes random number between \f$[0, 1]\f$.
 * @ingroup npr
 *
 * @details
 * @b Parameters
 * @arg @b noise: Generated noise image; will have floating-point, 1-channel pixel values.
 * @arg @b size: Size of the noise image.
 *
 * @b Python
 *
 * @code{.py}
 * noise = lime.randomNoise(size)
 * @endcode
 *
 * @b Parameters
 * @arg @b size - @c tuple of @c int : Noise image size.
 *
 * @b Returns
 * @arg @b noise - @c numpy.ndarray : The floating-point, 1-channel noise image.
 */
LIME_METHOD_API void randomNoise(cv::OutputArray noise, const cv::Size& size);

/**
 * Generate uniform noise such that its density is proportional to the grayscale image intensity.
 * @ingroup npr
 *
 * @details
 * @b Parameters
 * @arg @b noise: Generated noise image; will have floating-point, 1-channel pixel values.
 * @arg @b gray: Grayscale image to represent noise density.
 * @arg @b nNoise: Number of noise samples.
 *
 * @code{.py}
 * noise = lime.densityNoise(gray, noises)
 * @endcode
 *
 * @b Parameters
 * @arg @b gray - @c numpy.ndarray : The floating-point 1-channel image.
 * @arg @noises - @c int : Number of noise samples.
 *
 * @b Returns
 * @arg @b noise - @c numpy.ndarray : The floating-point 1-channel noise image.
 */
LIME_METHOD_API void densityNoise(cv::OutputArray noise, cv::InputArray gray, int nNoise);

/**
 * Generate Perlin noise.
 * @ingroup npr
 *
 * @details
 * @b Parameters
 * @arg @b noise: Generated noise image; will have floating-point, 1-channel pixel values.
 * @arg @b size: Size of the noise image.
 * @arg @b level: Maximum depth level used to generate the Perlin noise.
 *
 * @b Python
 *
 * @code{.py}
 * noise = lime.perlinNoise(size, level)
 * @endcode
 *
 * @b Parameters
 * @arg @b size - @c tuple of @c int : Noise image size.
 * @arg @b level - @c int
 *
 * @b Returns
 * @arg @b noise - @c numpy.ndarray : The floating-point, 1-channel noise image.
 */
LIME_METHOD_API void perlinNoise(cv::OutputArray noise, const cv::Size& size, int level);

}  // namespace lime

#ifndef LIME_USE_LIBRARY
#include "noise_detail.h"
#endif

#endif  // SRC_NPR_NOISE_H_

#ifndef _NPR_NOISE_H_
#define _NPR_NOISE_H_

#include <vector>

#include <opencv2/opencv.hpp>
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

		} /* namespace noise */

	} /* namespace npr */

} /* namespace lime */

#include "Noise_detail.h"

#endif /* _NPR_NOISE_H_ */

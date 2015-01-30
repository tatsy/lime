#ifndef _NPR_POISSON_DISK_H_
#define _NPR_POISSON_DISK_H_

#include <opencv2/opencv.hpp>
#include "../core/Point.hpp"

namespace lime {
	namespace npr {

		enum PdsMethod {
			PDS_RAND_QUEUE,
			PDS_FAST_PARALLEL
		};

		/* Generate sample points using Poisson disk sampling. If 'samplePoints' already has same items, they are used for initial samples.
		 * @param[in] grayImage: a grayscale image which specifies the density of sample points
		 * @param[in/out] samplePoints: sample points to be returned
		 * @param[in] pdsMethod: a method for Poisson disk sampling
		 * @param[in] minRadius: minimum radius of Poisson disk
		 * @param[in] maxRadius: maximum radius of Poisson disk
		 */
		inline void poissonDisk(cv::InputArray grayImage, std::vector<cv::Point2f>& samplePoints, PdsMethod pdsMethod = PDS_FAST_PARALLEL, double minRadius = 2.0, double maxRadius = 5.0);

	} /* namespace npr */

} /* namespace lime */

#include "PoissonDisk_detail.h"
 
#endif /* _NPR_POISSON_DISK_H_ */

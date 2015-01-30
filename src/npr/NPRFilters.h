#ifndef _NPR_FILTERS_H_
#define _NPR_FILTERS_H_

#include <opencv2/opencv.hpp>

namespace lime {

	namespace npr {

		namespace filter {

			// solve anistropic diffusion
			inline void solveAD(cv::InputArray img, cv::OutputArray out, double lambda, int maxiter);

			// solve PDE for shock filter
			inline void solveSF(cv::InputArray img, cv::OutputArray out, double lambda, int maxiter);

			// solve PDE for mean curvature flow
			inline void solveMCF(cv::InputArray img, cv::OutputArray out, double lambda, int maxiter);

			// compute erosion of mathematical morphology
			inline void morphErode(cv::InputArray img, cv::OutputArray out, int ksize);

			// compute dilation of mathematical morphology
			inline void morphDilate(cv::InputArray img, cv::OutputArray out, int ksize);

			// compute opening of mathematical morphology
			inline void morphOpen(cv::InputArray img, cv::OutputArray out, int ksize);

			// compute closing of mathematical morphology
			inline void morphClose(cv::InputArray img, cv::OutputArray out, int ksize);

			// compute gradient of mathematical morphology
			inline void morphGradient(cv::InputArray img, cv::OutputArray out, int ksize);

			// compute gradient of mathematical morphology
			inline void morphTophat(cv::InputArray img, cv::OutputArray out, int ksize);

			// compute gradient of mathematical morphology
			inline void morphBlackhat(cv::InputArray img, cv::OutputArray out, int ksize);

			// normal kuwahara filter
			inline void kuwaharaFilter(cv::InputArray img, cv::OutputArray out, int ksize);

			// generalized kuwahara filter
			inline void generalKF(cv::InputArray img, cv::OutputArray out, int n_div, int ksize);

			// anisotropic kuwahara filter
			inline void anisoKF(cv::InputArray img, cv::OutputArray out, int n_div, int ksize);

			// compute tangent field
			inline void calcTangent(cv::InputArray img, cv::OutputArray out, int ksize, int maxiter);

		} /* namespace filter */

	} /* namespace npr */

} /* namespace lime */

#include "NPRFilter_Kuwahara_detail.h"
#include "NPRFilter_Morphology_detail.h"
#include "NPRFilter_PDEBased_detail.h"

#endif /* end of _NPR_FILTERS_H_ */

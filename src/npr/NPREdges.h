#ifndef _NPR_EDGES_H_
#define _NPR_EDGES_H_

#include <opencv2/opencv.hpp>

namespace lime {

	namespace npr {

		enum DoGType {
			EDGE_XDOG,
			EDGE_FDOG
		};

		struct DoGParam {
			double kappa;
			double sigma;
			double tau;
			double phi;
			DoGType dogType;

			DoGParam(double kappa = 4.5, double sigma = 0.5, double tau = 0.95, double phi = 10.0, DoGType dogType = EDGE_XDOG);

		}; /* class DoGParam */

		inline void edgeDoG(cv::InputArray image, cv::OutputArray edge, const DoGParam& param = DoGParam());

	} /* namespace npr */

} /* namespace lime */

#include "NPREdges_detail.h"

#endif /* _NPR_EDGES_H_ */

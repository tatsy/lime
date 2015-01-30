#ifndef _NPR_VECTOR_FIELD_H_
#define _NPR_VECTOR_FIELD_H_

#include <vector>
#include <opencv2/opencv.hpp>

#include "../core/Point.hpp"
#include "../core/Array2d.h"
#include "Tensor.hpp"
#include "Singularity.hpp"

namespace lime {

	namespace npr {

		enum VFieldType {
			VECTOR_SST,
			VECTOR_ETF
		};

		enum EdgeDetector {
			EDGE_SOBEL,
			EDGE_ROTATIONAL
		};

		/* Compute vector field
		 * @param[in] img: input image from which a vector field is computed
		 * @param[out] angles: output array of CV_32FC1 depth which stores tangent direction of vectors
		 * @param[in] ksize: kernel size for smoothing the vector field
		 * @param[in] vfieldType: algorithm to detect vector field (SST or ETF)
		 * @param[in] edgeDetector: edge detection algorithm
		 */
		inline void calcVectorField(cv::InputArray input, cv::OutputArray angles, int ksize = 5, VFieldType vfieldType = VECTOR_SST, EdgeDetector edgeDetector = EDGE_SOBEL);

		// * detect singularity of vector field
		inline void detectSingular(Array2d<Tensor>& sst, std::vector<SingularPoint>& points, std::vector<cv::Point2f>& delauneyNodes = std::vector<cv::Point2f>());

	} /* namespace npr */

} /* namespace lime */

#include "VectorField_detail.h"

#endif /* end of _NPR_VECTOR_FIELD_H_ */

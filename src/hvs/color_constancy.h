#ifndef _LIME_COLOR_CONSTANCY_H_
#define _LIME_COLOR_CONSTANCY_H_

#include <opencv2/opencv.hpp>

namespace lime {

    inline void colorConstancyHorn(cv::InputArray input, cv::OutputArray output, double thre = 0.05);

    inline void colorConstancyRahman(cv::InputArray input, cv::OutputArray output, double sigma = 1.0, double scale = 0.16, int nLevel = 3);

    inline void colorConstancyFaugeras(cv::InputArray input, cv::OutputArray output);

} /* namespace lime */

#include "color_constancy_detail.h"

#endif /* _LIME_COLOR_CONSTANCY_H_ */

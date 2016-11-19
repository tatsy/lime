#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_LIC_H_
#define _NPR_LIC_H_

#include <cmath>

#include "../core/point.h"

namespace lime {

    namespace npr {

        enum class LICType : int {
            Classic,
            Eulerian,
            RungeKutta
        };

        /** Visualize a vector field using line integral convolusion (LIC).
         *  @param[out] out: output image
         *  @param[in] img: input image to be convoluted
         *  @param[in] tangent: cv::Mat of CV_32FC2 depth which specifies a tangent direction to each pixel
         *  @param[in] L: convolution length
         *  @param[in] algo_type: algorithm type used for LIC
         *  @details algorighms:
         *      NPR_LIC_CLASSIC: slow but outputs beautiful vector field
         *      NPR_LIC_EULARIAN: fast and stable algorithm (default)
         *      NPR_LIC_RUNGE_KUTTA: second-order line integration
         */
        inline void lic(cv::OutputArray out, cv::InputArray img,
                        const cv::Mat& tangent, int L, LICType licType = LICType::Eulerian);

        inline void angle2vector(cv::InputArray angle, cv::OutputArray vfield, double scale = 1.0);

        inline void vector2angle(cv::InputArray vfield, cv::OutputArray angle);

        }  // namespace npr

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "../npr/lic_detail.h"
#endif

#endif  // _NPR_LIC_H_

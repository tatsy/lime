#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_KUWAHARA_H_
#define _NPR_KUWAHARA_H_

namespace lime {

//! normal kuwahara filter
inline void kuwaharaFilter(cv::InputArray img, cv::OutputArray out, int ksize);

//! generalized kuwahara filter
inline void generalKF(cv::InputArray img, cv::OutputArray out, int n_div, int ksize);

//! anisotropic kuwahara filter
inline void anisoKF(cv::InputArray img, cv::OutputArray out, int n_div, int ksize);

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "kuwahara_detail.h"
#endif

#endif

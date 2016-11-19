#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_MORPHOLOGY_H_
#define _NPR_MORPHOLOGY_H_

namespace lime {

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

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "morphology_detail.h"
#endif

#endif  // _NPR_MORPHOLOGY_H_

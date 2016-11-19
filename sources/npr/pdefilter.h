#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_PDEFILTER_H_
#define _NPR_PDEFILTER_H_

namespace lime {

// solve anistropic diffusion
inline void solveAD(cv::InputArray img, cv::OutputArray out, double lambda, int maxiter);

// solve PDE for shock filter
inline void solveSF(cv::InputArray img, cv::OutputArray out, double lambda, int maxiter);

// solve PDE for mean curvature flow
inline void solveMCF(cv::InputArray img, cv::OutputArray out, double lambda, int maxiter);

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "pdefilter_detail.h"
#endif

#endif  // _NPR_PDEFILTER_H_

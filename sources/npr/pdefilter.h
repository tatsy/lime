#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_PDEFILTER_H_
#define _NPR_PDEFILTER_H_

namespace lime {

/**
 * PDE-based filter types.
 * @ingroup npr
 **/
enum PDETypes : int {
    PDE_ANISO_DIFFSION,     //!< Anisotropic diffusion.
    PDE_SHOCK_FILTER,       //!< Shock filter.
    PDE_MEAN_CURVATURE,     //!< Mean curvature flow.
};

/**
 * PDE-based filter (PDE = partial differential equation).
 * @ingroup npr
 * @param[in] src: Input image.
 * @param[out] dst: Output image.
 * @param[in] type: PDETypes enum.
 * @param[in] lambda: Filter strength for one iteration.
 * @param[in] maxiter: # of iteration for solving PDEs.
 *
 * @details
 * @code
 * # Python
 * dst = lime.pdeFilter(src, type, lambda, maxiter)
 * @endcode
 **/
inline void pdeFilter(cv::InputArray src, cv::OutputArray dst, int type, double lambda, int maxiter);

// solve anistropic diffusion
inline void anisoDiffusion(cv::InputArray img, cv::OutputArray out, double lambda, int maxiter);

// solve PDE for shock filter
inline void shockFilter(cv::InputArray img, cv::OutputArray out, double lambda, int maxiter);

// solve PDE for mean curvature flow
inline void meanCurveFlow(cv::InputArray img, cv::OutputArray out, double lambda, int maxiter);

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "pdefilter_detail.h"
#endif

#endif  // _NPR_PDEFILTER_H_

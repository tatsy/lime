#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_PDEFILTER_H_
#define _NPR_PDEFILTER_H_

#include "core/common.h"

namespace lime {

/**
 * PDE-based filter types.
 * @ingroup npr
 **/
enum PDEType : int {
    PDE_ANISO_DIFFUSION,     //!< Anisotropic diffusion.
    PDE_SHOCK_FILTER,       //!< Shock filter.
    PDE_MEAN_CURVATURE,     //!< Mean curvature flow.
};

/**
 * Apply PDE-based filtering (PDE = pertial differential equation).
 * @ingroup npr
 *
 * @details
 * @b Parameters
 * @arg @b src: The input floating-point, 1 or 3-channel image.
 * @arg @b dst: The destination image; will have the save type as <b>src</b>.
 * @arg @b type: Filter type (see <a>@c lime::PDEType</a>).
 * @arg @b lambda: Filter strength for one iteration.
 * @arg @b maxiter: # of iteration for solving the PDE.
 *
 * @b Python
 *
 * @code{.py}
 * # Python
 * dst = lime.pdeFilter(src, ftype, lambda, maxiter)
 * @endcode
 *
 * @b Parameters
 * @arg @b src - @c numpy.ndarray : The floating-point, 1 or 3-channel image.
 * @arg @b ftype - @c int : Filter type (see <a>@c lime::PDEType</a>).
 * @arg @b lamdba - @c double
 * @arg @b maxiter - @c int
 *
 * @b Returns
 * @arg @b dst - @c numpy.ndarray : The destination image with the same type as <b>src</b>.
 */
LIME_METHOD_API void pdeFilter(cv::InputArray src, cv::OutputArray dst, int type, double lambda, int maxiter);

// Solve anistropic diffusion
LIME_METHOD_API void anisoDiffusion(cv::InputArray img, cv::OutputArray out, double lambda, int maxiter);

// Solve PDE for shock filter
LIME_METHOD_API void shockFilter(cv::InputArray img, cv::OutputArray out, double lambda, int maxiter);

// Solve PDE for mean curvature flow
LIME_METHOD_API void meanCurveFlow(cv::InputArray img, cv::OutputArray out, double lambda, int maxiter);

}  // namespace lime

#ifndef LIME_USE_LIBRARY
#include "pdefilter_detail.h"
#endif

#endif  // _NPR_PDEFILTER_H_

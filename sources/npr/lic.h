#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_LIC_H_
#define _NPR_LIC_H_

#include <cmath>

#include "core/common.h"
#include "core/point.h"

namespace lime {

/**
 * LIC algorithm types, which represent methods to generate integration paths.
 * @ingroup npr
 */
enum LICType : int {
    LIC_CLASSICAL,      //!< Slow but outputs beautiful vector fields.
    LIC_EULERIAN,       //!< Fast and stable algorithm.
    LIC_RUNGE_KUTTA,    //!< Second-order line integration.
};

/**
 * Integrate the pixel values along flow field.
 * @ingroup npr
 *
 * @details
 * @b Parameters
 * @arg @b src: The source 8-bit or floating-point, 1-channel or 3-channel image.
 * @arg @b dst: The destination image; will have the same size and the same type as <b>src</b>.
 * @arg @b tangent: The floating-point, 2-channel image storing tangent directions of pixels.
 * @arg @b L: Convolution length.
 * @arg @b licType: The algorithm used for the convolution (see <a>@c lime::LICType</a>).
 *
 * @b Python
 *
 * @code{.py}
 * dst = lime.LIC(src, L, lictype = lime.LIC_EULERIAN)
 * @endcode
 *
 * @b Parameters
 * @arg @b src - <tt>numpy.ndarray</tt> : 1 or 3-channel, floating-point image.
 * @arg @b L - <tt>int</tt>
 * @arg @b lictype - <tt>int</tt> : Chosen from <a>@c lime::LICType</a>
 *
 * @b Returns
 * @arg @b dst - <tt>numpy.ndarray</tt> : The same type with <b>src</b>.
 */
LIME_METHOD_API void LIC(cv::InputArray src, cv::OutputArray dst,
                         const cv::Mat& tangent, int L, int licType = LIC_EULERIAN);

/**
 * Converts flow orientation angle to vector field.
 * @ingroup npr
 *
 * @details
 * @b Parameters
 * @arg @b angle: The floating-point, 1-channel image storing orientation angle in \f$[0, 2\pi\f$].
 * @arg @b vfield: The destination image; will have floating-point, 2-channel normalized vector values.
 * 
 * @b Python
 * This method is equivalent to following.
 *
 * @code{.py}
 * vfield = np.ndarray((angle.shape[0], angle.shape[1], 2), dtype=np.float32)
 * vfield[:,:,0] = np.cos(angle) * scale
 * vfield[:,:,1] = np.sin(angle) * scale
 * @endcode
 */
LIME_METHOD_API void angle2vector(cv::InputArray angle, cv::OutputArray vfield, double scale = 1.0);

/**
 * Converts vector field to orientation angles.
 * @ingroup npr
 *
 * @details
 * @b Parameters
 * @arg @b vfield: The floating-point, 2-channel image storing normalized vector values.
 * @arg @b angle: The destiation image; will have floting-point, 1-channel image storing orientation angles.
 *
 * @b Python
 * This method is equivalent to following.
 *
 * @code{.py}
 * angle = np.arctan(vfield[:,:,1], vfield[:,:,0])
 * @endcode
 */
LIME_METHOD_API void vector2angle(cv::InputArray vfield, cv::OutputArray angle);

}  // namespace lime

#ifndef LIME_USE_LIBRARY
#include "../npr/lic_detail.h"
#endif

#endif  // _NPR_LIC_H_

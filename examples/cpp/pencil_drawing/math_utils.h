#ifdef _MSC_VER
#pragma once
#endif

#ifndef _PENCIL_DRAWING_MATH_UTILS_H_
#define _PENCIL_DRAWING_MATH_UTILS_H_

inline double fast_exp(double y) {
    double d;
    *(reinterpret_cast<int*>(&d) + 0) = 0;
    *(reinterpret_cast<int*>(&d) + 1) = static_cast<int>(1512775 * y + 1072632447);
    return d;
}

inline double gauss(double x, double sigma) {
    double ratio = -(x * x) / sigma;
    return fast_exp(ratio);
}

#endif /* EXAMPLES_PENCIL_DRAWING_MATH_UTILS_H_ */

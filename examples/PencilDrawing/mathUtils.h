#pragma once
#ifndef _MATH_UTILS_H_
#define _MATH_UTILS_H_

inline double fast_exp(double y) { 
	double d;
	*((int*)(&d) + 0) = 0;
	*((int*)(&d) + 1) = (int)(1512775 * y + 1072632447);
	return d;
} 

inline double gauss(double x, double sigma) {
	double ratio = - (x * x) / sigma;
	return fast_exp(ratio);
}

#endif

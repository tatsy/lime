#ifndef _LIME_COMMON_H_
#define _LIME_COMMON_H_

#include <cmath>
#include <iostream>

static const double PI = 4.0 * atan(1.0);

#ifdef _OPENMP
#include <omp.h>
#define ompfor __pragma(omp parallel for) for
#else /* _OPENMP */
#define ompfor for
#endif /* _OPENMP */

#ifndef NDEBUG
#define msg_assert(PREDICATE, MSG) \
do { \
	if (!(PREDICATE)) { \
		std::cerr << "Asssertion \"" << #PREDICATE << "\" failed in " << __FILE__ \
		<< " line " << __LINE__ << " : " << MSG << std::endl; \
		std::abort(); \
	} \
} while (false)
#else /* NDEBUG */
#define msg_assert(PREDICATE, MSG) do {} while (false)
#endif /* NDEBUG */

#endif /* _LIME_COMMON_H_ */

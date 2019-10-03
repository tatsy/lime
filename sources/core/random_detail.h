#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CORE_RANDOM_DETAIL_H_
#define _CORE_RANDOM_DETAIL_H_

#include <cmath>
#include <ctime>

#include "common.h"

namespace lime {

LIME_METHOD_API Random::Random(uint32_t seed)
    : mt(seed == UINT_MAX ? std::random_device()() : seed)
{
}

LIME_METHOD_API int Random::nextInt() {
    return std::uniform_int_distribution<int>()(mt);
}

LIME_METHOD_API int Random::nextInt(const int n) {
    Assertion(n > 0, "Upper bound of random integers must be positive.");
    return std::uniform_int_distribution<int>(0, n - 1)(mt);
}

LIME_METHOD_API double Random::nextReal() {
    return std::uniform_real_distribution<double>()(mt);
}

LIME_METHOD_API double Random::normal() {
    const double r1 = nextReal();
    const double r2 = nextReal();
    return sqrt(-2.0 * std::log(r1)) * std::sin(2.0 * PI * r2);
}

}  // namespace lime

#endif  // _CORE_RANDOM_DETAIL_H_

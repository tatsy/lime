#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CORE_RANDOM_H_
#define _CORE_RANDOM_H_

#include <random>

#include "common.h"

namespace lime {

/**
 * Random number generator using Mersenne-twister.
 * @ingroup core
 **/
class LIME_API Random {
public:
    explicit Random(uint32_t seed = UINT_MAX);

    //! Generate a random 32-bit signed integer.
    int nextInt();

    //! Generate a random integer from [0, n-1].
    int nextInt(const int n);

    //! Generate a floating point random number from [0, 1).
    double nextReal();

    //! Generate a random number from normal distribution.
    double normal();

private:
    // Private methods
    std::mt19937 mt;

};  // class Random

}  // namespace lime

#ifndef LIME_USE_LIBRARY
#include "random_detail.h"
#endif

#endif  // _CORE_RANDOM_H_

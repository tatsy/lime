#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CORE_RANDOM_H_
#define _CORE_RANDOM_H_

#include "common.h"

namespace lime {

/**
 * Random number generator using Mersenne-twister.
 * @ingroup core
 **/
class LIME_EXPORTS Random {
public:
    explicit Random(unsigned int seed = 0);

    int nextInt();

    //! Generate a random integer from [0, n-1].
    int nextInt(const int n);

    //! Generate a floating point random number from [0, 1).
    double nextReal();

    //! Generate a random number from normal distribution.
    double normal();

private:
    // Private methods
    void init_genrand(unsigned int s);
    unsigned int genrand_int32(void);
    int genrand_int31(void);
    double genrand_real2(void);

    // Private parameters
    static const int N = 624;
    static const int M = 397;

    unsigned int mt[N];
    int mti;

};  // class Random

}  // namespace lime

#ifndef LIME_USE_STATIC_LIB
#include "random_detail.h"
#endif

#endif  // _CORE_RANDOM_H_

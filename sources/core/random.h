#ifdef _MSC_VER
#pragma once
#endif

#ifndef _CORE_RANDOM_H_
#define _CORE_RANDOM_H_

namespace lime {

    /**
     * A singleton class for generating random numbers.
     */
    class Random {
    public:
        static Random& getRNG(int seed = -1);

    private:
        explicit Random(int seed = -1);
        Random& operator=(const Random&) = delete;

    public:
        /** Generate a random integer from [0, n-1]
         */
        int randInt(const int n) const;

        /** Generate a floating point random number from [0, 1)
         */
        double randReal() const;

        /** Genrate a random number from a normal distribution with mean = 0 and STD = 1
         */
        double randNorm() const;

    };  // class Random

}  // namespace lime

#include "random_detail.h"

#endif  // _CORE_RANDOM_H_

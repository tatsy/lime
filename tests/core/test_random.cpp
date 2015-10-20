/******************************************************************************
Copyright 2015 Tatsuya Yatagawa (tatsy)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

#include "gtest/gtest.h"

#include "../../include/lime.hpp"

using lime::Random;

static const int nTest = 100;
static const int nLoop = 50000;
Random rng = Random::getRNG();

TEST(Random, RandInt) {
    int nBins = 10;
    int *countUp = new int[10];
    memset(countUp, 0, sizeof(int) * nBins);
    for (int i = 0; i < nLoop; i++) {
        countUp[rng.randInt(nBins)]++;
    }

    for (int i = 0; i < nBins; i++) {
        EXPECT_LT(abs(countUp[i] - nLoop / nBins), nLoop / (nBins * nBins));
    }

    delete[] countUp;
}

TEST(Random, RandReal) {
    for (int t = 0; t < nTest; t++) {
        double accum = 0.0;
        for (int i = 0; i < nLoop; i++) {
            double rx = rng.randReal();
            double ry = rng.randReal();
            if (hypot(rx, ry) <= 1.0) {
                accum += 1.0;
            }
        }
        ASSERT_NEAR(accum / nLoop, PI / 4.0, 1.0e-2);
    }
}

TEST(Random, RandNorm) {
    for (int t = 0; t < nTest; t++) {
        double avg = 0.0;
        double var = 0.0;
        for (int i = 0; i < nLoop; i++) {
            double r = rng.randNorm();
            avg += r;
            var += r * r;
        }
        avg = avg / nLoop;
        var = var / nLoop - avg * avg;
        ASSERT_NEAR(avg, 0.0, 0.05);
        ASSERT_NEAR(var, 1.0, 0.05);
    }
}

#include "gtest/gtest.h"

#include "lime.hpp"

using lime::Random;

static const int nTest = 100;
static const int nLoop = 50000;
Random rng = Random((unsigned int)time(0));

TEST(Random, RandInt) {
    int nBins = 10;
    int *countUp = new int[10];
    memset(countUp, 0, sizeof(int) * nBins);
    for (int i = 0; i < nLoop; i++) {
        countUp[rng.nextInt(nBins)]++;
    }

    for (int i = 0; i < nBins; i++) {
        EXPECT_LT(std::abs(countUp[i] - nLoop / nBins), nLoop / (nBins * nBins));
    }

    delete[] countUp;
}

TEST(Random, RandReal) {
    for (int t = 0; t < nTest; t++) {
        double accum = 0.0;
        for (int i = 0; i < nLoop; i++) {
            double rx = rng.nextReal();
            double ry = rng.nextReal();
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
            double r = rng.normal();
            avg += r;
            var += r * r;
        }
        avg = avg / nLoop;
        var = var / nLoop - avg * avg;
        ASSERT_NEAR(avg, 0.0, 0.05);
        ASSERT_NEAR(var, 1.0, 0.05);
    }
}

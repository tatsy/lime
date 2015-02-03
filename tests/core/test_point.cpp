/*
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
*/

#include "gtest/gtest.h"

#include "../../include/lime.hpp"
using lime::Point2d;

const int nSimpleLoop = 100;
lime::Random rng = lime::Random::getRNG();

TEST(Point2d, DefaultConstructor) {
    const Point2d p;
    EXPECT_EQ(p.x, 0.0);
    EXPECT_EQ(p.y, 0.0);
}

TEST(Point2d, AddAndSubtract) {
    for (int i = 0; i < nSimpleLoop; i++) {
        const Point2d p(rng.randReal(), rng.randReal());
        const Point2d q(rng.randReal(), rng.randReal());
        EXPECT_EQ(p.x + q.x, (p + q).x);
        EXPECT_EQ(p.y + q.y, (p + q).y);
        EXPECT_EQ(p.x - q.x, (p - q).x);
        EXPECT_EQ(p.y - q.y, (p - q).y);
    }
}

TEST(Point2D, MultiplyAndDiv) {
    for (int i = 0; i < nSimpleLoop; i++) {
        const Point2d p(rng.randReal(), rng.randReal());
        const double d = rng.randReal();
        EXPECT_EQ(p.x * d, (p * d).x);
        EXPECT_EQ(p.y * d, (p * d).y);
        EXPECT_EQ(p.x / d, (p / d).x);
        EXPECT_EQ(p.y / d, (p / d).y);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

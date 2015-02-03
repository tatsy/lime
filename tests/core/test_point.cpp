/*
Copyright [2015] <tatsuya yatagawa (tatsy)>

permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files(the "software"), to deal in
the software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of
the software, and to permit persons to whom the software is furnished to do so,
subject to the following conditions :

the above copyright notice and this permission notice shall be included in all
copies or substantial portions of the software.
the software is provided "as is", without warranty of any kind, express or
implied, including but not limited to the warranties of merchantability, fitness
for a particular purpose and noninfringement.in no event shall the authors or
copyright holders be liable for any claim, damages or other liability, whether
in an action of contract, tort or otherwise, arising from, out of or in
connection with the software or the use or other dealings in the software.
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

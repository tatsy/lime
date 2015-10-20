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
using lime::Array2d;

class Array2dTest : public ::testing::Test {
 protected:
    virtual void setUp() {
    }

    Array2d<int> array2d;
};

TEST_F(Array2dTest, Array2dSizeTest) {
    EXPECT_EQ(array2d.rows(), 0);
    EXPECT_EQ(array2d.cols(), 0);
}

TEST_F(Array2dTest, NormalOperation) {
    array2d = Array2d<int>(100, 100);
    EXPECT_EQ(array2d.rows(), 100);
    EXPECT_EQ(array2d.cols(), 100);
    EXPECT_EQ(array2d(0, 0), 0);

    ASSERT_DEATH(array2d(100, 0), "");
    ASSERT_DEATH(array2d(0, 100), "");
    ASSERT_DEATH(array2d(100, 100), "");
    ASSERT_DEATH(array2d(-1, -1), "");
}

TEST_F(Array2dTest, CopyOperation) {
    array2d = Array2d<int>(10, 10);
    array2d(5, 5) = 25;
    Array2d<int> temp = array2d;
    EXPECT_EQ(temp(0, 0), 0);
    EXPECT_EQ(temp(5, 5), 25);
    temp(5, 5) = 0;
    EXPECT_NE(array2d(5, 5), 0);
}

TEST_F(Array2dTest, OperatorEq) {
    array2d = Array2d<int>(10, 10);
    array2d(5, 5) = 25;
    Array2d<int> temp;
    temp = array2d;
    EXPECT_EQ(temp(0, 0), 0);
    EXPECT_EQ(temp(5, 5), 25);
    temp(5, 5) = 0;
    EXPECT_NE(array2d(5, 5), 0);
}

#include "gtest/gtest.h"

#include "lime.hpp"
using lime::Array2D;

class Array2DTest : public ::testing::Test {
 protected:
    virtual void setUp() {
    }

    Array2D<int> array2d;
};

TEST_F(Array2DTest, Array2DSizeTest) {
    EXPECT_EQ(array2d.rows(), 0);
    EXPECT_EQ(array2d.cols(), 0);
}

TEST_F(Array2DTest, NormalOperation) {
    array2d = Array2D<int>(100, 100);
    EXPECT_EQ(array2d.rows(), 100);
    EXPECT_EQ(array2d.cols(), 100);
    EXPECT_EQ(array2d(0, 0), 0);

    ASSERT_DEATH(array2d(100, 0), "");
    ASSERT_DEATH(array2d(0, 100), "");
    ASSERT_DEATH(array2d(100, 100), "");
    ASSERT_DEATH(array2d(-1, -1), "");
}

TEST_F(Array2DTest, CopyOperation) {
    array2d = Array2D<int>(10, 10);
    array2d(5, 5) = 25;
    Array2D<int> temp = array2d;
    EXPECT_EQ(temp(0, 0), 0);
    EXPECT_EQ(temp(5, 5), 25);
    temp(5, 5) = 0;
    EXPECT_NE(array2d(5, 5), 0);
}

TEST_F(Array2DTest, OperatorEq) {
    array2d = Array2D<int>(10, 10);
    array2d(5, 5) = 25;
    Array2D<int> temp;
    temp = array2d;
    EXPECT_EQ(temp(0, 0), 0);
    EXPECT_EQ(temp(5, 5), 25);
    temp(5, 5) = 0;
    EXPECT_NE(array2d(5, 5), 0);
}

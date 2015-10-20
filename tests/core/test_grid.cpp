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
using lime::Grid;

class GridTest : public ::testing::Test {
 protected:
    virtual void SetUp() {
    }

    Grid<int> grid;
};

TEST_F(GridTest, SizeTest) {
    EXPECT_EQ(grid.rows(), 0);
    EXPECT_EQ(grid.cols(), 0);

    grid.resize(10, 10);
    EXPECT_TRUE(grid(0, 0).empty());
    EXPECT_EQ(grid.rows(), 10);
    EXPECT_EQ(grid.cols(), 10);

    ASSERT_DEATH(grid.resize(0, 0), "");
    ASSERT_DEATH(grid.resize(-1, 10), "");
}

TEST_F(GridTest, DataCopy) {
    const int n = 5;
    const int m = 7;
    grid.resize(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            grid.pushAt(i, j, i * j);
        }
    }

    Grid<int> temp1 = grid;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            EXPECT_EQ(temp1(i, j)[0], i * j);
        }
    }

    Grid<int> temp2;
    temp2 = grid;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            EXPECT_EQ(temp2(i, j)[0], i * j);
        }
    }

    grid.pushAt(0, 0, 1);
    EXPECT_EQ(temp1.sizeAt(0, 0), 1);
    EXPECT_EQ(temp2.sizeAt(0, 0), 1);
}

TEST_F(GridTest, HasCell) {
    grid.resize(5, 5);
    EXPECT_TRUE(grid.hasCell(0, 0));
    EXPECT_FALSE(grid.hasCell(5, 5));
    EXPECT_FALSE(grid.hasCell(-1, -1));
}


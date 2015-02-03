#include "gtest/gtest.h"
#include "../../include/lime.hpp"

namespace {
    class PointTest : public ::testing::Test {
	protected:
        PointTest() {}
		
		virtual ~PointTest() {}

		virtual void SetUp() {}

		virtual void TearDown() {}

	}; /* class PointTest */

	TEST_F(PointTest, StandardAlgebra) {
		lime::Point2d p(1.0, 2.0);
		lime::Point2d q(2.0, 4.0);
		EXPECT_EQ(p * 2.0, q);
	}

} /* anonymous namespace */


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

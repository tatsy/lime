#include "gtest/gtest.h"

#include "../../include/lime.hpp"
using namespace lime;

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

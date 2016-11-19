#include "gtest/gtest.h"

#include "lime.hpp"
using lime::random_queue;

static const int nTest = 1 << 20;

class RandomQueueTest : public ::testing::Test {
 protected:
    virtual void setUp() {
    }

    random_queue<int> que;
};

TEST_F(RandomQueueTest, InEmpty) {
    EXPECT_TRUE(que.empty());
}

TEST_F(RandomQueueTest, EnqueueDequeueWorks) {
    for (int i = 0; i < nTest; i++) {
        EXPECT_EQ(que.size(), i);
        que.push(i);
    }
    EXPECT_FALSE(que.empty());
    for (int i = 0; i < nTest; i++) {
        EXPECT_EQ(que.size(), nTest - i);
        que.pop();
    }
    EXPECT_TRUE(que.empty());
}

TEST_F(RandomQueueTest, InvalidOperation) {
    ASSERT_DEATH(que.pop(), "");
}

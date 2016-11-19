#include "gtest/gtest.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
#if !GTEST_OS_WINDOWS
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
#endif
    return RUN_ALL_TESTS();
}

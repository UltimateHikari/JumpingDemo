#include "gtest/gtest.h"
#include "tritset.h"

TEST(Initial, constructor){
    TritSet set(1000);
    EXPECT_EQ(1000, set.capacity());
}
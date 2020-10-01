#include "gtest/gtest.h"
#include "tritset.h"
#include <iostream>

TEST(tritset, constructor){
    TritSet set(100);
    int allocated = set.capacity();
    EXPECT_GE(allocated, 100*2 / 8 / sizeof(std::size_t));
    EXPECT_LE(allocated, 100*2 / 8);
}

TEST(tritset, initialization){
    TritSet set(100);
    for(int i = 0; i < 100; i++){
        EXPECT_EQ(static_cast<Trit>(set[i]), Trit::Unknown);
    }
}

TEST(tritset, allocations){
    TritSet set(100);
    int allocated = set.capacity();
    set[1000] = Trit::Unknown; 
    EXPECT_EQ(allocated, set.capacity());

    if(set[2000] == Trit::True){} 
    EXPECT_EQ(allocated, set.capacity());

    set[1000] = Trit::True; 
    EXPECT_LT(allocated, set.capacity());

    allocated = set.capacity();
    set[10000] = Trit::Unknown; 
    set[100] = Trit::False; 
    EXPECT_EQ(allocated, set.capacity());
}

TEST(tritset, setnget){
    TritSet set(100);
    for(int i = 0; i < sizeof(std::size_t); i++){
        EXPECT_EQ(static_cast<Trit>(set[i]), Trit::Unknown);
        set[i] = Trit::False;
        EXPECT_EQ(static_cast<Trit>(set[i]), Trit::False);
        set[i] = Trit::True;
        EXPECT_EQ(static_cast<Trit>(set[i]), Trit::True);
        set[i] = Trit::Unknown;
        EXPECT_EQ(static_cast<Trit>(set[i]), Trit::Unknown);
    }
}

TEST(shrink, to_last){
    TritSet set(10);
    set[100] = Trit::False;
    int allocated = set.capacity();
    set[100] = Trit::Unknown;
    set[50] = Trit::False;
    set.shrink();
    EXPECT_GT(allocated, set.capacity());
}

TEST(shrink, to_min){
    TritSet set(10);
    set[20] = Trit::False;
    set[20] = Trit::Unknown;
    int allocated = set.capacity();
    set.shrink();
    EXPECT_GT(allocated, set.capacity());
}
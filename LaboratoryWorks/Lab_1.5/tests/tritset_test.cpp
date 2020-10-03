#include "gtest/gtest.h"
#include "tritset.h"
#include <iostream>

TEST(tritset, constructor){
    TritSet set(100);
    int allocated = set.underlaying_capacity();
    EXPECT_GE( 8*sizeof(size_t)*allocated, 100*2);
    EXPECT_LE(8*allocated, 100*2);
}

TEST(tritset, initialization){
    TritSet set(100);
    for(int i = 0; i < 100; i++){
        EXPECT_EQ(static_cast<Trit>(set[i]), Trit::Unknown);
    }
}

TEST(tritset, allocations){
    TritSet set(100);
    int allocated = set.underlaying_capacity();
    set[1000] = Trit::Unknown; 
    EXPECT_EQ(allocated, set.underlaying_capacity());

    if(set[2000] == Trit::True){} 
    EXPECT_EQ(allocated, set.underlaying_capacity());

    set[1000] = Trit::True; 
    EXPECT_LT(allocated, set.underlaying_capacity());

    allocated = set.underlaying_capacity();
    set[10000] = Trit::Unknown; 
    set[100] = Trit::False; 
    EXPECT_EQ(allocated, set.underlaying_capacity());
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
    int allocated = set.underlaying_capacity();
    set[100] = Trit::Unknown;
    set[50] = Trit::False;
    set.shrink();
    EXPECT_GT(allocated, set.underlaying_capacity());
}

TEST(shrink, to_min){
    TritSet set(10);
    set[20] = Trit::False;
    set[20] = Trit::Unknown;
    int allocated = set.underlaying_capacity();
    set.shrink();
    EXPECT_GT(allocated, set.underlaying_capacity());
}
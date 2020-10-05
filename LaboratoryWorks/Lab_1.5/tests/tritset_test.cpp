#include "gtest/gtest.h"
#include "tritset.h"
#include <iostream>
#include <unordered_map>

TEST(tritset, initialization){
    TritSet set(100);
    for(int i = 0; i < 100; i++){
        EXPECT_EQ(static_cast<Trit>(set[i]), Trit::Unknown);
    }
}

TEST(tritset, setnget){
    TritSet set(100);
    for(int i = 0; i <= 100; i++){
        ASSERT_EQ(set[i], Trit::Unknown) << "on index " << i;
        set[i] = Trit::False;
        ASSERT_EQ(set[i], Trit::False) << "on index " << i;
        set[i] = Trit::True;
        ASSERT_EQ(set[i], Trit::True) << "on index " << i;
        set[i] = Trit::Unknown;
        ASSERT_EQ(set[i], Trit::Unknown) << "on index " << i;
    }
}

TEST(len_functions, length){
    TritSet set(100);
    EXPECT_EQ(set.length(), 0);
    set[49] = Trit::False;
    set[52] = Trit::Unknown;
    EXPECT_EQ(set.length(), 50);
    set[49] = Trit::Unknown;
    set[39] = Trit::True;
    EXPECT_EQ(set.length(), 40);
}

TEST(len_functions, trim){
    TritSet set(100);
    set[60] = Trit::False;
    set[59] = Trit::True;
    set[61] = Trit::True;
    set.trim(60);
    EXPECT_EQ(set.length(), 60);
}

TEST(constructors, sized){
    TritSet set(100);
    int allocated = set.underlaying_capacity();
    EXPECT_GE( 8*sizeof(size_t)*allocated, 100*2);
    EXPECT_LE(8*allocated, 100*2);
}

TEST(constructors, copy){
    TritSet A(100);
    A[99] = Trit::True;
    A[88] = Trit::False;
    TritSet B(A);
    EXPECT_EQ(A.capacity(), B.capacity());
    EXPECT_EQ(A.underlaying_capacity(), B.underlaying_capacity());
    EXPECT_EQ(A[88], B[88]);
    EXPECT_EQ(A[99], B[99]);
}

TEST(resize, allocations){
    TritSet set(100);
    int allocated = set.underlaying_capacity();
    set[1000] = Trit::Unknown; 
    EXPECT_EQ(allocated, set.underlaying_capacity());

    if(set[2000] == Trit::True){} 
    EXPECT_EQ(allocated, set.underlaying_capacity());

    set[1000] = Trit::True; 
    EXPECT_LT(allocated, set.underlaying_capacity());
    for(int i = 0; i < 1000; i++){
        EXPECT_EQ(set[i], Trit::Unknown);
    }

    allocated = set.underlaying_capacity();
    set[10000] = Trit::Unknown; 
    set[500] = Trit::False; 
    EXPECT_EQ(allocated, set.underlaying_capacity());
}

TEST(resize, to_last){
    TritSet set(10);
    set[100] = Trit::False;
    std::cerr << set;
    int allocated = set.underlaying_capacity();
    set[100] = Trit::Unknown;
        std::cerr << set;
    set[50] = Trit::False;
        std::cerr << set;
    set.shrink();
    EXPECT_GT(allocated, set.underlaying_capacity()) << "and length is " << set.length();
}

TEST(resize, to_min){
    TritSet set(100);
    set[200] = Trit::False;
    set[200] = Trit::Unknown;
    int allocated = set.underlaying_capacity();
    set.shrink();
    EXPECT_GT(allocated, set.underlaying_capacity()) << "and length is " << set.length();
}

TEST(cardinality, all){
    TritSet set(100);
    for(int i = 0; i < 10; i++){
        set[i] = Trit::False;
    }
    for(int i = 20; i < 31; i++){
        set[i] = Trit::True;
    }
    set[40] = Trit::False;
    std::unordered_map< Trit, int > m = set.cardinality();
    EXPECT_EQ(m[Trit::False], 11);
    EXPECT_EQ(m[Trit::True], 11);
    EXPECT_EQ(m[Trit::Unknown], 19);
}

TEST(cardinality, Unknown){
    TritSet set(100);
    set[40] = Trit::False;
    EXPECT_EQ(set.cardinality(Trit::Unknown), 40);
}

TEST(cardinality, False){
    TritSet set(100);
    for(int i = 20; i < 31; i++){
        set[i] = Trit::False;
    }
    EXPECT_EQ(set.cardinality(Trit::False), 11);
}

TEST(cardinality, True){
    TritSet set(100);
    for(int i = 20; i < 31; i++){
        set[i] = Trit::True;
    }
    EXPECT_EQ(set.cardinality(Trit::True), 11);
}

// TEST(binary, capacity); //mb some tricky stuff with T in bigger?
// TEST(binary, OR);
// TEST(binary, AND);
// TEST(binary, NOT); //soon 
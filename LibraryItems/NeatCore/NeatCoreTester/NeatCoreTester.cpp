// NeatCoreTester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int fnNeatCore(int a, int b)
{
    return a + b;
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(NeatCoreUT, fnNeatCore)
{
    EXPECT_EQ(fnNeatCore(1, 2), 3);
    EXPECT_NE(fnNeatCore(2, 2), 2);
}
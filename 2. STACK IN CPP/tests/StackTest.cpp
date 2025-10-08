#include <gtest/gtest.h>
#include "../include/Stack.h"
#include <climits>
#include <stdexcept>  

TEST(InitializeStack, CreatesEmptyStack) {
    Stack stack;
    EXPECT_TRUE(stack.isEmpty());
}

TEST(BasicPushAndPop, SinglePushThenPop) {
    Stack stack;
    stack.push(21);
    EXPECT_FALSE(stack.isEmpty());
    EXPECT_EQ(21, stack.pop());
    EXPECT_TRUE(stack.isEmpty());
}

TEST(MultiplePushAndPop, LifoOrder) {
    Stack stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);

    EXPECT_EQ(3, stack.pop());
    EXPECT_EQ(2, stack.pop());
    EXPECT_EQ(1, stack.pop());
    EXPECT_TRUE(stack.isEmpty());
}

TEST(ResizingStack, ExpandsCapacityWhenNeeded) {
    Stack stack;
    for (int i = 0; i < 10; ++i) {
        stack.push(i);
    }
    EXPECT_EQ(9, stack.pop());
    EXPECT_EQ(8, stack.pop());
    EXPECT_EQ(7, stack.pop());
}

TEST(StressPushPop, PushAndFullPop) {
    Stack stack;
    for (int i = 1; i <= 10000; ++i) {
        stack.push(i);
    }
    for (int i = 10000; i >= 1; --i) {
        EXPECT_EQ(i, stack.pop());
    }
    EXPECT_TRUE(stack.isEmpty());
}

TEST(PushIntLimits, HandlesMinAndMaxInt) {
    Stack stack;
    stack.push(INT_MIN);
    stack.push(INT_MAX);

    EXPECT_EQ(INT_MAX, stack.pop());
    EXPECT_EQ(INT_MIN, stack.pop());
    EXPECT_TRUE(stack.isEmpty());
}

TEST(PushBeyondCapacity, MoreThanInitialCapacity) {
    Stack stack;
    for (int i = 0; i < 15; ++i) {
        stack.push(i);
    }
    EXPECT_EQ(14, stack.pop());
    EXPECT_EQ(13, stack.pop());
    EXPECT_EQ(12, stack.pop());
}

TEST(Exceptions, PopOnEmptyThrowsUnderflow) {
    Stack stack;
    EXPECT_THROW(stack.pop(), std::underflow_error);
}



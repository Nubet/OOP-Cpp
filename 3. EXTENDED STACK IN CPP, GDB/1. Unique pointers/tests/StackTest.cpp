#include <gtest/gtest.h>
#include "../include/Stack.h"
#include <climits>

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

TEST(CopyConstructor, CopiesNonEmptyStack) {
    Stack stack;
    stack.push(10);
    stack.push(20);
    Stack copied(stack);
    EXPECT_FALSE(copied.isEmpty());
    EXPECT_EQ(20, copied.pop());
    EXPECT_EQ(10, copied.pop());
    EXPECT_TRUE(copied.isEmpty());
    // Original should not be modified by operations on copy
    EXPECT_FALSE(stack.isEmpty());
    EXPECT_EQ(20, stack.pop());
    EXPECT_EQ(10, stack.pop());
    EXPECT_TRUE(stack.isEmpty());
}

TEST(CopyConstructor, CopiesEmptyStack) {
    Stack stack;
    Stack copied(stack);
    EXPECT_TRUE(copied.isEmpty());
}

TEST(CopyConstructor, InitializationViaCopy) {
    Stack stack;
    stack.push(47);
    Stack copied = stack;
    EXPECT_FALSE(copied.isEmpty());
    EXPECT_EQ(47, copied.pop());
    EXPECT_TRUE(copied.isEmpty());
}

TEST(AssignmentOperator, NonEmptyToNonEmpty) {
    Stack stack1;
    stack1.push(1);
    stack1.push(2);
    Stack stack2;
    stack2.push(3);
    stack2 = stack1;
    EXPECT_FALSE(stack2.isEmpty());
    EXPECT_EQ(2, stack2.pop());
    EXPECT_EQ(1, stack2.pop());
    EXPECT_TRUE(stack2.isEmpty());
    // Original should not be modified by operations on copy
    EXPECT_FALSE(stack1.isEmpty());
    EXPECT_EQ(2, stack1.pop());
    EXPECT_EQ(1, stack1.pop());
    EXPECT_TRUE(stack1.isEmpty());
}

TEST(AssignmentOperator, EmptyToEmpty) {
    Stack stack1;
    Stack stack2;
    stack2 = stack1;
    EXPECT_TRUE(stack2.isEmpty());
}

TEST(AssignmentOperator, NonEmptyToEmpty) {
    Stack stack1;
    stack1.push(14);
    Stack stack2;
    stack2 = stack1;
    EXPECT_FALSE(stack2.isEmpty());
    EXPECT_EQ(14, stack2.pop());
    EXPECT_TRUE(stack2.isEmpty());
}

TEST(AssignmentOperator, EmptyToNonEmpty) {
    Stack stack1;
    Stack stack2;
    stack2.push(99);
    stack2 = stack1;
    EXPECT_TRUE(stack2.isEmpty());
}

TEST(AssignmentOperator, SelfAssignment) {
    Stack stack;
    stack.push(2137);
    stack = stack;
    EXPECT_FALSE(stack.isEmpty());
    EXPECT_EQ(2137, stack.pop());
    EXPECT_TRUE(stack.isEmpty());
}

TEST(AssignmentOperator, Chaining) {
    Stack stack1, stack2, stack3;
    stack1.push(1);
    stack2.push(2);
    stack3.push(3);
    stack3 = stack2 = stack1;
    EXPECT_FALSE(stack2.isEmpty());
    EXPECT_FALSE(stack3.isEmpty());
    EXPECT_EQ(1, stack2.pop());
    EXPECT_EQ(1, stack3.pop());
    EXPECT_TRUE(stack2.isEmpty());
    EXPECT_TRUE(stack3.isEmpty());
}

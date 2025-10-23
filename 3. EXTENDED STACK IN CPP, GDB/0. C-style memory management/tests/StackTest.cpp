#include <gtest/gtest.h>
#include "Stack.h"
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
TEST(ResizingStack, SingleResize) {
    Stack stack;
    // default min capacity is 8
    for (int i = 0; i < 15; ++i) {
        stack.push(i);
    }
    EXPECT_EQ(14, stack.pop());
    EXPECT_EQ(13, stack.pop());
    EXPECT_EQ(12, stack.pop());
}
TEST(ResizingStack, ExpandsMultipleTimes) {
    Stack stack;
    const int N = 5000;
    for (int i = 0; i < N; ++i)
        stack.push(i);

    for (int i = N - 1; i >= 0; --i) {
        EXPECT_EQ(i, stack.pop());
    }
    EXPECT_TRUE(stack.isEmpty());
}
TEST(StressPushPop, PushAndFullPop) {
    Stack stack;
    const int N = 10000;
    for (int i = 1; i <= N; ++i) {
        stack.push(i);
    }
    for (int i = N; i >= 1; --i) {
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

TEST(AssignmentOperator, ChainingDeepCopyAndIndependence) {
    Stack source, firstAssigned, secondAssigned;
    source.push(1);
    source.push(2);
    source.push(3);

    firstAssigned.push(10);
    secondAssigned.push(20);

    secondAssigned = firstAssigned = source;

    EXPECT_EQ(3, firstAssigned.pop());
    EXPECT_EQ(2, firstAssigned.pop());
    EXPECT_EQ(1, firstAssigned.pop());
    EXPECT_TRUE(firstAssigned.isEmpty());

    EXPECT_EQ(3, secondAssigned.pop());
    EXPECT_EQ(2, secondAssigned.pop());
    EXPECT_EQ(1, secondAssigned.pop());
    EXPECT_TRUE(secondAssigned.isEmpty());

    EXPECT_EQ(3, source.pop());
    EXPECT_EQ(2, source.pop());
    EXPECT_EQ(1, source.pop());
    EXPECT_TRUE(source.isEmpty());
}
TEST(UnderflowDeath, PopOnEmptyExits) {
    Stack s;
    EXPECT_EXIT(
        s.pop(), ::testing::ExitedWithCode(1),
        ".*"); // EXPECT_EXIT checks stderr for the message output, but in my implementation the error message is printed using printf (stdout)
}

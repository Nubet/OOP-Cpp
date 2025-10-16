#include <gtest/gtest.h>

extern "C" {
#include "../include/Stack.h"
#include <limits.h>
}

TEST(InitializeStack, CreatesEmptyStack) {
    Stack stack;
    init(&stack);
    EXPECT_TRUE(isEmpty(&stack));
    destroy(&stack);
}

TEST(BasicPushAndPop, SinglePushThenPop) {
    Stack stack;
    init(&stack);
    push(&stack, 21);
    EXPECT_FALSE(isEmpty(&stack));
    EXPECT_EQ(21, pop(&stack));
    EXPECT_TRUE(isEmpty(&stack));
    destroy(&stack);
}

TEST(MultiplePushAndPop, LifoOrder) {
    Stack stack;
    init(&stack);
    push(&stack, 1);
    push(&stack, 2);
    push(&stack, 3);

    EXPECT_EQ(3, pop(&stack));
    EXPECT_EQ(2, pop(&stack));
    EXPECT_EQ(1, pop(&stack));
    EXPECT_TRUE(isEmpty(&stack));
    destroy(&stack);
}

TEST(ResizingStack, ExpandsCapacityWhenNeeded) {
    Stack stack;
    init(&stack);
    for (int i = 0; i < 10; ++i) {
        push(&stack, i);
    }
    EXPECT_EQ(9, pop(&stack));
    EXPECT_EQ(8, pop(&stack));
    EXPECT_EQ(7, pop(&stack));
    destroy(&stack);
}

TEST(StressPushPop, PushAndFullPop) {
    Stack stack;
    init(&stack);
    for (int i = 1; i <= 10000; ++i) {
        push(&stack, i);
    }
    for (int i = 10000; i >= 1; --i) {
        EXPECT_EQ(i, pop(&stack));
    }
    EXPECT_TRUE(isEmpty(&stack));
    destroy(&stack);
}


TEST(PushIntLimits, HandlesMinAndMaxInt) {
    Stack stack;
    init(&stack);
    push(&stack, INT_MIN);
    push(&stack, INT_MAX);

    EXPECT_EQ(INT_MAX, pop(&stack));
    EXPECT_EQ(INT_MIN, pop(&stack));
    EXPECT_TRUE(isEmpty(&stack));
    destroy(&stack);
}

TEST(PushBeyondCapacity, MoreThanInitialCapacity) {
    Stack stack;
    init(&stack);
    //  MIN_CAPACITY = 8
    for (int i = 0; i < 15; ++i) {
        push(&stack, i);
    }
    EXPECT_EQ(14, pop(&stack));
    EXPECT_EQ(13, pop(&stack));
    EXPECT_EQ(12, pop(&stack));
    destroy(&stack);
}
TEST(UnderflowDeath, PopOnEmptyExits) {
    Stack s;
    init(&s);
    EXPECT_EXIT(
        pop(&s), ::testing::ExitedWithCode(1),
        ".*"); // EXPECT_EXIT checks stderr for the message output, but in my implementation the error message is printed using printf (stdout)
    destroy(&s);
}

TEST(UnderflowDeath, PopAfterDestroyAlsoExits) {
    Stack s;
    init(&s);
    destroy(&s);
    EXPECT_EXIT(pop(&s), ::testing::ExitedWithCode(1), ".*");
}
TEST(DestroyContract, FieldsAreResetAndPointerNulled) {
    Stack s;
    init(&s);
    for (int i = 0; i < 30; i++) push(&s, i);
    destroy(&s);

    EXPECT_EQ(nullptr, s.items);
    EXPECT_EQ(0, s.capacity);
    EXPECT_EQ(-1, s.top);

}

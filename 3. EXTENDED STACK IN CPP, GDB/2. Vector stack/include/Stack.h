#pragma once
#include <vector>
#include <algorithm>

class Stack {
private:
    std::vector<int> items;
    static constexpr int MIN_CAPACITY = 8;

public:
    Stack();
    Stack(const Stack& toCopy) = default;
    Stack& operator=(const Stack& toCopy) = default;
    ~Stack() = default;

    // Stack operations
    void push(int item);
    int pop();
    bool isEmpty() const;
};

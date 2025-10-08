#pragma once
#include <memory>

class Stack {
private:
    std::unique_ptr<int[]> items;
    int capacity;
    int top;

    static constexpr int MIN_CAPACITY = 8;

public:
    Stack();
    Stack(const Stack& toCopy);               
    Stack& operator=(const Stack& toCopy);     
    ~Stack() = default;                       

    void push(int item);
    int pop();
    bool isEmpty() const;
};

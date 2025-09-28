#pragma once
#include <stdexcept>

class Stack {
private:
    int* items;
    int capacity;
    int top;

    static const int MIN_CAPACITY = 8;

public:
    Stack();
    ~Stack();

    void push(int item);
    int pop();
    bool isEmpty() const;
};

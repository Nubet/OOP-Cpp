#pragma once

class Stack {
private:
    int* items;
    int capacity;
    int top;

    static constexpr int MIN_CAPACITY = 8;

public:
    Stack();
    Stack(const Stack& toCopy);
    Stack& operator=(const Stack& toCopy);

    ~Stack();

    void push(int item);
    int pop();
    bool isEmpty() const;
};

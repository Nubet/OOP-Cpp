#pragma once
class Stack
{
  public:
    Stack();
    Stack(const Stack& toCopy);
    Stack& operator=(const Stack& toCopy);

    ~Stack();

    void push(int item);
    int pop();
    bool isEmpty() const;

  private:
    int* items;
    int capacity;
    int top;
    static constexpr int MIN_CAPACITY = 1;
};
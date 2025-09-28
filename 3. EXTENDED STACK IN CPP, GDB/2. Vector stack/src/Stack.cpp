#include "Stack.h"
#include <iostream>
#include <stdexcept>

// Constructor
Stack::Stack() {
    items.reserve(MIN_CAPACITY);
}

void Stack::push(int item) {
    items.push_back(item);
}

int Stack::pop() {
    if (isEmpty()) {
        throw std::runtime_error("Stack is empty");
    }
    int value = items.back();
    items.pop_back();
    return value;
}

bool Stack::isEmpty() const {
    return items.empty();
}

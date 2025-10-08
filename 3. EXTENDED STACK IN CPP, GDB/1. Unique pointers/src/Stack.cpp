#include "Stack.h"
#include <iostream>


Stack::Stack() {
    capacity = MIN_CAPACITY;
    top = -1;
    items = std::make_unique<int[]>(capacity);
}

Stack::Stack(const Stack& toCopy) {
    capacity = toCopy.capacity;
    top = toCopy.top;
    items = std::make_unique<int[]>(capacity);
    std::copy(toCopy.items.get(), toCopy.items.get() + (top + 1), items.get());
}


Stack& Stack::operator=(const Stack& toCopy) {
    if (this == &toCopy) return *this;

    auto newItems = std::make_unique<int[]>(toCopy.capacity);
    std::copy(toCopy.items.get(), toCopy.items.get() + (toCopy.top + 1), newItems.get());

    capacity = toCopy.capacity;
    top = toCopy.top;
    items = std::move(newItems);

    return *this;
}

void Stack::push(int item) {
    if (top + 1 >= capacity) {
        int newCapacity = capacity * 2;
        auto newItems = std::make_unique<int[]>(newCapacity);

        std::copy(items.get(), items.get() + (top + 1), newItems.get());

        items = std::move(newItems);
        capacity = newCapacity;
    }

    items[++top] = item;
}

int Stack::pop() {
    if (isEmpty()) {
        throw std::runtime_error("Stack is empty");
    }
    return items[top--];
}

bool Stack::isEmpty() const {
    return top == -1;
}

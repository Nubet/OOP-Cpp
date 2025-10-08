#include "../include/Stack.h"
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include <stdexcept>

Stack::Stack() {
    capacity = MIN_CAPACITY;
    items = static_cast<int*>(malloc(capacity * sizeof(int)));
    
    if (nullptr == items) {
        throw std::bad_alloc();
    }
    top = -1;
}

Stack::~Stack() {
    free(items);
    items = nullptr;
    capacity = 0;
    top = -1;
}

void Stack::push(int item) {
    if (capacity - top - 1 <= 0) {
        capacity *= 2;
        int* temp = static_cast<int*>(realloc(items, capacity * sizeof(int)));
        if (nullptr == temp) {
            throw std::bad_alloc();
        }
        items = temp;
    }
    top++;
    items[top] = item;
}

int Stack::pop() {
    if (isEmpty()) {
        throw std::underflow_error("Stack is empty");
    }
    int item = items[top];
    top--;
    return item;
}


Stack::Stack(const Stack& toCopy) {
    int elementsCount = toCopy.top + 1;
    capacity = toCopy.capacity < MIN_CAPACITY ? MIN_CAPACITY : toCopy.capacity;
    if (capacity < elementsCount) {
        capacity = elementsCount;
    }

    items = static_cast<int*>(malloc(capacity * sizeof(int)));
    if (nullptr == items) {
        printf("malloc failed\n");
        exit(1);
    }

    top = toCopy.top;
    if (elementsCount > 0) {
        memcpy(items, toCopy.items, elementsCount * sizeof(int));
    }
}

Stack& Stack::operator=(const Stack& toCopy) {
    if (this == &toCopy) {
        return *this;
    }

    const int elementsCount = toCopy.top + 1;
    const int requiredCapacity = std::max(toCopy.capacity, MIN_CAPACITY);
    const int actualCapacity = (requiredCapacity < elementsCount) ? elementsCount : requiredCapacity;

    int* newItems = static_cast<int*>(malloc(actualCapacity * sizeof(int)));
    if (!newItems) {
        printf("Memory allocation failed in operator=\n");
        exit(1);
    }

    if (elementsCount > 0) {
        memcpy(newItems, toCopy.items, elementsCount * sizeof(int));
    }

    free(items);
    items = newItems;
    capacity = actualCapacity;
    top = toCopy.top;

    return *this;
}

bool Stack::isEmpty() const {
    return top == -1;
}


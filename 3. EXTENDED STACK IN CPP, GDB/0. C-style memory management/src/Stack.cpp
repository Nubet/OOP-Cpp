#include "Stack.h"
#include <iostream>
#include <cstring>

Stack::Stack() {
    capacity = MIN_CAPACITY;
    items = static_cast<int*>(malloc(capacity * sizeof(int)));

    if (nullptr == items) {
        printf("malloc failed \n");
        exit(1);
    }

    top = -1;
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

Stack::~Stack() {
    free(items);
    items = nullptr;
    capacity = 0;
    top = -1;
}

void Stack::push(int item) {
    if (capacity - top - 1 <= 0) {
        capacity *= 2;
        int *temp = static_cast<int*>(realloc(items, capacity * sizeof(int)));

        if (nullptr == temp) {
            printf("realloc failed \n");
            free(items);
            exit(1);
        }
        items = temp;
    }
    top++;
    items[top] = item;
}

int Stack::pop() {
    if (isEmpty()) {
        printf("Stack is empty \n");
        exit(1);
    }

    int item = items[top];
    top--;
    return item;
}

bool Stack::isEmpty() const {
    return top == -1;
}

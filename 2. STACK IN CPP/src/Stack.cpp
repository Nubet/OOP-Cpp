#include "../include/Stack.h"
#include <cstdlib>

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

bool Stack::isEmpty() const {
    return top == -1;
}

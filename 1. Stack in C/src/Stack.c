#include "Stack.h"
#include <stdio.h>
#include <stdlib.h>

#define MIN_CAPACITY 8

void init(Stack* s) {
    s->capacity = MIN_CAPACITY;
    s->items = malloc(s->capacity * sizeof(int));

    if (NULL == s->items) {
        printf("malloc failed \n");
        exit(1);
    }

    s->top = -1;
}

void destroy(Stack* s) {
    free(s->items);
    s->items = NULL;
    s->capacity = 0;
    s->top = -1;
}

void push(Stack* s, int item) {
    if (s->capacity - s->top - 1 <= 0) {
        s->capacity *= 2;

        int* temp = realloc(s->items, s->capacity * sizeof(int));
        if (NULL == temp) {
            printf("realloc failed \n");
            destroy(s);
            exit(1);
        }
        s->items = temp;
    }
    s->top++;
    s->items[s->top] = item;
}

int pop(Stack* s) {
    if (isEmpty(s)) {
        printf("Stack is empty \n");
        destroy(s);
        exit(1);
    }

    int item = s->items[s->top];
    s->top--;
    return item;
}

bool isEmpty(const Stack* s) { return s->top == -1; }

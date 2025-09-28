#include <stdbool.h>

typedef struct {
    int* items;
    int capacity;
    int top;
} Stack;

void init(Stack* s);
void destroy(Stack* s);
void push(Stack* s, int item);
int pop(Stack* s);
bool isEmpty(const Stack* s);

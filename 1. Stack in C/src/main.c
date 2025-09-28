#include <stdio.h>
#include "../include/Stack.h"

int main(void) {
    Stack stack;
    init(&stack);
    int n, value;

    printf("How many number you want to enter:");
    scanf("%d", &n);

    printf("Enter %d values: \n", n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &value);
        push(&stack,value);
    }

    printf("Your numbers in reverse: \n");
    while (!isEmpty(&stack)) {
        printf("%d ", pop(&stack));
    }
    destroy(&stack);
}
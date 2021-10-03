#include <stdio.h>

#include "stack.h"


int main() {
    printf("Stack ctor: Hello!\n");
    stack_struct Stack;

    stack_ctor(&Stack);

    for (int i = 0; i < 50; i++) {
        stack_push(&Stack, i);
        printf("size: %5d, Capacity: %5d\n", Stack.size, Stack.capacity);
    }

    Stack.ptr_begin = nullptr;

    for (int i = 0; i < 20; i++) {
        stack_pop(&Stack, 2);
        printf("size: %5d, Capacity: %5d\n", Stack.size, Stack.capacity);
    }

    return 0;
}

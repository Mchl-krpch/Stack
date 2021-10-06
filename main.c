#include <stdio.h>

#include "stack.h"

int main() {
    Stack stack;

    stack_ctor(&stack, 2);

    for (int elem = 0; elem < 100; elem++) {
        stack_push(&stack, elem);
        printf("capacity: %d, size: %d\n", stack.capacity, stack.size);
    }

    for (int elem = 0; elem < stack.size; elem++) {
        printf("%x\n", stack.data[elem]);
    }


    for (int elem = 0; elem < 80; elem++) {
        stack_pop(&stack);
        printf("capacity: %d, size: %d\n", stack.capacity, stack.size);
    }

    stack_dtor(&stack);

    return 0;
}

#include <stdio.h>

#include "stack.h"

int main() {
    Stack stack;

    stack_ctor(&stack, 8);


    for (int elem = 0; elem < 100; elem++) {
        stack_push(&stack, elem);
        printf("capacity: %d, size: %d, last elem: %d\n", stack.capacity, stack.size, stack.data[stack.size - 1]);
    }

    stack_change_cap(&stack, stack.size);
    stack.data[10] = 1000;
    //stack.canary_end = 0xDDD;
    //printf("canary beg: %x, canary end: %x\n", stack.canary_beg, stack.canary_end);

    for (int elem = 0; elem < 80; elem++) {
        stack_pop(&stack);
        printf("capacity: %d, size: %d\n", stack.capacity, stack.size);
    }

    stack_dtor(&stack);

    return 0;
}

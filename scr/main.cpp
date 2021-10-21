#include <stdio.h>

#include "stack.h"

int main() {
    Stack stack;

    stack_ctor(&stack, 8);

    for (int elem = 0; elem < 10; elem++) {
        stack_push(&stack, elem);
        printf("capacity: %d, size: %d, last elem: %d\n", stack.capacity, stack.size, stack.data[stack.size - 1]);
    }

    /* stack hash check
    stack.data[13] = 0;
    */

    /* stack canary check
    stack.canary_beg = 0;
    */

    /* stack size check
    stack.size = -1;
    */

    /* stack size check
    stack.capacity = -1;
    */

    for (int elem = 0; elem < 5; elem++) {
        stack_pop(&stack);
        printf("capacity: %d, size: %d\n", stack.capacity, stack.size);
    }

    stack_dtor(&stack);

    return 0;
}

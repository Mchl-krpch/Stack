#include <stdio.h>

#include "stack.h"

#if LOG_INFO == 1
char name[60] = "logfile";
#else
char name[60];
#endif

int main() {
    Stack stack;
    int code = 0;
    sType top_element;

    code = stack_ctor(&stack, 8, name);
    if (code != NO_ERRORS) {
        return -1;
    }

    for (int elem = 0; elem < 10; elem++) {
        code = stack_push(&stack, elem);
        printf("capacity: %d, size: %d, last elem: %d\n", stack.capacity, stack.size, stack.data[stack.size - 1]);
    }

    code = stack_top(&stack, &top_element);
    if (code != NO_ERRORS) {
        return -1;
    }

    for (int elem = 0; elem < 5; elem++) {
        code = stack_pop(&stack);
        if (code != NO_ERRORS) {
        return -1;
        }
        printf("capacity: %d, size: %d\n", stack.capacity, stack.size);
    }

    code = stack_dtor(&stack);
    if (code != NO_ERRORS) {
        return -1;
    }

    return 0;
}

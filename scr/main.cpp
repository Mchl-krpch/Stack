#include <stdio.h>

#include "stack.h"

int main() {
    Stack stack;
    int code = 0;
    sType top_element;

    code = stack_ctor(&stack, 8);
    if (code != NO_ERRORS) {
        return -1;
    }

    for (int elem = 0; elem < 10; elem++) {
        code = stack_push(&stack, elem);
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
    }

    code = stack_dtor(&stack);
    if (code != NO_ERRORS) {
        return -1;
    }

    return 0;
}

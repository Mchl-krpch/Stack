#include "stack.h"


int main() {
    Stack stack;

    stack_ctor(&stack, 2);

    for (int elem = 0; elem < 40; elem++) {
        stack_push(&stack, elem);
    }

    for (int elem = 0; elem < 10; elem++) {
        stack_pop(&stack);
    }

    stack_dtor(&stack);

    return 0;
}

#include "stack.h"


int main() {
    stack_struct Stack;

    stack_ctor(&Stack);

    for (int i = 0; i < 50; i++) {
        stack_push(&Stack, i);
    }

    for (int i = 0; i < 20; i++) {
        stack_pop(&Stack, 2);
    }

    return 0;
}

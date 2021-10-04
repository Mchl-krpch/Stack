#include "stack.h"


int main() {
    stack_struct Stack;

    stack_ctor(&Stack);


    for (int i = 0; i < 40; i++) {
        stack_push(&Stack, i);
    }

    for (int i = 0; i < 10; i++) {
        stack_pop(&Stack, 2);
    }

    stack_dtor(&Stack);

    return 0;
}

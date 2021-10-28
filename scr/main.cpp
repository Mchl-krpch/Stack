#include <stdio.h>
#include <stdlib.h>

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
        if (code != NO_ERRORS) {
            stack_dtor(&stack);

            return -1;
        }
    }

    code = stack_top(&stack, &top_element);
    if (code != NO_ERRORS) {
        stack_dtor(&stack);

        return -1;
    }

    //===============CHECKS================

    /*
    ---------change stack's data-----------
    stack hash check
    stack.data[1] = 10000000;
    ---------change stack's data-----------
    */
    

    /*
    ---------change stack's canary_beg-----
    stack.canary_beg = 0;
    ---------change stack's canary_beg-----
    */

    /*
    ---------change stack's size-----------
    stack.size = -10;
    ---------change stack's size-----------
    */

    /*
    ---------change stack's capacity-------
    stack.capacity = -1;
    ---------change stack's capacity-------
    */

    /*
    ---------change stack's end------------
    stack.canary_end = 0;
    ---------change stack's end------------
    */

    //===============CHECKS================

    for (int elem = 0; elem < 5; elem++) {
        code = stack_pop(&stack);
        if (code != NO_ERRORS) {
            stack_dtor(&stack);

            return -1;
        }
    }

    code = stack_dtor(&stack);
    if (code != NO_ERRORS) {
        return -1;
    }

    return 0;
}

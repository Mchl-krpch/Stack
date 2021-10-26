#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

int main() {
    Stack stack;

    int code = 0;

    sType top_element;

    #ifdef DEBUG_LVL_2
        stack.output_file = fopen("log3", "w+");
    #endif

    code = stack_ctor(&stack, 8);
    if (code != NO_ERRORS) {
        free(stack.data);
        #ifdef DEBUG_LVL_2
            fclose(stack.output_file);
        #endif

        return -1;
    }

    for (int elem = 0; elem < 10; elem++) {
        code = stack_push(&stack, elem);
        if (code != NO_ERRORS) {
            free(stack.data);
            #ifdef DEBUG_LVL_2
                fclose(stack.output_file);
            #endif

            return -1;
        }

        printf("capacity: %d, size: %d, last elem: %d\n", stack.capacity, stack.size, stack.data[stack.size - 1]);
    }

    code = stack_top(&stack, &top_element);
    if (code != NO_ERRORS) {
        free(stack.data);
        #ifdef DEBUG_LVL_2
            fclose(stack.output_file);
        #endif

        return -1;
    }

    //-----------------------------------------------------CHECKS

    //printf("TOP ELEMENT: %d\n\n", top_element);

    /*stack hash check
    stack.data[1] = 10000000;
    */
    

    /* stack canary begin check
    */
    stack.canary_beg = 0;

    /* stack size check
    stack.size = -10;
    */

    /* stack size check
    stack.capacity = -1;
    */

    /* stack canary end check
    */
    stack.canary_end = 0;

    //-----------------------------------------------------CHECKS


    for (int elem = 0; elem < 5; elem++) {
        code = stack_pop(&stack);
        if (code != NO_ERRORS) {
            free(stack.data);
            #ifdef DEBUG_LVL_2
                fclose(stack.output_file);
            #endif

            return -1;
        }

        printf("capacity: %d, size: %d\n", stack.capacity, stack.size);
    }

    code = stack_dtor(&stack);
    if (code != NO_ERRORS) {
        free(stack.data);
        #ifdef DEBUG_LVL_2
            fclose(stack.output_file);
        #endif

        return -1;
    }

    return 0;
}

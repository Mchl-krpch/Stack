#include <stdio.h>

#include "stack.h"
#include "output.h"

void STACK_IS_OK(stack_struct *cur_stack, char *path, int line, char *func_name) {
    if (cur_stack->size < 0) {
        ERROR_CODE = BAD_SIZE;
        printf(RED"\nBAD_SIZE. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    if (cur_stack->capacity < 0) {
        ERROR_CODE = BAD_CAPACITY;
        printf(RED"\nBAD_CAPACITY. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    if (cur_stack->ptr_begin == NULL) {
        ERROR_CODE = BAD_PTR;
        printf(RED"\nBAD_PTR. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }
    if (cur_stack->hash != stack_hash_sum(cur_stack) ) {
        ERROR_CODE = BAD_HASH;
        canary value = stack_hash_sum(cur_stack);
        printf("hash: %X. new value: %X", cur_stack->hash, value);
        printf(RED"\nBAD HASH. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    if (cur_stack->canary_beg != cur_stack->ptr_begin[0]) {
        ERROR_CODE = BAD_CANARY_BEGIN;
        printf(RED"\nBAD_CANARY_BEGIN. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }
    abort();

}

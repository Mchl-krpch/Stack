#include <stdio.h>

#include "stack.h"
#include "output.h"

int STACK_IS_OK(stack_struct *cur_stack, char *path, int line, char *func_name) {
    //{ # # # # # } - error code with any error
    //1)size 2) capacity 3) ptr 4) hash 5) canary

    int special_code = 0;
    if (cur_stack->size < 0) {
        ERROR_CODE = BAD_SIZE;
        special_code += 10000;
        printf(RED"\nBAD_SIZE. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    if (cur_stack->capacity < 0) {
        ERROR_CODE = BAD_CAPACITY;
        special_code += 1000;
        printf(RED"\nBAD_CAPACITY. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    if (cur_stack->ptr_begin == nullptr) {
        ERROR_CODE = BAD_PTR;
        special_code += 100;
        printf(RED"\nBAD_PTR. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }
    if (cur_stack->hash != stack_hash_sum(cur_stack) ) {
        ERROR_CODE = BAD_HASH;
        special_code += 10;
        canary value = stack_hash_sum(cur_stack);
        //printf("hash: %X. new value: %X", cur_stack->hash, value);
        printf(RED"\nBAD HASH. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    if (cur_stack->canary_beg != cur_stack->ptr_begin[0]) {
        ERROR_CODE = BAD_CANARY_BEGIN;
        special_code += 1;
        printf(RED"\nBAD_CANARY_BEGIN. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    if (cur_stack->canary_end != cur_stack->ptr_begin[cur_stack->capacity - 1]) {
        ERROR_CODE = BAD_CANARY_END;
        special_code += 100000;
        printf(RED"\nBAD_CANARY_END. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    return special_code;
}

void stack_dump(stack_struct *cur_stack, char *func_name, int line) {
    if (cur_stack->ptr_begin == nullptr) {
        printf(RED"BAD STACK PTR: \t(ERROR)\n");
        abort();
    }

    int code = STACK_IS_OK(cur_stack, __FILE__, line, func_name);

    if (code == 0) {
        return;
    }

    else {

        printf("code: %d\n", code);

        printf(WHITE"%15s\t%-15s", "stack dump", "[ canary_end ]");
        if (cur_stack->ptr_begin[cur_stack->capacity - 1] != cur_stack->canary_end) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(WHITE"%15s\t%-15s", "stack dump", "[ Stack size ]");

        if (cur_stack->size < 0 || cur_stack->size > cur_stack->capacity) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(WHITE"%15s\t%-15s", "stack dump", "[ capacity ]");
        if (cur_stack->capacity < 0) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(WHITE"%15s\t%-15s", "stack dump", "[ ptr_begin ]");
        if (cur_stack->ptr_begin == nullptr) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(WHITE"%15s\t%-15s", "stack dump", "[ hash ]");
        if (cur_stack->hash != stack_hash_sum(cur_stack)) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(WHITE"%15s\t%-15s", "stack dump", "[ canary_beg ]");
        if (cur_stack->ptr_begin[0] != cur_stack->ptr_begin[0]) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        abort();
    }
}

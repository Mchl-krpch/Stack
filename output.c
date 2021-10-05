#include <stdio.h>

#include "stack.h"
#include "output.h"

int stack_is_ok(Stack *stack, char *path, int line, const char *func_name) {
    int special_code = 0;
    if (stack->size < 0) {
        special_code += 1;
        printf(RED"\nBAD_SIZE. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    if (stack->capacity < 0) {
        special_code += 1;
        printf(RED"\nBAD_CAPACITY. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    if (stack->data == nullptr) {
        special_code += 1;
        printf(RED"\nBAD_PTR. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }
    if (stack->hash != stack_hash(stack) ) {
        special_code += 1;
        printf(RED"\nBAD HASH. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    if (stack->canary_beg != stack->data[0]) {
        special_code += 1;
        printf(RED"\nBAD_CANARY_BEGIN. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    if (stack->canary_end != stack->data[stack->capacity - 1]) {
        special_code += 1;
        printf(RED"\nBAD_CANARY_END. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    return special_code;
}

void stack_dump(Stack *stack, const char *func_name, int line) {
    if (stack->data == nullptr) {
        printf(WHITE"%15s\t%-15s", "stack dump", "[ canary_end ]");
        printf(RED"%10s", "(ERROR)\n");
        abort();
    }

    int code = stack_is_ok(stack, __FILE__, line, func_name);

    if (code == 0) {
        return;
    }

    else {
        printf(WHITE"%15s\t%-15s", "stack dump", "[ canary_end ]");
        if (stack->data[stack->capacity - 1] != stack->canary_end) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(WHITE"%15s\t%-15s", "stack dump", "[ Stack size ]");
        if (stack->size < 0 || stack->size > stack->capacity) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(WHITE"%15s\t%-15s", "stack dump", "[ capacity ]");
        if (stack->capacity < 0) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(WHITE"%15s\t%-15s", "stack dump", "[ data ]");
        if (stack->data == nullptr) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(WHITE"%15s\t%-15s", "stack dump", "[ hash ]");
        if (stack->hash != stack_hash(stack)) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(WHITE"%15s\t%-15s", "stack dump", "[ canary_beg ]");
        if (stack->data[0] != stack->data[0]) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(RED"ERROR endng\n");

        abort();
    }
}

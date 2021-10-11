#include <stdio.h>

#include "stack.h"
#include "output.h"

#if DEBUG_LVL == 2
    #define DEBUG_LVL_2
    #define DEBUG_LVL_1
#else
    #if DEBUG_LVL == 1
        #define DEBUG_LVL_1
    #endif
#endif

#ifdef DEBUG_LVL_1
int stack_is_ok(Stack *stack, char *path, int line, const char *func_name) {
    int special_code = 0;

    if (stack->canary_beg != CANARY) {
        special_code += 1;
        printf(RED"\nBAD_CANARY_BEGIN. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    if (stack->capacity < 0) {
        special_code += 1;
        printf(RED"\nBAD_CAPACITY. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    if (stack->size < 0) {
        special_code += 1;
        printf(RED"\nBAD_SIZE. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    if (stack->data == nullptr) {
        special_code += 1;
        printf(RED"\nBAD_PTR. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
    }

    #ifdef DEBUG_LVL_2
        if (stack->hash != stack_hash(stack) ) {
            special_code += 1;
            printf(RED"\nBAD HASH. FILE: %s\nfunction: %s(%d)\n", path, func_name, line);
        }
    #endif //DEBUG_LVL_2

    if (stack->canary_end != CANARY) {
        printf("stack canary: %x const canary: %x", stack->canary_end, CANARY);
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
        printf(WHITE"%15s\t%-15s", "stack dump", "[ canary_beg ]");
        if (stack->canary_beg != CANARY) {
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

        #ifdef DEBUG_LVL_2
            printf(WHITE"%15s\t%-15s", "stack dump", "[ hash ]");
            if (stack->hash != stack_hash(stack)) {
                printf(RED"%10s", "\t(ERROR)\n");
            } else {
                printf(GREEN"%10s", "(OK)\n");
            }
        #endif //DEBUG_LVL_2

        printf(WHITE"%15s\t%-15s", "stack dump", "[ canary_end ]");
        if (stack->canary_end != CANARY) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(RED"ERROR endng\n");

        abort();
    }
}
#endif //DEBUG_LVL_1

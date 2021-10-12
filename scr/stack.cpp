#include <stdio.h>
#include <cstdint>
#include <assert.h>

#include "stack.h"
#include "stack_log.h"

#if DEBUG_LVL == 2
    #define DEBUG_LVL_1
    #define DEBUG_LVL_2
#else
    #if DEBUG_LVL == 1
        #define DEBUG_LVL_1
    #else
        #define HASH_CALCULATE
        #define stack_dump(stack, __func__, __LINE__);
    #endif
#endif

static uint64_t hash_buffer(char *buffer, size_t size) {
    int hash = 0;
    int add = 0x5bd1e995;

    for (int index = 0; index < size; index++) {
        hash += buffer[index] * index;
        hash = hash << 1;
    }
    //printf("lol: %d\n", hash);

    return hash;
}

static uint64_t stack_hash(Stack *stack) {
    assert(stack != NULL && "hash_sum: cur_stack = NULL");

    int m = 0x5bd1e995;

    int hash = 0;

    hash += hash_buffer((char *)stack->data, stack->capacity - 1);
    hash += hash_buffer((char *)stack + 4, sizeof(Stack) - 5 * sizeof(int));

    hash == hash >> 1;
    hash += 0x5bd1e995;

    //printf("%d", hash);

    return hash;
}


#ifdef DEBUG_LVL_1
static int stack_is_ok(Stack *stack) {
    int special_code = 0;

    if (stack == nullptr) {
        special_code += BAD_STRUCT_PTR;
    }

    if (stack->canary_beg != CANARY) {
        special_code += DEAD_CANARY_BEGIN;
    }

    if (stack->capacity < 0) {
        special_code += BAD_CAPACITY;
    }

    if (stack->size < 0) {
        special_code += BAD_SIZE;
    }

    if (stack->data == nullptr) {
        special_code += DATA_ERROR;
    }

    #ifdef DEBUG_LVL_2
        if (stack->hash != stack_hash(stack) ) {
            special_code += BAD_HASH;
            //printf("%d\n", stack_hash);
        }
    #endif //DEBUG_LVL_2

    if (stack->canary_end != CANARY) {
        special_code += DEAD_CANARY_END;
    }

    return special_code;
}

static void error_message(Stack *stack, const char *func_name, int line) {
    printf(RED"Error in function: %s(%d)\n", func_name, line);
}

static void stack_dump(Stack *stack, const char *func_name, int line) {
    if (stack == nullptr) {
        printf("Bad stack - struct ptr\n");
    }


    int code = stack_is_ok(stack);

    if (code == 0) {
        return;
    }

    else {
        error_message(stack, func_name, line);

        if (code % BAD_STRUCT_PTR != 0) {
            printf(WHITE"%15s\t%-15s", "stack dump", "[ stack_ptr ]");
            printf(RED"%10s", "(ERROR)\n");
            return;
        }

        printf(WHITE"%15s\t%-15s", "stack dump", "[ canary_beg ]");
        if ((code / DEAD_CANARY_BEGIN) % 10 != 0) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(WHITE"%15s\t%-15s", "stack dump", "[ Stack size ]");
        if ((code / BAD_CAPACITY) % 10 != 0) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(WHITE"%15s\t%-15s", "stack dump", "[ capacity ]");
        if ((code / BAD_SIZE) % 10 != 0) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(WHITE"%15s\t%-15s", "stack dump", "[ data ]");
        if ((code / DATA_ERROR) % 10 != 0) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        #ifdef DEBUG_LVL_2
            printf(WHITE"%15s\t%-15s", "stack dump", "[ hash ]");
            if ((code / BAD_HASH) % 10 != 0) {
                printf(RED"%10s", "\t(ERROR)\n");
            } else {
                printf(GREEN"%10s", "(OK)\n");
            }
        #endif //DEBUG_LVL_2

        printf(WHITE"%15s\t%-15s", "stack dump", "[ canary_end ]");
        if ((code / DEAD_CANARY_END) % 10 != 0) {
            printf(RED"%10s", "\t(ERROR)\n");
        } else {
            printf(GREEN"%10s", "(OK)\n");
        }

        printf(RED"ERROR endng\n");

        return;
    }
}
#endif //DEBUG_LVL_1

int stack_ctor(Stack *stack, size_t start_capacity) {
    assert(stack != nullptr && "stack_ctor: stack = nullptr");

    stack->size = 0;
    stack->capacity = start_capacity;

    #ifdef DEBUG_LVL_1
        stack->canary_beg = CANARY;
        stack->canary_end = CANARY;
    #endif //DEBUG_LVL_1

    stack->data = (sType *)calloc(stack->capacity, sizeof(sType));

    if (stack->data == nullptr) {
        #ifdef DEBUG_LVL_1
            stack_dump(stack, __func__, __LINE__);
        #endif //DEBUG_LVL_1

        return 0;
    }

    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif //DEBUG_LVL_2

    #ifdef DEBUG_LVL_1
            stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    return 0;
}

int stack_push (Stack *stack, sType new_elem) {
    assert(stack != nullptr && "std_push: cur_stack = NULL");

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    if (stack->size >= stack->capacity) {
        stack_resize(stack, stack->capacity * REALLOC_COEFF);
    }

    stack->data[stack->size] = new_elem;
    stack->size++;

    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif //DEBUG_LVL_2

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    return 0;
}

int stack_resize(Stack *stack, size_t increase_by) {
    assert(stack != nullptr);

    stack->capacity = increase_by;
    sType *temporary_data = (sType *) realloc(stack->data, increase_by * sizeof(sType));

    if (stack->data == nullptr) {
        #ifdef DEBUG_LVL_1
            stack_dump(stack, __func__, __LINE__);
        #endif //DEBUG_LVL_1

        return 0;
    }

    stack->data = temporary_data;

    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif //DEBUG_LVL_2

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    return 0;
}

int stack_pop (Stack *stack) {
    assert(stack != nullptr);

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    stack->size--;
    
    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif //DEBUG_LVL_2

    if (stack->size <= (stack->capacity / REALLOC_COEFF / REALLOC_COEFF) ) {
        stack_resize(stack, stack->capacity / REALLOC_COEFF);
    }

    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif //DEBUG_LVL_2

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    return 0;
}

int stack_dtor(Stack *stack) {
    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    for (int elem = 0; elem < stack->capacity - 1; elem++) {
        stack->data[elem] = 0;
    }

    stack->size       = 0;
    stack->capacity   = 0;

    #ifdef DEBUG_LVL_1
        stack->canary_end = 0;
        stack->canary_beg = 0;
    #endif //DEBUG_LVL_1

    free(stack->data);

    return 0;
}

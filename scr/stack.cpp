#include <stdio.h>
#include <cstdint>
#include <assert.h>

#include "stack.h"
#include "output.h"

#if DEBUG_LVL == 2
    #define DEBUG_LVL_1
    #define DEBUG_LVL_2
    #define HASH stack->hash = stack_hash(stack);
#else
    #if DEBUG_LVL == 1
        #define DEBUG_LVL_1
        #define HASH
    #else
        #define HASH
        #define stack_dump(stack, __func__, __LINE__);
    #endif
#endif

int CODE = 0;

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
        stack_dump(stack, __func__, __LINE__);
        return 0;
    }

    HASH

    return 0;
}

int stack_push (Stack *stack, sType new_elem) {
    assert(stack != nullptr && "std_push: cur_stack = NULL");

    stack_dump(stack, __func__, __LINE__);

    if (stack->size >= stack->capacity) {
        stack_change_cap(stack, stack->capacity * REALLOC_COEFF);
    }

    stack->data[stack->size] = new_elem;
    stack->size++;
    HASH


    stack_dump(stack, __func__, __LINE__);

    return 0;
}

int stack_change_cap(Stack *stack, size_t increase_by) {
    assert(stack != nullptr);

    stack->capacity = increase_by;
    stack->data = (sType *) realloc(stack->data, increase_by * sizeof(sType));

    if (stack->data == nullptr) {
        stack_dump(stack, __func__, __LINE__);
        return 0;
    }

    HASH

    stack_dump(stack, __func__, __LINE__);

    return 0;
}

int stack_pop (Stack *stack) {
    assert(stack != nullptr);

    stack_dump(stack, __func__, __LINE__);

    stack->size--;
    HASH

    if (stack->size <= (stack->capacity / REALLOC_COEFF / REALLOC_COEFF) ) {
        stack_change_cap(stack, stack->capacity / REALLOC_COEFF);
    }

    HASH

    stack_dump(stack, __func__, __LINE__);

    return 0;
}

uint64_t stack_hash(Stack *stack) {
    assert(stack != NULL && "hash_sum: cur_stack = NULL");

    int hash = 0;

    hash += hash_buffer((char *)stack->data, stack->capacity - 1);
    hash += hash_buffer((char *)stack, sizeof(Stack) - 4 * sizeof(int));

    return hash;
}

uint64_t hash_buffer(char *buffer, size_t size) {
    int hash = 0;

    for (int index = 0; index < size; index++) {
        hash += buffer[index] * index;
    }

    return hash;
}

int stack_dtor(Stack *stack) {
    for (int elem = 0; elem < stack->capacity - 1; elem++) {
        stack->data[elem] = FREE;
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

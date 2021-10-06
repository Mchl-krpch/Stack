#include <stdio.h>
#include <cstdint>

#include "assert.h"

#include "stack.h"
#include "output.h"

int CODE = 0;

int stack_ctor(Stack *stack, int start_capacity) {
    assert(stack != nullptr && "stack_ctor: stack = nullptr");

    stack->size = 0;
    stack->capacity = start_capacity;

    stack->canary_beg = Canary;
    stack->canary_end = Canary;

    stack->data = (sType *)calloc(stack->capacity, sizeof(sType));

    if (stack->data == nullptr) {
        stack_dump(stack, __func__, __LINE__);
    }

    stack->hash = stack_hash(stack);

    return 0;
}

int stack_push (Stack *stack, sType new_elem) {
    assert(stack != nullptr && "std_push: cur_stack = NULL");

    stack_dump(stack, __func__, __LINE__);

    if (stack->size >= stack->capacity) {
        stack_increase(stack);
    }

    else {
        stack->data[stack->size] = new_elem;
        stack->size++;
    }
    
    stack->hash = stack_hash(stack);

    stack_dump(stack, __func__, __LINE__);

    return 0;
}

int stack_increase (Stack *stack) {
    assert(stack != nullptr);

    stack_dump(stack, __func__, __LINE__);


    stack->capacity *= IncreaseMultiplier;
    stack->data = (sType *) realloc(stack->data, stack->capacity * sizeof(sType));

    if (stack->data == nullptr) {
        stack_dump(stack, __func__, __LINE__);
    }

    stack->hash = stack_hash(stack);

    stack_dump(stack, __func__, __LINE__);

    return 0;
}

int stack_pop (Stack *stack) {
    assert(stack != nullptr);

    stack_dump(stack, __func__, __LINE__);

    stack->size--;
    stack->hash = stack_hash(stack);

    if (stack->size <= (stack->capacity * 0.4) ) {
        stack_decrease(stack);
        stack->hash = stack_hash(stack);
    }

    stack_dump(stack, __func__, __LINE__);

    return 0;
}

int stack_decrease (Stack *stack) {
    assert(stack != nullptr);

    stack_dump(stack, __func__, __LINE__);

    stack->capacity /= IncreaseMultiplier;

    stack->data = (int *) realloc(stack->data, stack->capacity * sizeof(int));

    if (stack->data == nullptr) {
        stack_dump(stack, __func__, __LINE__);
    }

    stack->hash = stack_hash(stack);

    stack_dump(stack, __func__, __LINE__);


    return 0;
}

int stack_shrink (Stack *stack) {
    assert(stack != nullptr);

    stack_dump(stack, __func__, __LINE__);

    stack->data = (int *) realloc(stack->data, stack->size * sizeof(int));

    stack->capacity = stack->size;

    if (stack->data == nullptr) {
        stack_dump(stack, __func__, __LINE__);
    }

    stack->data[stack->capacity - 1] = Canary;

    stack->hash = stack_hash(stack);

    stack_dump(stack, __func__, __LINE__);


    return 0;
}

uint64_t stack_hash(Stack *stack) {
    assert(stack != NULL && "hash_sum: cur_stack = NULL");

    int res = 0;

    char *data = (char *)stack->data;

    for (int index = 0; index < stack->capacity - 1; index++) {
        res += data[index] * index;
    }

    char *data2 = (char *)stack;
    for (int index = 0; index < 4 * sizeof(int); index++) {
        res += data2[index] * index;
    }

    //printf("hash: %d sizeof: %d", res, sizeof(Stack));

    return res;
}

uint64_t murmurhash(char *key, unsigned int len) {
    const unsigned int mur = 0x5bd1e995;
    const unsigned int special_code = 0;
    const int r = 24;

    unsigned int h = special_code ^ len;
    const unsigned char *data = (const unsigned char *)key;

    uint64_t k;
    while (len >= 4) {
        k = data[0];
        k |= data[1] << 8;
        k |= data[1] << 16;
        k |= data[1] << 24;

        k *= mur;
        k ^= k >> r;
        k *= mur;

        h *= mur;
        h ^=k;

        data += 4;
        len -=4;
    }

    switch (len) {
        case 3:
            h ^= data[2] <<16;
        case 2:
            h ^= data[1] <<8;
        case 1:
            h ^= data[0];
            h *= mur;

    };

    h ^= h >> 13;
    h *= mur;
    h ^= h >> 15;

    return h;
}

int stack_dtor(Stack *stack) {
    for (int elem = 0; elem < stack->capacity - 1; elem++) {
        stack->data[elem] = Free;
    }

    stack->size       = 0;
    stack->capacity   = 0;
    stack->canary_end = 0;
    stack->canary_beg = 0;

    free(stack->data);

    return 0;
}

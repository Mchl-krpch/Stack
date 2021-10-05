#include <stdio.h>

#include "assert.h"

#include "stack.h"
#include "output.h"

const int INCREASE_MULTIPLIER = 2;

const int TOXIC  = 0xBADD;
const int FREE   = 0xF2EE;

int CODE = 0;


int stack_ctor(Stack *stack, int start_size) {
    assert(stack != nullptr && "stack_ctor: stack = nullptr");

    stack->size = start_size;
    stack->capacity = start_size + 2;

    stack->canary_beg = CANARY;
    stack->canary_end = CANARY;

    stack->ptr_begin = (int *)calloc(stack->capacity, sizeof(int));
    for (int index = 0; index < stack->capacity - 1; index++) {
        stack->ptr_begin[index] = TOXIC;
    }

    stack->ptr_begin[0] = stack->canary_beg;

    stack->ptr_begin[stack->capacity - 1] = stack->canary_end;

    stack->hash = stack_hash(stack);
}

int stack_push (Stack *stack, int new_elem) {
    assert(stack != nullptr && "std_push: cur_stack = NULL");

    stack_dump(stack, (char *)__func__, __LINE__);

    if (stack->size >= stack->capacity - 2) {
        stack_increase(stack);
    }

    stack->size++;
    if ((stack->ptr_begin[stack->size] != 0xBADD)) {
        stack_dump(stack, (char *)__func__ , __LINE__);
    }
    else {
        stack->ptr_begin[stack->size] = new_elem;
    }

    stack->hash = stack_hash(stack);

    stack_dump(stack, (char *)__func__, __LINE__);
}

int stack_increase (Stack *stack) {
    assert(stack != nullptr);

    stack_dump(stack, (char *)__func__, __LINE__);

    int save_canary = stack->canary_end;
    stack->ptr_begin[stack->capacity - 1] = TOXIC;

    stack->capacity *= INCREASE_MULTIPLIER;
    stack->ptr_begin = (int *) realloc(stack->ptr_begin, stack->capacity * sizeof(int));

    for (int elem = stack->size + 1; elem < stack->capacity - 1; elem++) {
        stack->ptr_begin[elem] = TOXIC;
    }

    stack->ptr_begin[stack->capacity-1] = save_canary;

    stack->hash = stack_hash(stack);

    stack_dump(stack, (char *)__func__, __LINE__);
}

int stack_pop (Stack *stack, size_t pop_by) {
    assert(stack != nullptr);

    stack_dump(stack, (char *)__func__, __LINE__);

    for (int step = 0; step < pop_by; step++) {
        stack->size--;
        stack->ptr_begin[stack->size + 1] = TOXIC;
        stack->hash = stack_hash(stack);

        if (stack->size < ( (stack->capacity / INCREASE_MULTIPLIER)) - 2) {
            stack_decrease(stack);
        }

        stack->hash = stack_hash(stack);
    }

    stack_dump(stack, (char *)__func__, __LINE__);
    return 0;
}

int stack_decrease (Stack *stack) {
    assert(stack != nullptr);

    stack_dump(stack, (char *)__func__, __LINE__);


    int save_canary = stack->ptr_begin[stack->capacity - 1];
    stack->ptr_begin[stack->capacity - 1] = TOXIC;

    stack->capacity /= INCREASE_MULTIPLIER;

    stack->ptr_begin = (int *) realloc(stack->ptr_begin, stack->capacity * sizeof(int));

    stack->ptr_begin[stack->capacity - 1] = save_canary;

    stack->hash = stack_hash(stack);

    stack_dump(stack, (char *)__func__, __LINE__);
    
    return 0;
}

int stack_hash(Stack *stack) {
    assert(stack != NULL && "hash_sum: cur_stack = NULL");

    int res = 0;

    char *data = (char *)stack->ptr_begin;

    for (int index = 0; index < stack->capacity - 1; index++) {
        res += data[index] * index;
    }

    char *data2 = (char *)stack;
    for (int index = 0; index < sizeof(Stack) - sizeof(int) * 2; index++) {
        res += data2[index] * index;
    }

    return res;
}

int murmurhash(char *key, unsigned int len) {
    const unsigned int mur = 0x5bd1e995;
    const unsigned int special_code = 0;
    const int r = 24;

    unsigned int h = special_code ^ len;
    const unsigned char *data = (const unsigned char *)key;

    unsigned int k;
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
        stack->ptr_begin[elem] = FREE;
    }

    stack->size       = 0;
    stack->capacity   = 0;
    stack->canary_end = 0;
    stack->canary_beg = 0;

    free(stack->ptr_begin);
    return 0;
}

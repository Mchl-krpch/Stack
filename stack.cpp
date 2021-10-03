#include <stdio.h>
#include "assert.h"

#include "stack.h"
#include "output.h"


const int start_cap = 2;
const int stack_increase_multiplier = 2;

int toxic = 0xBADD;

int ERROR_CODE = -1;


void stack_ctor(stack_struct *new_stack) {
    assert(new_stack != NULL && "std_ctor: new_stack = NULL");

    new_stack->capacity = start_cap;
    new_stack->size     = new_stack->capacity - 2;

    new_stack->canary_beg = 0xB12DBE << sizeof(stack_type);
    new_stack->canary_end = 0xB12DE  << sizeof(stack_type);

    new_stack->ptr_begin = (stack_type *)calloc(new_stack->capacity, sizeof(stack_type));
    for (int index = 0; index < new_stack->capacity - 1; index++) {
        new_stack->ptr_begin[index] = toxic;
    }

    new_stack->hash = stack_hash_sum(new_stack);

    new_stack->ptr_begin[0] = new_stack->canary_beg;

    new_stack->ptr_begin[new_stack->capacity - 1] = new_stack->canary_end;

    printf("stack constructor: created\n");
}

void stack_push (stack_struct *cur_stack, stack_type new_elem) {
    assert(cur_stack != nullptr && "std_push: cur_stack = NULL");

    if (cur_stack->size >= cur_stack->capacity - 2) {
        stack_increase(cur_stack);
    }

    cur_stack->size++;
    if (!(cur_stack->ptr_begin[cur_stack->size] == toxic)) {
        printf("BAD VALUE IN STACK size: %d value: %d\n", cur_stack->size, cur_stack->ptr_begin[cur_stack->size]);
    }
    else {
        cur_stack->ptr_begin[cur_stack->size] = new_elem;
    }

    cur_stack->hash = stack_hash_sum(cur_stack);

    //printf("%d, %d", cur_stack->hash, stack_hash_sum(cur_stack));
    stack_dump(cur_stack, (char *)__func__, __LINE__);
}

void stack_increase (stack_struct *cur_stack) {
    assert(cur_stack != nullptr);

    canary save_canary = cur_stack->canary_end;
    cur_stack->ptr_begin[cur_stack->capacity - 1] = toxic;

    cur_stack->capacity *= stack_increase_multiplier;
    cur_stack->ptr_begin = (int *) realloc(cur_stack->ptr_begin, cur_stack->capacity * sizeof(int));

    for (int elem = cur_stack->size + 1; elem < cur_stack->capacity; elem++) {
        cur_stack->ptr_begin[elem] = toxic;
    }

    cur_stack->ptr_begin[cur_stack->capacity-1] = save_canary;

    cur_stack->hash = stack_hash_sum(cur_stack);
}

void stack_pop (stack_struct *cur_stack, size_t pop_by) {
    assert(cur_stack != nullptr);

    stack_dump(cur_stack, (char *)__func__, __LINE__);

    for (int step = 0; step < pop_by; step++) {
        cur_stack->size--;
        cur_stack->ptr_begin[cur_stack->size + 1] = toxic;

        if (cur_stack->size < ( (cur_stack->capacity / stack_increase_multiplier)) - 2) {
            stack_decrease(cur_stack);
        }

        cur_stack->hash = stack_hash_sum(cur_stack);
    }

    stack_dump(cur_stack, (char *)__func__, __LINE__);
}

void stack_decrease (stack_struct *cur_stack) {
    assert(cur_stack != nullptr);

    //stack_dump(cur_stack, (char *)__func__, __LINE__);


    canary save_canary = cur_stack->ptr_begin[cur_stack->capacity - 1];
    cur_stack->ptr_begin[cur_stack->capacity - 1] = toxic;

    cur_stack->capacity /= stack_increase_multiplier;

    cur_stack->ptr_begin = (int *) realloc(cur_stack->ptr_begin, cur_stack->capacity * sizeof(int));

    cur_stack->ptr_begin[cur_stack->capacity - 1] = save_canary;

    cur_stack->hash = stack_hash_sum(cur_stack);

    stack_dump(cur_stack, (char *)__func__, __LINE__);

}

canary stack_hash_sum (stack_struct *cur_stack) {
    assert(cur_stack != NULL && "hash_sum: cur_stack = NULL");

    unsigned long long res = 0;

    for (unsigned index = 0; index < cur_stack->size; index++) {
        res += cur_stack->ptr_begin[index];
        //res  = res ^ cur_stack->hash / 2;
    }

    return res;
}

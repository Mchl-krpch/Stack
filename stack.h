#ifndef STACK_2_STACK_H
#define STACK_2_STACK_H

#include <cmath>

int const CANARY = 0xB12D00;

typedef  struct Stack{
    int canary_beg;    //begin bird
    int capacity;    //max size
    int size;    //cur number of element
    int *ptr_begin;    //stack path
    int hash;    //stack hash
    int canary_end;    //end bird

}Stack;

int stack_ctor (Stack *new_stack, int start_size);
int stack_increase (Stack *cur_stack);
int stack_push (Stack *cur_stack, int value);
int stack_decrease (Stack *cur_stack);
int stack_pop (Stack *cur_stack, size_t pop_by);
int stack_hash (Stack *cur_stack);
//alternative hash function
int murmurhash (char *key, unsigned int len);
int stack_dtor (Stack *new_stack);


#endif //STACK_2_STACK_H

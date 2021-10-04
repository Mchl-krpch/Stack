#ifndef STACK_2_STACK_H
#define STACK_2_STACK_H

#include <cmath>


typedef       int stack_type;
typedef long long int canary;

extern int toxic;
extern int ERROR_CODE;


typedef  struct stk{
    canary         canary_beg;    //begin bird
    int              capacity;    //max size
    int                  size;    //cur number of element
    stack_type     *ptr_begin;    //stack path
    canary               hash;    //stack hash
    canary         canary_end;    //end bird

}stack_struct;

void stack_ctor         (stack_struct *new_stack               );
void stack_increase     (stack_struct *cur_stack               );
void stack_push         (stack_struct *cur_stack, int value    );
void stack_decrease     (stack_struct *cur_stack               );
void stack_pop          (stack_struct *cur_stack, size_t pop_by);
canary stack_hash_sum   (stack_struct *cur_stack               );
void stack_dtor         (stack_struct *new_stack               );


#endif //STACK_2_STACK_H

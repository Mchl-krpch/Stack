#ifndef STACK_2_STACK_H
#define STACK_2_STACK_H

#include <cmath>


typedef       int stack_type;
typedef long long int canary;

extern int toxic;
extern int ERROR_CODE;


typedef  struct stk{
    int              capacity = -1;    //максимальный размер
    int                  size = -1;    //количество элементов сейчас
    canary         canary_beg = NULL;    //канарейка в начале стека
    stack_type     *ptr_begin = NULL;    //путь к началу стака
    canary         canary_end = NULL;    //канарейка в конце массива
    canary               hash = NULL;    //Хэш стака

}stack_struct;

void stack_ctor         (stack_struct *new_stack               );
void stack_increase     (stack_struct *cur_stack               );
void stack_push         (stack_struct *cur_stack, int value    );
void stack_decrease     (stack_struct *cur_stack               );
void stack_pop          (stack_struct *cur_stack, size_t pop_by);
canary stack_hash_sum   (stack_struct *cur_stack               );
void stack_dtor         (stack_struct *new_stack               );


#endif //STACK_2_STACK_H

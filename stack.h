#ifndef STACK_2_STACK_H
#define STACK_2_STACK_H

#include <cmath>


typedef       int stack_type;
typedef long long int canary;

extern int toxic;
extern int ERROR_CODE;


typedef  struct stk{
    int              capacity;    //максимальный размер
    int                  size;    //количество элементов сейчас
    canary         canary_beg;    //канарейка в начале стека
    stack_type     *ptr_begin;    //путь к началу стака
    canary         canary_end;    //канарейка в конце массива
    canary               hash;    //Хэш стака

}stack_struct;

void stack_ctor         (stack_struct *new_stack               );
void stack_increase     (stack_struct *cur_stack               );
void stack_push         (stack_struct *cur_stack, int value    );
void stack_decrease     (stack_struct *cur_stack               );
void stack_pop          (stack_struct *cur_stack, size_t pop_by);
canary stack_hash_sum   (stack_struct *cur_stack               );
void stack_dtor         (stack_struct *new_stack               );


void place_canaries(stack_struct *cur_stack);

#define check_stack(message) \
    printf("path: %s(%d), mes: %s", __FILE__, __LINE__, #message);

#endif //STACK_2_STACK_H

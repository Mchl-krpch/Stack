#ifndef STACK_2_STACK_H
#define STACK_2_STACK_H

#include <cmath>
#include <cstdint>

int const Canary = 0xB12D00;
int const IncreaseMultiplier = 2;
int const DecreaseMultiplier = 0.4;

int const Toxic  = 0xBADD;
int const Free   = 0xF2EE;

typedef int sType;

typedef  struct{
    int canary_beg;   //begin bird
    int capacity;     //max size
    int size;         //cur number of element
    int *data;        //stack path
    uint64_t hash;    //stack hash
    int canary_end;	  //end bird

}Stack;

int stack_ctor (Stack *new_stack, int start_capacity );
int stack_increase (Stack *cur_stack );
int stack_push (Stack *cur_stack, sType value );
int stack_decrease (Stack *cur_stack );
int stack_shrink (Stack *stack ); 
int stack_pop (Stack *cur_stack );
uint64_t stack_hash (Stack *cur_stack );
uint64_t murmurhash (char *key, unsigned int  );
int stack_dtor (Stack *new_stack );


#endif //STACK_2_STACK_H

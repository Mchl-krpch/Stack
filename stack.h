#ifndef STACK_2_STACK_H
#define STACK_2_STACK_H

#include <cmath>
#include <cstdint>

int const Canary = 0xB12D00;
typedef int sType;

typedef  struct{
    int 	   canary_beg;  //begin bird
    int 	   capacity;    //max size
    int 	   size;        //cur number of element
    int 	  *data;   //stack path
    uint64_t   hash;    	//stack hash
    int 	   canary_end;	//end bird

}Stack;

int stack_ctor 		  (Stack *new_stack, int start_size		);
int stack_increase 	  (Stack *cur_stack                		);
int stack_push 	      (Stack *cur_stack, sType value		);
int stack_decrease 	  (Stack *cur_stack                		);
int stack_shrink      (Stack *stack                         ); 
int stack_pop         (Stack *cur_stack              		);
uint64_t stack_hash   (Stack *cur_stack                		);
uint64_t murmurhash   (char        *key, unsigned int len	);
int stack_dtor		  (Stack *new_stack                		);


#endif //STACK_2_STACK_H

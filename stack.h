#ifndef STACK_2_STACK_H
#define STACK_2_STACK_H

#include <cmath>
#include <cstdint>

#define DEBUG_LVL 0

#if DEBUG_LVL == 2
    #define DEBUG_LVL_2
	#define DEBUG_LVL_1
#else
	#if DEBUG_LVL == 1
		#define DEBUG_LVL_1
	#endif
#endif

typedef int sType;

enum CONSTANS{
	CANARY = 0xB12D00,
	REALLOC_COEFF = 2,

	TOXIC = 0xBADD,
	FREE = 0xF2EE
};

typedef  struct{
	#ifdef DEBUG_LVL_1
    	int		canary_beg;	//begin bird
    #endif DEBUG_LVL_1

    size_t		capacity;	//max size
    size_t		size;		//cur number of element
    sType		*data;		//stack path

    #ifdef DEBUG_LVL_2
    	uint64_t	hash;	//stack hash
    #endif DEBUG_LVL_2

    #ifdef DEBUG_LVL_1
    	int		canary_end;	//end bird
    #endif DEBUG_LVL_1

}Stack;

int stack_ctor			(Stack *new_stack, size_t start_capacity);
int stack_change_cap	(Stack *stack, size_t increase_by);

int stack_push			(Stack *stack, sType value);
int stack_pop			(Stack *stack);
uint64_t stack_hash		(Stack *stack);
uint64_t hash_buffer	(char *buffer, size_t size);
int stack_dtor			(Stack *stack);


#endif //STACK_2_STACK_H

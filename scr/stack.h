#ifndef STACK_2_STACK_H
#define STACK_2_STACK_H

#include <cmath>
#include <cstdint>

#define DEBUG_LVL 2

#if DEBUG_LVL == 2
    #define DEBUG_LVL_2
	#define DEBUG_LVL_1
#else
	#if DEBUG_LVL == 1
		#define DEBUG_LVL_1
	#endif
#endif

typedef int sType;

#define GREEN  "\x1b[32m"
#define RED    "\x1b[31m"
#define WHITE  "\e[0;37m"

enum CONSTANS{
	CANARY = 0xB12D00,
	REALLOC_COEFF = 2,
};

enum EXIT_CODES {
    NO_ERRORS = 0,
    BAD_STRUCT_PTR = 1,

    #ifdef DEBUG_LVL_1
        DEAD_CANARY_BEGIN = 10,
    #endif DEBUG_LVL_1

    BAD_CAPACITY = 100,
    BAD_SIZE = 1000,
    DATA_ERROR = 10000,

    #ifdef DEBUG_LVL_2
        BAD_HASH = 100000,
    #endif DEBUG_LVL_2

    #ifdef DEBUG_LVL_1
        DEAD_CANARY_END = 1000000
    #endif DEBUG_LVL_1
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

int stack_resize		(Stack *stack, size_t increase_by);
int stack_push			(Stack *stack, sType value);
int stack_pop			(Stack *stack);

int stack_dtor			(Stack *stack);

#endif //STACK_2_STACK_H

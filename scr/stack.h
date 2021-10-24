#ifndef STACK_2_STACK_H
#define STACK_2_STACK_H

#include <cmath>
#include <cstdint>

///\param DEBUG_LVL mesure up level of debug check
///\param DEBUG_LVL_1 contains only canaries checks
///\param DEBUG_LVL_2 contains optional hash check
#define DEBUG_LVL 2


///\param LOG_INFO contains information that user want or not create logfile
///param LOG_INFO_1 create logfile
///param LOG_INFO_0 don't use logfile
#define LOG_INFO 1

#if DEBUG_LVL == 2
    #define DEBUG_LVL_2
	#define DEBUG_LVL_1
#else
	#if DEBUG_LVL == 1
		#define DEBUG_LVL_1
	#endif
#endif

typedef int sType;

#if LOG_INFO == 1
extern char name[60];
#endif

///\brief contains main constants in stack
///\param CANARY special value which control content in stack
///\param REALLOC_COEFF corresponds to multiplier of stack increase/decrease
enum CONSTANS{
	CANARY = 0xB12D00,
	REALLOC_COEFF = 2
};

///\brief contains speciale codes that mesure up what mistakes hapans if something went wrong 
///\param NO_ERRORS
///\param BAD_STRUCT_PTR
///\param DEAD_CANARY_BEGIN
///\param BAD_CAPACITY
///\param BAD_SIZE
///\param DATA_ERROR
///\param BAD_HASH
///\param DEAD_CANARY_END
enum EXIT_CODES {
    NO_ERRORS = 0,
    BAD_STRUCT_PTR = 1,

    #ifdef DEBUG_LVL_1
        DEAD_CANARY_BEGIN = 1 << 1,
    #endif DEBUG_LVL_1

    BAD_CAPACITY = 1 << 2,
    BAD_SIZE = 1 << 3,
    DATA_ERROR = 1 << 4,

    #ifdef DEBUG_LVL_2
        BAD_HASH = 1 << 5,
    #endif DEBUG_LVL_2

    #ifdef DEBUG_LVL_1
        DEAD_CANARY_END = 1 << 6,
    #endif DEBUG_LVL_1
};

///\brief it is a structure of the stack
///\param canary_beg spacial value that places at the begining structure of stack and save it's content
///\param capacity max number of elements in stack
///\param size current number of elements
///\param data content of the stack
///\param hash special value that control conten of stack's stack
///\param canary_end spacial value that places at the end structure of stack and save it's content
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
        int     stack_is_not_work; //begin bird
    #endif DEBUG_LVL_1

    #ifdef DEBUG_LVL_1
    	int		canary_end;	//end bird
    #endif DEBUG_LVL_1

}Stack;

///\brief create stack
///\param new_stack stack example 
///\param start_capacity start max number of elements in stack
EXIT_CODES stack_ctor			(Stack *new_stack, size_t start_capacity, char *log_file);

///\brief changes capacity of stack 
///\param new_stack stack example 
///\param increase_by in what capacity we change current capacity of stack
EXIT_CODES stack_resize		(Stack *stack, size_t increase_by);

///\brief changes capacity of stack 
///\param new_stack stack example 
///\param increase_by in what capacity we change current capacity of stack
EXIT_CODES stack_push			(Stack *stack, sType value);

///\brief returns last element in stack
///\param new_stack stack example 
EXIT_CODES stack_top           (Stack *stack, sType *top_element);

///\brief delete last element
///\param new_stack stack example 
EXIT_CODES stack_pop			(Stack *stack);

///\brief delete stack example
///\param new_stack stack example 
EXIT_CODES stack_dtor			(Stack *stack);

///\brief create special file which contains stack work information
///\param path path of logfile
void create_log_file(char *path);

#endif //STACK_2_STACK_H

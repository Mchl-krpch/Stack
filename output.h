#ifndef STACK_2_OUTPUT_H
#define STACK_2_OUTPUT_H

#include "stack.h"

#define DEBUG_LEVEL_DISABLE   0
#define DEBUG_LEVEL_FAST      1
#define DEBUG_LEVEL_EXPENSIVE 2

#define LOCATION __func__, __LINE__


#define GREEN  "\x1b[32m"
#define RED    "\x1b[31m"
#define WHITE "\e[0;37m"


enum ERRORS_CODES {
    NO_ERRORS                    = 0,
    REALLOCATION_INCREASE_ERROR  = 1,
    BAD_PTR                      = 2,
    BAD_CAPACITY                 = 3,
    BAD_SIZE                     = 4,
    BAD_HASH                     = 5,
    BAD_CANARY_BEGIN             = 6,
    BAD_CANARY_END               = 7

};

int STACK_IS_OK(stack_struct *cur_stack, char *path, int line, char *func_name);

void stack_dump(stack_struct *cur_stack, char *func_name, int line);

void STACK_VALID(stack_struct *cur_stack);

#endif //STACK_2_OUTPUT_HS

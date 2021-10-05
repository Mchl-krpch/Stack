#ifndef STACK_2_OUTPUT_H
#define STACK_2_OUTPUT_H

#include "stack.h"

#define GREEN  "\x1b[32m"
#define RED    "\x1b[31m"
#define WHITE  "\e[0;37m"


int stack_is_ok(Stack *cur_stack, char *path, int line, const char *func_name);

void stack_dump(Stack *cur_stack, const char *func_name, int line);

#endif //STACK_2_OUTPUT_HS

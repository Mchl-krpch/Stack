#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <assert.h>

#include "stack.h"

#if DEBUG_LVL == 2
    #define DEBUG_LVL_1
    #define DEBUG_LVL_2
#else
    #if DEBUG_LVL == 1
        #define DEBUG_LVL_1
    #endif
#endif

///\brief calculate hash of buffer of bites
///\param buffer example of buffer
///\param size size of example of buffer
static uint64_t hash_buffer(char *buffer, size_t size) {
    int hash = 0;
    int add = 0x5bd1e995;

    for (int index = 0; index < size; index++) {
        hash += buffer[index] * index;
        hash = hash << 1;
        hash = hash ^ add;
    }
    //printf("Hash now: %d\n", hash);

    return hash;
}

///\brief calculate hash of stack 
///\param stack example of stack
static uint64_t stack_hash(Stack *stack) {
    assert(stack != NULL && "hash_sum: cur_stack = NULL");

    int m = 0x5bd1e995;

    int hash = 0;

    hash += hash_buffer((char *)stack->data, stack->capacity - 1);
    hash += hash_buffer((char *)stack + 4, sizeof(Stack) - 5 * sizeof(int));

    //hash == hash >> 1;
    //hash += 0x5bd1e995;

    //printf("%d", hash);

    return hash;
}

///\brief count errors in stack's work
///\param buffer example of stack
#ifdef DEBUG_LVL_1
static int stack_is_ok(Stack *stack) {
    int special_code = 0;

    if (stack == nullptr) {
        special_code += BAD_STRUCT_PTR;
    }

    if (stack->canary_beg != CANARY) {
        special_code += DEAD_CANARY_BEGIN;
    }

    if (stack->capacity < 0) {
        special_code += BAD_CAPACITY;
    }

    if (stack->size < 0) {
        special_code += BAD_SIZE;
    }

    if (stack->data == nullptr) {
        special_code += DATA_ERROR;
    }

    #ifdef DEBUG_LVL_2
        if (stack->hash != stack_hash(stack) ) {
            special_code += BAD_HASH;
            //printf("%d\n", stack_hash);
        }
    #endif //DEBUG_LVL_2

    if (stack->canary_end != CANARY) {
        special_code += DEAD_CANARY_END;
    }

    return special_code;
}

///\brief can write custom message in stdout with adress of error
///\param stack example of stack
///\param func_name function which contains error
///\param line line where happens error
static void error_message(Stack *stack, const char *func_name, int line) {
    printf("Error in function: %s(%d)\n", func_name, line);
}

///\brief checks stack's condition
///\param output_file output file
///\param stack example of stack
static void print_check_stack(FILE *output_file, Stack *stack) {
    fprintf(output_file,
        "Stack preview:\n"
        "\tcanary begin: %x\n"
        "\tcapacity: %d\n"
        "\tsize: %d\n"
        "\thash: %d\n"
        "\tcanary end: %x\n\n",

        stack->canary_beg,
        stack->capacity,
        stack->size,
        stack->hash,
        stack->canary_end);
}

///\brief can write information about stack's condition
///\param func_name name of current function
///\param line current line which may contain an error
///\param error_code this code measure up that function contains or not error
static void update_log_file(Stack *stack, const char *func_name, int line, int error_code) {
    FILE* output_file = fopen("../output.txt", "a");
    fputs("_____________________\n", output_file);
    //fputs("%s\n", __TIME__);

    if (error_code == 0) {
        fprintf(output_file, "Function: %s(%d) Work correct\n\n", func_name, line);

        print_check_stack(output_file, stack);

        fprintf(output_file, "%s\n\n", __TIME__);
    }

    if ((error_code / DEAD_CANARY_BEGIN) % 10 != 0) {
        fprintf(output_file, "Function: %s(%d) ERROR\n\n", func_name, line);

        fprintf(output_file, "BAD CANARY:\n");

        print_check_stack(output_file, stack);

        fprintf(output_file, "%s\n\n", __TIME__);
    }

    if ((error_code / BAD_CAPACITY) % 10 != 0) {
        fprintf(output_file, "Function: %s(%d) ERROR\n\n", func_name, line);

        fprintf(output_file, "BAD CAPACITY:\n");

        print_check_stack(output_file, stack);

        fprintf(output_file, "%s\n\n", __TIME__);
    }

    if ((error_code / BAD_SIZE) % 10 != 0) {
        fprintf(output_file, "Function: %s(%d) ERROR\n\n", func_name, line);

        fprintf(output_file, "BAD SIZE:\n");

        print_check_stack(output_file, stack);

        fprintf(output_file, "%s\n\n", __TIME__);
    }

    if ((error_code / DATA_ERROR) % 10 != 0) {
        fprintf(output_file, "Function: %s(%d) ERROR\n\n", func_name, line);

        fprintf(output_file, "BAD DATA:\n");

        print_check_stack(output_file, stack);

        fprintf(output_file, "%s\n\n", __TIME__);
    }

    #ifdef DEBUG_LVL_2
    if ((error_code / BAD_HASH) % 10 != 0) {
        fprintf(output_file, "Function: %s(%d) ERROR\n\n", func_name, line);

        fprintf(output_file, "BAD HASH:\n");

        print_check_stack(output_file, stack);

        fprintf(output_file, "%s\n\n", __TIME__);
    }
     #endif //DEBUG_LVL_2

    if ((error_code / DEAD_CANARY_END) % 10 != 0) {
        fprintf(output_file, "Function: %s(%d) ERROR\n\n", func_name, line);

        fprintf(output_file, "BAD CANARY_END:\n");

        print_check_stack(output_file, stack);

        fprintf(output_file, "%s\n\n", __TIME__);
    }

    return;
}

///\brief analog of function "update_log_file" but it is work with console out
static void stack_dump(Stack *stack, const char *func_name, int line) {
    if (stack == nullptr) {
        printf("Bad stack - struct ptr\n");
    }


    int code = stack_is_ok(stack);
    update_log_file(stack, func_name, line, code);

    if (code == 0) {
        return;
    }

    else {
        error_message(stack, func_name, line);

        if (code % BAD_STRUCT_PTR != 0) {
            printf("%15s\t%-15s", "stack", "[ stack_ptr ]");
            printf("%10s", "\t(ERROR)\n");
            return;
        }

        printf("%15s\t%-15s", "stack", "[ canary_beg ]");
        if ((code / DEAD_CANARY_BEGIN) % 10 != 0) {
            printf("%10s", "\t(ERROR)\n");
        } else {
            printf("%10s", "(OK)\n");
        }

        printf("%15s\t%-15s", "stack", "[ Stack size ]");
        if ((code / BAD_CAPACITY) % 10 != 0) {
            printf("%10s", "\t(ERROR)\n");
        } else {
            printf("%10s", "(OK)\n");
        }

        printf("%15s\t%-15s", "stack", "[  capacity  ]");
        if ((code / BAD_SIZE) % 10 != 0) {
            printf("%10s", "\t(ERROR)\n");
        } else {
            printf("%10s", "(OK)\n");
        }

        printf("%15s\t%-15s", "stack", "[    data    ]");
        if ((code / DATA_ERROR) % 10 != 0) {
            printf("%10s", "\t(ERROR)\n");
        } else {
            printf("%10s", "(OK)\n");
        }

        #ifdef DEBUG_LVL_2
            printf("%15s\t%-15s", "stack", "[    hash    ]");
            if ((code / BAD_HASH) % 10 != 0) {
                printf("%10s", "\t(ERROR)\n");
            } else {
                printf("%10s", "(OK)\n");
            }
        #endif //DEBUG_LVL_2

        printf("%15s\t%-15s", "stack", "[ canary_end ]");
        if ((code / DEAD_CANARY_END) % 10 != 0) {
            printf("%10s", "\t(ERROR)\n");
        } else {
            printf("%10s", "(OK)\n");
        }

        return;
    }
}
#endif //DEBUG_LVL_1

int stack_ctor(Stack *stack, size_t start_capacity) {
    assert(stack != nullptr && "stack_ctor: stack = nullptr");

    stack->size = 0;
    stack->capacity = start_capacity;

    #ifdef DEBUG_LVL_1
        stack->canary_beg = CANARY;
        stack->canary_end = CANARY;
    #endif //DEBUG_LVL_1

    stack->data = (sType *)calloc(stack->capacity, sizeof(sType));

    if (stack->data == nullptr) {
        #ifdef DEBUG_LVL_1
            stack_dump(stack, __func__, __LINE__);
        #endif //DEBUG_LVL_1

        return 0;
    }

    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif //DEBUG_LVL_2

    #ifdef DEBUG_LVL_1
            stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    return 0;
}

int stack_push (Stack *stack, sType new_elem) {
    assert(stack != nullptr && "std_push: cur_stack = NULL");

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    if (stack->size >= stack->capacity) {
        stack_resize(stack, stack->capacity * REALLOC_COEFF);
    }

    stack->data[stack->size] = new_elem;
    stack->size++;

    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif //DEBUG_LVL_2

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    return 0;
}

int stack_resize(Stack *stack, size_t increase_by) {
    assert(stack != nullptr);

    stack->capacity = increase_by;
    sType *temporary_data = (sType *) realloc(stack->data, increase_by * sizeof(sType));

    if (stack->data == nullptr) {
        #ifdef DEBUG_LVL_1
            stack_dump(stack, __func__, __LINE__);
        #endif //DEBUG_LVL_1

        return 0;
    }

    stack->data = temporary_data;

    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif //DEBUG_LVL_2

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    return 0;
}

int stack_pop (Stack *stack) {
    assert(stack != nullptr);

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    stack->size--;
    
    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif //DEBUG_LVL_2

    if (stack->size <= (stack->capacity / REALLOC_COEFF / REALLOC_COEFF) ) {
        stack_resize(stack, stack->capacity / REALLOC_COEFF);
    }

    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif //DEBUG_LVL_2

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    return 0;
}

int stack_dtor(Stack *stack) {
    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    for (int elem = 0; elem < stack->capacity - 1; elem++) {
        stack->data[elem] = 0;
    }

    stack->size       = 0;
    stack->capacity   = 0;

    #ifdef DEBUG_LVL_1
        stack->canary_end = 0;
        stack->canary_beg = 0;
    #endif //DEBUG_LVL_1

    free(stack->data);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <assert.h>

#include "stack.h"
#include "string.h"

#if DEBUG_LVL == 2
    #define DEBUG_LVL_1
    #define DEBUG_LVL_2
    //#define output_file stdout
#else
    #if DEBUG_LVL == 1
        #define DEBUG_LVL_1
    #endif
#endif

///\brief calculate hash of buffer of bites
///\param buffer example of buffer
///\param size size of example of buffer
static uint64_t hash_buffer(char *buffer, size_t size) {
    assert(buffer != NULL && "hash_buffer: buffer = NULL");

    int hash = 0;
    int add = 0x5bd1e995;

    for (int index = 0; index < size; index++) {
        hash += buffer[index] * index;
        hash = hash << 1;
        hash = hash ^ add;
    }

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

    return hash;
}

///\brief count errors in stack's work
///\param buffer example of stack
#ifdef DEBUG_LVL_1
static int stack_is_ok(Stack *stack) {
    assert(stack != NULL && "stack_is_ok: cur_stack = NULL");

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

    if (stack->size < 0 || stack->size > stack->capacity) {
        special_code += BAD_SIZE;
    }

    if (stack->data == nullptr) {
        special_code += DATA_ERROR;
    }

    #ifdef DEBUG_LVL_2
        if (stack->hash != stack_hash(stack) ) {
            special_code += BAD_HASH;
        }
    #endif //DEBUG_LVL_2

    if (stack->canary_end != CANARY) {
        special_code += DEAD_CANARY_END;
    }

    return special_code;
}
#endif

///\brief can write custom message in stdout with adress of error
///\param stack example of stack
///\param func_name function which contains error
///\param line line where happens error
static void error_message(Stack *stack, const char *func_name, int line) {
    assert(stack != NULL && "error_message: cur_stack = NULL");

    printf("Error in function: %s(%d)\n", func_name, line);
}

///\brief print information about stack's data
///\param stack stack example 
#ifdef DEBUG_LVL_2
    static void print_stack_data(Stack *stack) {
        assert(stack != NULL && "print_stack_data: cur_stack = NULL");

        FILE* output_file = fopen(name, "a");

        fprintf(output_file, "\n\tStack data{\n");
                for (int elem = 0; elem < stack->size; elem++) {
                    fprintf(output_file, "\t\te: %d\n", stack->data[elem]);
                }
        fprintf(output_file, "\t}\n\n");

        return;
    }
#else

///\brief print information about stack's data
///\param stack stack example 
static void print_stack_data(Stack *stack) {
    assert(stack != NULL && "print_stack_data: cur_stack = NULL");

    fprintf(stdout, "\n\tStack data{\n");
            for (int elem = 0; elem < stack->size; elem++) {
                fprintf(stdout, "\t\te: %d\n", stack->data[elem]);
            }
    fprintf(stdout, "\t}\n\n");

    return;
}
#endif

///\brief checks stack's condition
///\param output_file output file
///\param stack example of stack
static void print_check_stack(FILE *output_file, Stack *stack) {
    assert(stack != NULL && "print_check_stack: cur_stack = NULL");
    assert(output_file != NULL && "print_check_stack: output_file = NULL");

    #if DEBUG_LVL == 2
    fprintf(output_file, "Stack preview:\n\tcanary begin: %x\n\tcapacity: %d\n\tsize: %d \n\thash: %d \n\tcanary end: %x\n\n",stack->canary_beg, stack->capacity, stack->size, (int)stack->hash, stack->canary_end);
    #else
    fprintf(stdout, "Stack preview:\n\tcanary begin: %x\n\tcapacity: %d\n\tsize: %d \n\tcanary end: %x\n\n",stack->canary_beg, stack->capacity, stack->size, stack->canary_end);
    #endif 
}

///\brief analog of function "update_log_file" but it is work with console out
#ifdef DEBUG_LVL_1
static void stack_dump(Stack *stack, const char *func_name, int line) {
    assert(stack != NULL && "stack_dump: cur_stack = NULL");

    int error_code = stack_is_ok(stack);

        #if DEBUG_LVL == 2
        FILE* output_file = fopen(name, "a");
        #else
        #define output_file stdout
        #endif 

        fputs("______________________________\n", output_file);
        //fputs("%s\n", __TIME__);

        if (error_code == 0) {
             fprintf(output_file, "INFO: Function: %s(%d)\n", func_name, line);
             fprintf(output_file, "Work correct\n\n");
        }

        if (error_code > 0) {
            fprintf(output_file, "Function: %s(%d)\n", func_name, line);
        }

        if ((error_code & DEAD_CANARY_BEGIN) != 0) {
            fprintf(output_file, "ERROR::DEAD_CANARY_BEGIN\n\n");
            fprintf(output_file, "expected canary begin value: %X\n\n", CANARY);
        }

        if ((error_code & BAD_CAPACITY) != 0) {
            fprintf(output_file, "Function: %s(%d) ERROR::BAD_CAPACITY\n\n", func_name, line);
            fprintf(output_file, "capacity_value should be above zero\n\n");
        }

        if ((error_code & BAD_SIZE) != 0) {
            fprintf(output_file, "ERROR::BAD_SIZE\n\n");
            fprintf(output_file, "size_value should be above zero\n\n");
        }

        if ((error_code & DATA_ERROR) != 0) {
            fprintf(output_file, "ERROR::DATA_ERROR\n\n");
            fprintf(output_file, "data is not nullptr\n\n");
        }

        #ifdef DEBUG_LVL_2
        if ((error_code & BAD_HASH) != 0) {
            fprintf(output_file, "ERROR::BAD_HASH\n\n");
        }
         #endif //DEBUG_LVL_2

        if ((error_code & DEAD_CANARY_END) != 0) {
            fprintf(output_file, "ERROR::DEAD_CANARY_END\n\n");
            fprintf(output_file, "expected canary end value: %X\n\n", CANARY);
        }

        fprintf(output_file, "%s\n\n", __TIME__);

        #if DEBUG_LVL == 2
        if ( ((error_code & BAD_HASH) != 0) || ((error_code & DEAD_CANARY_END) != 0) || ((error_code & DEAD_CANARY_BEGIN) != 0)) {
            print_stack_data(stack);
        }
        #endif

        #if DEBUG_LVL == 1
        if ( ((error_code & DEAD_CANARY_END) != 0) || ((error_code & DEAD_CANARY_BEGIN) != 0)) {
            print_stack_data(stack);
        }
        #endif

        print_check_stack(output_file, stack);

        if ( ((error_code ^ NO_ERRORS) != 0) && (stack->stack_is_not_work == 0)) {
            fprintf(output_file, "An error has occurred, the further behavior of the stack is undefined, the enforcement is at your own risk\n");
        }


        if (error_code ^ NO_ERRORS != 0) {
            stack->stack_is_not_work = 1;
        }

        if (stack->stack_is_not_work == 1) {
            fprintf(output_file, "This action happaned after stack error\n");
        }

}
#endif //DEBUG_LVL_1

EXIT_CODES stack_ctor(Stack *stack, size_t start_capacity, char *log_file) {
    if (stack == nullptr) {
        return EXIT_CODES::BAD_STRUCT_PTR;
    }

    stack->size = 0;
    stack->capacity = start_capacity;
    stack->stack_is_not_work = 0;

    #if DEBUG_LVL == 2
        if (log_file != nullptr) {
            FILE *INPUT_FILE = fopen(name, "w+");
        }
    #endif

    #ifdef DEBUG_LVL_1
        stack->canary_beg = 0xb12d00;
        stack->canary_end = 0xb12d00;
    #endif //DEBUG_LVL_1

    stack->data = (sType *)calloc(stack->capacity, sizeof(sType));

    if (stack->data == nullptr) {
        #ifdef DEBUG_LVL_1
            stack_dump(stack, __func__, __LINE__);
        #endif //DEBUG_LVL_1

        return EXIT_CODES::BAD_STRUCT_PTR;
    }

    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif //DEBUG_LVL_2

    #ifdef DEBUG_LVL_1
            stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stack_push (Stack *stack, sType new_elem) {
    if (stack == nullptr){
        return EXIT_CODES::BAD_STRUCT_PTR;
    }

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

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stack_resize(Stack *stack, size_t increase_by) {
    if (stack == nullptr) {
        return BAD_STRUCT_PTR;
    }

    stack->capacity = increase_by;
    sType *temporary_data = (sType *) realloc(stack->data, increase_by * sizeof(sType));

    if (stack->data == nullptr) {
        #ifdef DEBUG_LVL_1
            stack_dump(stack, __func__, __LINE__);
        #endif //DEBUG_LVL_1

        return EXIT_CODES::DATA_ERROR;
    }

    stack->data = temporary_data;

    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif //DEBUG_LVL_2

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stack_top (Stack *stack, sType *top_element) {
    if (stack == nullptr) {
        return EXIT_CODES::BAD_STRUCT_PTR;
    }

    if (stack->data == nullptr) {
        return EXIT_CODES::DATA_ERROR;
    }

    *top_element = stack->data[stack->size - 1];

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif //DEBUG_LVL_1

    return NO_ERRORS;
}

EXIT_CODES stack_pop (Stack *stack) {
    if (stack == nullptr) {
        return EXIT_CODES::BAD_STRUCT_PTR;
    }

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

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stack_dtor(Stack *stack) {
    if (stack == nullptr) {
        return EXIT_CODES::BAD_STRUCT_PTR;
    }

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

    return EXIT_CODES::NO_ERRORS;
}

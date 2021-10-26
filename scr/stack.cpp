#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <assert.h>

#include "stack.h"
#include "string.h"

#if DEBUG_LVL == 2
    #define DEBUG_LVL_1
    #define DEBUG_LVL_2
    #define OUT stack->output_file
#else
    #if DEBUG_LVL == 1
        #define DEBUG_LVL_1
        #define OUT stdout
    #endif
#endif

///\brief calculate hash of buffer of bites
///\param buffer example of buffer
///\param size size of example of buffer
#ifdef DEBUG_LVL_1
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
#endif

///\brief calculate hash of stack 
///\param stack example of stack
#ifdef DEBUG_LVL_1
    static uint64_t stack_hash(Stack *stack) {
        assert(stack != NULL && "hash_sum: cur_stack = NULL");

        int m = 0x5bd1e995;

        int hash = 0;

        hash += hash_buffer((char *)stack->data, stack->capacity - 1);
        hash += hash_buffer((char *)stack, 4 * sizeof(int));

        return hash;
    }
#endif

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

        if (stack->size < 0 ) {
            special_code += BAD_SIZE;
        }

        if (stack->data == nullptr) {
            special_code += DATA_ERROR;
        }

        #ifdef DEBUG_LVL_2
            if (stack->size >=0 && stack->capacity >=0 && stack->hash != stack_hash(stack)) {
                special_code += BAD_HASH;
            }
        #endif

        if (stack->canary_end != CANARY) {
            special_code += DEAD_CANARY_END;
        }

        return special_code;
    }
#endif

///\brief print information about stack's data
///\param stack stack example 
#ifdef DEBUG_LVL_1
    static void print_stack_data(Stack *stack) {
        assert(stack != NULL && "print_stack_data: cur_stack = NULL");

        fprintf(OUT, "\n\tStack data{\n");
                for (int index = 0; index < stack->size; index++) {
                    fprintf(OUT, "\t\te: %d\n", stack->data[index]);
                }
        fprintf(OUT, "\t}\n\n");

        return;
    }
#endif

///\brief checks stack's condition
///\param OUT output file
///\param stack example of stack
#ifdef DEBUG_LVL_1
    static void print_check_stack(FILE *file, Stack *stack) {
        assert(stack != NULL && "print_check_stack: cur_stack = NULL");
        assert(file != NULL && "print_check_stack: OUT = NULL");

        #ifdef DEBUG_LVL_2
            fprintf(file,
                "Stack preview:"
                "\n\tcanary begin: %x"
                "\n\tcapacity: %d"
                "\n\tsize: %d"
                "\n\thash: %d"
                "\n\tcanary end: %x\n\n",

                stack->canary_beg,
                stack->capacity,
                stack->size,
                (int)stack->hash,
                stack->canary_end);
        #else
            fprintf(stdout, "Stack preview:"
                "\n\tcanary begin: %x"
                "\n\tcapacity: %d"
                "\n\tsize: %d"
                "\n\tcanary end: %x\n\n",

                stack->canary_beg,
                stack->capacity,
                stack->size,
                stack->canary_end);
        #endif 
    }
#endif

///\brief log work info in stdout or special logfile
#ifdef DEBUG_LVL_1
    static void stack_dump(Stack *stack, const char *func_name, int line) {
        assert(stack != NULL && "stack_dump: cur_stack = NULL");

        int error_code = stack_is_ok(stack);

        fputs("______________________________\n", OUT);

        if (error_code == 0) {
             fprintf(OUT, "INFO: Function: %s(%d)\n", func_name, line);
             fprintf(OUT, "Work correct\n\n");
        }

        if (error_code > 0) {
            fprintf(OUT, "Function: %s(%d)\n", func_name, line);
        }

        if ((error_code & DEAD_CANARY_BEGIN) != 0) {
            fprintf(OUT, "ERROR::DEAD_CANARY_BEGIN\n\n");
            fprintf(OUT, "expected canary begin value: %X\n\n", CANARY);
        }

        if (stack->capacity < 0) {

            fprintf(OUT, "Function: %s(%d) ERROR::BAD_CAPACITY\n\n", func_name, line);
            fprintf(OUT, "capacity_value should be above zero\n\n");
        }

        if ((error_code & BAD_SIZE) != 0) {
            fprintf(OUT, "ERROR::BAD_SIZE\n\n");
            fprintf(OUT, "size_value should be above zero\n\n");
        }

        if ((error_code & DATA_ERROR) != 0) {
            fprintf(OUT, "ERROR::DATA_ERROR\n\n");
            fprintf(OUT, "data is not nullptr\n\n");
        }

        #ifdef DEBUG_LVL_2
            if ((error_code & BAD_HASH) != 0) {
                fprintf(OUT, "ERROR::BAD_HASH\n\n");
            }
        #endif

        if ((error_code & DEAD_CANARY_END) != 0) {
            fprintf(OUT, "ERROR::DEAD_CANARY_END\n\n");
            fprintf(OUT, "expected canary end value: %X\n\n", CANARY);
        }

        fprintf(OUT, "%s\n", __TIME__);

        #ifdef DEBUG_LVL_2
            if ( ((error_code & BAD_HASH) != 0) || ((error_code & DEAD_CANARY_END) != 0) || ((error_code & DEAD_CANARY_BEGIN) != 0)) {
                print_stack_data(stack);
            }
        #endif
        
        #ifdef DEBUG_LVL_1
            #ifndef DEBUG_LVL_2
                if ( ((error_code & DEAD_CANARY_END) != 0) || ((error_code & DEAD_CANARY_BEGIN) != 0)) {
                    print_stack_data(stack);
                }
            #endif
        #endif

        print_check_stack(OUT, stack);

        if ( ((error_code ^ NO_ERRORS) != 0) && (stack->stack_is_not_work == 0)) {
            fprintf(OUT, "An error has occurred, the further behavior of"
                "the stack is undefined, the enforcement is at your own risk\n");
        }


        if (error_code ^ NO_ERRORS != 0) {
            stack->stack_is_not_work = 1;
        }

        if (stack->stack_is_not_work == 1) {
            fprintf(OUT, "This action happaned after stack error\n");
        }
    }
#endif

EXIT_CODES stack_ctor(Stack *stack, size_t start_capacity) {
    if (stack == nullptr) {
        return EXIT_CODES::BAD_STRUCT_PTR;
    }

    stack->size = 0;
    stack->capacity = start_capacity;

    #ifdef DEBUG_LVL_1
        stack->stack_is_not_work = 0;
    #endif

    #ifdef DEBUG_LVL_1
        stack->canary_beg = 0xb12d00;
        stack->canary_end = 0xb12d00;
    #endif

    stack->data = (sType *)calloc(stack->capacity, sizeof(sType));

    if (stack->data == nullptr) {
        #ifdef DEBUG_LVL_1
            stack_dump(stack, __func__, __LINE__);
        #endif

        return EXIT_CODES::BAD_STRUCT_PTR;
    }

    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif

    #ifdef DEBUG_LVL_1
            stack_dump(stack, __func__, __LINE__);
    #endif

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stack_push (Stack *stack, sType new_elem) {
    if (stack == nullptr){
        return EXIT_CODES::BAD_STRUCT_PTR;
    }

    if(stack->capacity < 0) {
        return EXIT_CODES::BAD_CAPACITY;
    }

    if(stack->size < 0) {
        return EXIT_CODES::BAD_SIZE;
    }

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif

    if (stack->size >= stack->capacity) {
        stack_resize(stack, stack->capacity * REALLOC_COEFF);
    }

    stack->data[stack->size] = new_elem;
    stack->size++;

    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stack_resize(Stack *stack, size_t increase_by) {
    if (stack == nullptr) {
        return BAD_STRUCT_PTR;
    }

    if(stack->capacity < 0) {
        return EXIT_CODES::BAD_CAPACITY;
    }

    if(stack->size < 0) {
        return EXIT_CODES::BAD_SIZE;
    }

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif

    stack->capacity = increase_by;
    sType *temporary_data = (sType *) realloc(stack->data, increase_by * sizeof(sType));

    if (stack->data == nullptr) {
        #ifdef DEBUG_LVL_1
            stack_dump(stack, __func__, __LINE__);
        #endif

        return EXIT_CODES::DATA_ERROR;
    }

    stack->data = temporary_data;

    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stack_top (Stack *stack, sType *top_element) {
    if (stack == nullptr) {
        return EXIT_CODES::BAD_STRUCT_PTR;
    }

    if(stack->capacity < 0) {
        return EXIT_CODES::BAD_CAPACITY;
    }

    if(stack->size < 0) {
        return EXIT_CODES::BAD_SIZE;
    }

    if (stack->data == nullptr) {
        return EXIT_CODES::DATA_ERROR;
    }

    *top_element = stack->data[stack->size - 1];

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif

    return NO_ERRORS;
}

EXIT_CODES stack_pop (Stack *stack) {

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif

    if (stack == nullptr) {
        return EXIT_CODES::BAD_STRUCT_PTR;
    }

    if(stack->capacity < 0) {
        return EXIT_CODES::BAD_CAPACITY;
    }

    if(stack->size < 0) {
        return EXIT_CODES::BAD_SIZE;
    }


    stack->size--;
    
    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif

    if (stack->size <= (stack->capacity / REALLOC_COEFF / REALLOC_COEFF) ) {
        stack_resize(stack, stack->capacity / REALLOC_COEFF);
    }

    #ifdef DEBUG_LVL_2
        stack->hash = stack_hash(stack);
    #endif

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stack_dtor(Stack *stack) {
    if (stack == nullptr) {
        return EXIT_CODES::BAD_STRUCT_PTR;
    }

    if(stack->size < 0) {
        return EXIT_CODES::BAD_SIZE;
    }

    if(stack->capacity < 0) {
        return EXIT_CODES::BAD_CAPACITY;
    }

    #ifdef DEBUG_LVL_1
        stack_dump(stack, __func__, __LINE__);
    #endif

    for (int index = 0; index < stack->capacity - 1; index++) {
        stack->data[index] = 0;
    }

    stack->size     = 0;
    stack->capacity = 0;

    #ifdef DEBUG_LVL_1
        stack->canary_end = 0;
        stack->canary_beg = 0;
    #endif

    #ifdef DEBUG_LVL_2
        fclose(OUT);
    #endif

    free(stack->data);

    return EXIT_CODES::NO_ERRORS;
}

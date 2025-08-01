#include "common.h"
#include "vm.h"
#include "compiler.h"
#include "debug.h"
#include "memory.h"
#include "object.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>


VM vm;

static void resetStack()
{
    vm.stackTop = vm.stack;
}

void initVM()
{
    resetStack();
    vm.objects = NULL;
    initTable(&vm.strings);
    
}

static void runtimeError(const char* format, ...){
    va_list args;
    va_start(args,format);
    vfprintf(stderr,format,args);
    va_end(args);
    fputs("\n",stderr);

    size_t instruction = vm.ip - vm.chunk->code - 1;  //vm.ip is the succeeding ins after error ins and ->code is pointer to beginning of bytecode array so subs of both gives number of elements between them
    int line = vm.chunk->lines[instruction];
    fprintf(stderr,"[line %d] in script\n",line);
    resetStack();
}

void freeVM()
{
    freeObjects();
    freeTable(&vm.strings);
}

void push(Value value)
{
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop()
{
    vm.stackTop--;
    return *vm.stackTop;
}

static Value peek(int distance){
    return vm.stackTop[-1 - distance];
}

static bool isFalsey(Value value){
    
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

void concatenate(){
    ObjString* b = AS_STRING(pop());
    ObjString* a = AS_STRING(pop());

    int length = a->length + b->length;
    char* new = ALLOCATE(char, length + 1);
    memcpy(new, a->chars, a->length);
    memcpy(new + a->length, b->chars, b->length);
    new[length] = '\0';
    
    ObjString* result = takeString(new, length);
    
    push(OBJ_VAL(result));

}

static InterpretResult run()
{
    #define READ_BYTE() (*vm.ip++)
    #define READ_CONSTANT() (vm.chunk->constants.value[READ_BYTE()])
    #define BINARY_OP(valueType, op) \
        do{ \
            if(!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(0))){ \
                runtimeError("Both operands must be numbers"); \
                return INTERPRET_RUNTIME_ERROR; \
            } \
            double b = AS_NUMBER(pop()); \
            double a = AS_NUMBER(pop());\
            push(valueType(a op b)); \
        } while(false)

    for(;;){
#ifdef DEBUG_TRACE_EXECUTION
        printf("      ");
        for(Value* slot = vm.stack; slot < vm.stackTop;slot++)
        {
            printf("[");
            printValue(*slot);
            printf("]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk,
                                (int)(vm.ip-vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()){

            case OP_CONSTANT:{
                Value constant = READ_CONSTANT();
                push(constant);
                printf("\n");
                break;
            }

            case OP_NIL: push(NIL_VAL); break;
            case OP_TRUE : push(BOOL_VAL(true)); break;
            case OP_FALSE : push(BOOL_VAL(false)); break;

            case OP_EQUAL:{
                Value b = pop();
                Value a = pop();
                push(BOOL_VAL(valuesEqual(a,b)));
                break;
            }
            case OP_GREATER: BINARY_OP(BOOL_VAL, >); break;
            case OP_LESS: BINARY_OP(BOOL_VAL, <); break;

            case OP_ADD :{
                if(IS_STRING(peek(0)) && IS_STRING(peek(1))){
                    concatenate();
                }
                else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))){
                    double b = AS_NUMBER(pop());
                    double a = AS_NUMBER(pop());
                    push(NUMBER_VAL(a + b));
                }
                else{
                    runtimeError("Operands must be two numbers or two strings");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;

            } 

                
            case OP_SUBTRACT : BINARY_OP(NUMBER_VAL, -); break;
            case OP_MULTIPLY : BINARY_OP(NUMBER_VAL, *); break;
            case OP_DIVIDE : BINARY_OP(NUMBER_VAL, /); break;

            case OP_NOT : push(BOOL_VAL(isFalsey(pop()))); break;

            case OP_NEGATE:{
                if(!IS_NUMBER(peek(0))){
                    runtimeError("Operand must be a number");
                    return INTERPRET_RUNTIME_ERROR;
                }
                
                push(NUMBER_VAL(-AS_NUMBER(pop())));
                break;
            }

            case OP_RETURN:{
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            
            }
    }        
    }
    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef BINARY_OP
}


InterpretResult interpret(const char* source) {
    
    Chunk chunk;
    initChunk(&chunk);
    if (!compile(source, &chunk)) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }
    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;
    InterpretResult result = run();
    
    freeChunk(&chunk);
    
    return result; // Return run() result
}

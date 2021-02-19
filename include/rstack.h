#ifndef __R_STACK_H__
#define __R_STACK_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "rcommon.h"
#include "rarray.h"

struct RStack
{
    Array stack;
};

#define STACK_INIT(stack, type) stack_init(stack, 8, sizeof(type))
#define STACK_INIT_CAP(stack, type, cap) stack_init(stack, cap, sizeof(type))
#define STACK_PUSH(stack, type, value) \
    do \
    { \
        type _ = value; \
        stack_push(stack, &_); \
    } while(false)

RLIB_API void stack_init(Stack* stack, int cap, int elemSize);
RLIB_API void stack_free(Stack* stack);

RLIB_API void stack_push(Stack* stack, void* value);
RLIB_API bool stack_pop(Stack* stack, /* out */ void* value);
RLIB_API bool stack_peek(Stack* stack, /* out */ void* value);
RLIB_API bool stack_isEmpty(Stack* stack);
RLIB_API int  stack_size(Stack* stack);

#ifdef __cplusplus
}
#endif

#endif // __R_STACK_H__
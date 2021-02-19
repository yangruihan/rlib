#include "rstack.h"

void stack_init(Stack* stack, int cap, int elemSize)
{
    if (stack == NULL)
        return;

    array_init(&stack->stack, cap, elemSize);
}

void stack_free(Stack* stack)
{
    if (stack == NULL)
        return;

    array_free(&stack->stack);
}

void stack_push(Stack* stack, void* value)
{
    if (stack == NULL)
        return;

    array_push(&stack->stack, value);
}

bool stack_pop(Stack* stack, void* value)
{
    if (stack == NULL)
        return false;

    return array_pop(&stack->stack, value);
}

bool stack_peek(Stack* stack, void* value)
{
    if (stack == NULL)
        return false;

    return array_get(&stack->stack, stack->stack.count - 1, value);
}

bool stack_isEmpty(Stack* stack)
{
    if (stack == NULL)
        return false;

    return stack->stack.count == 0;
}

int stack_size(Stack* stack)
{
    if (stack == NULL)
        return 0;

    return stack->stack.count;
}
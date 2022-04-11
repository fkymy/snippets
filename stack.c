#include "stack.h"

#include <stdlib.h>
#include <string.h>

void stack_initialize(struct stack *stack, uint32_t element_size)
{
	stack->elements = NULL;
	stack->count = stack->capacity = 0;
	stack->element_size = element_size;
}

void stack_free(struct stack *stack)
{
	free(stack->elements);
}

void *stack_get(struct stack *stack, uint32_t index)
{
	return stack->elements + stack->element_size * index;
}

void *stack_pop(struct stack *stack)
{
	if(stack->count > 0)
		return stack->elements + --stack->count * stack->element_size;
	else
		return NULL;
}

void stack_push(struct stack *stack, void *data)
{
	if(++stack->count > stack->capacity)
	{
		stack->capacity <<= 1;
		if(stack->capacity < stack->count) stack->capacity = stack->count;
		stack->elements = realloc(stack->elements, stack->element_size * stack->capacity);
	}

	memcpy(stack->elements + stack->element_size * (stack->count - 1), data, stack->element_size);
}

void stack_remove(struct stack *stack, uint32_t index)
{
	if(index != --stack->count)
	{
		memmove(stack->elements + index * stack->element_size,
				stack->elements + (index + 1) * stack->element_size,
				(stack->count - index) * stack->element_size);
	}
}

void stack_clear(struct stack *stack)
{
	stack->count = 0;
}

uint32_t stack_size(struct stack *stack)
{
	return stack->count;
}

int32_t stack_index_of(struct stack *stack, void *element)
{
	uint32_t i;

	for(i = 0; i < stack->count; i++)
	{
		if(memcmp(element, stack->elements + stack->element_size * i, stack->element_size) == 0) return i;
	}

	return -1;
}


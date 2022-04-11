#pragma once

#include <stdint.h>

struct stack {
	uint8_t *elements;
	uint32_t count;
	uint32_t capacity;
	uint32_t element_size;
};

void stack_initialize(struct stack *stack, uint32_t element_size);
void stack_free(struct stack *stack);

void *stack_get(struct stack *stack, uint32_t index);
void *stack_pop(struct stack *stack);
void stack_push(struct stack *stack, void *data);
void stack_remove(struct stack *stack, uint32_t index);
void stack_clear(struct stack *stack);
uint32_t stack_size(struct stack *stack);
int32_t stack_index_of(struct stack *stack, void *element);

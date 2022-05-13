#include <stdio.h>	/*printf ()*/
#include <stdlib.h> /*malloc () free()*/
#include <assert.h> /* assert()*/
#include "d_stack.h"

#define GROWTH_FACTOR (50)

struct double_stack
{
	double *arr;
	size_t max_size;
	size_t top_index;
};

double_stack_t *DStackCreate(size_t num_of_elements)
{
	double_stack_t *stack = malloc(sizeof(double_stack_t));

	if (!stack)
	{
		printf("There is not memory to allocate!\n");

		return NULL;
	}

	stack->arr = malloc(num_of_elements * sizeof(double));

	if (!stack->arr)
	{
		printf("There is not memory to allocate!\n");

		return NULL;
	}

	stack->max_size = num_of_elements;
	stack->top_index = 0;

	return stack;
}

void DStackDestroy(double_stack_t *stack_ptr)
{
	assert(stack_ptr);

	free(stack_ptr->arr);
	stack_ptr->arr = NULL;
	free(stack_ptr);
	stack_ptr = NULL;
}

void DStackPush(double_stack_t *stack_ptr, double number)
{
	assert(stack_ptr);

	if (DStackCapacity(stack_ptr) == stack_ptr->top_index)
	{
		stack_ptr->arr = realloc(stack_ptr->arr, stack_ptr->top_index * GROWTH_FACTOR);
		stack_ptr->max_size *= GROWTH_FACTOR;
	}

	stack_ptr->arr[stack_ptr->top_index] = number;
	stack_ptr->top_index++;
}

double DStackPop(double_stack_t *stack_ptr)
{
	double num = 0;

	assert(stack_ptr);

	num = DStackPeek(stack_ptr);
	stack_ptr->top_index--;

	return num;
}

double DStackPeek(double_stack_t *stack_ptr)
{
	assert(stack_ptr);

	return stack_ptr->arr[stack_ptr->top_index - 1];
}

size_t DStackSize(const double_stack_t *stack_ptr)
{
	assert(stack_ptr);

	return (stack_ptr->top_index);
}

int DStackIsEmpty(const double_stack_t *stack_ptr)
{
	assert(stack_ptr);

	return (0 == stack_ptr->top_index);
}

size_t DStackCapacity(const double_stack_t *stack_ptr)
{
	assert(stack_ptr);

	return (stack_ptr->max_size);
}

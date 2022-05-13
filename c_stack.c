#include <stdio.h>	/*printf ()*/
#include <stdlib.h> /*malloc () free()*/
#include <assert.h> /* assert()*/
#include "c_stack.h"

#define GROWTH_FACTOR (50)

struct char_stack
{
	char *arr;
	size_t max_size;
	size_t top_index;
};

char_stack_t *CStackCreate(size_t num_of_elements)
{
	char_stack_t *stack = malloc(sizeof(char_stack_t));

	if (!stack)
	{
		printf("There is not memory to allocate!\n");

		return NULL;
	}

	stack->arr = malloc(num_of_elements * sizeof(char));

	if (!stack->arr)
	{
		printf("There is not memory to allocate!\n");

		return NULL;
	}

	stack->max_size = num_of_elements;
	stack->top_index = 0;

	return stack;
}

void CStackDestroy(char_stack_t *stack_ptr)
{
	assert(stack_ptr);

	free(stack_ptr->arr);
	stack_ptr->arr = NULL;
	free(stack_ptr);
	stack_ptr = NULL;
}

void CStackPush(char_stack_t *stack_ptr, char number)
{
	assert(stack_ptr);

	if (CStackCapacity(stack_ptr) == stack_ptr->top_index)
	{
		stack_ptr->arr = realloc(stack_ptr->arr, stack_ptr->max_size * GROWTH_FACTOR);
		stack_ptr->max_size *= GROWTH_FACTOR;
	}

	stack_ptr->arr[stack_ptr->top_index] = number;
	stack_ptr->top_index++;
}

char CStackPop(char_stack_t *stack_ptr)
{
	char ch = 0;

	assert(stack_ptr);

	ch = CStackPeek(stack_ptr);
	stack_ptr->top_index--;

	return ch;
}

char CStackPeek(char_stack_t *stack_ptr)
{
	assert(stack_ptr);

	return stack_ptr->arr[stack_ptr->top_index - 1];
}

size_t CStackSize(const char_stack_t *stack_ptr)
{
	assert(stack_ptr);

	return (stack_ptr->top_index);
}

int CStackIsEmpty(const char_stack_t *stack_ptr)
{
	assert(stack_ptr);

	return (0 == stack_ptr->top_index);
}

size_t CStackCapacity(const char_stack_t *stack_ptr)
{
	assert(stack_ptr);

	return (stack_ptr->max_size);
}

#ifndef __OL113DSTACK_H__
#define __OL113DSTACK_H__

#include <stddef.h> /* size_t */

typedef struct double_stack double_stack_t;

double_stack_t *DStackCreate(size_t num_of_elements);
void DStackDestroy(double_stack_t *stack_ptr);
double DStackPop(double_stack_t *stack_ptr);
void DStackPush(double_stack_t *stack_ptr, double number);
double DStackPeek(double_stack_t *stack_ptr);
size_t DStackSize(const double_stack_t *stack_ptr);
int DStackIsEmpty(const double_stack_t *stack_ptr);
size_t DStackCapacity(const double_stack_t *stack_ptr);

#endif /* __OL113DSTACK_H__ */

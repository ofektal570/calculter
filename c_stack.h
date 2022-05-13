#ifndef __OL113CSTACK_H__
#define __OL113CSTACK_H__

#include <stddef.h> /* size_t */

typedef struct char_stack char_stack_t;

char_stack_t *CStackCreate(size_t num_of_elements);
void CStackDestroy(char_stack_t *stack_ptr);
char CStackPop(char_stack_t *stack_ptr);
void CStackPush(char_stack_t *stack_ptr, char number);
char CStackPeek(char_stack_t *stack_ptr);
size_t CStackSize(const char_stack_t *stack_ptr);
int CStackIsEmpty(const char_stack_t *stack_ptr);
size_t CStackCapacity(const char_stack_t *stack_ptr);

#endif /* __OL113CSTACK_H__ */

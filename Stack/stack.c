#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

extern int stackInit(intstack_t *self)
{
	self->top = NULL;
	self->size = 0;
	return 0;
}

extern void stackRelease(intstack_t *self)
{
	while(self->top != NULL)
	{
		stackPop(self);
	}

	stackInit(self);
}

extern void stackPush(intstack_t *stack, int i)
{
	struct elem *next;

	if((next = (struct elem *)malloc(sizeof(struct elem))) == NULL)
	{
		fprintf(stderr, "Out of memory.\r\n");
		stackRelease(stack);
		exit(EXIT_FAILURE);
	}
	else
	{
		next->val = i;
		next->prev = stack->top;
		stack->top = next;
		stack->size++;
	}
}

extern int stackTop(const intstack_t *self)
{
	if(stackIsEmpty(self))
	{
		fprintf(stderr, "Stack empty.\r\n");
		stackRelease((intstack_t *)self);
		exit(EXIT_FAILURE);
	}
	else return self->top->val;
}

extern int stackPop(intstack_t *self)
{
	struct elem *tmp;
	int ret = 0;

	if(stackIsEmpty(self))
	{
		fprintf(stderr, "Stack empty.\r\n");
		stackRelease(self);
		exit(EXIT_FAILURE);
	}
	else
	{
		tmp = self->top;
		ret = tmp->val;
		self->top = tmp->prev;
		self->size--;
		free(tmp);
		return ret;
	}
}

extern int stackIsEmpty(const intstack_t *self)
{
	if((self->size) == 0) return 1;
	else return 0;
}

extern void stackPrint(const intstack_t *self)
{
	struct elem *tmp = self->top;

	while(tmp != NULL)
	{
		fprintf(stdout, "%d ", tmp->val);
		tmp = tmp->prev;
	}

	fprintf(stdout, "\r\n");
}

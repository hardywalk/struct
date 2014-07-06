/* A simple stack framework demo
 * Copyright (c) 2014 Hardy Liu <hardy.liu at foxmail dot com> 
 **/
#include <stdio.h>
#include <string.h>
#include "simple_stack.h"

struct userdata {
	int id;
	int num;
};

void *dup_userdata(void *ptr)
{
	struct userdata *p;

	if(ptr == NULL) 
		return NULL;

	p = malloc(sizeof(struct userdata));
	if (p == NULL)
		return NULL;
	memcpy(p, ptr, sizeof(struct userdata));

	return p;
}

void free_userdata(void *ptr)
{
	if (ptr != NULL)
		free(ptr);
	return;
}

int main(void)
{
	int i;
	void *stack, *stack_new;
	struct userdata *node;

	stack = ss_create(SS_UNLIMITED);
	if (stack == NULL)
		return -1;

	ss_set_dup_method(stack, dup_userdata); 
	ss_set_free_method(stack, free_userdata); 


	for (i = 0; i < 10; i++) {
		node = malloc(sizeof(struct userdata));
		if (node == NULL)
			continue;
		node->id = i;
		node->num = 99 - i;
		printf("PUSH:ID = %d, NUM = %d\n", node->id, node->num);
		ss_push(stack, node);
	}

	stack_new = ss_dup(stack);

	if (stack_new == NULL) {
		printf("Dup failed!\n");
		ss_destroy(stack);
		return -1;
	}

	i = 0;
	while ((node = ss_pop(stack)) != NULL) {
		printf("POP:ID = %d, NUM = %d\n", node->id, node->num);
		free(node);
		if (i == 5)
			break;
		i++;
	}

	i = 0;
	while ((node = ss_pop(stack_new)) != NULL) {
		printf("POP(new):ID = %d, NUM = %d\n", node->id, node->num);
		free(node);
		if (i == 4)
			break;
		i++;
	}
	printf("Stack count = %d\n", ss_count(stack));
	printf("Stack(new) count = %d\n", ss_count(stack_new));

	ss_destroy(stack);
	ss_destroy(stack_new);

	return 0;
}


/* A simple stack framework
 * Copyright (c) 2014 Hardy Liu <hardy.liu at foxmail dot com> 
 **/
#include "simple_stack.h"

struct simple_node {
	struct simple_node *next;
	void *pdata;
};

struct simple_stack {
	struct simple_node *top;
	int count; 
	int limit;
	void *(*dup)(void *ptr);
	void (*free)(void *ptr);
};

void *ss_create(int limit)
{
	struct simple_stack *pstack; 

	pstack = (struct simple_stack *)malloc(sizeof(struct simple_stack));

	if (pstack == NULL)
		return NULL;

	memset(pstack, 0, sizeof(struct simple_stack));
	pstack->limit = limit;

	return pstack;
}


int ss_push(void *stack, void *pdata)
{
	struct simple_stack *pstack = stack;
	struct simple_node *pnode;

	if (pstack == NULL || pdata == NULL)
		return -1;

	if (pstack->limit != 0 && pstack->limit < pstack->count + 1)
		return -2;

	pnode = malloc(sizeof(struct simple_node));
	if (pnode == NULL)
		return -3;

	pnode->next = pstack->top;
	pnode->pdata = pdata;
	pstack->top = pnode;
	pstack->count++;
	
	return 0;
}

void *ss_pop(void *stack)
{
	struct simple_stack *pstack = stack;
	struct simple_node *pnode;
	void *pdata;

	if (pstack == NULL)
		return NULL;

	if (pstack->top == NULL)
		return NULL;

	pnode = pstack->top;
	pstack->top = pnode->next;
	pstack->count--;

	pdata = pnode->pdata;
	free(pnode);

	return pdata;	
}


int ss_set_dup_method(void *stack, void *(*dup_cb)(void *ptr))
{
	struct simple_stack *pstack = stack;

	if (pstack == NULL)
		return -1;
	pstack->dup = dup_cb;

	return 0;
}

int ss_set_free_method(void *stack, void (*free_cb)(void *ptr))
{
	struct simple_stack *pstack = stack;

	if (pstack == NULL)
		return -1;
	pstack->free = free_cb;

	return 0;
}

void ss_destroy(void *stack)
{
	struct simple_stack *pstack = stack;
	struct simple_node *pnode;

	if (pstack == NULL)
		return;

	while (pstack->top != NULL) {
		pnode = pstack->top;
		pstack->top = pnode->next;
		if (pstack->free != NULL && pnode->pdata != NULL)
			pstack->free(pnode->pdata);
		free(pnode);
	}

	free(pstack);

	return;
}

static struct simple_node *node_dup(struct simple_node *node, void *(*dup_cb)(void *ptr))
{
	struct simple_node *new_node;

	if (node == NULL)
		return NULL;

	new_node = malloc(sizeof(struct simple_node));
	if (new_node == NULL)
		return NULL;
	memset(new_node, 0, sizeof(struct simple_node));
	if (dup_cb != NULL && node->pdata != NULL) {
		new_node->pdata = dup_cb(node->pdata);
		if (new_node->pdata == NULL) {
			free(new_node);
			return NULL;
		}
	}

	return new_node;
}


void *ss_dup(void *stack)
{
	struct simple_stack *pstack_new, *pstack = stack;
	struct simple_node *pnode, *pnode_new;

	if (pstack == NULL)
		return NULL;

	pstack_new = ss_create(pstack->limit);
	if (pstack_new == NULL)
		return NULL;
	pstack_new->count = pstack->count;
	pstack_new->dup = pstack->dup;
	pstack_new->free = pstack->free;

	if (pstack->top == NULL)
		return pstack_new;

	pstack_new->top = node_dup(pstack->top, pstack->dup);
	if (pstack_new->top == NULL) {
		ss_destroy(pstack_new);
		return NULL;
	}

	pnode = pstack->top;
	pnode_new = pstack_new->top;

	while (pnode->next != NULL) {
		pnode_new->next = node_dup(pnode->next, pstack->dup);
		if (pnode_new->next == NULL) {
			ss_destroy(pstack_new);
			return NULL;
		}
		pnode = pnode->next;
		pnode_new = pnode_new->next;
	}

	return pstack_new;
}

int ss_limit(void *stack)
{
	struct simple_stack *pstack = stack;
	if (pstack == NULL)
		return -1;

	return pstack->limit;
}

int ss_count(void *stack)
{
	struct simple_stack *pstack = stack;
	if (pstack == NULL)
		return -1;

	return pstack->count;
}


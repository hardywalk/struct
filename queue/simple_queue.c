/* A simple queue framework
 * Copyright (c) 2014 Hardy Liu <hardy.liu at foxmail dot com> 
 **/
#include "simple_queue.h"
struct simple_node {
	struct simple_node *next;
	void *pdata;
};

struct simple_queue {
	struct simple_node *head;
	struct simple_node *tail;
	int count; 
	int limit;
	void *(*dup)(void *ptr);
	void (*free)(void *ptr);
};

void *sq_create(int limit)
{
	struct simple_queue *pqueue; 

	pqueue = (struct simple_queue *)malloc(sizeof(struct simple_queue));

	if (pqueue == NULL)
		return NULL;

	memset(pqueue, 0, sizeof(struct simple_queue));
	pqueue->limit = limit;

	return pqueue;
}


int sq_enqueue(void *queue, void *pdata)
{
	struct simple_queue *pqueue = queue;
	struct simple_node *pnode;

	if (pqueue == NULL || pdata == NULL)
		return -1;

	if (pqueue->limit != 0 && pqueue->limit < pqueue->count + 1)
		return -2;

	pnode = malloc(sizeof(struct simple_node));
	if (pnode == NULL)
		return -3;
	memset(pnode, 0, sizeof(struct simple_node));

	pnode->pdata = pdata;
	if (pqueue->tail == NULL) {
		pqueue->head = pnode;
	} else {
		pqueue->tail->next = pnode;
	}
	pqueue->tail = pnode;
	pqueue->count++;
	
	return 0;
}

int sq_enqueue_node(void *queue, void *node)
{
	struct simple_queue *pqueue = queue;
	struct simple_node *pnode = node;

	if (pqueue == NULL || pnode == NULL)
		return -1;

	if (pqueue->limit != 0 && pqueue->limit < pqueue->count + 1)
		return -2;

	if (pqueue->tail == NULL) {
		pqueue->head = pnode;
	} else {
		pqueue->tail->next = pnode;
	}
	pqueue->tail = pnode;
	pqueue->count++;
	
	return 0;
}


void *sq_dequeue(void *queue)
{
	struct simple_queue *pqueue = queue;
	struct simple_node *pnode;
	void *pdata;

	if (pqueue == NULL)
		return NULL;

	if (pqueue->head == NULL)
		return NULL;

	pnode = pqueue->head;
	pqueue->head = pnode->next;
	pqueue->count--;

	if (pqueue->head == NULL)
		pqueue->tail = NULL;

	pdata = pnode->pdata;
	free(pnode);

	return pdata;	
}

void *sq_dequeue_node(void *queue)
{
	struct simple_queue *pqueue = queue;
	struct simple_node *pnode;

	if (pqueue == NULL)
		return NULL;

	if (pqueue->head == NULL)
		return NULL;

	pnode = pqueue->head;
	pqueue->head = pnode->next;
	pqueue->count--;

	if (pqueue->head == NULL)
		pqueue->tail = NULL;

	pnode->next = NULL;

	return pnode;	
}

void *sq_node_data(void *node)
{
	struct simple_node *pnode = node;
	if (pnode == NULL)
		return NULL;
	return pnode->pdata;
}

int sq_set_dup_method(void *queue, void *(*dup_cb)(void *ptr))
{
	struct simple_queue *pqueue = queue;

	if (pqueue == NULL)
		return -1;
	pqueue->dup = dup_cb;

	return 0;
}

int sq_set_free_method(void *queue, void (*free_cb)(void *ptr))
{
	struct simple_queue *pqueue = queue;

	if (pqueue == NULL)
		return -1;
	pqueue->free = free_cb;

	return 0;
}

void sq_destroy(void *queue)
{
	struct simple_queue *pqueue = queue;
	struct simple_node *pnode;

	if (pqueue == NULL)
		return;

	while (pqueue->head != NULL) {
		pnode = pqueue->head;
		pqueue->head = pnode->next;
		if (pqueue->free != NULL && pnode->pdata != NULL)
			pqueue->free(pnode->pdata);
		free(pnode);
	}

	free(pqueue);

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


void *sq_dup(void *queue)
{
	struct simple_queue *pqueue_new, *pqueue = queue;
	struct simple_node *pnode, *pnode_new;

	if (pqueue == NULL)
		return NULL;

	pqueue_new = sq_create(pqueue->limit);
	if (pqueue_new == NULL)
		return NULL;
	memcpy(pqueue_new, pqueue, sizeof(struct simple_queue));

	if (pqueue->head == NULL)
		return pqueue_new;

	pqueue_new->head = node_dup(pqueue->head, pqueue->dup);
	if (pqueue_new->head == NULL) {
		sq_destroy(pqueue_new);
		return NULL;
	}

	pnode = pqueue->head;
	pnode_new = pqueue_new->head;

	while (pnode->next != NULL) {
		pnode_new->next = node_dup(pnode->next, pqueue->dup);
		if (pnode_new->next == NULL) {
			sq_destroy(pqueue_new);
			return NULL;
		}
		pnode = pnode->next;
		pnode_new = pnode_new->next;
	}

	pqueue_new->tail = pnode_new;

	return pqueue_new;
}

int sq_limit(void *queue)
{
	struct simple_queue *pqueue = queue;
	if (pqueue == NULL)
		return -1;

	return pqueue->limit;
}

int sq_count(void *queue)
{
	struct simple_queue *pqueue = queue;
	if (pqueue == NULL)
		return -1;

	return pqueue->count;
}


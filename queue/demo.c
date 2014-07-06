/* A simple queue framework
 * Copyright (c) 2014 Hardy Liu <hardy.liu at foxmail dot com> 
 */
#include <stdio.h>
#include <string.h>
#include "simple_queue.h"

struct userdata {
	int id;
	int num;
};

void *dup_userdata(void *ptr)
{
	struct userdata *p, *pt;

	p = ptr;
	if (p == NULL)
		return NULL;

	pt = malloc(sizeof(struct userdata));
	if (pt == NULL)
		return NULL;
	memcpy(pt, p, sizeof(struct userdata));

	return pt;
}


void free_userdata(void *ptr)
{
	free(ptr);
	return;
}

int main(void)
{
	void *queue, *queue_new;
	struct userdata *node;
	int i;

	queue = sq_create(SQ_UNLIMITED);
	if (queue == NULL)
		return -1;

	sq_set_dup_method(queue, dup_userdata); 
	sq_set_free_method(queue, free_userdata); 

	for (i = 0; i < 20; i++) {
		node = malloc(sizeof(struct userdata));
		if (node == NULL)
			continue;
		node->id = i;
		node->num = 99 - i;
		printf("Enqueue: ID = %d, Num = %d\n", node->id, node->num);
		sq_enqueue(queue, node);
	}

	queue_new = sq_dup(queue);
	i = 0;
	while ((node = sq_dequeue(queue_new)) != NULL) {
		printf("Dequeue: ID = %d, Num = %d\n", node->id, node->num);
		free(node);

		if (i == 4)
			break;
		i++;
	}

	i = 0;
	while ((node = sq_dequeue(queue)) != NULL) {
		printf("Dequeue(new):ID = %d, Num = %d\n", node->id, node->num);
		free(node);
		if (i == 5)
			break;
		i++;
	}

	printf("Queue count = %d\n", sq_count(queue));
	printf("Queue(new) count = %d\n", sq_count(queue_new));

	sq_destroy(queue);
	sq_destroy(queue_new);

	return 0;
}


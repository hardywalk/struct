/* A simple queue framework
 * Copyright (c) 2014 Hardy Liu <hardy.liu at foxmail dot com> 
 **/
#ifndef SIPMLE_QUEUE_H
#define SIPMLE_QUEUE_H
#include <string.h>
#include <stdlib.h>

#define SQ_UNLIMITED	0

void *sq_create(int limit);
void sq_destroy(void *queue);
int sq_enqueue(void *queue, void *pdata);
void *sq_dequeue(void *queue);
int sq_enqueue_node(void *queue, void *node);
void *sq_dequeue_node(void *queue);
void *sq_dup(void *queue);
void *sq_node_data(void *node);
int sq_limit(void *queue);
int sq_count(void *queue);
int sq_set_dup_method(void *queue, void *(*dup_cb)(void *ptr));
int sq_set_free_method(void *queue, void (*free_cb)(void *ptr));

#endif

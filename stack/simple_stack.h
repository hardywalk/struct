/* A simple stack framework
 * Copyright (c) 2014 Hardy Liu <hardy.liu at foxmail dot com> 
 **/
#ifndef SIMPLE_STACK_H
#define SIMPLE_STACK_H
#include <string.h>
#include <stdlib.h>
#define SS_UNLIMITED	0

void *ss_create(int limit);
void ss_destroy(void *stack);
int ss_push(void *stack, void *pdata);
void *ss_pop(void *stack);
void *ss_dup(void *stack);
int ss_limit(void *stack);
int ss_count(void *stack);
int ss_set_dup_method(void *stack, void *(*dup_cb)(void *ptr));
int ss_set_free_method(void *stack, void (*free_cb)(void *ptr));

#endif

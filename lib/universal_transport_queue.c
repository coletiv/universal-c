/*

universal_transport_queue.c
universal-c

Copyright (c) 2016 Coletiv Studio

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include "universal_transport_queue.h"

struct transport_queue_node_s {
	transport_queue_object_t object;
	struct transport_queue_node_s * next;
};

typedef struct transport_queue_node_s * transport_queue_node_t;

struct transport_queue_s {
    transport_queue_node_t head;
	transport_queue_node_t tail;
	transport_queue_node_t reserved; // allocated, not used
};

transport_queue_t transport_queue_create()
{
	transport_queue_t new_queue;
	new_queue = (transport_queue_t)calloc(sizeof(struct transport_queue_s));
	if (new_queue)
	{
		new_queue->head = NULL;
		new_queue->tail = NULL;
		new_queue->reserved = NULL;
	}
	
	return new_queue;
}

void transport_queue_destroy(transport_queue_t q)
{
	transport_queue_node_t iter, tmp;
	
	iter = q->head; // enqueued
	while(iter != NULL) 
	{
		tmp = iter;
		iter = iter->next;
		free(tmp);
	}
	
	iter = q->reserved; // reserved
	while(iter != NULL) 
	{
		tmp = iter;
		iter = iter->next;
		free(tmp);
	}
	
	free(q);
}

bool transport_queue_is_empty(transport_queue_t q)
{
	return (q->head == NULL && q->tail == NULL);
}

void transport_queue_push(transport_queue_t q, transport_queue_object_t o)
{
	transport_queue_node_t push_queue_node;
	
	if (q->reserved)
	{
		push_queue_node = q->reserved;
		q->reserved = q->reserved->next;
	}
	else
	{
		push_queue_node = (transport_queue_node_t)calloc(sizeof(struct transport_queue_node_s));
	}
	
	if (push_queue_node)
	{
		push_queue_node->object = o;
		push_queue_node->next = NULL;
		
		if (q->head == NULL && q->tail == NULL) 
		{			
			q->head = push_queue_node;
		}
		else 
		{
			q->tail->next = push_queue_node;
		}
			
		q->tail = push_queue_node;
	}
}

transport_queue_object_t transport_queue_pop(transport_queue_t q)
{
	transport_queue_object_t pop_queue_object = NULL;
	
	if (transport_queue_is_empty(q) == false) // not empty
	{
		transport_queue_node_t pop_queue_node = q->head;
	 	pop_queue_object = pop_queue_node->object;
		
		if (q->head == q->tail) // 1 node
		{
			q->head = NULL;
			q->tail = NULL;
		}
		else
		{
			q->head = q->head->next;
		}
		
		pop_queue_node->next = q->reserved; // reserved node
		q->reserved = pop_queue_node;
	}
	
	return pop_queue_object;
}

int debug_transport_queue_enqueued_count(transport_queue_t q)
{
	int enqueued_count = 0;
	transport_queue_node_t iter;
	
	iter = q->head; // enqueued
	while(iter != NULL) 
	{
		iter = iter->next;
		++enqueued_count;
	}
	
	return enqueued_count;
}

int debug_transport_queue_reserved_count(transport_queue_t q)
{
	int reserved_count = 0;
	transport_queue_node_t iter;
	
	iter = q->reserved; // reserved
	while(iter != NULL) 
	{
		iter = iter->next;
		++reserved_count;
	}
	
	return reserved_count;	
}

/*

universal_transport_queue.h
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

#ifndef __UNIVERSAL_C_TRANSPORT_QUEUE_H__
#define __UNIVERSAL_C_TRANSPORT_QUEUE_H__

#include "universal_internal.h"

UNIVERSAL_EXTERN_C_BEGIN
  
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef void * transport_queue_object_t;
typedef struct transport_queue_s * transport_queue_t;

transport_queue_t transport_queue_create();
void transport_queue_destroy(transport_queue_t q);

bool transport_queue_is_empty(transport_queue_t q);

void transport_queue_push(transport_queue_t q, transport_queue_object_t o);
transport_queue_object_t transport_queue_pop(transport_queue_t q);

int debug_transport_queue_enqueued_count(transport_queue_t q);
int debug_transport_queue_reserved_count(transport_queue_t q);

UNIVERSAL_EXTERN_C_END

#endif /* __UNIVERSAL_C_TRANSPORT_QUEUE_H__ */
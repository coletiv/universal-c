/*

universal_transport_reachability.h
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

#ifndef __UNIVERSAL_C_TRANSPORT_REACHABILITY_H__
#define __UNIVERSAL_C_TRANSPORT_REACHABILITY_H__

#include "universal_internal.h"

UNIVERSAL_EXTERN_C_BEGIN
    
/*!
* @header
*
* This is an abstract implementation of transport_reachability_t. 
* Each platform SDK should provide a bridge implementation for network reachability.
*/

enum {
    UNIVERSAL_TRANSPORT_REACHABILITY_NOT_REACHABLE = 0,
    UNIVERSAL_TRANSPORT_REACHABILITY_REACHABLE,
    UNIVERSAL_TRANSPORT_REACHABILITY_REACHABLE_VIA_WIFI,
    UNIVERSAL_TRANSPORT_REACHABILITY_REACHABLE_VIA_WWAN
};

typedef void (*transport_reachability_callback_t)(int status, const void * context);

struct transport_reachability_s {
    transport_reachability_callback_t callback;
    void * context;
    void * ref; // platform specific implementation
};

typedef struct transport_reachability_s * transport_reachability_t;

transport_reachability_t transport_reachability_create(transport_reachability_callback_t callback, const void * context);
void transport_reachability_release(transport_reachability_t transport_reachability);

int transport_reachability_status(transport_reachability_t transport_reachability);

void transport_reachability_start_observing(transport_reachability_t transport_reachability);
void transport_reachability_stop_observing(transport_reachability_t transport_reachability);

UNIVERSAL_EXTERN_C_END

#endif /* __UNIVERSAL_C_TRANSPORT_REACHABILITY_H__ */
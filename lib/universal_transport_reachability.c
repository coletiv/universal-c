/*

universal_transport_reachability.c
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

#include "universal_transport_reachability.h"

transport_reachability_t transport_reachability_create(transport_reachability_callback_t callback, const void * context)
{
    return NULL; // platform specific implementation
}

void transport_reachability_release(transport_reachability_t transport_reachability)
{
    // platform specific implementation  
}

int transport_reachability_status(transport_reachability_t transport_reachability)
{
    return UNIVERSAL_TRANSPORT_REACHABILITY_REACHABLE; // platform specific implementation, default is UNIVERSAL_TRANSPORT_REACHABILITY_REACHABLE
}

void transport_reachability_start_observing(transport_reachability_t transport_reachability)
{
    // platform specific implementation
}

void transport_reachability_stop_observing(transport_reachability_t transport_reachability)
{
    // platform specific implementation
}
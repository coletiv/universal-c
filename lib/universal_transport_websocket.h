/*

universal_transport_websocket.h
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

#ifndef __UNIVERSAL_C_TRANSPORT_WEBSOCKET_H__
#define __UNIVERSAL_C_TRANSPORT_WEBSOCKET_H__

#include "universal_internal.h"
#include "universal_transport_messages.h"

#include <libwebsockets.h>

UNIVERSAL_EXTERN_C_BEGIN

typedef void (*transport_websocket_open_callback_t)(const void *context);
typedef void (*transport_websocket_close_callback_t)(const void *context);
typedef void (*transport_websocket_fail_callback_t)(const void *context);
typedef void (*transport_websocket_read_callback_t)(const void *context, transport_message_t transport_message);

struct transport_websocket_handler {
    const void * context;
    transport_websocket_open_callback_t open_callback;
    transport_websocket_close_callback_t close_callback;
    transport_websocket_fail_callback_t fail_callback;
    transport_websocket_read_callback_t read_callback; // the handler is responsible for releasing the read transport_message_t
};

typedef struct transport_websocket_s * transport_websocket_t;

transport_websocket_t transport_websocket_create(struct transport_websocket_handler handler, const char * url);
void transport_websocket_release(transport_websocket_t transport_websocket);

void transport_websocket_open(transport_websocket_t transport_websocket);
void transport_websocket_close(transport_websocket_t transport_websocket);

// will automatically release transport_message_t
void transport_websocket_write(transport_websocket_t transport_websocket, transport_message_t transport_message);

void transport_websocket_run(transport_websocket_t transport_websocket);

UNIVERSAL_EXTERN_C_END

#endif /* __UNIVERSAL_C_TRANSPORT_WEBSOCKET_H__ */

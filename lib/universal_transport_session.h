/*

universal_transport_session.h
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

#ifndef __UNIVERSAL_C_TRANSPORT_SESSION_H__
#define __UNIVERSAL_C_TRANSPORT_SESSION_H__

#include "universal_base.h"

#include "universal_transport_types.h"

UNIVERSAL_EXTERN_C_BEGIN

typedef void (*transport_session_open_callback_t)(const void *context); // Open Session
typedef void (*transport_session_close_callback_t)(const void *context); // Close Session

/*!
* @typedef struct transport_session_handler
*
* @abstract
* Universal transport agent handler info struct.
*
* @discussion
* The handler is responsible for releasing the transport_type_t for each callback.
*/
struct transport_session_handler {
	const void * context;
    transport_session_open_callback_t open_callback;
    transport_session_close_callback_t close_callback;
};    

typedef struct transport_session_s * transport_session_t;

transport_session_t transport_session_create(const char * user_id, const char * device_token, const char * api_endpoint_url, const char * api_shared_secret , struct transport_session_handler handler);    
void transport_session_release(transport_session_t transport_session);

void transport_session_open(transport_session_t transport_session);
bool transport_session_is_open(transport_session_t transport_session);
void transport_session_close(transport_session_t transport_session);

// TODO Threading model
void transport_session_run(transport_session_t transport_session);

// TODO write documentation for each function that sends a message to the API
void transport_session_example_ping(transport_session_t transport_session, transport_type_example_t example_object);
void transport_session_example_echo(transport_session_t transport_session, transport_type_example_t example_object);


UNIVERSAL_EXTERN_C_END

#endif /* __UNIVERSAL_C_TRANSPORT_SESSION_H__ */
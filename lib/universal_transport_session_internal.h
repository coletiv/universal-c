/*

universal_transport_session_internal.h
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

#ifndef __UNIVERSAL_C_TRANSPORT_SESSION_INTERNAL_H__
#define __UNIVERSAL_C_TRANSPORT_SESSION_INTERNAL_H__

#include "universal_internal.h"

#include "universal_transport_types.h"
#include "universal_transport_messages.h"
#include "universal_transport_reachability.h"
#include "universal_transport_websocket.h"

#include "universal_transport_session_state.h"

UNIVERSAL_EXTERN_C_BEGIN

struct transport_session_s {
	
	// User and Device
    const char * user_id;
	const char * device_token;	
	
	// API
	const char * api_endpoint_url;
	const char * api_shared_secret;
	
    struct transport_session_handler handler;
    transport_reachability_t reachability;
    transport_websocket_t websocket;
    
    transport_session_state state;
    transport_session_state_event current_event; // current state event being processed
    transport_session_state_event next_event; // next state event to be processed (pushed)
};

/*!
 * message
 */
void transport_session_send_message(transport_session_t transport_session, uint8_t name, transport_type_t arg);
void transport_session_receive_message(transport_session_t transport_session, uint8_t name, transport_type_t arg);

/*!
 * websocket
 */
void transport_session_websocket_setup(transport_session_t transport_session);
void transport_session_websocket_open_callback(const void *context);
void transport_session_websocket_close_callback(const void *context);
void transport_session_websocket_fail_callback(const void *context);
void transport_session_websocket_read_callback(const void *context, transport_message_t transport_message);

UNIVERSAL_EXTERN_C_END

#endif /* __UNIVERSAL_C_TRANSPORT_SESSION_INTERNAL_H__ */
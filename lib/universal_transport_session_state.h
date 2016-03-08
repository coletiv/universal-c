/*

universal_transport_session_state.h
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

#ifndef __UNIVERSAL_C_TRANSPORT_SESSION_STATE_H__
#define __UNIVERSAL_C_TRANSPORT_SESSION_STATE_H__

#include "universal_base.h"

UNIVERSAL_EXTERN_C_BEGIN

enum {
    UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_BEGIN = 1,
    UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_BECOME_ACTIVE = 11,
    UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_RESIGN_ACTIVE = 10,
    UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_CONNECTION_ESTABLISHED = 21,
    UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_CONNECTION_FAILED = 22,
    UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_CONNECTION_DISCONNECTED = 23,
    UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_CONNECTION_RETRY = 24,
    UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_SESSION_INFO_EXISTS = 31,
    UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_SESSION_INFO_NIL = 32,
    UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_AUTHENTICATED_RECEIVED = 41,
    UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_CLOSE_RECEIVED = 42,
    UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_ERROR_RECEIVED = 43,
    UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_REACHABILITY_IS_REACHABLE = 61,
    UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_REACHABILITY_IS_UNREACHABLE = 62,
    UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_TIMEOUT = 71
};

typedef int transport_session_state;
typedef int transport_session_state_event;

void transport_session_state_begin(transport_session_t transport_session);
void transport_session_state_end(transport_session_t transport_session);
void transport_session_state_set_event(transport_session_t transport_session, transport_session_state_event event); // state
void transport_session_state_push_event(transport_session_t transport_session, transport_session_state_event event); // state queue

bool transport_session_state_is_idle(transport_session_t transport_session);
bool transport_session_state_is_connecting(transport_session_t transport_session);
bool transport_session_state_is_connecting_unreachable(transport_session_t transport_session);
bool transport_session_state_is_connected(transport_session_t transport_session);
bool transport_session_state_is_authenticating(transport_session_t transport_session);
bool transport_session_state_is_authenticated(transport_session_t transport_session);

UNIVERSAL_EXTERN_C_END

#endif /* __UNIVERSAL_C_TRANSPORT_SESSION_STATE_H__ */
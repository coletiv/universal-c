
#line 1 "../lib/universal_transport_session.rl"
/*

universal_transport_session.c
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

#include "universal_transport_session.h"
#include "universal_transport_session_internal.h"
#include "universal_transport_session_state.h"

#include "universal_internal.h"
#include "universal_transport_constants.h"

#pragma mark -
#pragma mark ragel state machine

#define kNSEC_PER_MILLISEC (1000000ull) // 1 ms = 10^6 ns = 1 000 000 ns
#define kDefaultWaitRetryMilliseconds 10000 // 10 seconds
#define kDefaultTimeoutMilliseconds 5000 // 5 seconds

/*!
 States defined by Ragel:
 transport_session_state_start
 transport_session_state_first_final
 transport_session_state_error
 */
enum {
  transport_session_state_idle = 1,
  transport_session_state_connecting = 2,
  transport_session_state_connecting_retry = 4,
  transport_session_state_connecting_unreachable = 5,
  transport_session_state_connected = 3,
  transport_session_state_authenticating = 6, // establishing session
  transport_session_state_authenticated = 7
};

/*!
 Ragel State Machine compiler
 UniversalTransportAgent+State.m is automatically generated from this file
 */

#line 274 "../lib/universal_transport_session.rl"



#line 69 "../lib/universal_transport_session.c"
static const int transport_session_state_start = 8;
static const int transport_session_state_first_final = 8;
static const int transport_session_state_error = 0;

static const int transport_session_state_en_main = 8;


#line 277 "../lib/universal_transport_session.rl"

void transport_session_state_begin(transport_session_t self)
{
    self->current_event = 0;
    self->next_event = 0;
    self->state = transport_session_state_start;
    transport_session_state_set_event(self, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_BEGIN);
}

void transport_session_state_end(transport_session_t self)
{
    self->state = transport_session_state_first_final;
}

void transport_session_state_set_event(transport_session_t self, transport_session_state_event event)
{
    Logc("transport_session_state_event Current State: %d, Event: %d\n", self->state, event);

    // Set current event
    self->current_event = event;
    
    transport_session_state cs = self->state;
    const transport_session_state_event * p = &event;
    const transport_session_state_event * pe = &event + 1;
    
    
#line 104 "../lib/universal_transport_session.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 8:
	if ( (*p) == 1 )
		goto st1;
	goto st0;
st0:
cs = 0;
	goto _out;
tr3:
#line 166 "../lib/universal_transport_session.rl"
	{
       Logc("action do_disconnect_and_save");
       
       // Close websocket
       transport_websocket_close(self->websocket);
    }
	goto st1;
tr12:
#line 120 "../lib/universal_transport_session.rl"
	{
        // mActionBackgroundDispatchAsync(^{
        //    Logc("ACTION do_stop_observing_reachability (state: %d)", _state);
        //     [self reachabilityStopObserving];
        // });
        
        transport_reachability_stop_observing(self->reachability);
    }
#line 166 "../lib/universal_transport_session.rl"
	{
       Logc("action do_disconnect_and_save");
       
       // Close websocket
       transport_websocket_close(self->websocket);
    }
	goto st1;
tr15:
#line 180 "../lib/universal_transport_session.rl"
	{
        // [_stateTimeout cancel];
    }
#line 166 "../lib/universal_transport_session.rl"
	{
       Logc("action do_disconnect_and_save");
       
       // Close websocket
       transport_websocket_close(self->websocket);
    }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 161 "../lib/universal_transport_session.c"
	if ( (*p) == 11 )
		goto tr0;
	if ( 22 <= (*p) && (*p) <= 23 )
		goto st1;
	goto st0;
tr0:
#line 80 "../lib/universal_transport_session.rl"
	{
       Logc("ACTION do_connect (state: %d)", self->state);
       transport_websocket_open(self->websocket);
    }
	goto st2;
tr14:
#line 120 "../lib/universal_transport_session.rl"
	{
        // mActionBackgroundDispatchAsync(^{
        //    Logc("ACTION do_stop_observing_reachability (state: %d)", _state);
        //     [self reachabilityStopObserving];
        // });
        
        transport_reachability_stop_observing(self->reachability);
    }
#line 80 "../lib/universal_transport_session.rl"
	{
       Logc("ACTION do_connect (state: %d)", self->state);
       transport_websocket_open(self->websocket);
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 194 "../lib/universal_transport_session.c"
	switch( (*p) ) {
		case 10: goto tr3;
		case 11: goto st2;
		case 21: goto tr5;
		case 22: goto tr6;
	}
	goto st0;
tr5:
#line 129 "../lib/universal_transport_session.rl"
	{
       Logc("ACTION do_check_session_info (state: %d)", self->state);
        // if(strlen(self->user_id) > 0) // valid agent_id?
//         {
            transport_session_state_push_event(self, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_SESSION_INFO_EXISTS);
        // }
        // else
        // {
        //     transport_session_state_push_event(self, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_SESSION_INFO_NIL);
        // }
    }
	goto st3;
tr20:
#line 191 "../lib/universal_transport_session.rl"
	{
        Logc("action do_close_session");
        
        // Close session
        transport_type_session_t _transport_session = transport_type_session_create((char*)self->user_id, (char*)self->device_token, (char*)self->api_shared_secret);
        transport_session_send_message(self, UNIVERSAL_TRANSPORT_MESSAGE_SESSION_CLOSE, (transport_type_t)_transport_session);
        transport_type_session_release(_transport_session); 
        
        // Push next state event
        transport_session_state_push_event(self, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_RESIGN_ACTIVE);
    }
#line 203 "../lib/universal_transport_session.rl"
	{
        Logc("action did_close_session");
        
        // Notify handler
        if(self->handler.close_callback != NULL)
            self->handler.close_callback(self->handler.context);
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 242 "../lib/universal_transport_session.c"
	switch( (*p) ) {
		case 10: goto tr3;
		case 11: goto st3;
		case 31: goto tr8;
	}
	if ( 22 <= (*p) && (*p) <= 23 )
		goto tr6;
	goto st0;
tr6:
#line 94 "../lib/universal_transport_session.rl"
	{
        // mActionBackgroundDispatchAsync(^{
        //    Logc("ACTION do_check_reachability (state: %d)", _state);
        //     UniversalTransportAgentReachabilityStatus reachabilityStatus = [self reachabilityStatus];
        //     if(reachabilityStatus == UniversalTransportAgentReachabilityStatusNotReachable)
        //         [self stateEvent:UniversalTransportAgentEvent_reachability_is_unreachable];
        //     else
        //         [self stateEvent:UniversalTransportAgentEvent_reachability_is_reachable];
        // });
        
        // TODO needs to be async, dispatch
        if(UNIVERSAL_TRANSPORT_REACHABILITY_NOT_REACHABLE == transport_reachability_status(self->reachability))
            transport_session_state_push_event(self, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_REACHABILITY_IS_UNREACHABLE);
        else
            transport_session_state_push_event(self, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_REACHABILITY_IS_REACHABLE);
    }
	goto st4;
tr10:
#line 85 "../lib/universal_transport_session.rl"
	{
       Logc("set ACTION do_wait_and_retry (state: %d)", self->state);
        // mActionBackgroundDispatchAfterMilliseconds(kDefaultWaitRetryMilliseconds, ^{
        //    Logc("ACTION do_wait_and_retry (state: %d)", _state);
        //     if(self.isConnecting)
        //         [self stateEvent:UniversalTransportAgentEvent_connection_retry];
        // });
    }
	goto st4;
tr17:
#line 180 "../lib/universal_transport_session.rl"
	{
        // [_stateTimeout cancel];
    }
#line 94 "../lib/universal_transport_session.rl"
	{
        // mActionBackgroundDispatchAsync(^{
        //    Logc("ACTION do_check_reachability (state: %d)", _state);
        //     UniversalTransportAgentReachabilityStatus reachabilityStatus = [self reachabilityStatus];
        //     if(reachabilityStatus == UniversalTransportAgentReachabilityStatusNotReachable)
        //         [self stateEvent:UniversalTransportAgentEvent_reachability_is_unreachable];
        //     else
        //         [self stateEvent:UniversalTransportAgentEvent_reachability_is_reachable];
        // });
        
        // TODO needs to be async, dispatch
        if(UNIVERSAL_TRANSPORT_REACHABILITY_NOT_REACHABLE == transport_reachability_status(self->reachability))
            transport_session_state_push_event(self, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_REACHABILITY_IS_UNREACHABLE);
        else
            transport_session_state_push_event(self, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_REACHABILITY_IS_REACHABLE);
    }
	goto st4;
tr19:
#line 180 "../lib/universal_transport_session.rl"
	{
        // [_stateTimeout cancel];
    }
	goto st4;
tr22:
#line 203 "../lib/universal_transport_session.rl"
	{
        Logc("action did_close_session");
        
        // Notify handler
        if(self->handler.close_callback != NULL)
            self->handler.close_callback(self->handler.context);
    }
#line 94 "../lib/universal_transport_session.rl"
	{
        // mActionBackgroundDispatchAsync(^{
        //    Logc("ACTION do_check_reachability (state: %d)", _state);
        //     UniversalTransportAgentReachabilityStatus reachabilityStatus = [self reachabilityStatus];
        //     if(reachabilityStatus == UniversalTransportAgentReachabilityStatusNotReachable)
        //         [self stateEvent:UniversalTransportAgentEvent_reachability_is_unreachable];
        //     else
        //         [self stateEvent:UniversalTransportAgentEvent_reachability_is_reachable];
        // });
        
        // TODO needs to be async, dispatch
        if(UNIVERSAL_TRANSPORT_REACHABILITY_NOT_REACHABLE == transport_reachability_status(self->reachability))
            transport_session_state_push_event(self, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_REACHABILITY_IS_UNREACHABLE);
        else
            transport_session_state_push_event(self, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_REACHABILITY_IS_REACHABLE);
    }
	goto st4;
tr23:
#line 203 "../lib/universal_transport_session.rl"
	{
        Logc("action did_close_session");
        
        // Notify handler
        if(self->handler.close_callback != NULL)
            self->handler.close_callback(self->handler.context);
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 351 "../lib/universal_transport_session.c"
	switch( (*p) ) {
		case 10: goto tr3;
		case 11: goto st4;
		case 24: goto tr0;
		case 61: goto tr10;
		case 62: goto tr11;
	}
	goto st0;
tr11:
#line 111 "../lib/universal_transport_session.rl"
	{
        // mActionBackgroundDispatchAsync(^{
        //    Logc("ACTION do_start_observing_reachability (state: %d)", _state);
        //     [self reachabilityStartObserving];
        // });
        
        transport_reachability_start_observing(self->reachability);
    }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 375 "../lib/universal_transport_session.c"
	switch( (*p) ) {
		case 10: goto tr12;
		case 11: goto st5;
		case 61: goto tr14;
		case 62: goto st5;
	}
	goto st0;
tr8:
#line 146 "../lib/universal_transport_session.rl"
	{
        Logc("action do_authenticate");
        
        transport_type_session_t _transport_session = transport_type_session_create((char *)self->user_id, (char*)self->device_token, (char*)self->api_shared_secret);
        transport_session_send_message(self, UNIVERSAL_TRANSPORT_MESSAGE_SESSION_AUTHENTICATE, (transport_type_t)_transport_session);
        transport_type_session_release(_transport_session); 
    }
#line 173 "../lib/universal_transport_session.rl"
	{
        // _stateTimeout = [UniversalTimeout timeoutAfter:kDefaultTimeoutMilliseconds queue:_backgroundSerialQueue block:^{ // setup timeout
        //     LogErrorc("timeout");
        //     [self stateEvent:UniversalTransportAgentEvent_timeout];
        // }];
    }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 404 "../lib/universal_transport_session.c"
	switch( (*p) ) {
		case 10: goto tr15;
		case 11: goto st6;
		case 41: goto tr18;
		case 42: goto tr19;
		case 71: goto tr8;
	}
	if ( 22 <= (*p) && (*p) <= 23 )
		goto tr17;
	goto st0;
tr18:
#line 180 "../lib/universal_transport_session.rl"
	{
        // [_stateTimeout cancel];
    }
#line 184 "../lib/universal_transport_session.rl"
	{
        Logc("action did_open_session");
        
        if(self->handler.open_callback != NULL)
            self->handler.open_callback(self->handler.context); // did open to handler
    }
#line 154 "../lib/universal_transport_session.rl"
	{ // check configuration after authenticated for further actions
//         mActionBackgroundDispatchAsync(^{
// //            if(_streamIsEnabled) // check stream flag
// //                [self startStream];
// 
//             if(_handler && [_handler respondsToSelector:@selector(open)])
//             {
//                 [_handler open];
//             }
//         });
    }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 444 "../lib/universal_transport_session.c"
	switch( (*p) ) {
		case 10: goto tr20;
		case 11: goto st7;
		case 41: goto st7;
		case 42: goto tr23;
	}
	if ( 22 <= (*p) && (*p) <= 23 )
		goto tr22;
	goto st0;
	}
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 303 "../lib/universal_transport_session.rl"
    
    if(cs == transport_session_state_error)
    {
      LogErrorc("Error - transport_session_state_error");
    }
    if(cs >= transport_session_state_first_final)
    {
      LogErrorc("Error - cs >= transport_session_state_first_final");
      transport_session_state_begin(self);
    }
    
    Logc("Set State: %d", cs);
    
    self->state = cs;
    
    Logc("Final State: %d", self->state);
    
    // Set current event to nil
    self->current_event = 0;
    
    // Check next event
    if(self->next_event) // state machine is single threaded, this is safe
    {
        transport_session_state_event next_event = self->next_event;
        self->next_event = 0;
        transport_session_state_set_event(self, next_event);
    }
}

void transport_session_state_push_event(transport_session_t self, transport_session_state_event event)
{
    if(self->current_event) // state machine is single threaded, this is safe
        self->next_event = event; // current event being processed
    else
        transport_session_state_set_event(self, event); // not event being processed, just set
}

bool transport_session_state_is_idle(transport_session_t self)
{
    return (self->state == transport_session_state_idle);
}

bool transport_session_state_is_connecting(transport_session_t self)
{
    return (self->state == transport_session_state_connecting || self->state == transport_session_state_connecting_retry || self->state == transport_session_state_connecting_unreachable);
}

bool transport_session_state_is_connecting_unreachable(transport_session_t self)
{
    return (self->state == transport_session_state_connecting_unreachable);
}

bool transport_session_state_is_connected(transport_session_t self)
{
    return (self->state == transport_session_state_connected);
}

bool transport_session_state_is_authenticating(transport_session_t self)
{
    return (self->state == transport_session_state_authenticating);
}

bool transport_session_state_is_authenticated(transport_session_t self)
{
    return (self->state == transport_session_state_authenticated);
}

#pragma mark -
#pragma mark transport_reachability_t callback

void transport_session_reachability_callback(int status, const void * context)
{
    transport_session_t transport_session = (transport_session_t)context;

    if(status == UNIVERSAL_TRANSPORT_REACHABILITY_NOT_REACHABLE)
        transport_session_state_push_event(transport_session, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_REACHABILITY_IS_UNREACHABLE);
    else
        transport_session_state_push_event(transport_session, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_REACHABILITY_IS_REACHABLE);
}

#pragma mark -
#pragma mark transport_session_t

transport_session_t transport_session_create(const char * user_id, const char * device_token, const char * api_endpoint_url, const char * api_shared_secret, struct transport_session_handler handler)   
{
    transport_session_t transport_session = (transport_session_t)calloc(1, sizeof(struct transport_session_s));
    if(transport_session)
    {
		assert(user_id);
        transport_session->user_id = strdup(user_id); // device identifier used to open the session with service endpoint
        transport_session->device_token = device_token != NULL ? strdup(device_token) : NULL; // shared secret used to open the session with service endpoint
        
		assert(api_endpoint_url);
		transport_session->api_endpoint_url = strdup(api_endpoint_url);
        assert(api_shared_secret);
		transport_session->api_shared_secret = strdup(api_shared_secret); // shared secret used to open the session with service endpoint
        
        transport_session->handler = handler; // handler of transport agent callbacks
        
        // initialize reachability
        transport_session->reachability = transport_reachability_create(&transport_session_reachability_callback, (const void *)transport_session);
        
        // initialize websocket
        transport_session_websocket_setup(transport_session); 
        
        // initialize state
        transport_session_state_begin(transport_session); 
    }
    return transport_session;
}  

void transport_session_release(transport_session_t transport_session)
{
    transport_reachability_release(transport_session->reachability);
    transport_websocket_release(transport_session->websocket);
	
	free((void *)transport_session->user_id);
	if (transport_session->device_token) {
		free((void*)transport_session->device_token);
	}
	
	free((void*)transport_session->api_endpoint_url);
	free((void*)transport_session->api_shared_secret);
	
    free(transport_session);
}

// TODO Threading model
void transport_session_run(transport_session_t transport_session)
{
    transport_websocket_run(transport_session->websocket);
}

#pragma mark -
#pragma mark open or close

void transport_session_open(transport_session_t transport_session)
{
    transport_session_state_push_event(transport_session, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_BECOME_ACTIVE);
}

bool transport_session_is_open(transport_session_t transport_session)
{
    return transport_session_state_is_authenticated(transport_session);
}

void transport_session_close(transport_session_t transport_session)
{
    transport_session_state_push_event(transport_session, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_RESIGN_ACTIVE);  
}

#pragma mark -
#pragma mark agent to service messages

void transport_session_example_ping(transport_session_t transport_session, transport_type_example_t example_object)
{
	transport_session_send_message(transport_session, UNIVERSAL_TRANSPORT_MESSAGE_EXAMPLE_PING, (transport_type_t)example_object);
}

void transport_session_example_echo(transport_session_t transport_session, transport_type_example_t example_object)
{
	transport_session_send_message(transport_session, UNIVERSAL_TRANSPORT_MESSAGE_EXAMPLE_ECHO, (transport_type_t)example_object);
}

#pragma mark -
#pragma mark send/receive message

void transport_session_send_message(transport_session_t transport_session, uint8_t name, transport_type_t arg)
{
	Logc("transport_session_send_message: %d", name);
    transport_message_t transport_message = transport_message_create(name, arg); // allocate transport message
    transport_websocket_write(transport_session->websocket, transport_message); // write to transport websocket
}

void transport_session_receive_message(transport_session_t transport_session, uint8_t name, transport_type_t arg)
{
    switch(name) 
    {
        case UNIVERSAL_TRANSPORT_MESSAGE_SESSION_AUTHENTICATED:
        {
            transport_session_state_push_event(transport_session, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_AUTHENTICATED_RECEIVED);
            transport_type_session_release((transport_type_session_t)arg);
        }
        break;
        case UNIVERSAL_TRANSPORT_MESSAGE_SESSION_CLOSE:
        {
            transport_session_state_push_event(transport_session, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_CLOSE_RECEIVED);
            transport_type_session_release((transport_type_session_t)arg);
        }
        break;

		// TODO handle the error messages
        
        default:
        {
            Logc("transport_session_receive_message: message not handled %d\n", name);
        }      
    }
}

#pragma mark -
#pragma mark websocket

void transport_session_websocket_setup(transport_session_t transport_session)
{
    struct transport_websocket_handler websocket_handler = {
        transport_session,
        &transport_session_websocket_open_callback,
        &transport_session_websocket_close_callback, 
        &transport_session_websocket_fail_callback, 
        &transport_session_websocket_read_callback
    };

    transport_session->websocket = transport_websocket_create(websocket_handler, transport_session->api_endpoint_url);
}

void transport_session_websocket_open_callback(const void *context)
{
    PrettyLogc;
    transport_session_state_push_event((transport_session_t)context, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_CONNECTION_ESTABLISHED);
}

void transport_session_websocket_close_callback(const void *context)
{
    PrettyLogc;
    transport_session_state_push_event((transport_session_t)context, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_CONNECTION_DISCONNECTED);
}

void transport_session_websocket_fail_callback(const void *context)
{
    PrettyLogc;
    transport_session_state_push_event((transport_session_t)context, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_CONNECTION_FAILED);
}

void transport_session_websocket_read_callback(const void *context, transport_message_t transport_message)
{
    PrettyLogc;
    
    uint8_t name = 0;
    transport_type_t arg = NULL;
    transport_message_unpack(transport_message, &name, (transport_type_t *)&arg);
    transport_session_receive_message((transport_session_t)context, name, arg); // the transport_type_t arg must be released by the handler
    transport_message_release(transport_message); // release message
}

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
%%{
    machine transport_session_state;

    begin=1;
    become_active=11;
    resign_active=10;
    connection_established=21;
    connection_failed=22;
    connection_disconnected=23;
    connection_retry=24;
    session_info_exists=31;
    session_info_nil=32;
    transport_authenticated_received=41;
    transport_close_received=42;
    transport_error_received=43;
    reachability_is_reachable=61;
    reachability_is_unreachable=62;
    timeout=71;
    
    action do_connect {
       Logc("ACTION do_connect (state: %d)", self->state);
       transport_websocket_open(self->websocket);
    }
    
    action do_wait_and_retry {
       Logc("set ACTION do_wait_and_retry (state: %d)", self->state);
        // mActionBackgroundDispatchAfterMilliseconds(kDefaultWaitRetryMilliseconds, ^{
        //    Logc("ACTION do_wait_and_retry (state: %d)", _state);
        //     if(self.isConnecting)
        //         [self stateEvent:UniversalTransportAgentEvent_connection_retry];
        // });
    }
    
    action do_check_reachability {
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
    
    action do_start_observing_reachability {
        // mActionBackgroundDispatchAsync(^{
        //    Logc("ACTION do_start_observing_reachability (state: %d)", _state);
        //     [self reachabilityStartObserving];
        // });
        
        transport_reachability_start_observing(self->reachability);
    }
    
    action do_stop_observing_reachability {
        // mActionBackgroundDispatchAsync(^{
        //    Logc("ACTION do_stop_observing_reachability (state: %d)", _state);
        //     [self reachabilityStopObserving];
        // });
        
        transport_reachability_stop_observing(self->reachability);
    }
    
    action do_check_session_info {
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
    
    action do_raise_session_error {
       Logc("action do_raise_session_error");
        LogErrorc("Could not establish session because session info is nil");
    }
    
    action do_authenticate {
        Logc("action do_authenticate");
        
        transport_type_session_t _transport_session = transport_type_session_create((char *)self->user_id, (char*)self->device_token, (char*)self->api_shared_secret);
        transport_session_send_message(self, UNIVERSAL_TRANSPORT_MESSAGE_SESSION_AUTHENTICATE, (transport_type_t)_transport_session);
        transport_type_session_release(_transport_session); 
    }
    
    action do_check_config { // check configuration after authenticated for further actions
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
    
    action do_disconnect_and_save {
       Logc("action do_disconnect_and_save");
       
       // Close websocket
       transport_websocket_close(self->websocket);
    }
    
    action do_set_timeout {
        // _stateTimeout = [UniversalTimeout timeoutAfter:kDefaultTimeoutMilliseconds queue:_backgroundSerialQueue block:^{ // setup timeout
        //     LogErrorc("timeout");
        //     [self stateEvent:UniversalTransportAgentEvent_timeout];
        // }];
    }
    
    action do_cancel_timeout {
        // [_stateTimeout cancel];
    }
    
    action did_open_session {
        Logc("action did_open_session");
        
        if(self->handler.open_callback != NULL)
            self->handler.open_callback(self->handler.context); // did open to handler
    }
    
    action do_close_session {
        Logc("action do_close_session");
        
        // Close session
        transport_type_session_t _transport_session = transport_type_session_create((char*)self->user_id, (char*)self->device_token, (char*)self->api_shared_secret);
        transport_session_send_message(self, UNIVERSAL_TRANSPORT_MESSAGE_SESSION_CLOSE, (transport_type_t)_transport_session);
        transport_type_session_release(_transport_session); 
        
        // Push next state event
        transport_session_state_push_event(self, UNIVERSAL_TRANSPORT_SESSION_STATE_EVENT_RESIGN_ACTIVE);
    }
    
    action did_close_session {
        Logc("action did_close_session");
        
        // Notify handler
        if(self->handler.close_callback != NULL)
            self->handler.close_callback(self->handler.context);
    }

    assistant = (
        start: (
            begin -> IDLE
        ),

        IDLE: (
            become_active @do_connect -> CONNECTING |
            connection_disconnected* -> IDLE |
            connection_failed* -> IDLE
        ),

        CONNECTING: (
            connection_established @do_check_session_info -> CONNECTED |
            connection_failed @do_check_reachability -> CONNECTING_RETRY |
            resign_active @do_disconnect_and_save -> IDLE |
            become_active -> CONNECTING
        ),
                 
        CONNECTING_RETRY: (
            connection_retry @do_connect -> CONNECTING |
            reachability_is_reachable @do_wait_and_retry -> CONNECTING_RETRY |
            reachability_is_unreachable @do_start_observing_reachability -> CONNECTING_UNREACHABLE |
            resign_active @do_disconnect_and_save -> IDLE |
            become_active -> CONNECTING_RETRY
        ),
                 
        CONNECTING_UNREACHABLE: (
            reachability_is_reachable @do_stop_observing_reachability @do_connect -> CONNECTING |
            reachability_is_unreachable* -> CONNECTING_UNREACHABLE |
            resign_active @do_stop_observing_reachability @do_disconnect_and_save -> IDLE |
            become_active -> CONNECTING_UNREACHABLE
        ),

        CONNECTED: (
            connection_disconnected @do_check_reachability -> CONNECTING_RETRY |
            connection_failed @do_check_reachability -> CONNECTING_RETRY |
            session_info_exists @do_authenticate @do_set_timeout -> AUTHENTICATING |
            resign_active @do_disconnect_and_save -> IDLE |
            become_active -> CONNECTED
        ),
    
        AUTHENTICATING: (
            timeout* @do_authenticate @do_set_timeout -> AUTHENTICATING |
            connection_disconnected @do_cancel_timeout @do_check_reachability -> CONNECTING_RETRY |
            connection_failed @do_cancel_timeout @do_check_reachability -> CONNECTING_RETRY |
            transport_close_received @do_cancel_timeout -> CONNECTING_RETRY |
            transport_authenticated_received @do_cancel_timeout @did_open_session @do_check_config -> AUTHENTICATED |
            resign_active @do_cancel_timeout @do_disconnect_and_save -> IDLE |
            become_active -> AUTHENTICATING
        ),

        AUTHENTICATED: (
            connection_disconnected @did_close_session @do_check_reachability -> CONNECTING_RETRY |
            connection_failed @did_close_session @do_check_reachability -> CONNECTING_RETRY |
            transport_close_received @did_close_session -> CONNECTING_RETRY |
            transport_authenticated_received* -> AUTHENTICATED |
            resign_active @do_close_session @did_close_session -> CONNECTED |
            become_active -> AUTHENTICATED
        )
    );

    main := ( assistant )*;

}%%

%% write data;

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
    
    %% write exec;
    
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

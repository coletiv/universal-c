/*

universal_transport_websocket.c
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

#include "universal_transport_websocket.h"
#include "universal_transport_constants.h"
#include "universal_transport_queue.h"

#include <sys/types.h>

#pragma mark -
#pragma mark libwebsocket session data

#define TRANSPORT_WEBSOCKET_MAX_FRAME_SIZE 2048

/*!
* @typedef transport_websocket_t
*
* @abstract
* Transport websocket internal struct.
* Contains information about libwebsocket state,
* host, port and queue of pending messages
*/
struct transport_websocket_s {
    
    struct lws_context_creation_info ws_context_creation_info;
    struct lws_context * ws_context;
    struct lws * ws;

    int ws_status;
    
    struct transport_websocket_handler handler;
    
    char * host;
    unsigned int port;
    unsigned int ssl; // ssl connection 0 = ws://, 1 = wss:// encrypted, 2 = wss:// allow self signed certs
    
    transport_queue_t queue; // queue used to keep messages waiting for space available in the write buffer
};

/*!
* @typedef transport_websocket_session_data
*
* @abstract
* Transport websocket internal session data struct.
* It is a read/write buffer.
*/
struct transport_websocket_session_data {
    uint8_t bytes[LWS_SEND_BUFFER_PRE_PADDING + TRANSPORT_WEBSOCKET_MAX_FRAME_SIZE + LWS_SEND_BUFFER_POST_PADDING];
	size_t size;
	int index;
};

#pragma mark -
#pragma mark libwebsocket callback function

static int websocket_callback(struct lws * wsi, enum lws_callback_reasons reason, void * user, void * in_bytes, size_t in_size)
{
	if (wsi != NULL) 
	{	
        struct lws_context * context = lws_get_context(wsi);
        transport_websocket_t transport_websocket = (transport_websocket_t)lws_context_user(context);
        struct transport_websocket_session_data * session_data = (struct transport_websocket_session_data *)user;
        
    	switch (reason) 
        {
            case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            {
        		//Logc("transport_websocket: LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n");
                transport_websocket->ws_status = LWS_CALLBACK_CLIENT_CONNECTION_ERROR;
                
                if(transport_websocket->handler.fail_callback != NULL)
                {
                    transport_websocket->handler.fail_callback(transport_websocket->handler.context);
                }
            }   
        		break;

            case LWS_CALLBACK_CLIENT_ESTABLISHED:
            {
        		//Logc("transport_websocket: LWS_CALLBACK_CLIENT_ESTABLISHED\n");
                session_data->size = 0;
                session_data->index = 0;
                transport_websocket->ws_status = LWS_CALLBACK_CLIENT_ESTABLISHED;
                
                // LWS_CALLBACK_CLIENT_WRITEABLE will come next service
                lws_callback_on_writable(wsi);

                if(transport_websocket->handler.open_callback != NULL)
                {
                    transport_websocket->handler.open_callback(transport_websocket->handler.context);
                }
            }
        		break;

            case LWS_CALLBACK_CLOSED:
            {
        		//Logc("transport_websocket: LWS_CALLBACK_CLOSED\n");
                transport_websocket->ws_status = LWS_CALLBACK_CLOSED;    
                
                if(transport_websocket->handler.close_callback != NULL)
                {
                    transport_websocket->handler.close_callback(transport_websocket->handler.context);
                }
            }
                break;

            case LWS_CALLBACK_CLIENT_RECEIVE:
            {
                //Logc("transport_websocket: LWS_CALLBACK_CLIENT_RECEIVE (%lu)\n", in_size);
                
                if(transport_websocket->handler.read_callback != NULL && in_size > 0)
                {
                    transport_message_t message = transport_message_copy(in_bytes, in_size);
                    transport_websocket->handler.read_callback(transport_websocket->handler.context, message); // handler should release message
                }
            }
                break;

        	case LWS_CALLBACK_CLIENT_WRITEABLE:
            {
                //Logc("transport_websocket: LWS_CALLBACK_CLIENT_WRITEABLE\n");
    		
                // dequeue
                while (!transport_queue_is_empty(transport_websocket->queue)) 
                {
                    transport_message_t message = (transport_message_t)transport_queue_pop(transport_websocket->queue);
                    
                    memcpy((void *)&session_data->bytes[LWS_SEND_BUFFER_PRE_PADDING], message->data, message->size);
                    session_data->size = message->size;
                    int n = lws_write(wsi, &session_data->bytes[LWS_SEND_BUFFER_PRE_PADDING], session_data->size, LWS_WRITE_BINARY);
                    
                    if (n < 0) 
                    {
                        return -1;
                    }
                    
                    if (n < (int)session_data->size)
                    {
                        LogErrorc("LWS_CALLBACK_CLIENT_WRITEABLE: Partial write");
                        return -1;
                    }
                    
                    transport_message_release(message);
                }
            }
        		break;

        	default:
        		break;
    	}
    }

	return 0;
}

#pragma mark -
#pragma mark libwebsocket protocols

static struct lws_protocols websocket_protocols[] = {
    {
		"com.coletiv.universal", // name
		websocket_callback, // callback
		sizeof (struct transport_websocket_session_data),
        TRANSPORT_WEBSOCKET_MAX_FRAME_SIZE,
        0, 0
	},
	{
		NULL, NULL, 0, 0, 0, 0 /* End of list */
    }
};

#pragma mark -
#pragma mark url

void transport_websocket_configure_url(transport_websocket_t transport_websocket, const char * url)
{
	if (transport_websocket && url && strlen(url) < 256) 
	{
		Logc("transport_websocket URL: %s", url);
		
		// Host
		char host[256];
			
		// Port
		unsigned int port;
		
		// SSL Enabled
		unsigned int ssl_enabled = 0;
		
		// Try parsing url with port and ssl (ie. wss://host:port)
		int result = sscanf(url, "wss://%99[^:]:%99d[^\n]", host, &port);
		if (result == 2)
		{
			ssl_enabled = 1;
		} 
		else 
		{
			// Try parsing url without port (ie. wss://host)
			result = sscanf(url, "wss://%99[^:][^\n]", host);
			if (result == 1) 
			{
				ssl_enabled = 1;
				port = TRANSPORT_WEBSOCKET_DEFAULT_SSL_PORT;
			}
			else 
			{
				// Try parsing url with port and no ssl (ie. ws://host:port)
				result = sscanf(url, "ws://%99[^:]:%99d[^\n]", host, &port);
				if (result == 2)
				{
					ssl_enabled = 0;
				}
				else
				{
					// Try parsing url without port and no sll (ie. ws://host)
					result = sscanf(url, "ws://%99[^:][^\n]", host);
					if (result == 1) 
					{
						ssl_enabled = 0;
						port = TRANSPORT_WEBSOCKET_DEFAULT_NON_SSL_PORT;
					}
					else
					{
						LogErrorc("transport_websocket invalid url format");
                        return;
					}
				}
			}
		}
		
		Logc("transport_websocket Host: %s Port: %d SSL Enabled: %d", host, port, ssl_enabled);
		
		transport_websocket->host = strdup(host);
		transport_websocket->port = port;
		
		// ssl 0 = ws://, 1 = wss:// encrypted, 2 = wss:// allow self signed certs
		transport_websocket->ssl = ssl_enabled ? TRANSPORT_WEBSOCKET_DEFAULT_SSL_MODE : 0;
	}
}

#pragma mark -
#pragma mark transport_websocket_t

transport_websocket_t transport_websocket_create(struct transport_websocket_handler handler, const char * url)
{
    transport_websocket_t transport_websocket = (transport_websocket_t)calloc(1, sizeof(struct transport_websocket_s));
    if(transport_websocket)
    {
        // handler
        transport_websocket->handler = handler;
        
        // Configure host, port and ssl mode from url
		transport_websocket_configure_url(transport_websocket, url);
        
        // queue
        transport_websocket->queue = transport_queue_create();
        
        // ws
        transport_websocket->ws_context = NULL; // NULL until open
        transport_websocket->ws_status = LWS_CALLBACK_CLOSED;
        transport_websocket->ws = NULL; // NULL until open
    }
    return transport_websocket;
}

void transport_websocket_release(transport_websocket_t transport_websocket)
{
    transport_websocket_close(transport_websocket);
	
	free(transport_websocket->host);
    free(transport_websocket);
    
    transport_queue_destroy(transport_websocket->queue);    
}

#pragma mark -
#pragma mark open/close

void transport_websocket_open(transport_websocket_t transport_websocket)
{
    if(transport_websocket->ws_status == LWS_CALLBACK_CLOSED && transport_websocket->host != NULL)
    {
        // Set ws log level
        lws_set_log_level(LLL_ERR, NULL);
        
        // Create ws context
        struct lws_context_creation_info ws_context_creation_info;
        memset(&ws_context_creation_info, 0, sizeof(ws_context_creation_info));
    	ws_context_creation_info.port = CONTEXT_PORT_NO_LISTEN;
    	ws_context_creation_info.protocols = websocket_protocols;
        ws_context_creation_info.extensions = lws_get_internal_extensions();
    	ws_context_creation_info.gid = -1;
    	ws_context_creation_info.uid = -1;
        ws_context_creation_info.user = transport_websocket;
        struct lws_context * ws_context = lws_create_context(&ws_context_creation_info);
        
        // Check ws context
        if (ws_context == NULL) 
        {
            LogErrorc("transport_websocket: Creating libwebsocket context failed");
            return;
        }

        transport_websocket->ws_context_creation_info = ws_context_creation_info;
        transport_websocket->ws_context = ws_context;
        
        // Try ws connect
        char address_port[256]; // TODO dynamic
        snprintf(address_port, 256, "%s:%u", transport_websocket->host, transport_websocket->port & 65535);
        struct lws * ws = lws_client_connect(ws_context, transport_websocket->host, transport_websocket->port, transport_websocket->ssl, "/", address_port, "origin", websocket_protocols[0].name, -1);

        // Check ws
        if (ws == NULL) 
        {
            LogErrorc("transport_websocket: Connecting libwebsocket failed");
            return;
        }
        
        transport_websocket->ws = ws;
    }
}

void transport_websocket_close(transport_websocket_t transport_websocket)
{
    if(transport_websocket->ws_context != NULL)
    {
        lws_context_destroy(transport_websocket->ws_context);
        transport_websocket->ws_context = NULL;
    }
}

#pragma mark -
#pragma mark write/read

void transport_websocket_write(transport_websocket_t transport_websocket, transport_message_t transport_message)
{
    if(transport_websocket->ws_context != NULL)
    {
        // enqueue
        transport_queue_push(transport_websocket->queue, transport_message);    
            
        lws_callback_on_writable(transport_websocket->ws);
    }
}

#pragma mark -
#pragma mark run

void transport_websocket_run(transport_websocket_t transport_websocket)
{
    lws_service(transport_websocket->ws_context, 0); // timeout_ms = 0, don't wait   
}
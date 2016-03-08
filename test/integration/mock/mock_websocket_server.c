/*
 
 mock_websocket_server.c
 Universal-Transport-C
 
 Copyright (cc) 2012 Luis Laugga.
 Some rights reserved, all wrongs deserved.
 
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

#include "mock_websocket_server.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_PAYLOAD 1400

// static void dump_handshake_info(struct libwebsocket *wsi)
// {
//     int n;
//     static const char *token_names[] = {
//         /*[WSI_TOKEN_GET_URI]        =*/ "GET URI",
//         /*[WSI_TOKEN_HOST]        =*/ "Host",
//         /*[WSI_TOKEN_CONNECTION]    =*/ "Connection",
//         /*[WSI_TOKEN_KEY1]        =*/ "key 1",
//         /*[WSI_TOKEN_KEY2]        =*/ "key 2",
//         /*[WSI_TOKEN_PROTOCOL]        =*/ "Protocol",
//         /*[WSI_TOKEN_UPGRADE]        =*/ "Upgrade",
//         /*[WSI_TOKEN_ORIGIN]        =*/ "Origin",
//         /*[WSI_TOKEN_DRAFT]        =*/ "Draft",
//         /*[WSI_TOKEN_CHALLENGE]        =*/ "Challenge",
// 
//         /* new for 04 */
//         /*[WSI_TOKEN_KEY]        =*/ "Key",
//         /*[WSI_TOKEN_VERSION]        =*/ "Version",
//         /*[WSI_TOKEN_SWORIGIN]        =*/ "Sworigin",
// 
//         /* new for 05 */
//         /*[WSI_TOKEN_EXTENSIONS]    =*/ "Extensions",
// 
//         /* client receives these */
//         /*[WSI_TOKEN_ACCEPT]        =*/ "Accept",
//         /*[WSI_TOKEN_NONCE]        =*/ "Nonce",
//         /*[WSI_TOKEN_HTTP]        =*/ "Http",
// 
//         "Accept:",
//         "If-Modified-Since:",
//         "Accept-Encoding:",
//         "Accept-Language:",
//         "Pragma:",
//         "Cache-Control:",
//         "Authorization:",
//         "Cookie:",
//         "Content-Type:",
//         "Date:",
//         "Range:",
//         "Referer:",
//         "Uri-Args:",
// 
//         /*[WSI_TOKEN_MUXURL]    =*/ "MuxURL",
//     };
//     char buf[256];
// 
//     for (n = 0; n < sizeof(token_names) / sizeof(token_names[0]); n++) {
//         if (!lws_hdr_total_length(wsi, n))
//             continue;
// 
//         lws_hdr_copy(wsi, buf, sizeof buf, n);
// 
//         fprintf(stderr, "    %s = %s\n", token_names[n], buf);
//     }
// }

#define MAX_MESSAGE_QUEUE 32

struct per_session_data__universal {
	struct lws *wsi;
	int ringbuffer_tail;
};

struct a_message {
	void *payload;
	size_t len;
};

static struct a_message ringbuffer[MAX_MESSAGE_QUEUE];
static int ringbuffer_head;

static int callback_websocket_universal(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
	int n;
	struct per_session_data__universal *pss = (struct per_session_data__universal *)user;
    
	switch (reason) {

	case LWS_CALLBACK_ESTABLISHED:
        printf("transport_mock_websocket_server_t LWS_CALLBACK_ESTABLISHED\n");
		pss->ringbuffer_tail = ringbuffer_head;
		pss->wsi = wsi;
		break;

	case LWS_CALLBACK_PROTOCOL_DESTROY:
       printf("transport_mock_websocket_server_t LWS_CALLBACK_PROTOCOL_DESTROY\n");
		for (n = 0; n < sizeof ringbuffer / sizeof ringbuffer[0]; n++)
			if (ringbuffer[n].payload)
				free(ringbuffer[n].payload);
		break;
	case LWS_CALLBACK_SERVER_WRITEABLE:
        printf("transport_mock_websocket_server_t LWS_CALLBACK_SERVER_WRITEABLE\n");
		while (pss->ringbuffer_tail != ringbuffer_head) {

			n = lws_write(wsi, (unsigned char *)
				   ringbuffer[pss->ringbuffer_tail].payload +
				   LWS_SEND_BUFFER_PRE_PADDING,
				   ringbuffer[pss->ringbuffer_tail].len,
								LWS_WRITE_BINARY);
			if (n < ringbuffer[pss->ringbuffer_tail].len) {
				lwsl_err("ERROR %d writing to mirror socket\n", n);
				return -1;
			}
			if (n < ringbuffer[pss->ringbuffer_tail].len)
				lwsl_err("mirror partial write %d vs %d\n",
				       n, ringbuffer[pss->ringbuffer_tail].len);

			if (pss->ringbuffer_tail == (MAX_MESSAGE_QUEUE - 1))
				pss->ringbuffer_tail = 0;
			else
				pss->ringbuffer_tail++;

			if (((ringbuffer_head - pss->ringbuffer_tail) &
				  (MAX_MESSAGE_QUEUE - 1)) == (MAX_MESSAGE_QUEUE - 15))
				lws_rx_flow_allow_all_protocol(lws_get_context(wsi), lws_get_protocol(wsi));

			// lwsl_debug("tx fifo %d\n", (ringbuffer_head - pss->ringbuffer_tail) & (MAX_MESSAGE_QUEUE - 1));

			if (lws_send_pipe_choked(wsi)) {
				lws_callback_on_writable(wsi);
				break;
			}
			/*
			 * for tests with chrome on same machine as client and
			 * server, this is needed to stop chrome choking
			 */
			usleep(1);
		}
		break;

	case LWS_CALLBACK_RECEIVE:
        printf("transport_mock_websocket_server_t LWS_CALLBACK_RECEIVE (%lu)\n", len);
		if (((ringbuffer_head - pss->ringbuffer_tail) &
				  (MAX_MESSAGE_QUEUE - 1)) == (MAX_MESSAGE_QUEUE - 1)) {
			lwsl_err("dropping!\n");
			goto choke;
		}

		if (ringbuffer[ringbuffer_head].payload)
			free(ringbuffer[ringbuffer_head].payload);

		ringbuffer[ringbuffer_head].payload =
				malloc(LWS_SEND_BUFFER_PRE_PADDING + len +
						  LWS_SEND_BUFFER_POST_PADDING);
		ringbuffer[ringbuffer_head].len = len;
		memcpy((char *)ringbuffer[ringbuffer_head].payload +
					  LWS_SEND_BUFFER_PRE_PADDING, in, len);
		if (ringbuffer_head == (MAX_MESSAGE_QUEUE - 1))
			ringbuffer_head = 0;
		else
			ringbuffer_head++;

		if (((ringbuffer_head - pss->ringbuffer_tail) &
				  (MAX_MESSAGE_QUEUE - 1)) != (MAX_MESSAGE_QUEUE - 2))
			goto done;

choke:
		lwsl_debug("LWS_CALLBACK_RECEIVE: throttling %p\n", wsi);
		lws_rx_flow_control(wsi, 0);

//		lwsl_debug("rx fifo %d\n", (ringbuffer_head - pss->ringbuffer_tail) & (MAX_MESSAGE_QUEUE - 1));
done:
		lws_callback_on_writable_all_protocol(lws_get_context(wsi), lws_get_protocol(wsi));
		break;

	/*
	 * this just demonstrates how to use the protocol filter. If you won't
	 * study and reject connections based on header content, you don't need
	 * to handle this callback
	 */
	case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
        printf("transport_mock_websocket_server_t LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION\n");
		//dump_handshake_info(wsi);
		/* you could return non-zero here and kill the connection */
		break;

	default:
		break;
	}

	return 0;
}

#define MAX_FRAME_SIZE 2048

static struct lws_protocols protocols[] = {
	{
		"com.coletiv.universal",
		callback_websocket_universal,
		sizeof(struct per_session_data__universal),
		MAX_FRAME_SIZE,
	},
	{ NULL, NULL, 0, 0 } /* terminator */
};

transport_mock_websocket_server_t transport_mock_websocket_server_create(unsigned int port)
{
    transport_mock_websocket_server_t transport_mock_websocket_server = (transport_mock_websocket_server_t)calloc(1, sizeof(struct transport_mock_websocket_server_s));
    if(transport_mock_websocket_server)
    {
        struct lws_context_creation_info ws_info;
        memset(&ws_info, 0, sizeof(ws_info));
    	ws_info.port = port; // listening port
        ws_info.iface = NULL; // bind to all available interfaces
    	ws_info.protocols = protocols;
        ws_info.extensions = NULL;
    	ws_info.gid = -1;
    	ws_info.uid = -1;
    	ws_info.options = 0;
        
        transport_mock_websocket_server->ws_context = lws_create_context(&ws_info);
            
    	if (transport_mock_websocket_server->ws_context == NULL) 
        {
    		lwsl_err("libwebsocket init failed\n");
    		return NULL;
    	}
        
        
    }
    return transport_mock_websocket_server;
}

void transport_mock_websocket_server_release(transport_mock_websocket_server_t transport_mock_websocket_server)
{
    lws_context_destroy(transport_mock_websocket_server->ws_context);
    free(transport_mock_websocket_server);
}

void transport_mock_websocket_server_run(transport_mock_websocket_server_t transport_mock_websocket_server)
{
	lws_callback_on_writable_all_protocol(transport_mock_websocket_server->ws_context, &protocols[0]);
    lws_service(transport_mock_websocket_server->ws_context, 50);
}

/*
 
 mock_websocket_server.h
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

#ifndef __UNIVERSAL_C_TRANSPORT_MOCK_WEBSOCKET_SERVER_H__
#define __UNIVERSAL_C_TRANSPORT_MOCK_WEBSOCKET_SERVER_H__

#include "../../../lib/universal_internal.h"

#include <libwebsockets.h>

UNIVERSAL_EXTERN_C_BEGIN

struct transport_mock_websocket_server_s {
    struct lws_context * ws_context;
		struct lws * wsi;
};

typedef struct transport_mock_websocket_server_s * transport_mock_websocket_server_t;

transport_mock_websocket_server_t transport_mock_websocket_server_create(unsigned int port);
void transport_mock_websocket_server_release(transport_mock_websocket_server_t transport_mock_websocket_server);

void transport_mock_websocket_server_run(transport_mock_websocket_server_t transport_mock_websocket_server);

UNIVERSAL_EXTERN_C_END

#endif /* __UNIVERSAL_C_TRANSPORT_MOCK_WEBSOCKET_SERVER_H__ */

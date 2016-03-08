/*

universal_transport_messages.h
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

#ifndef __UNIVERSAL_C_TRANSPORT_MESSAGES_H__
#define __UNIVERSAL_C_TRANSPORT_MESSAGES_H__

#include "universal_internal.h"

#include "universal_transport_types.h"

#include <msgpack.h>

UNIVERSAL_EXTERN_C_BEGIN

enum {
UNIVERSAL_TRANSPORT_MESSAGE_ERROR_THROW = 11,
UNIVERSAL_TRANSPORT_MESSAGE_SESSION_AUTHENTICATE = 21,
UNIVERSAL_TRANSPORT_MESSAGE_SESSION_AUTHENTICATED = 22,
UNIVERSAL_TRANSPORT_MESSAGE_SESSION_CLOSE = 23,
UNIVERSAL_TRANSPORT_MESSAGE_EXAMPLE_PING = 31,
UNIVERSAL_TRANSPORT_MESSAGE_EXAMPLE_ECHO = 32,
};

/*!
* @typedef transport_message_t
*
* @abstract
* Universal transport message type
*
* @discussion
* The message type is a msgpack buffer with the serialized data.
* The data contains the message name and its arguments (transport types)
*/
typedef msgpack_sbuffer * transport_message_t;

transport_message_t transport_message_create(uint8_t type_verb, transport_type_t arg);
transport_message_t transport_message_copy(const void * data, size_t size);
void transport_message_release(transport_message_t transport_message);

/*!
* supports all message types
*/
void transport_message_pack(transport_message_t transport_message, uint8_t type_verb, transport_type_t arg);

/*!
* supports all message types
*/
int transport_message_unpack(transport_message_t transport_message, uint8_t * type_verb, transport_type_t * arg);

UNIVERSAL_EXTERN_C_END

#endif /* __UNIVERSAL_C_TRANSPORT_MESSAGES_H__ */

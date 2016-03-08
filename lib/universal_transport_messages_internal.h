/*

universal_transport_messages_internal.h
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

#ifndef __UNIVERSAL_C_TRANSPORT_MESSAGES_INTERNAL_H__
#define __UNIVERSAL_C_TRANSPORT_MESSAGES_INTERNAL_H__

#include "universal_internal.h"
#include "universal_transport_types.h"

#include <msgpack.h>

UNIVERSAL_EXTERN_C_BEGIN
               
/*!
* @header
* message_t internal serialization details.
*
* The message is serialized to a msgpack array type.
* The first and second element of the msgpack array is the type and action that identifies the message (and argument type).
* The third element of the msgpack array match the argument type fields, following the order they were declared in the struct.
*
*      +------------------------------------------------+
*      |                MESSAGE                         |
*      +------------------------------------------------+
*      +----------+-----------+-------------------------+
*      | array(N) |  array[0] |  array[1] | array[2]    |
*      +----------+-----------+-------------------------+
*                 +-----------+-------------------------+
*                 | type      | action    | type object |
*                 +-----------+-------------------------+
*
*/

#define UNIVERSAL_TRANSPORT_MESSAGE_ARRAY_SIZE 3 // [type, action, object]

void transport_message_pack_error(msgpack_packer * msgpack_pk,transport_type_error_t object);
void transport_message_unpack_error(msgpack_object * msgpack_obj,transport_type_error_t * object);
void transport_message_pack_session(msgpack_packer * msgpack_pk,transport_type_session_t object);
void transport_message_unpack_session(msgpack_object * msgpack_obj,transport_type_session_t * object);
void transport_message_pack_example(msgpack_packer * msgpack_pk,transport_type_example_t object);
void transport_message_unpack_example(msgpack_object * msgpack_obj,transport_type_example_t * object);

UNIVERSAL_EXTERN_C_END

#endif /* __UNIVERSAL_C_TRANSPORT_MESSAGES_INTERNAL_H__ */

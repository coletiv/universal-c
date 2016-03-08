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

#include "universal_transport_messages.h"
#include "universal_transport_messages_internal.h"

#pragma mark -
#pragma mark transport_message_t

transport_message_t transport_message_create(uint8_t name, transport_type_t arg)
{
	transport_message_t transport_message = msgpack_sbuffer_new();
    transport_message_pack(transport_message, name, arg);
    return transport_message;
}

transport_message_t transport_message_copy(const void * data, size_t size)
{
	transport_message_t transport_message = msgpack_sbuffer_new();
    msgpack_sbuffer_write(transport_message, (const char *)data, (unsigned int)size);
    return transport_message;
}

void transport_message_release(transport_message_t transport_message)
{
    msgpack_sbuffer_free(transport_message);
}

#pragma mark -
#pragma mark pack/unpack

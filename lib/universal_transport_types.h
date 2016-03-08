/*

universal_transport_types.h
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

#ifndef __UNIVERSAL_C_TRANSPORT_TYPES_H__
#define __UNIVERSAL_C_TRANSPORT_TYPES_H__

#include "universal_base.h"

UNIVERSAL_EXTERN_C_BEGIN
  
struct transport_type_s {
    int isa;
    int reserved[16]; // TODO: this size should be equal to the size of the biggest struct we have
};

typedef struct transport_type_s * transport_type_t;
enum {
UNIVERSAL_TRANSPORT_TYPE_ERROR = 1,
UNIVERSAL_TRANSPORT_TYPE_SESSION = 2,
UNIVERSAL_TRANSPORT_TYPE_EXAMPLE = 3,
};
struct transport_type_error_s {
int isa;
const char * level;
const char * description;
};
#define UNIVERSAL_TRANSPORT_TYPE_ERROR_SIZE 2
typedef struct transport_type_error_s * transport_type_error_t;
transport_type_error_t transport_type_error_copy(transport_type_error_t object);
transport_type_error_t transport_type_error_create(const char * level, const char * description);
void transport_type_error_release(transport_type_error_t object);
void transport_type_error_log(transport_type_error_t object);
struct transport_type_session_s {
int isa;
const char * user_id;
const char * device_token;
const char * shared_secret;
};
#define UNIVERSAL_TRANSPORT_TYPE_SESSION_SIZE 3
typedef struct transport_type_session_s * transport_type_session_t;
transport_type_session_t transport_type_session_copy(transport_type_session_t object);
transport_type_session_t transport_type_session_create(const char * user_id, const char * device_token, const char * shared_secret);
void transport_type_session_release(transport_type_session_t object);
void transport_type_session_log(transport_type_session_t object);
struct transport_type_example_s {
int isa;
const char * text;
};
#define UNIVERSAL_TRANSPORT_TYPE_EXAMPLE_SIZE 1
typedef struct transport_type_example_s * transport_type_example_t;
transport_type_example_t transport_type_example_copy(transport_type_example_t object);
transport_type_example_t transport_type_example_create(const char * text);
void transport_type_example_release(transport_type_example_t object);
void transport_type_example_log(transport_type_example_t object);
UNIVERSAL_EXTERN_C_END

#endif /* __UNIVERSAL_C_TRANSPORT_TYPES_H__ */
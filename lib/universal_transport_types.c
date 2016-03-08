/*

universal_transport_types.c
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

#include "universal_transport_types.h"

transport_type_error_t transport_type_error_copy(transport_type_error_t object){
return transport_type_error_create(object->level, object->description);
}
transport_type_error_t transport_type_error_create(const char * level, const char * description){
transport_type_error_t object = (transport_type_error_t)calloc(1, sizeof(struct transport_type_error_s));
if(object){
object->isa = UNIVERSAL_TRANSPORT_TYPE_ERROR;
object->level = level != NULL ? strdup(level) : NULL;
object->description = description != NULL ? strdup(description) : NULL;
}
return object;
}
void transport_type_error_release(transport_type_error_t object){
if(object){
if(object->level){free((void *)object->level);};
if(object->description){free((void *)object->description);};
free(object);
}
}
void transport_type_error_log(transport_type_error_t object){
Logc("{");Logc("  %s",object->level);Logc("  %s",object->description);Logc("}");
}
transport_type_session_t transport_type_session_copy(transport_type_session_t object){
return transport_type_session_create(object->user_id, object->device_token, object->shared_secret);
}
transport_type_session_t transport_type_session_create(const char * user_id, const char * device_token, const char * shared_secret){
transport_type_session_t object = (transport_type_session_t)calloc(1, sizeof(struct transport_type_session_s));
if(object){
object->isa = UNIVERSAL_TRANSPORT_TYPE_SESSION;
object->user_id = user_id != NULL ? strdup(user_id) : NULL;
object->device_token = device_token != NULL ? strdup(device_token) : NULL;
object->shared_secret = shared_secret != NULL ? strdup(shared_secret) : NULL;
}
return object;
}
void transport_type_session_release(transport_type_session_t object){
if(object){
if(object->user_id){free((void *)object->user_id);};
if(object->device_token){free((void *)object->device_token);};
if(object->shared_secret){free((void *)object->shared_secret);};
free(object);
}
}
void transport_type_session_log(transport_type_session_t object){
Logc("{");Logc("  %s",object->user_id);Logc("  %s",object->device_token);Logc("  %s",object->shared_secret);Logc("}");
}
transport_type_example_t transport_type_example_copy(transport_type_example_t object){
return transport_type_example_create(object->text);
}
transport_type_example_t transport_type_example_create(const char * text){
transport_type_example_t object = (transport_type_example_t)calloc(1, sizeof(struct transport_type_example_s));
if(object){
object->isa = UNIVERSAL_TRANSPORT_TYPE_EXAMPLE;
object->text = text != NULL ? strdup(text) : NULL;
}
return object;
}
void transport_type_example_release(transport_type_example_t object){
if(object){
if(object->text){free((void *)object->text);};
free(object);
}
}
void transport_type_example_log(transport_type_example_t object){
Logc("{");Logc("  %s",object->text);Logc("}");
}



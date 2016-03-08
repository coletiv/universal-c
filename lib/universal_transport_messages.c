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
void transport_message_pack(transport_message_t transport_message, uint8_t name, transport_type_t arg){
msgpack_packer * pk = msgpack_packer_new(transport_message, msgpack_sbuffer_write);
switch(name){
case UNIVERSAL_TRANSPORT_MESSAGE_ERROR_THROW:
{
msgpack_pack_array(pk, UNIVERSAL_TRANSPORT_MESSAGE_ARRAY_SIZE);
const char * type = "error";
msgpack_pack_str(pk, strlen(type));
msgpack_pack_str_body(pk, type, strlen(type));
const char * message = "throw";
msgpack_pack_str(pk, strlen(message));
msgpack_pack_str_body(pk, message, strlen(message));
transport_message_pack_error(pk, (transport_type_error_t)arg);
} break;
case UNIVERSAL_TRANSPORT_MESSAGE_SESSION_AUTHENTICATE:
{
msgpack_pack_array(pk, UNIVERSAL_TRANSPORT_MESSAGE_ARRAY_SIZE);
const char * type = "session";
msgpack_pack_str(pk, strlen(type));
msgpack_pack_str_body(pk, type, strlen(type));
const char * message = "authenticate";
msgpack_pack_str(pk, strlen(message));
msgpack_pack_str_body(pk, message, strlen(message));
transport_message_pack_session(pk, (transport_type_session_t)arg);
} break;
case UNIVERSAL_TRANSPORT_MESSAGE_SESSION_AUTHENTICATED:
{
msgpack_pack_array(pk, UNIVERSAL_TRANSPORT_MESSAGE_ARRAY_SIZE);
const char * type = "session";
msgpack_pack_str(pk, strlen(type));
msgpack_pack_str_body(pk, type, strlen(type));
const char * message = "authenticated";
msgpack_pack_str(pk, strlen(message));
msgpack_pack_str_body(pk, message, strlen(message));
transport_message_pack_session(pk, (transport_type_session_t)arg);
} break;
case UNIVERSAL_TRANSPORT_MESSAGE_SESSION_CLOSE:
{
msgpack_pack_array(pk, UNIVERSAL_TRANSPORT_MESSAGE_ARRAY_SIZE);
const char * type = "session";
msgpack_pack_str(pk, strlen(type));
msgpack_pack_str_body(pk, type, strlen(type));
const char * message = "close";
msgpack_pack_str(pk, strlen(message));
msgpack_pack_str_body(pk, message, strlen(message));
transport_message_pack_session(pk, (transport_type_session_t)arg);
} break;
case UNIVERSAL_TRANSPORT_MESSAGE_EXAMPLE_PING:
{
msgpack_pack_array(pk, UNIVERSAL_TRANSPORT_MESSAGE_ARRAY_SIZE);
const char * type = "example";
msgpack_pack_str(pk, strlen(type));
msgpack_pack_str_body(pk, type, strlen(type));
const char * message = "ping";
msgpack_pack_str(pk, strlen(message));
msgpack_pack_str_body(pk, message, strlen(message));
transport_message_pack_example(pk, (transport_type_example_t)arg);
} break;
case UNIVERSAL_TRANSPORT_MESSAGE_EXAMPLE_ECHO:
{
msgpack_pack_array(pk, UNIVERSAL_TRANSPORT_MESSAGE_ARRAY_SIZE);
const char * type = "example";
msgpack_pack_str(pk, strlen(type));
msgpack_pack_str_body(pk, type, strlen(type));
const char * message = "echo";
msgpack_pack_str(pk, strlen(message));
msgpack_pack_str_body(pk, message, strlen(message));
transport_message_pack_example(pk, (transport_type_example_t)arg);
} break;
}
msgpack_packer_free(pk);
}
int transport_message_unpack(transport_message_t transport_message, uint8_t * name, transport_type_t * arg){
int no_err = 0;
msgpack_unpacked msg;
msgpack_unpacked_init(&msg);
if(msgpack_unpack_next(&msg, transport_message->data, transport_message->size, NULL)){
msgpack_object obj = msg.data;
msgpack_object_print(stdout, obj);
if(obj.type == MSGPACK_OBJECT_ARRAY){
if(obj.via.array.size == UNIVERSAL_TRANSPORT_MESSAGE_ARRAY_SIZE){
msgpack_object * array_obj = obj.via.array.ptr;
if(array_obj->type == MSGPACK_OBJECT_STR){
char * type = (char *)calloc(array_obj->via.str.size+1, sizeof(char));
strncpy(type, array_obj->via.str.ptr, array_obj->via.str.size);
if(strcmp(type,"error") == 0){
++array_obj;
if(array_obj->type == MSGPACK_OBJECT_STR){
char * action = (char *)calloc(array_obj->via.str.size+1, sizeof(char));
strncpy(action, array_obj->via.str.ptr, array_obj->via.str.size);
if (strcmp(action, "throw") == 0){
*name=UNIVERSAL_TRANSPORT_MESSAGE_ERROR_THROW;
}
++array_obj;
if(array_obj->type == MSGPACK_OBJECT_MAP){
transport_message_unpack_error(array_obj,(transport_type_error_t*)arg);
}
free(action);
}
}
else if(strcmp(type,"session") == 0){
++array_obj;
if(array_obj->type == MSGPACK_OBJECT_STR){
char * action = (char *)calloc(array_obj->via.str.size+1, sizeof(char));
strncpy(action, array_obj->via.str.ptr, array_obj->via.str.size);
if (strcmp(action, "authenticate") == 0){
*name=UNIVERSAL_TRANSPORT_MESSAGE_SESSION_AUTHENTICATE;
}
else if (strcmp(action, "authenticated") == 0){
*name=UNIVERSAL_TRANSPORT_MESSAGE_SESSION_AUTHENTICATED;
}
else if (strcmp(action, "close") == 0){
*name=UNIVERSAL_TRANSPORT_MESSAGE_SESSION_CLOSE;
}
++array_obj;
if(array_obj->type == MSGPACK_OBJECT_MAP){
transport_message_unpack_session(array_obj,(transport_type_session_t*)arg);
}
free(action);
}
}
else if(strcmp(type,"example") == 0){
++array_obj;
if(array_obj->type == MSGPACK_OBJECT_STR){
char * action = (char *)calloc(array_obj->via.str.size+1, sizeof(char));
strncpy(action, array_obj->via.str.ptr, array_obj->via.str.size);
if (strcmp(action, "ping") == 0){
*name=UNIVERSAL_TRANSPORT_MESSAGE_EXAMPLE_PING;
}
else if (strcmp(action, "echo") == 0){
*name=UNIVERSAL_TRANSPORT_MESSAGE_EXAMPLE_ECHO;
}
++array_obj;
if(array_obj->type == MSGPACK_OBJECT_MAP){
transport_message_unpack_example(array_obj,(transport_type_example_t*)arg);
}
free(action);
}
}
free(type);
}
}
}
}
msgpack_unpacked_destroy(&msg);
return no_err;
}
void transport_message_pack_error(msgpack_packer * msgpack_pk,transport_type_error_t object){
msgpack_pack_map(msgpack_pk,UNIVERSAL_TRANSPORT_TYPE_ERROR_SIZE);
msgpack_pack_str(msgpack_pk,strlen("level"));msgpack_pack_str_body(msgpack_pk,"level",strlen("level"));if(object->level != NULL){msgpack_pack_str(msgpack_pk,strlen(object->level));msgpack_pack_str_body(msgpack_pk,object->level,strlen(object->level));} else {msgpack_pack_nil(msgpack_pk);}
msgpack_pack_str(msgpack_pk,strlen("description"));msgpack_pack_str_body(msgpack_pk,"description",strlen("description"));if(object->description != NULL){msgpack_pack_str(msgpack_pk,strlen(object->description));msgpack_pack_str_body(msgpack_pk,object->description,strlen(object->description));} else {msgpack_pack_nil(msgpack_pk);}
}
void transport_message_unpack_error(msgpack_object * msgpack_obj,transport_type_error_t* object){
*object = NULL;
if (msgpack_obj->type == MSGPACK_OBJECT_MAP) {
if (msgpack_obj->via.map.size == UNIVERSAL_TRANSPORT_TYPE_ERROR_SIZE) {
const char *level=NULL;
const char *description=NULL;
msgpack_object_kv* const kv_end = msgpack_obj->via.map.ptr + msgpack_obj->via.map.size;
for(msgpack_object_kv* kv = msgpack_obj->via.map.ptr; kv < kv_end; kv++){
msgpack_object k_obj = kv->key;
char * key = (char * )calloc(k_obj.via.str.size+1, sizeof(char));
strncpy(key, k_obj.via.str.ptr, k_obj.via.str.size);
msgpack_object v_obj = kv->val;
if (v_obj.type == MSGPACK_OBJECT_STR){if (strcmp(key,"level") == 0){char * value = (char * )calloc(v_obj.via.str.size+1, sizeof(char));strncpy(value, v_obj.via.str.ptr, v_obj.via.str.size);level = (char * )calloc(strlen(value), sizeof(char));strcpy((char*)level, value);free((void*)value);}}
if (v_obj.type == MSGPACK_OBJECT_STR){if (strcmp(key,"description") == 0){char * value = (char * )calloc(v_obj.via.str.size+1, sizeof(char));strncpy(value, v_obj.via.str.ptr, v_obj.via.str.size);description = (char * )calloc(strlen(value), sizeof(char));strcpy((char*)description, value);free((void*)value);}}
free((void*)key);
}
*object = transport_type_error_create(level, description);
if(level!=NULL){
free((void*)level);
}
if(description!=NULL){
free((void*)description);
}
}
}
}
void transport_message_pack_session(msgpack_packer * msgpack_pk,transport_type_session_t object){
msgpack_pack_map(msgpack_pk,UNIVERSAL_TRANSPORT_TYPE_SESSION_SIZE);
msgpack_pack_str(msgpack_pk,strlen("user_id"));msgpack_pack_str_body(msgpack_pk,"user_id",strlen("user_id"));if(object->user_id != NULL){msgpack_pack_str(msgpack_pk,strlen(object->user_id));msgpack_pack_str_body(msgpack_pk,object->user_id,strlen(object->user_id));} else {msgpack_pack_nil(msgpack_pk);}
msgpack_pack_str(msgpack_pk,strlen("device_token"));msgpack_pack_str_body(msgpack_pk,"device_token",strlen("device_token"));if(object->device_token != NULL){msgpack_pack_str(msgpack_pk,strlen(object->device_token));msgpack_pack_str_body(msgpack_pk,object->device_token,strlen(object->device_token));} else {msgpack_pack_nil(msgpack_pk);}
msgpack_pack_str(msgpack_pk,strlen("shared_secret"));msgpack_pack_str_body(msgpack_pk,"shared_secret",strlen("shared_secret"));if(object->shared_secret != NULL){msgpack_pack_str(msgpack_pk,strlen(object->shared_secret));msgpack_pack_str_body(msgpack_pk,object->shared_secret,strlen(object->shared_secret));} else {msgpack_pack_nil(msgpack_pk);}
}
void transport_message_unpack_session(msgpack_object * msgpack_obj,transport_type_session_t* object){
*object = NULL;
if (msgpack_obj->type == MSGPACK_OBJECT_MAP) {
if (msgpack_obj->via.map.size == UNIVERSAL_TRANSPORT_TYPE_SESSION_SIZE) {
const char *user_id=NULL;
const char *device_token=NULL;
const char *shared_secret=NULL;
msgpack_object_kv* const kv_end = msgpack_obj->via.map.ptr + msgpack_obj->via.map.size;
for(msgpack_object_kv* kv = msgpack_obj->via.map.ptr; kv < kv_end; kv++){
msgpack_object k_obj = kv->key;
char * key = (char * )calloc(k_obj.via.str.size+1, sizeof(char));
strncpy(key, k_obj.via.str.ptr, k_obj.via.str.size);
msgpack_object v_obj = kv->val;
if (v_obj.type == MSGPACK_OBJECT_STR){if (strcmp(key,"user_id") == 0){char * value = (char * )calloc(v_obj.via.str.size+1, sizeof(char));strncpy(value, v_obj.via.str.ptr, v_obj.via.str.size);user_id = (char * )calloc(strlen(value), sizeof(char));strcpy((char*)user_id, value);free((void*)value);}}
if (v_obj.type == MSGPACK_OBJECT_STR){if (strcmp(key,"device_token") == 0){char * value = (char * )calloc(v_obj.via.str.size+1, sizeof(char));strncpy(value, v_obj.via.str.ptr, v_obj.via.str.size);device_token = (char * )calloc(strlen(value), sizeof(char));strcpy((char*)device_token, value);free((void*)value);}}
if (v_obj.type == MSGPACK_OBJECT_STR){if (strcmp(key,"shared_secret") == 0){char * value = (char * )calloc(v_obj.via.str.size+1, sizeof(char));strncpy(value, v_obj.via.str.ptr, v_obj.via.str.size);shared_secret = (char * )calloc(strlen(value), sizeof(char));strcpy((char*)shared_secret, value);free((void*)value);}}
free((void*)key);
}
*object = transport_type_session_create(user_id, device_token, shared_secret);
if(user_id!=NULL){
free((void*)user_id);
}
if(device_token!=NULL){
free((void*)device_token);
}
if(shared_secret!=NULL){
free((void*)shared_secret);
}
}
}
}
void transport_message_pack_example(msgpack_packer * msgpack_pk,transport_type_example_t object){
msgpack_pack_map(msgpack_pk,UNIVERSAL_TRANSPORT_TYPE_EXAMPLE_SIZE);
msgpack_pack_str(msgpack_pk,strlen("text"));msgpack_pack_str_body(msgpack_pk,"text",strlen("text"));if(object->text != NULL){msgpack_pack_str(msgpack_pk,strlen(object->text));msgpack_pack_str_body(msgpack_pk,object->text,strlen(object->text));} else {msgpack_pack_nil(msgpack_pk);}
}
void transport_message_unpack_example(msgpack_object * msgpack_obj,transport_type_example_t* object){
*object = NULL;
if (msgpack_obj->type == MSGPACK_OBJECT_MAP) {
if (msgpack_obj->via.map.size == UNIVERSAL_TRANSPORT_TYPE_EXAMPLE_SIZE) {
const char *text=NULL;
msgpack_object_kv* const kv_end = msgpack_obj->via.map.ptr + msgpack_obj->via.map.size;
for(msgpack_object_kv* kv = msgpack_obj->via.map.ptr; kv < kv_end; kv++){
msgpack_object k_obj = kv->key;
char * key = (char * )calloc(k_obj.via.str.size+1, sizeof(char));
strncpy(key, k_obj.via.str.ptr, k_obj.via.str.size);
msgpack_object v_obj = kv->val;
if (v_obj.type == MSGPACK_OBJECT_STR){if (strcmp(key,"text") == 0){char * value = (char * )calloc(v_obj.via.str.size+1, sizeof(char));strncpy(value, v_obj.via.str.ptr, v_obj.via.str.size);text = (char * )calloc(strlen(value), sizeof(char));strcpy((char*)text, value);free((void*)value);}}
free((void*)key);
}
*object = transport_type_example_create(text);
if(text!=NULL){
free((void*)text);
}
}
}
}

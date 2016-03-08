
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

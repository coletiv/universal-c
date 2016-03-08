#include "gtest/gtest.h"
#include "universal_transport_messages.h"

TEST(universal_transport_messages_test, transport_messages_session_authenticate) 
{ 
    const char * _session_user_id = "27aeae53-f5d3-429d-82a9-35d0355b875c";
    const char * _session_device_token = "27aeae53-f5d3-429d-82a9-35d0355b875c";
    const char * _session_shared_secret = "367b254f4f5c453b662b614038316f7123332a793366436b41383e433d";
    
    transport_type_session_t _session = transport_type_session_create(_session_user_id, _session_device_token, _session_shared_secret);
    
    transport_message_t packed_message = transport_message_create(UNIVERSAL_TRANSPORT_MESSAGE_SESSION_AUTHENTICATE, (transport_type_t)_session);
    transport_message_t unpacked_message = transport_message_copy(packed_message->data, packed_message->size);
    
    ASSERT_TRUE(packed_message->size > 0);
    ASSERT_TRUE(memcmp(packed_message->data, unpacked_message->data, packed_message->size) == 0);

    uint8_t name;
    transport_type_t arg;
    transport_message_unpack(packed_message, &name, &arg);

    ASSERT_EQ(name, UNIVERSAL_TRANSPORT_MESSAGE_SESSION_AUTHENTICATE);
    ASSERT_EQ(arg->isa, UNIVERSAL_TRANSPORT_TYPE_SESSION);

    transport_type_session_t session = (transport_type_session_t)arg;

    ASSERT_STREQ(session->user_id, _session->user_id);
    ASSERT_STREQ(session->shared_secret, _session->shared_secret);

    transport_message_release(packed_message);
    transport_message_release(unpacked_message);
    transport_type_session_release(session);
    transport_type_session_release(_session);
}

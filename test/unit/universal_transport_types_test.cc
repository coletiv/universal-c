#include "gtest/gtest.h"
#include "universal_transport_types.h"

TEST(universal_transport_types_test, transport_type_session) 
{ 
    const char * _session_user_id = "27aeae53-f5d3-429d-82a9-35d0355b875c";
    const char * _session_device_token = "27aeae53f5d3429d-82a935d0355b875";
    const char * _session_shared_secret = "367b254f4f5c453b662b614038316f7123332a793366436b41383e433d";
	
    transport_type_session_t transport_session = NULL;
      
    transport_session = transport_type_session_create((char*)_session_user_id, (char*)_session_device_token, (char*)_session_shared_secret);
    
    ASSERT_TRUE(NULL != transport_session);
    
    ASSERT_STREQ(_session_user_id, transport_session->user_id);
    ASSERT_STREQ(_session_shared_secret, transport_session->shared_secret);
    
    transport_type_session_t transport_session_copy = transport_type_session_copy(transport_session);
    
    ASSERT_TRUE(NULL != transport_session_copy);
    ASSERT_TRUE(transport_session != transport_session_copy);
    
    ASSERT_STREQ(transport_session->user_id, transport_session_copy->user_id);
    ASSERT_STREQ(transport_session->shared_secret, transport_session_copy->shared_secret);
    
    transport_type_session_release(transport_session);
    transport_type_session_release(transport_session_copy);
}
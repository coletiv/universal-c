#include "gtest/gtest.h"
//#include "mock/mock_service.h"
#include "universal_transport_websocket.h"
#include "universal_transport_session.h"

static void transport_session_test_runloop(transport_session_t agent)
{
    int n = 0;
    unsigned int oldus = 0;
    while (n < 50) 
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);

        if (((unsigned int)tv.tv_usec - oldus) > 50000) 
        {
            oldus = tv.tv_usec; 
            ++n;
            
            if(agent)
                transport_session_run(agent);
        }
    }
}

static const int _test_context = 1;
static const char * _test_user_id = "27aeae53-f5d3-429d-82a9-35d0355b875c";
static const char * _test_device_token = NULL;

static const char * _test_api_shared_secret = "367b254f4f5c453b662b614038316f7123332a793366436b41383e433d";
static const char * _test_api_endpoint_url = "wss://localhost";

enum _test_expected_callbacks {
    _TEST_EXPECT_OPEN,
    _TEST_EXPECT_CLOSE,
	_TEST_EXPECT_PUBLISH_WARNING,
	_TEST_EXPECT_FOUND_HOST
};

static int _test_expected_callback;
static transport_type_t _test_expected_type = NULL;

void transport_session_open_callback(const void * context)
{
    ASSERT_TRUE(NULL != context);
    ASSERT_EQ(_TEST_EXPECT_OPEN, _test_expected_callback);
}

void transport_session_close_callback(const void * context)
{
    ASSERT_TRUE(NULL != context);
    ASSERT_EQ(_TEST_EXPECT_CLOSE, _test_expected_callback);
}

// void transport_session_publish_warning_callback(const void * context, transport_type_warning_t transport_warning)
// {
// 	Logc("transport_session_publish_warning_callback %s\n", transport_warning->message);
//
//     ASSERT_TRUE(NULL != context);
//     //ASSERT_EQ(_TEST_EXPECT_PUBLISH_WARNING, _test_expected_callback);
//
// 	transport_type_warning_log(transport_warning);
//     _test_expected_type = (transport_type_t)transport_warning;
// }
//
// void transport_session_found_host_callback(const void * context, transport_type_host_t transport_host)
// {
//     ASSERT_TRUE(NULL != context);
//     //ASSERT_EQ(_TEST_EXPECT_FOUND_HOST, _test_expected_callback);
//
//     _test_expected_type = (transport_type_t)transport_host;
// }

struct transport_session_handler _test_handler = {
    &_test_context,
    &transport_session_open_callback,
    &transport_session_close_callback
};

TEST(universal_transport_session_test, transport_session_open) 
{
    transport_session_t transport_session = NULL;
    transport_session = transport_session_create(_test_user_id, _test_device_token, _test_api_endpoint_url, _test_api_shared_secret, _test_handler);
    
    ASSERT_TRUE(NULL != transport_session);
    
    // open
    _test_expected_callback = _TEST_EXPECT_OPEN;
    transport_session_open(transport_session);
    
    // run loop
    transport_session_test_runloop(transport_session);
    
    // run loop
    transport_session_test_runloop(transport_session);

    // run loop
    transport_session_test_runloop(transport_session);
    
    ASSERT_TRUE(transport_session_is_open(transport_session));
    
    // close
    _test_expected_callback = _TEST_EXPECT_CLOSE;
    transport_session_close(transport_session);
    
    // run loop
    transport_session_test_runloop(transport_session);
    
    // run loop
    transport_session_test_runloop(transport_session);
    
    ASSERT_FALSE(transport_session_is_open(transport_session));

    transport_session_release(transport_session);
}

TEST(universal_transport_session_test, transport_session_service_open_retry)
{
	// TODO
}

TEST(universal_transport_session_test, transport_session_example_ping)
{
    struct transport_session_handler _test_empty_handler = {
    NULL,
    NULL,
    NULL
};

    transport_session_t transport_session = NULL;
    transport_session = transport_session_create(_test_user_id, _test_device_token, _test_api_endpoint_url, _test_api_shared_secret, _test_empty_handler);
   
    ASSERT_TRUE(NULL != transport_session);

    // open
    transport_session_open(transport_session);

    // run loop
    transport_session_test_runloop(transport_session);
	
	  struct timeval tv;
	  gettimeofday(&tv, DST_NONE);
	  uint64_t timestamp_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	  char text[256];
    snprintf(text, 256, "@coletivstudio hello world! %lu", timestamp_ms);
    
    transport_type_example_t _example = transport_type_example_create(text);

    transport_session_example_ping(transport_session, _example);
    transport_session_test_runloop(transport_session);

    transport_type_example_release(_example);

    // close
    transport_session_close(transport_session);
    transport_session_release(transport_session);
}

// TEST(universal_transport_session_test, transport_session_publish_warning)
// {
//     transport_session_t transport_session = NULL;
//     transport_session = transport_session_create(_test_device_identifier, _test_shared_secret, _test_token, _test_handler);
//
//     ASSERT_TRUE(NULL != transport_session);
//
//     // open
//     transport_session_open(transport_session);
//
//     // run loop
//     transport_session_test_runloop(transport_session);
//
//     // publish warning
//     transport_type_warning_t _warning = transport_type_warning_create("critical", "Port 443 is open");
// 	transport_session_test_runloop(transport_session);
//
//     _test_expected_callback = _TEST_EXPECT_PUBLISH_WARNING;
//     _test_expected_type = NULL;
//
//     transport_session_publish_warning(transport_session, _warning);
//     transport_session_test_runloop(transport_session);
//
//     //ASSERT_TRUE(NULL != _test_expected_type);
//     ASSERT_TRUE(UNIVERSAL_TRANSPORT_TYPE_WARNING == _test_expected_type->isa);
//     transport_type_warning_t warning = (transport_type_warning_t)_test_expected_type;
//     ASSERT_STREQ(_warning->level, warning->level);
// 	ASSERT_STREQ(_warning->message, warning->message);
//
//     transport_type_warning_release(warning);
//     transport_type_warning_release(_warning);
//
//     // close
//     transport_session_close(transport_session);
//
//     transport_session_release(transport_session);
// }

/*
TEST(transport_session_test, transport_session_service_unavailable)
{
    transport_session_t transport_session = NULL;
    transport_session = transport_session_create(_test_agent_id, _test_handler);
    
    ASSERT_TRUE(NULL != transport_session);
    
    // open
    transport_session_open(transport_session);
    
    // run loop
    transport_session_test_runloop(transport_session, NULL);
    
    // run loop
    transport_session_test_runloop(transport_session, NULL);
    
    ASSERT_FALSE(transport_session_is_open(transport_session));
    
    // close
    transport_session_close(transport_session);
    
    ASSERT_FALSE(transport_session_is_open(transport_session));

    transport_session_release(transport_session);
}

TEST(transport_session_test, transport_session_throw_error)
{

}

TEST(transport_session_test, transport_session_create_document)
{
    transport_mock_service_t transport_mock_service = transport_mock_service_create();
    
    transport_session_t transport_session = NULL;
    transport_session = transport_session_create(_test_agent_id, _test_handler);
    
    ASSERT_TRUE(NULL != transport_session);
    
    // open
    transport_session_open(transport_session);
    
    // run loop
    transport_session_test_runloop(transport_session, transport_mock_service);
    
    // create document
    _test_expected_callback = _TEST_EXPECT_READ_DOCUMENT;
    _test_expected_type = NULL;
    const uint8_t _document_oid[16] = {0xda,0x89,0x2e,0xec,0xf3,0x65,0xe3,0x6a,0x05,0x08,0x06,0xac,0x81,0xd9,0x92,0xb5};
    transport_type_document_t _document = transport_type_document_create(_document_oid, NULL);
    transport_session_create_document(transport_session, _document);
    transport_session_test_runloop(transport_session, transport_mock_service);
    ASSERT_TRUE(NULL != _test_expected_type);
    ASSERT_TRUE(UNIVERSAL_TRANSPORT_TYPE_DOCUMENT == _test_expected_type->isa);
    transport_type_document_t document = (transport_type_document_t)_test_expected_type;
    ASSERT_TRUE(oid_is_equal(_document->oid, document->oid));
    transport_type_document_release(_document);
    transport_type_document_release(document);
    
    // close
    transport_session_close(transport_session);
    
    transport_session_release(transport_session);
    transport_mock_service_release(transport_mock_service);
}

TEST(transport_session_test, transport_session_read_document)
{
    transport_mock_service_t transport_mock_service = transport_mock_service_create();
    
    transport_session_t transport_session = NULL;
    transport_session = transport_session_create(_test_agent_id, _test_handler);
    
    ASSERT_TRUE(NULL != transport_session);
    
    // open
    transport_session_open(transport_session);
    
    // run loop
    transport_session_test_runloop(transport_session, transport_mock_service);
    
    // create document
    _test_expected_callback = _TEST_EXPECT_READ_DOCUMENT;
    _test_expected_type = NULL;
    const uint8_t _document_oid[16] = {0xda,0x89,0x2e,0xec,0xf3,0x65,0xe3,0x6a,0x05,0x08,0x06,0xac,0x81,0xd9,0x92,0xb5};
    transport_type_document_t _document = transport_type_document_create(_document_oid, NULL);
    transport_session_read_document(transport_session, _document);
    transport_session_test_runloop(transport_session, transport_mock_service);
    ASSERT_TRUE(NULL != _test_expected_type);
    ASSERT_TRUE(UNIVERSAL_TRANSPORT_TYPE_DOCUMENT == _test_expected_type->isa);
    transport_type_document_t document = (transport_type_document_t)_test_expected_type;
    ASSERT_TRUE(oid_is_equal(_document->oid, document->oid));
    transport_type_document_release(_document);
    transport_type_document_release(document);
    
    // close
    transport_session_close(transport_session);
    
    transport_session_release(transport_session);
    transport_mock_service_release(transport_mock_service);
}

TEST(transport_session_test, transport_session_update_document)
{
    transport_mock_service_t transport_mock_service = transport_mock_service_create();
    
    transport_session_t transport_session = NULL;
    transport_session = transport_session_create(_test_agent_id, _test_handler);
    
    ASSERT_TRUE(NULL != transport_session);
    
    // open
    transport_session_open(transport_session);
    
    // run loop
    transport_session_test_runloop(transport_session, transport_mock_service);
    
    // create document
    _test_expected_callback = _TEST_EXPECT_UPDATE_DOCUMENT;
    _test_expected_type = NULL;
    const uint8_t _document_oid[16] = {0xda,0x89,0x2e,0xec,0xf3,0x65,0xe3,0x6a,0x05,0x08,0x06,0xac,0x81,0xd9,0x92,0xb5};
    transport_type_document_t _document = transport_type_document_create(_document_oid, NULL);
    transport_session_update_document(transport_session, _document);
    transport_session_test_runloop(transport_session, transport_mock_service);
    ASSERT_TRUE(NULL != _test_expected_type);
    ASSERT_TRUE(UNIVERSAL_TRANSPORT_TYPE_DOCUMENT == _test_expected_type->isa);
    transport_type_document_t document = (transport_type_document_t)_test_expected_type;
    ASSERT_TRUE(oid_is_equal(_document->oid, document->oid));
    transport_type_document_release(_document);
    transport_type_document_release(document);
    
    // close
    transport_session_close(transport_session);
    
    transport_session_release(transport_session);
    transport_mock_service_release(transport_mock_service);
}

TEST(transport_session_test, transport_session_publish_event)
{
    transport_mock_service_t transport_mock_service = transport_mock_service_create();
    
    transport_session_t transport_session = NULL;
    transport_session = transport_session_create(_test_agent_id, _test_handler);
    
    ASSERT_TRUE(NULL != transport_session);
    
    // open
    transport_session_open(transport_session);
    
    // run loop
    transport_session_test_runloop(transport_session, transport_mock_service);
    
    // subscribe event
    _test_expected_type = NULL;
    const uint8_t _event_oid[16] = {0xda,0x89,0x2e,0xec,0xf3,0x65,0xe3,0x6a,0x05,0x08,0x06,0xac,0x81,0xd9,0x92,0xb5};
    transport_type_event_t _event = transport_type_event_create(_event_oid, "keypath", "name", value_null());
    transport_session_subscriuniversal_event(transport_session, _event);
    transport_session_test_runloop(transport_session, transport_mock_service);
    ASSERT_TRUE(NULL == _test_expected_type);
    
    // publish event
    _test_expected_callback = _TEST_EXPECT_PUBLISH_EVENT;
    _test_expected_type = NULL;
    transport_session_publish_event(transport_session, _event);
    transport_session_test_runloop(transport_session, transport_mock_service);
    ASSERT_TRUE(NULL != _test_expected_type);
    ASSERT_TRUE(UNIVERSAL_TRANSPORT_TYPE_EVENT == _test_expected_type->isa);
    transport_type_event_t event = (transport_type_event_t)_test_expected_type;
    ASSERT_TRUE(oid_is_equal(_event->oid, event->oid));
    ASSERT_TRUE(strcmp(_event->keypath, event->keypath) == 0);
    ASSERT_TRUE(strcmp(_event->name, event->name) == 0);
    transport_type_event_release(event);
    
    // unsubscribe event
    _test_expected_type = NULL;
    transport_session_unsubscriuniversal_event(transport_session, _event);
    transport_session_test_runloop(transport_session, transport_mock_service);
    ASSERT_TRUE(NULL == _test_expected_type);
    
    // publish event
    _test_expected_type = NULL;
    transport_session_publish_event(transport_session, _event);
    transport_session_test_runloop(transport_session, transport_mock_service);
    ASSERT_TRUE(NULL == _test_expected_type);
    
    transport_type_event_release(_event);
    
    // close
    transport_session_close(transport_session);
    
    transport_session_release(transport_session);
    transport_mock_service_release(transport_mock_service);
}*/

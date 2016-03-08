#include "gtest/gtest.h"
#include "mock/mock_websocket_server.h"
#include "universal_transport_websocket.h"
#include "universal_transport_constants.h"

static const char * _test_url = "ws://localhost:13647";
static const unsigned int _test_port = 13647;

enum _test_expected_callbacks {
    _TEST_EXPECT_OPEN,
    _TEST_EXPECT_CLOSE,
    _TEST_EXPECT_FAIL,
    _TEST_EXPECT_READ
};

static int _test_expected_callback;
static int _test_expected_read_size;

static void transport_websocket_test_runloop(transport_websocket_t websocket, transport_mock_websocket_server_t mock_websocket_server)
{
    int n = 0;
    unsigned int oldus = 0;
    while (n < 10) 
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);

        if (((unsigned int)tv.tv_usec - oldus) > 50000) 
        {
            oldus = tv.tv_usec; 
            ++n;
            
            if(websocket)
                transport_websocket_run(websocket);
            
            if(mock_websocket_server)
                transport_mock_websocket_server_run(mock_websocket_server);
        }
    }
}

static void transport_websocket_open_callback(const void *context)
{
    ASSERT_TRUE(NULL != context);
    ASSERT_EQ(_TEST_EXPECT_OPEN, _test_expected_callback);
}

static void transport_websocket_close_callback(const void *context)
{
    ASSERT_TRUE(NULL != context);
    ASSERT_EQ(_TEST_EXPECT_CLOSE, _test_expected_callback);
}

static void transport_websocket_fail_callback(const void *context)
{
    ASSERT_TRUE(NULL != context);
    ASSERT_EQ(_TEST_EXPECT_FAIL, _test_expected_callback);
}

static void transport_websocket_read_callback(const void *context, transport_message_t transport_message)
{
    ASSERT_TRUE(NULL != context);
    ASSERT_TRUE(NULL != transport_message);
    ASSERT_EQ(_TEST_EXPECT_READ, _test_expected_callback);
    ASSERT_EQ(transport_message->size, _test_expected_read_size);
    
    transport_message_release(transport_message);
}

TEST(universal_transport_websocket_test, transport_websocket_create) 
{ 
    struct transport_websocket_handler handler = {
        NULL,
        NULL,
        NULL, 
        NULL, 
        NULL
    };
    
    transport_websocket_t transport_websocket = transport_websocket_create(handler, _test_url);
  
    ASSERT_TRUE(NULL != transport_websocket);
    
    transport_websocket_release(transport_websocket);
}

TEST(universal_transport_websocket_test, transport_websocket_open) 
{
    transport_mock_websocket_server_t transport_mock_websocket_server = transport_mock_websocket_server_create(_test_port);
    
    struct transport_websocket_handler handler = {
        &_test_port,
        &transport_websocket_open_callback,
        &transport_websocket_close_callback, 
        &transport_websocket_fail_callback, 
        &transport_websocket_read_callback
    };
    
    transport_websocket_t transport_websocket = transport_websocket_create(handler, _test_url);
  
    // open
    _test_expected_callback = _TEST_EXPECT_OPEN;
    transport_websocket_open(transport_websocket);
    
    // run loop
    transport_websocket_test_runloop(transport_websocket, transport_mock_websocket_server);
  
    // close client
    _test_expected_callback = _TEST_EXPECT_CLOSE;
    transport_websocket_close(transport_websocket);
    
    // run loop
    transport_websocket_test_runloop(transport_websocket, transport_mock_websocket_server);
  
    transport_websocket_release(transport_websocket);
    transport_mock_websocket_server_release(transport_mock_websocket_server);
}

TEST(universal_transport_websocket_test, transport_websocket_fail_server_down) 
{
    transport_mock_websocket_server_t transport_mock_websocket_server = transport_mock_websocket_server_create(_test_port+1); // different port to fail
    
    struct transport_websocket_handler handler = {
        &_test_port,
        &transport_websocket_open_callback,
        &transport_websocket_close_callback, 
        &transport_websocket_fail_callback, 
        &transport_websocket_read_callback
    };
    
    transport_websocket_t transport_websocket = transport_websocket_create(handler, _test_url);
  
    // open
    _test_expected_callback = _TEST_EXPECT_FAIL;
    transport_websocket_open(transport_websocket);
    
    // run loop
    transport_websocket_test_runloop(transport_websocket, transport_mock_websocket_server);
    
    transport_websocket_release(transport_websocket);
    transport_mock_websocket_server_release(transport_mock_websocket_server);
}

TEST(universal_transport_websocket_test, transport_websocket_remote_close_connection) 
{
    transport_mock_websocket_server_t transport_mock_websocket_server = transport_mock_websocket_server_create(_test_port);
    
    struct transport_websocket_handler handler = {
        &_test_port,
        &transport_websocket_open_callback,
        &transport_websocket_close_callback, 
        &transport_websocket_fail_callback, 
        &transport_websocket_read_callback
    };
    
    transport_websocket_t transport_websocket = transport_websocket_create(handler, _test_url);
  
    // open
    _test_expected_callback = _TEST_EXPECT_OPEN;
    transport_websocket_open(transport_websocket);
    
    // run loop
    transport_websocket_test_runloop(transport_websocket, transport_mock_websocket_server);
  
    // close server
    _test_expected_callback = _TEST_EXPECT_CLOSE;
    transport_mock_websocket_server_release(transport_mock_websocket_server); // stop server to simulate drop connection
    
    // run loop
    transport_websocket_test_runloop(transport_websocket, NULL);
  
    transport_websocket_release(transport_websocket);
}

TEST(universal_transport_websocket_test, transport_websocket_write) 
{
    transport_mock_websocket_server_t transport_mock_websocket_server = transport_mock_websocket_server_create(_test_port);
    
    struct transport_websocket_handler handler = {
        &_test_port,
        &transport_websocket_open_callback,
        &transport_websocket_close_callback, 
        &transport_websocket_fail_callback, 
        &transport_websocket_read_callback
    };
    
    transport_websocket_t transport_websocket = transport_websocket_create(handler, _test_url);
  
    // open
    _test_expected_callback = _TEST_EXPECT_OPEN;
    transport_websocket_open(transport_websocket);
    
    // run loop
    transport_websocket_test_runloop(transport_websocket, transport_mock_websocket_server);
    transport_websocket_test_runloop(transport_websocket, transport_mock_websocket_server);
    
    // write
    const size_t _data_size = 34;
    const uint8_t _data_bytes[34] = {0x83, 0xa6, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0xa7, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0xa6, 0x6e, 0x75, 0x6d, 0x62, 0x65, 0x72, 0x02, 0xa5, 0x61, 0x72, 0x72, 0x61, 0x79, 0x93, 0x01, 0x02, 0x03};
    transport_message_t _transport_message = transport_message_copy(_data_bytes, _data_size);
    _test_expected_callback = _TEST_EXPECT_READ;
    _test_expected_read_size = 34;
    transport_websocket_write(transport_websocket, _transport_message);
    
    // run loop
    transport_websocket_test_runloop(transport_websocket, transport_mock_websocket_server);
    transport_websocket_test_runloop(transport_websocket, transport_mock_websocket_server);
    
    _test_expected_callback = _TEST_EXPECT_CLOSE;
    transport_websocket_release(transport_websocket);
    transport_mock_websocket_server_release(transport_mock_websocket_server);
}
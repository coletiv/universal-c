GTEST_DIR = /usr/local

# Google test
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/

# Directories
LIB_DIR = lib
TEST_DIR = test
BUILD_DIR = build
INSTALL_DIR = install
UNIT_TEST_DIR = test/unit
INTEGRATION_TEST_DIR = test/integration

# Mock
INTEGRATION_TEST_MOCK_DIR = test/integration/mock

# Flags passed to the C++ compiler.
CPPFLAGS += -isystem $(GTEST_DIR)/include
CXXFLAGS += -g -Wall -Wextra -DGTEST_HAS_PTHREAD=0 -DDEBUG=1 -DTEST=1 -L/usr/local/lib
CXX = g++

# All source object files
OBJS = universal_transport_reachability.o universal_transport_types.o universal_transport_messages.o universal_transport_websocket.o universal_transport_session.o

# All tests
UNIT_TESTS = universal_transport_types_test universal_transport_messages_test universal_transport_reachability_test
INTEGRATION_TESTS = universal_transport_websocket_test universal_transport_session_test
TESTS = $(UNIT_TESTS) $(INTEGRATION_TESTS)

# Static library
LIB = libuniversal.a
LIB_NAME = universal

# Dependencies
LIB_DEPS = -Ideps/wolfssl -Ldeps/wolfssl/lib -lwolfssl -Ideps/libwebsockets/include -Ldeps/libwebsockets -lwebsockets -Ideps/msgpack-c/include -Ldeps/msgpack-c -lmsgpack -lz

all: $(LIB)

unit: $(UNIT_TESTS)
integration: $(INTEGRATION_TESTS)

clean:
	rm -rf $(TESTS) gtest.a gtest_main.a *.o *.a build install

check: $(TESTS) 
	./universal_transport_reachability_test
	./universal_transport_types_test
	./universal_transport_messages_test
	./universal_transport_websocket_test
	./universal_transport_session_test

%.o: $(LIB_DIR)/%.c $(LIB_DIR)/%.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIB_DEPS) -I$(LIB_DIR) -c -o $@ $<

mock_%.o : $(INTEGRATION_TEST_MOCK_DIR)/mock_%.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIB_DEPS) -I$(LIB_DIR) -c -o $@ $<

# %_integration_test.o: $(INTEGRATION_TEST_DIR)/%_integration_test.cc
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIB_DEPS) -I$(LIB_DIR) -I$(GTEST_HEADERS) -c -o $@ $<
#
# %_test.o: $(UNIT_TEST_DIR)/%_test.cc
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIB_DEPS) -I$(LIB_DIR) -I$(GTEST_HEADERS) -c -o $@ $<

%_test.o: $(UNIT_TEST_DIR)/%_test.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIB_DEPS) -I$(LIB_DIR) -c -o $@ $<
	
%_test.o: $(INTEGRATION_TEST_DIR)/%_test.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIB_DEPS) -I$(LIB_DIR) -c -o $@ $<
	
universal_transport_websocket_test: $(OBJS) universal_transport_websocket_test.o mock_websocket_server.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIB_DEPS) -I$(LIB_DIR) -lgtest_main -lgtest $(LIB_DEPS) $^ -o $@

# universal_transport_%_test: $(TRANSPORT_OBJS) universal_transport_%_test.o
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIB_DEPS) -I$(LIB_DIR) -I$(TRANSPORT_LIB_DIR) -lgtest_main -lgtest $(LIB_DEPS) $^ -o $@
#
# %_integration_test: %_integration_test.o $(OBJS)
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIB_DEPS) -lgtest_main -lgtest $^ -o $@
	
%_test: %_test.o $(OBJS) 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LIB_DEPS) -lgtest_main -lgtest $^ -o $@

mock: $(MOCK_OBJS)
$(LIB): $(OBJS)
	ar ru $@ $^
	ranlib $@
	ar -x deps/wolfssl/lib/libwolfssl.a
	ar -x deps/libwebsockets/libwebsockets.a
	ar -x deps/msgpack-c/libmsgpack.a
	ar -qc $(LIB) *.o
	mkdir -p $(INSTALL_DIR)
	cp -p $(LIB) $(INSTALL_DIR)
	mkdir -p $(INSTALL_DIR)/include/$(LIB_NAME)
	cp -p `find $(LIB_DIR) -name "*.h" -not -path "*_internal.h"` $(INSTALL_DIR)/include/$(LIB_NAME)/
	rm *.o
	rm *SYMDEF*

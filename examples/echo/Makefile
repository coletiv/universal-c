# Directories
SRC_DIR = src
TEST_DIR = test
BUILD_DIR = build

# Dependencies
LOCAL_LIB_DIR = ../../install
LOCAL_INCLUDE_DIR = ../../install/include

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -DGTEST_HAS_PTHREAD=0 -DDEBUG=1 -DTEST=1 -I$(LOCAL_INCLUDE_DIR) -L$(LOCAL_LIB_DIR) -lz
CXX = g++

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
APP = universal_example_echo

# All source object files
OBJS = universal_example_echo.o

# Dependencies

all : $(APP) 

clean :
	rm -f $(APP) *.o

%.o : $(SRC_DIR)/%.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(SRC_DIR) -c -o $@ $<
			
universal_example_echo: $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(SRC_DIR) -luniversal $^ -o $@

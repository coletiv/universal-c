#include "gtest/gtest.h"
#include "universal_transport_reachability.h"

TEST(universal_transport_reachability_test, transport_reachability_status) 
{ 
    transport_reachability_t transport_reachability = transport_reachability_create(NULL, NULL);
    
    ASSERT_EQ(UNIVERSAL_TRANSPORT_REACHABILITY_REACHABLE, transport_reachability_status(transport_reachability));
    
    transport_reachability_release(transport_reachability);
}
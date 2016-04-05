#include "gtest/gtest.h"
#include "universal_transport_queue.h"

TEST(universal_transport_queue_test, create_queue_unqueue) 
{ 
    transport_queue_t test_queue = transport_queue_create();

    int numberOne = 1;
    int numberTwo = 2;

    transport_queue_object_t test_obj_1 = &numberOne;
    transport_queue_object_t test_obj_2 = &numberTwo;

    assert(transport_queue_is_empty(test_queue) == true);
    assert(debug_transport_queue_enqueued_count(test_queue) == 0);
    assert(debug_transport_queue_reserved_count(test_queue) == 0);

    transport_queue_push(test_queue, test_obj_1);

    assert(transport_queue_is_empty(test_queue) == false);
    assert(debug_transport_queue_enqueued_count(test_queue) == 1);
    assert(debug_transport_queue_reserved_count(test_queue) == 0);

    transport_queue_push(test_queue, test_obj_2);

    assert(transport_queue_is_empty(test_queue) == false);
    assert(debug_transport_queue_enqueued_count(test_queue) == 2);
    assert(debug_transport_queue_reserved_count(test_queue) == 0);

    transport_queue_object_t test_obj;

    test_obj = transport_queue_pop(test_queue);

    assert(test_obj == test_obj_1);
    assert(transport_queue_is_empty(test_queue) == false);
    assert(debug_transport_queue_enqueued_count(test_queue) == 1);
    assert(debug_transport_queue_reserved_count(test_queue) == 1);

    test_obj = transport_queue_pop(test_queue);

    assert(test_obj == test_obj_2);
    assert(transport_queue_is_empty(test_queue) == true);
    assert(debug_transport_queue_enqueued_count(test_queue) == 0);
    assert(debug_transport_queue_reserved_count(test_queue) == 2);

    transport_queue_push(test_queue, test_obj_2);

    assert(transport_queue_is_empty(test_queue) == false);
    assert(debug_transport_queue_enqueued_count(test_queue) == 1);
    assert(debug_transport_queue_reserved_count(test_queue) == 1);

    transport_queue_push(test_queue, test_obj_2);

    assert(transport_queue_is_empty(test_queue) == false);
    assert(debug_transport_queue_enqueued_count(test_queue) == 2);
    assert(debug_transport_queue_reserved_count(test_queue) == 0);

    test_obj = transport_queue_pop(test_queue);

    assert(test_obj == test_obj_2);
    assert(transport_queue_is_empty(test_queue) == false);
    assert(debug_transport_queue_enqueued_count(test_queue) == 1);
    assert(debug_transport_queue_reserved_count(test_queue) == 1);

    test_obj = transport_queue_pop(test_queue);

    assert(test_obj == test_obj_2);
    assert(transport_queue_is_empty(test_queue) == true);
    assert(debug_transport_queue_enqueued_count(test_queue) == 0);
    assert(debug_transport_queue_reserved_count(test_queue) == 2);

    transport_queue_destroy(test_queue);
}

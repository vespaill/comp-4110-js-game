/* Copyright 2016 Zachary Krausman */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>
#include <stdint.h>
#include <iostream>
#include <string>

#include "ring_buffer_class.hpp"

BOOST_AUTO_TEST_CASE(RBcontructor) {
typedef ringbuffer::ringBuffer<int, 4> ringbuff;
BOOST_REQUIRE_NO_THROW(ringbuff x);
  }

BOOST_AUTO_TEST_CASE(RBenque_dequeue) {
  ringbuffer::ringBuffer<int, 3> rb;

  rb.push_back(2);
  rb.push_back(1);
  rb.push_back(0);
  BOOST_REQUIRE(rb.isFull() == 1);

  BOOST_REQUIRE(rb.pop_front() == 2);
  BOOST_REQUIRE(rb.pop_front() == 1);
  BOOST_REQUIRE(rb.pop_front() == 0);
  BOOST_REQUIRE(rb.isEmpty() == 1);

  BOOST_REQUIRE_THROW(rb.pop_front(), std::runtime_error);
}
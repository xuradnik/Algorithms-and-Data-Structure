#ifndef LIBDS_TESTS_ADT_QUEUE_TEST_HPP
#define LIBDS_TESTS_ADT_QUEUE_TEST_HPP

#include <libds/adt/queue.hpp>

#include <libstest/test.hpp>


namespace ds::tests {


template<template<typename> typename QueueType>
class QueueTests : public stest::CompositeTest {
public:
    QueueTests();
};


class ImplicitQueueTests : public stest::CompositeTest {
public:
    ImplicitQueueTests();
};


class ExplicitQueueTests : public stest::CompositeTest {
public:
    ExplicitQueueTests();
};


} // namespace ds::tests

#endif

#ifndef LIBDS_TESTS_ADT_STACK_TEST_HPP
#define LIBDS_TESTS_ADT_STACK_TEST_HPP

#include <libds/adt/stack.hpp>

#include <libstest/test.hpp>


namespace ds::tests {


template<template<typename> typename StackType>
class StackTests : public stest::CompositeTest {
public:
    StackTests();
};


class ImplicitStackTests : public stest::CompositeTest {
public:
    ImplicitStackTests();
};


class ExplicitStackTests : public stest::CompositeTest {
public:
    ExplicitStackTests();
};


} // namespace ds::tests

#endif

#ifndef LIBDS_TESTS_ADT_LIST_TEST_HPP
#define LIBDS_TESTS_ADT_LIST_TEST_HPP

#include <libds/adt/list.hpp>

#include <libstest/test.hpp>


namespace ds::tests {


template<template<typename> typename ListType>
class ListTests : public stest::CompositeTest {
public:
    ListTests();
};


class ImplicitListTests : public stest::CompositeTest {
public:
    ImplicitListTests();
};


class SinglyLinkedListTests : public stest::CompositeTest {
public:
    SinglyLinkedListTests();
};


class DoublyLinkedListTests : public stest::CompositeTest {
public:
    DoublyLinkedListTests();
};


} // namespace ds::tests

#endif

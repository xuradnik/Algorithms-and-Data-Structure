#ifndef LIBDS_TESTS_ADT_TABLE_TEST_HPP
#define LIBDS_TESTS_ADT_TABLE_TEST_HPP

#include <libds/adt/table.hpp>

#include <libstest/test.hpp>


namespace ds::tests {


template<template<typename, typename> typename TableType>
class TableTests : public stest::CompositeTest {
public:
    TableTests();
};


class UnsortedImplicitSequenceTableTests : public stest::CompositeTest {
public:
    UnsortedImplicitSequenceTableTests();
};


class UnsortedExplicitSequenceTableTests : public stest::CompositeTest {
public:
    UnsortedExplicitSequenceTableTests();
};


class SortedSequenceTableTests : public stest::CompositeTest {
public:
    SortedSequenceTableTests();
};


class BinarySearchTreeTests : public stest::CompositeTest {
public:
    BinarySearchTreeTests();
};


class TreapTests : public stest::CompositeTest {
public:
    TreapTests();
};


class HashTableTests : public stest::CompositeTest {
public:
    HashTableTests();
};


} // namespace ds::tests

#endif

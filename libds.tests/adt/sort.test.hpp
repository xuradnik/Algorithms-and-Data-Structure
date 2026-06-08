#ifndef LIBDS_TESTS_ADT_SORTS_TEST_HPP
#define LIBDS_TESTS_ADT_SORTS_TEST_HPP

#include <libds/adt/sorts.hpp>
#include <libds/amt/implicit_sequence.hpp>

#include <libstest/test.hpp>


namespace ds::tests {


template<template<typename> typename Sorter>
class SortTests : public stest::CompositeTest {
public:
    SortTests();
};


class SelectSortTests : public stest::CompositeTest {
public:
    SelectSortTests();
};


class InsertSortTests : public stest::CompositeTest {
public:
    InsertSortTests();
};


class BubbleSortTests : public stest::CompositeTest {
public:
    BubbleSortTests();
};


class QuickSortTests : public stest::CompositeTest {
public:
    QuickSortTests();
};


class HeapSortTests : public stest::CompositeTest {
public:
    HeapSortTests();
};


class ShellSortTests : public stest::CompositeTest {
public:
    ShellSortTests();
};


class RadixSortTests : public stest::CompositeTest {
public:
    RadixSortTests();
};


class MergeSortTests : public stest::CompositeTest {
public:
    MergeSortTests();
};


} // namespace ds::tests

#endif

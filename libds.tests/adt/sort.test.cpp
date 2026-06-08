#include "sort.test.hpp"

#include <libds/adt/sorts.hpp>
#include <libds/amt/implicit_sequence.hpp>

#include <algorithm>
#include <random>


namespace ds::tests {


template<template<typename> typename Sorter>
SortTests<Sorter>::SortTests() : stest::CompositeTest("Sort") {

this->add_test("empty", [](stest::LeafTest &s){
    Sorter<int> t;
    amt::ImplicitSequence<int> xs;
    t.sort(xs);
    s.assert_true(xs.isEmpty(), "Sorted sequence is empty.");
    s.assert_equals(0UZ, xs.size(), "Sorted sequence has size 0.");
});

this->add_test("one", [](stest::LeafTest &s){
    Sorter<int> t;
    amt::ImplicitSequence<int> xs;
    xs.emplaceLast().data = 1;
    t.sort(xs);
    s.pass("Single element is allways sorted.");
});

this->add_test("two_asc", [](stest::LeafTest &s){
    Sorter<int> t;
    amt::ImplicitSequence<int> xs;
    xs.emplaceLast().data = 1;
    xs.emplaceLast().data = 2;
    t.sort(xs);
    s.assert_true(std::is_sorted(xs.begin(), xs.end()), "Sequence is sorted.");
});

this->add_test("two_desc", [](stest::LeafTest &s){
    Sorter<int> t;
    amt::ImplicitSequence<int> xs;
    xs.emplaceLast().data = 2;
    xs.emplaceLast().data = 1;
    t.sort(xs);
    s.assert_true(std::is_sorted(xs.begin(), xs.end()), "Sequence is sorted.");
});

this->add_test("three_sortedAsc", [](stest::LeafTest &s){
    Sorter<int> t;
    amt::ImplicitSequence<int> xs;
    xs.emplaceLast().data = 1;
    xs.emplaceLast().data = 2;
    xs.emplaceLast().data = 3;
    t.sort(xs);
    s.assert_true(std::is_sorted(xs.begin(), xs.end()), "Sequence is sorted.");
});

this->add_test("three_sortedDesc", [](stest::LeafTest &s){
    Sorter<int> t;
    amt::ImplicitSequence<int> xs;
    xs.emplaceLast().data = 3;
    xs.emplaceLast().data = 2;
    xs.emplaceLast().data = 1;
    t.sort(xs);
    s.assert_true(std::is_sorted(xs.begin(), xs.end()), "Sequence is sorted.");
});

this->add_test("three_unsorted", [](stest::LeafTest &s){
    Sorter<int> t;
    amt::ImplicitSequence<int> xs;
    xs.emplaceLast().data = 2;
    xs.emplaceLast().data = 3;
    xs.emplaceLast().data = 1;
    t.sort(xs);
    s.assert_true(std::is_sorted(xs.begin(), xs.end()), "Sequence is sorted.");
});

this->add_test("many_sortedAsc", [](stest::LeafTest &s){
    Sorter<int> t;
    amt::ImplicitSequence<int> xs;
    for (int i = 0; i < 100; ++i) {
        xs.emplaceLast().data = i;
    }
    t.sort(xs);
    s.assert_true(std::is_sorted(xs.begin(), xs.end()), "Sequence is sorted.");
});

this->add_test("many_sortedDesc", [](stest::LeafTest &s){
    Sorter<int> t;
    amt::ImplicitSequence<int> xs;
    for (int i = 0; i < 100; ++i) {
        xs.emplaceLast().data = 100 - i;
    }
    t.sort(xs);
    s.assert_true(std::is_sorted(xs.begin(), xs.end()), "Sequence is sorted.");
});

this->add_test("many_unsorted", [](stest::LeafTest &s){
    Sorter<int> t;
    amt::ImplicitSequence<int> xs;
    xs.reserveCapacity(100);
    std::ranlux48 rng(144);
    std::uniform_int_distribution<int> dist(1,100);
    for (int i = 0; i < 100; ++i) {
        xs.emplaceLast().data = dist(rng);
    }
    t.sort(xs);
    s.assert_true(std::is_sorted(xs.begin(), xs.end()), "Sequence is sorted.");
});

} // SortTests::SortTests


SelectSortTests::SelectSortTests() : stest::CompositeTest("SelectSort") {

this->add_test<SortTests<adt::SelectSort>>();

} // SelectSortTests::SelectSortTests


InsertSortTests::InsertSortTests() : stest::CompositeTest("InsertSort") {

this->add_test<SortTests<adt::InsertSort>>();

} // InsertSortTests::InsertSortTests


BubbleSortTests::BubbleSortTests() : stest::CompositeTest("BubbleSort") {

this->add_test<SortTests<adt::BubbleSort>>();

} // BubbleSortTests::BubbleSortTests


QuickSortTests::QuickSortTests() : stest::CompositeTest("QuickSort") {

this->add_test<SortTests<adt::QuickSort>>();

} // QuickSortTests::QuickSortTests


HeapSortTests::HeapSortTests() : stest::CompositeTest("HeapSort") {

this->add_test<SortTests<adt::HeapSort>>();

} // HeapSortTests::HeapSortTests


ShellSortTests::ShellSortTests() : stest::CompositeTest("ShellSort") {

this->add_test<SortTests<adt::ShellSort>>();

} // ShellSortTests::ShellSortTests


template<typename Key>
using RadixSortHelper = adt::RadixSort<Key, Key>;

RadixSortTests::RadixSortTests() : stest::CompositeTest("RadixSort") {

this->add_test<SortTests<RadixSortHelper>>();

} // RadixSortTests::RadixSortTests


MergeSortTests::MergeSortTests() : stest::CompositeTest("MergeSort") {

this->add_test<SortTests<adt::MergeSort>>();

} // MergeSortTests::MergeSortTests


} // namespace ds::tests

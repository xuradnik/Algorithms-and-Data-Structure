#include "priority_queue.test.hpp"

#include <libds/adt/priority_queue.hpp>

#include <algorithm>
#include <concepts>


namespace ds::tests {


template<typename PQType>
PQType createPQ() {
    if constexpr (std::same_as<PQType, adt::TwoLists<int, int>>) {
        return adt::TwoLists<int, int>(16);
    } else {
        return PQType();
    }
}


template<template<typename, typename> typename PriorityQueueType>
PriorityQueueTests<PriorityQueueType>::PriorityQueueTests() :
    stest::CompositeTest("PriorityQueue")
{

this->add_test("push_empty", [](stest::LeafTest &s){
    PriorityQueueType<int, int> q = createPQ<PriorityQueueType<int, int>>();

    q.push(1,1);

    s.assert_false(q.isEmpty(), "Queue is not empty.");
    s.assert_equals(1UZ, q.size(), "Size is 1.");
    s.assert_equals(1, q.peek(), "Element 1 is pushed.");
});

this->add_test("push_notEmpty", [](stest::LeafTest &s){
    PriorityQueueType<int, int> q = createPQ<PriorityQueueType<int, int>>();

    q.push(2,2);
    q.push(1,1);

    s.assert_equals(2UZ, q.size(), "Size is 2.");
    s.assert_equals(1, q.peek(), "Priority order is preserved.");
});

this->add_test("peek_constEmpty", [](stest::LeafTest &s){
    PriorityQueueType<int, int> q = createPQ<PriorityQueueType<int, int>>();

    s.assert_throws([&](){ q.peek(); }, "Peek throws on empty.");
});

this->add_test("peek_notConstEmpty", [](stest::LeafTest &s){
    PriorityQueueType<int, int> q = createPQ<PriorityQueueType<int, int>>();

    s.assert_throws([&](){ q.peek(); }, "Peek throws on empty.");
});

this->add_test("peek_constNotEmpty", [](stest::LeafTest &s){
    PriorityQueueType<int, int> q = createPQ<PriorityQueueType<int, int>>();

    q.push(2,2);
    q.push(1,1);

    const PriorityQueueType<int, int> &qRef = q;

    s.assert_equals(1, qRef.peek(), "Priority order is preserved.");
});

this->add_test("peek_notConstNotEmpty", [](stest::LeafTest &s){
    PriorityQueueType<int, int> q = createPQ<PriorityQueueType<int, int>>();

    q.push(2,2);
    q.push(1,1);

    s.assert_equals(1, q.peek(), "Priority order is preserved.");
});

this->add_test("pop_empty", [](stest::LeafTest &s){
    PriorityQueueType<int, int> q = createPQ<PriorityQueueType<int, int>>();

    s.assert_throws([&](){ q.pop(); }, "Throws on empty.");
});

this->add_test("pop_notEmptyOne", [](stest::LeafTest &s){
    PriorityQueueType<int, int> q = createPQ<PriorityQueueType<int, int>>();

    q.push(1,1);
    q.push(2,2);
    q.pop();

    s.assert_false(q.isEmpty(), "Queue is not empty.");
    s.assert_equals(1UZ, q.size(), "Size is 1.");
});

this->add_test("pop_notEmptyMany", [](stest::LeafTest &s){
    PriorityQueueType<int, int> q = createPQ<PriorityQueueType<int, int>>();
    const std::vector<int> data = {5,4,10,13,6,12,20,8,20,13};

    for (int i = 0; i < 10; ++i) {
        q.push(data[i], data[i]);
    }

    for (int i = 0; i < 5; ++i) {
        q.pop();
    }

    s.assert_false(q.isEmpty(), "Queue is not empty.");
    s.assert_equals(5UZ, q.size(), "Size is 5.");
});

this->add_test("pop_notEmptyAll", [](stest::LeafTest &s){
    PriorityQueueType<int, int> q = createPQ<PriorityQueueType<int, int>>();
    const std::vector<int> data = {5,4,10,13,6,12,20,8,20,13};

    for (int i = 0; i < 10; ++i) {
        q.push(data[i], data[i]);
    }

    for (int i = 0; i < 10; ++i) {
        q.pop();
    }

    s.assert_true(q.isEmpty(), "Queue is empty.");
    s.assert_equals(0UZ, q.size(), "Size is 0.");
});

this->add_test("pop_prioOrder", [](stest::LeafTest &s){
    PriorityQueueType<int, int> q = createPQ<PriorityQueueType<int, int>>();
    std::vector<int> data = {5,4,10,13,6,12,20,8,20,13};

    for (const int e : data) {
        q.push(e,e);
    }

    std::sort(data.begin(), data.end());

    for (const int e : data) {
        s.assert_equals(e, q.peek(), "Priority is preserved.");
        q.pop();
    }

    s.assert_true(q.isEmpty(), "Queue is left empty.");
});

this->add_test("pop_same", [](stest::LeafTest &s){
    PriorityQueueType<int, int> q = createPQ<PriorityQueueType<int, int>>();

    for (int i = 0; i < 10; ++i) {
        q.push(1,1);
    }

    for (int i = 0; i < 10; ++i) {
        s.assert_equals(1, q.peek(), "All elements are 1.");
        q.pop();
    }

    s.assert_true(q.isEmpty(), "Queue is empty.");
    s.assert_equals(0UZ, q.size(), "Size is 0.");
});

} // PriorityQueueTests::PriorityQueueTests


UnosortedImplicitSequencePQTests::UnosortedImplicitSequencePQTests() :
    stest::CompositeTest("UnosortedImplicitSequencePriorityQueue")
{

this->add_test<PriorityQueueTests<adt::UnsortedImplicitSequencePriorityQueue>>();

} // UnosortedImplicitSequencePQTests::UnosortedImplicitSequencePQTests


UnosortedExplicitSequencePQTests::UnosortedExplicitSequencePQTests() :
    stest::CompositeTest("UnosortedExplicitSequencePriorityQueue")
{

this->add_test<PriorityQueueTests<adt::UnsortedExplicitSequencePriorityQueue>>();

} // UnosortedExplicitSequencePQTests::UnosortedExplicitSequencePQTests


SortedImplicitSequencePQTests::SortedImplicitSequencePQTests() :
    stest::CompositeTest("SortedImplicitSequencePriorityQueue")
{

this->add_test<PriorityQueueTests<adt::SortedImplicitSequencePriorityQueue>>();

} // SortedImplicitSequencePQTests::SortedImplicitSequencePQTests


SortedExplicitSequencePQTests::SortedExplicitSequencePQTests() :
    stest::CompositeTest("SortedExplicitSequencePriorityQueue")
{

this->add_test<PriorityQueueTests<adt::SortedExplicitSequencePriorityQueue>>();

} // SortedExplicitSequencePQTests::SortedExplicitSequencePQTests


TwoListsTests::TwoListsTests() :
    stest::CompositeTest("TwoLists")
{

this->add_test<PriorityQueueTests<adt::TwoLists>>();

} // TwoListsPQTests::TwoListsPQTests


BinaryHeapTests::BinaryHeapTests() :
    stest::CompositeTest("BinaryHeap")
{

this->add_test<PriorityQueueTests<adt::BinaryHeap>>();

} // BinaryHeapPQTests::BinaryHeapPQTests


} // namespace ds::tests

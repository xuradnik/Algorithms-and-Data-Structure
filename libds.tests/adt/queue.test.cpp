#include "queue.test.hpp"

#include <libds/adt/queue.hpp>


namespace ds::tests {


template<template<typename> typename QueueType>
QueueTests<QueueType>::QueueTests() : stest::CompositeTest("Queue") {

this->add_test("push_empty", [](stest::LeafTest &s){
    QueueType<int> q;

    q.push(1);

    s.assert_false(q.isEmpty(), "Queue is not empty.");
    s.assert_equals(1UZ, q.size(), "Size is 1.");
    s.assert_equals(1, q.peek(), "Element 1 is pushed.");
});

this->add_test("push_notEmpty", [](stest::LeafTest &s){
    QueueType<int> q;

    q.push(1);
    q.push(2);

    s.assert_equals(2UZ, q.size(), "Size is 2.");
    s.assert_equals(1, q.peek(), "FIFO is preserved.");
});

this->add_test("peek_constEmpty", [](stest::LeafTest &s){
    const QueueType<int> q;

    s.assert_throws([&](){ q.peek(); }, "Peek throws on empty.");
});

this->add_test("peek_notConstEmpty", [](stest::LeafTest &s){
    QueueType<int> q;

    s.assert_throws([&](){ q.peek(); }, "Peek throws on empty.");
});

this->add_test("peek_constNotEmpty", [](stest::LeafTest &s){
    QueueType<int> q;

    q.push(1);
    q.push(2);

    const QueueType<int> &qRef = q;

    s.assert_equals(1, qRef.peek(), "FIFO is preserved.");
});

this->add_test("peek_notConstNotEmpty", [](stest::LeafTest &s){
    QueueType<int> q;

    q.push(1);
    q.push(2);

    s.assert_equals(1, q.peek(), "FIFO is preserved.");
});

this->add_test("pop_empty", [](stest::LeafTest &s){
    QueueType<int> q;

    s.assert_throws([&](){ q.pop(); }, "Throws on empty.");
});

this->add_test("pop_notEmptyOne", [](stest::LeafTest &s){
    QueueType<int> q;

    q.push(1);
    q.push(2);
    q.pop();

    s.assert_false(q.isEmpty(), "Queue is not empty.");
    s.assert_equals(1UZ, q.size(), "Size is 1.");
});

this->add_test("pop_notEmptyMany", [](stest::LeafTest &s){
    QueueType<int> q;

    for (int i = 0; i < 10; ++i) {
        q.push(i);
    }

    for (int i = 0; i < 5; ++i) {
        q.pop();
    }

    s.assert_false(q.isEmpty(), "Queue is not empty.");
    s.assert_equals(5UZ, q.size(), "Size is 5.");
});

this->add_test("pop_notEmptyAll", [](stest::LeafTest &s){
    QueueType<int> q;

    for (int i = 0; i < 10; ++i) {
        q.push(i);
    }

    for (int i = 0; i < 10; ++i) {
        q.pop();
    }

    s.assert_true(q.isEmpty(), "Queue is empty.");
    s.assert_equals(0UZ, q.size(), "Size is 0.");
});

this->add_test("pop_fifoOrder", [](stest::LeafTest &s){
    QueueType<int> q;

    q.push(1);
    q.push(2);
    q.push(3);

    s.assert_equals(1, q.peek(), "Top is 1.");
    q.pop();
    s.assert_equals(2, q.peek(), "Top is 2 after one pop.");
    q.pop();
    s.assert_equals(3, q.peek(), "Top is 3 after two pops.");
});

this->add_test("copyConstruct", [](stest::LeafTest &s){
    QueueType<int> q1;

    for (int i = 0; i < 10; ++i) {
        q1.push(i);
    }

    QueueType<int> q2(q1);

    s.assert_equals(q1.size(), q2.size(), "Sizes are equal.");
    q1.pop();
    s.assert_equals(10UZ, q2.size(), "Copy is independent.");
    s.assert_equals(0, q2.peek(), "Copy is not modified.");
});

this->add_test("assign_emptyBoth", [](stest::LeafTest &s){
    QueueType<int> q1;
    QueueType<int> q2;
    q2.assign(q1);

    s.assert_equals(q1.size(), q2.size(), "Sizes are equal.");
    s.assert_true(q2.isEmpty(), "Assigned is empty.");
});

this->add_test("assign_emptyOther", [](stest::LeafTest &s){
    QueueType<int> q1;

    q1.push(1);
    q1.push(2);
    q1.push(3);

    QueueType<int> q2;
    q1.assign(q2);

    s.assert_equals(q1.size(), q2.size(), "Sizes are equal.");
    s.assert_true(q2.isEmpty(), "Both are empty.");
});

this->add_test("assign_emptyThis", [](stest::LeafTest &s){
    QueueType<int> q1;

    q1.push(1);
    q1.push(2);
    q1.push(3);

    QueueType<int> q2;
    q2.assign(q1);

    s.assert_equals(q1.size(), q2.size(), "Sizes are equal.");
    s.assert_equals(q1.peek(), q2.peek(), "First elements are equal.");
    q1.pop();
    q2.pop();
    s.assert_equals(q1.peek(), q2.peek(), "Second elements are equal.");
    q1.pop();
    q2.pop();
    s.assert_equals(q1.peek(), q2.peek(), "Third elements are equal.");
});

this->add_test("assign_notEmptyBoth", [](stest::LeafTest &s){
    QueueType<int> q1;

    q1.push(1);
    q1.push(2);
    q1.push(3);

    QueueType<int> q2;

    q2.push(10);
    q2.push(20);

    q1.assign(q2);

    s.assert_equals(q1.size(), q2.size(), "Sizes are equal.");
    s.assert_equals(q1.peek(), q2.peek(), "First elements are equal.");
    q1.pop();
    q2.pop();
    s.assert_equals(q1.peek(), q2.peek(), "Second elements are equal.");
});

this->add_test("equals_equal", [](stest::LeafTest &s){
    QueueType<int> q1;

    q1.push(1);
    q1.push(2);
    q1.push(3);

    QueueType<int> q2;

    q2.push(1);
    q2.push(2);
    q2.push(3);

    s.assert_true(q1.equals(q2), "Lists are equal.");
});

this->add_test("equals_notEqual", [](stest::LeafTest &s){
    QueueType<int> q1;

    q1.push(1);
    q1.push(2);
    q1.push(3);

    QueueType<int> q2;

    q2.push(1);
    q2.push(2);
    q2.push(2);

    s.assert_false(q1.equals(q2), "Lists are not equal.");
});

} // QueueTests<QueueType>::QueueTests


ImplicitQueueTests::ImplicitQueueTests() :
    stest::CompositeTest("ImplicitQueue")
{

this->add_test<QueueTests<ds::adt::ImplicitQueue>>();

this->add_test("isEmpty_full", [](stest::LeafTest &s){
    adt::ImplicitQueue<int> q(4);

    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);

    s.assert_false(q.isEmpty(), "Full queue is not empty.");
});

this->add_test("push_cyclic", [](stest::LeafTest &s){
    adt::ImplicitQueue<int> q(4);

    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);
    q.pop();
    q.pop();
    q.pop();
    q.pop();
    q.push(5);
    q.push(6);

    s.assert_equals(5, q.peek(), "Index cycling works.");
});

} // ImplicitQueueTests::ImplicitQueueTests


ExplicitQueueTests::ExplicitQueueTests() :
    stest::CompositeTest("ExplicitQueue")
{

this->add_test<QueueTests<ds::adt::ExplicitQueue>>();

} // ExplicitQueueTests::ExplicitQueueTests


} // namespace ds::tests

#include "stack.test.hpp"

#include <libds/adt/stack.hpp>


namespace ds::tests {


template<template<typename> typename StackType>
StackTests<StackType>::StackTests() : stest::CompositeTest("Stack") {

this->add_test("push_empty", [](stest::LeafTest &s){
    StackType<int> stack;

    stack.push(1);

    s.assert_false(stack.isEmpty(), "Stack is not empty.");
    s.assert_equals(1UZ, stack.size(), "Size is 1.");
    s.assert_equals(1, stack.peek(), "Element 1 is pushed.");
});

this->add_test("push_notEmpty", [](stest::LeafTest &s){
    StackType<int> stack;

    stack.push(1);
    stack.push(2);

    s.assert_equals(2UZ, stack.size(), "Size is 2.");
    s.assert_equals(2, stack.peek(), "LIFO is preserved.");
});

this->add_test("peek_constEmpty", [](stest::LeafTest &s){
    const StackType<int> stack;

    s.assert_throws([&](){ stack.peek(); }, "Peek throws on empty.");
});

this->add_test("peek_notConstEmpty", [](stest::LeafTest &s){
    StackType<int> stack;

    s.assert_throws([&](){ stack.peek(); }, "Peek throws on empty.");
});

this->add_test("peek_constNotEmpty", [](stest::LeafTest &s){
    StackType<int> stack;

    stack.push(1);
    stack.push(2);

    const StackType<int> &stackRef = stack;

    s.assert_equals(2, stackRef.peek(), "LIFO is preserved.");
});

this->add_test("peek_notConstNotEmpty", [](stest::LeafTest &s){
    StackType<int> stack;

    stack.push(1);
    stack.push(2);

    s.assert_equals(2, stack.peek(), "LIFO is preserved.");
});

this->add_test("pop_empty", [](stest::LeafTest &s){
    StackType<int> stack;

    s.assert_throws([&](){ stack.pop(); }, "Throws on empty.");
});

this->add_test("pop_notEmptyOne", [](stest::LeafTest &s){
    StackType<int> stack;

    stack.push(1);
    stack.push(2);
    stack.pop();

    s.assert_false(stack.isEmpty(), "Stack is not empty.");
    s.assert_equals(1UZ, stack.size(), "Size is 1.");
    s.assert_equals(1, stack.peek(), "LIFO is preserved after pop.");
});

this->add_test("pop_notEmptyMany", [](stest::LeafTest &s){
    StackType<int> stack;

    for (int i = 0; i < 10; ++i) {
        stack.push(i);
    }

    for (int i = 0; i < 5; ++i) {
        stack.pop();
    }

    s.assert_false(stack.isEmpty(), "Stack is not empty.");
    s.assert_equals(5UZ, stack.size(), "Size is 5.");
    s.assert_equals(4, stack.peek(), "LIFO is preserved after multiple pops.");
});

this->add_test("pop_notEmptyAll", [](stest::LeafTest &s){
    StackType<int> stack;

    for (int i = 0; i < 10; ++i) {
        stack.push(i);
    }

    for (int i = 0; i < 10; ++i) {
        stack.pop();
    }

    s.assert_true(stack.isEmpty(), "Stack is empty.");
    s.assert_equals(0UZ, stack.size(), "Size is 0.");
});

this->add_test("pop_lifoOrder", [](stest::LeafTest &s){
    StackType<int> stack;

    stack.push(1);
    stack.push(2);
    stack.push(3);

    s.assert_equals(3, stack.peek(), "Top is 3.");
    stack.pop();
    s.assert_equals(2, stack.peek(), "Top is 2 after one pop.");
    stack.pop();
    s.assert_equals(1, stack.peek(), "Top is 1 after two pops.");
});

this->add_test("copyConstruct", [](stest::LeafTest &s){
    StackType<int> s1;

    for (int i = 0; i < 10; ++i) {
        s1.push(i);
    }

    StackType<int> s2(s1);

    s.assert_equals(s1.size(), s2.size(), "Sizes are equal.");
    s1.pop();
    s.assert_equals(10UZ, s2.size(), "Copy is independent.");
    s.assert_equals(9, s2.peek(), "Copy is not modified.");
});

this->add_test("assign_emptyBoth", [](stest::LeafTest &s){
    StackType<int> s1;
    StackType<int> s2;
    s2.assign(s1);

    s.assert_equals(s1.size(), s2.size(), "Sizes are equal.");
    s.assert_true(s2.isEmpty(), "Assigned is empty.");
});

this->add_test("assign_emptyOther", [](stest::LeafTest &s){
    StackType<int> s1;

    s1.push(1);
    s1.push(2);
    s1.push(3);

    StackType<int> s2;
    s1.assign(s2);

    s.assert_equals(s1.size(), s2.size(), "Sizes are equal.");
    s.assert_true(s1.isEmpty(), "Both are empty.");
});

this->add_test("assign_emptyThis", [](stest::LeafTest &s){
    StackType<int> s1;

    s1.push(1);
    s1.push(2);
    s1.push(3);

    StackType<int> s2;
    s2.assign(s1);

    s.assert_equals(s1.size(), s2.size(), "Sizes are equal.");
    s.assert_equals(s1.peek(), s2.peek(), "Top elements are equal.");
    s1.pop();
    s2.pop();
    s.assert_equals(s1.peek(), s2.peek(), "Second elements are equal.");
    s1.pop();
    s2.pop();
    s.assert_equals(s1.peek(), s2.peek(), "Third elements are equal.");
});

this->add_test("assign_notEmptyBoth", [](stest::LeafTest &s){
    StackType<int> s1;

    s1.push(1);
    s1.push(2);
    s1.push(3);

    StackType<int> s2;

    s2.push(10);
    s2.push(20);

    s1.assign(s2);

    s.assert_equals(s1.size(), s2.size(), "Sizes are equal.");
    s.assert_equals(s1.peek(), s2.peek(), "Top elements are equal.");
    s1.pop();
    s2.pop();
    s.assert_equals(s1.peek(), s2.peek(), "Second elements are equal.");
});

this->add_test("equals_equal", [](stest::LeafTest &s){
    StackType<int> s1;

    s1.push(1);
    s1.push(2);
    s1.push(3);

    StackType<int> s2;

    s2.push(1);
    s2.push(2);
    s2.push(3);

    s.assert_true(s1.equals(s2), "Stacks are equal.");
});

this->add_test("equals_notEqual", [](stest::LeafTest &s){
    StackType<int> s1;

    s1.push(1);
    s1.push(2);
    s1.push(3);

    StackType<int> s2;

    s2.push(1);
    s2.push(2);
    s2.push(2);

    s.assert_false(s1.equals(s2), "Stacks are not equal.");
});

} // StackTests<StackType>::StackTests


ImplicitStackTests::ImplicitStackTests() :
    stest::CompositeTest("ImplicitStack")
{

this->add_test<StackTests<ds::adt::ImplicitStack>>();

} // ImplicitStackTests::ImplicitStackTests


ExplicitStackTests::ExplicitStackTests() :
    stest::CompositeTest("ExplicitStack")
{

this->add_test<StackTests<ds::adt::ExplicitStack>>();

} // ExplicitStackTests::ExplicitStackTests


} // namespace ds::tests

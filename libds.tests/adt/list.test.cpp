#include "list.test.hpp"

#include <libds/adt/list.hpp>


namespace ds::tests {


template<template<typename> typename ListType>
ListTests<ListType>::ListTests() : stest::CompositeTest("List") {

this->add_test("insertLast_empty", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertLast(1);

    s.assert_false(l.isEmpty(), "List is not empty.");
    s.assert_equals(size_t{1}, l.size(), "Size is 1.");
    s.assert_equals(1, l.accessLast(), "Element is inserted as last.");
});

this->add_test("insertLast_notEmpty", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertLast(1);
    l.insertLast(2);

    s.assert_equals(size_t{2}, l.size(), "Size is 2.");
    s.assert_equals(2, l.accessLast(), "Element is inserted as last.");
});

this->add_test("insertFirst_empty", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertFirst(1);

    s.assert_false(l.isEmpty(), "List is not empty.");
    s.assert_equals(size_t{1}, l.size(), "Size is 1.");
    s.assert_equals(1, l.accessLast(), "Element is inserted as first.");
});

this->add_test("insertFirst_notEmpty", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertFirst(1);
    l.insertFirst(2);

    s.assert_equals(size_t{2}, l.size(), "Size is 2.");
    s.assert_equals(2, l.accessFirst(), "Element is inserted as first.");
});

this->add_test("insert_emptyValid", [](stest::LeafTest &s){
    ListType<int> l;

    l.insert(1, size_t{0});

    s.assert_false(l.isEmpty(), "List is not empty.");
    s.assert_equals(size_t{1}, l.size(), "Size is 1.");
    s.assert_equals(1, l.accessFirst(), "Element is accessible as first.");
    s.assert_equals(1, l.accessLast(), "Element is accessible as last.");
});

this->add_test("insert_emptyInvalid", [](stest::LeafTest &s){
    ListType<int> l;

    s.assert_throws([&l](){ l.insert(1, size_t{1}); }, "Throws on invalid index.");
});

this->add_test("insert_notEmptyValid", [](stest::LeafTest &s){
    ListType<int> l;

    l.insert(1, size_t{0});
    l.insert(3, size_t{1});
    l.insert(2, size_t{1});

    s.assert_equals(size_t{3}, l.size(), "Size is 3.");
    s.assert_equals(1, l.access(0), "First element is 1.");
    s.assert_equals(2, l.access(1), "Second element is 2.");
    s.assert_equals(3, l.access(2), "Third element is 3 (elements are moved).");
});

this->add_test("accessFirst_empty", [](stest::LeafTest &s){
    ListType<int> l;

    s.assert_throws([&l](){ l.accessFirst(); }, "Throws on empty.");
});

this->add_test("accessFirst_notEmptyMutable", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertLast(1);

    int &first = l.accessFirst();
    s.assert_equals(1, first, "First element is 1.");
    first = 2;
    s.assert_equals(2, l.accessFirst(), "First element is mutated to 2.");
});

this->add_test("accessFirst_notEmptyImmutable", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertLast(1);

    const int &first = l.accessFirst();
    s.assert_equals(1, first, "First element is 1.");
});

this->add_test("accessLast_empty", [](stest::LeafTest &s){
    ListType<int> l;

    s.assert_throws([&l](){ l.accessLast(); }, "Throws on empty.");
});

this->add_test("accessLast_notEmptyMutable", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertLast(1);

    int &last = l.accessLast();
    s.assert_equals(1, last, "Last element is 1.");
    last = 2;
    s.assert_equals(2, l.accessLast(), "Last element is mutated to 2.");
});

this->add_test("accessLast_notEmptyImmutable", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertLast(1);

    const int &last = l.accessLast();
    s.assert_equals(1, last, "First element is 1.");
});

this->add_test("removeFirst_empty", [](stest::LeafTest &s){
    ListType<int> l;

    s.assert_throws([&l](){ l.removeFirst(); }, "Throws on empty.");
});

this->add_test("removeFirst_notEmpty", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertLast(1);
    l.insertLast(2);
    l.removeFirst();

    s.assert_equals(size_t{1}, l.size(), "Size is 1.");
    s.assert_equals(2, l.accessFirst(), "First element is removed.");
});

this->add_test("removeLast_empty", [](stest::LeafTest &s){
    ListType<int> l;

    s.assert_throws([&l](){ l.removeLast(); }, "Throws on empty.");
});

this->add_test("removeLast_notEmpty", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertLast(1);
    l.insertLast(2);
    l.removeLast();

    s.assert_equals(size_t{1}, l.size(), "Size is 1.");
    s.assert_equals(1, l.accessLast(), "Last element is removed.");
});

this->add_test("remove_empty", [](stest::LeafTest &s){
    ListType<int> l;

    s.assert_throws([&l](){ l.remove(0); }, "Throws on empty.");
});

this->add_test("remove_notEmpty", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertLast(1);
    l.insertLast(2);
    l.insertLast(3);
    l.remove(2);

    s.assert_equals(size_t{2}, l.size(), "Size is 2.");
    s.assert_equals(1, l.accessFirst(), "First element did not change.");
    s.assert_equals(2, l.accessLast(), "Last element did not change.");
});

this->add_test("calculateIndex_empty", [](stest::LeafTest &s){
    ListType<int> l;
    s.assert_equals(INVALID_INDEX, l.calculateIndex(2), "List has no elements.");
});

this->add_test("calculateIndex_notEmptyValid", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertLast(1);
    l.insertLast(2);
    l.insertLast(3);

    s.assert_equals(size_t{1}, l.calculateIndex(2), "Index of 2 is 1.");
});

this->add_test("calculateIndex_notEmptyInvalid", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertLast(1);
    l.insertLast(2);
    l.insertLast(3);

    s.assert_equals(INVALID_INDEX, l.calculateIndex(5), "5 is not inside.");
});

this->add_test("contains_empty", [](stest::LeafTest &s){
    ListType<int> l;

    s.assert_false(l.contains(1), "Empty list does not contain anything.");
});

this->add_test("contains_notEmptyValid", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertLast(1);
    l.insertLast(2);
    l.insertLast(3);

    s.assert_true(l.contains(2), "List contains 2.");
});

this->add_test("contains_notEmptyInvalid", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertLast(1);
    l.insertLast(2);
    l.insertLast(3);

    s.assert_false(l.contains(4), "List does not contain 4.");
});

this->add_test("clear_empty", [](stest::LeafTest &s){
    ListType<int> l;

    l.clear();

    s.assert_true(l.isEmpty(), "Cleared empty list is empty.");
    s.assert_equals(size_t{0}, l.size(), "Size is 0.");
});

this->add_test("clear_notEmpty", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertLast(1);
    l.insertLast(2);
    l.insertLast(3);

    l.clear();

    s.assert_true(l.isEmpty(), "Cleared list is empty.");
    s.assert_equals(size_t{0}, l.size(), "Size is 0.");
});

this->add_test("iterators_empty", [](stest::LeafTest &s){
    ListType<int> l;

    auto begin = l.begin();
    auto end = l.end();

    s.assert_true(begin == end, "begin == end for empty.");
});

this->add_test("iterators_notEmpty", [](stest::LeafTest &s){
    ListType<int> l;

    l.insertLast(1);
    l.insertLast(2);
    l.insertLast(3);

    auto begin = l.begin();
    auto end = l.end();
    const int first = *begin++;
    const int second = *begin++;
    const int third = *begin++;

    s.assert_true(begin == end, "All elements were accessed.");
    s.assert_equals(1, first, "First element is 1.");
    s.assert_equals(2, second, "First element is 2.");
    s.assert_equals(3, third, "First element is 3.");
});

this->add_test("copyConstruct", [](stest::LeafTest &s){
    ListType<int> l1;

    l1.insertLast(1);
    l1.insertLast(2);
    l1.insertLast(3);

    ListType<int> l2(l1);

    s.assert_equals(l1.size(), l2.size(), "Sizes are equal.");
    s.assert_equals(l1.access(0), l2.access(0), "First elements are equal.");
    s.assert_equals(l1.access(1), l2.access(1), "Second elements are equal.");
    s.assert_equals(l1.access(2), l2.access(2), "Third elements are equal.");

    l1.access(0) = 100;

    s.assert_equals(l2.access(0), 1, "Copy does not share memory.");
});

this->add_test("assign_emptyBoth", [](stest::LeafTest &s){
    ListType<int> l1;
    ListType<int> l2;
    l2.assign(l1);

    s.assert_equals(l1.size(), l2.size(), "Sizes are equal.");
    s.assert_true(l2.isEmpty(), "Assigned is empty.");
});

this->add_test("assign_emptyOther", [](stest::LeafTest &s){
    ListType<int> l1;

    l1.insertLast(1);
    l1.insertLast(2);
    l1.insertLast(3);

    ListType<int> l2;
    l1.assign(l2);

    s.assert_equals(l1.size(), l2.size(), "Sizes are equal.");
    s.assert_true(l2.isEmpty(), "Both are empty.");
});

this->add_test("assign_emptyThis", [](stest::LeafTest &s){
    ListType<int> l1;

    l1.insertLast(1);
    l1.insertLast(2);
    l1.insertLast(3);

    ListType<int> l2;
    l2.assign(l1);

    s.assert_equals(l1.size(), l2.size(), "Sizes are equal.");
    s.assert_equals(l1.access(0), l2.access(0), "First elements are equal.");
    s.assert_equals(l1.access(1), l2.access(1), "Second elements are equal.");
    s.assert_equals(l1.access(2), l2.access(2), "Third elements are equal.");

    l1.access(0) = 100;

    s.assert_equals(l2.access(0), 1, "Copy does not share memory.");
});

this->add_test("assign_notEmptyBoth", [](stest::LeafTest &s){
    ListType<int> l1;

    l1.insertLast(1);
    l1.insertLast(2);
    l1.insertLast(3);

    ListType<int> l2;

    l2.insertLast(10);
    l2.insertLast(20);

    l1.assign(l2);

    s.assert_equals(l1.size(), l2.size(), "Sizes are equal.");
    s.assert_equals(l1.access(0), l2.access(0), "First elements are equal.");
    s.assert_equals(l1.access(1), l2.access(1), "Second elements are equal.");

    l2.access(0) = 100;

    s.assert_equals(l1.access(0), 10, "Copy does not share memory.");
});

this->add_test("equals_equal", [](stest::LeafTest &s){
    ListType<int> l1;

    l1.insertLast(1);
    l1.insertLast(2);
    l1.insertLast(3);

    ListType<int> l2;

    l2.insertLast(1);
    l2.insertLast(2);
    l2.insertLast(3);

    s.assert_true(l1.equals(l2), "Lists are equal.");
});

this->add_test("equals_notEqual", [](stest::LeafTest &s){
    ListType<int> l1;

    l1.insertLast(1);
    l1.insertLast(2);
    l1.insertLast(3);

    ListType<int> l2;

    l2.insertLast(1);
    l2.insertLast(2);
    l2.insertLast(2);

    s.assert_false(l1.equals(l2), "Lists are not equal.");
});

} // ListTests<ListType>::ListTests


ImplicitListTests::ImplicitListTests() : stest::CompositeTest("ImplicitList") {

this->add_test<ListTests<adt::ImplicitList>>();

} // ImplicitListTests::ImplicitListTests


SinglyLinkedListTests::SinglyLinkedListTests() :
    stest::CompositeTest("SinglyLinkedList")
{

this->add_test<ListTests<adt::SinglyLinkedList>>();

} // SinglyLinkedListTests::SinglyLinkedListTests


DoublyLinkedListTests::DoublyLinkedListTests() :
    stest::CompositeTest("DoublyLinkedList")
{

this->add_test<ListTests<adt::DoublyLinkedList>>();

} // SinglyLinkedListTests::DoublyLinkedListTests


} // namespace ds::tests

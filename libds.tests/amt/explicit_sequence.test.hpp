#ifndef LIBDS_TESTS_AMT_EXPLICIT_SEQUENCE_TEST_HPP
#define LIBDS_TESTS_AMT_EXPLICIT_SEQUENCE_TEST_HPP

#include <libstest/test.hpp>


namespace ds::tests {


/**
 * @brief All tests for singly linked sequence.
 */
class SinglyLinkedSequenceTests : public stest::CompositeTest {
public:
    SinglyLinkedSequenceTests();
};


/**
 * @brief All tests for doubly linked sequence.
 */
class DoublyLinkedSequenceTests : public stest::CompositeTest {
public:
    DoublyLinkedSequenceTests();
};


/**
 * @brief Tests for all explicit sequences.
 */
class ExplicitSequenceTests : public stest::CompositeTest {
public:
    ExplicitSequenceTests();
};


} // namespace ds::tests

#endif

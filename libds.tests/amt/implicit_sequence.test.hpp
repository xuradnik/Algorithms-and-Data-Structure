#ifndef LIBDS_TESTS_AMT_IMPLICIT_SEQUENCE_TEST_HPP
#define LIBDS_TESTS_AMT_IMPLICIT_SEQUENCE_TEST_HPP

#include <libds/constants.hpp>
#include <libds/amt/implicit_sequence.hpp>

#include <libstest/test.hpp>


namespace ds::tests {


/**
 *  @brief Tests index of next.
 */
class ImplicitSequenceTestIndexOfNext : public stest::LeafTest {
public:
    ImplicitSequenceTestIndexOfNext();
protected:
    void test() override;
};

/**
 *  @brief Tests index of previous.
 */
class ImplicitSequenceTestIndexOfPrevious : public stest::LeafTest {
public:
    ImplicitSequenceTestIndexOfPrevious();
protected:
    void test() override;
};


/**
 *  @brief Tests index of next.
 */
class CyclicImplicitSequenceTestIndexOfNext : public stest::LeafTest {
public:
    CyclicImplicitSequenceTestIndexOfNext();
protected:
    void test() override;
};

/**
 *  @brief Tests index of previous.
 */
class CyclicImplicitSequenceTestIndexOfPrevious : public stest::LeafTest {
public:
    CyclicImplicitSequenceTestIndexOfPrevious();
protected:
    void test() override;
};


/**
 *  @brief All ImplicitSequenceTests.
 */
class ImplicitSequenceTests : public stest::CompositeTest {
public:
    ImplicitSequenceTests();
};


/**
 *  @brief All CyclicImplicitSequenceTests.
 */
class CyclicImplicitSequenceTests : public stest::CompositeTest {
public:
    CyclicImplicitSequenceTests();
};


} // namespace ds::tests

#endif

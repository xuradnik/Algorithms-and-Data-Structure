#ifndef LIBDS_TESTS_MM_COMPACT_MEMORY_TRIVIAL_MANAGER_TEST_HPP
#define LIBDS_TESTS_MM_COMPACT_MEMORY_TRIVIAL_MANAGER_TEST_HPP

#include <libds/mm/compact_memory_trivial_manager.hpp>

#include <libstest/test.hpp>


namespace ds::tests {


/**
 * @brief Tests getter for allocated count.
 */
class TrivialCMMTestGetters : public stest::LeafTest {
public:
    TrivialCMMTestGetters();
protected:
    void test() override;
};


/**
 * @brief Tests allocation at the end.
 */
class TrivialCMMTestAllocate : public stest::LeafTest {
public:
    TrivialCMMTestAllocate();
protected:
    void test() override;
};


/**
 * @brief Tests allocation at an arbitrary index.
 */
class TrivialCMMTestAllocateAt : public stest::LeafTest {
public:
    TrivialCMMTestAllocateAt();
protected:
    void test() override;
};


/**
 * @brief Tests automatic capacity expansion.
 */
class TrivialCMMTestExpansion : public stest::LeafTest {
public:
    TrivialCMMTestExpansion();
protected:
    void test() override;
};


/**
 * @brief Tests deallocation at the end.
 */
class TrivialCMMTestReleaseLast : public stest::LeafTest {
public:
    TrivialCMMTestReleaseLast();
protected:
    void test() override;
};


/**
 * @brief Tests deallocation of a pointer.
 */
class TrivialCMMTestReleasePtr : public stest::LeafTest {
public:
    TrivialCMMTestReleasePtr();
protected:
    void test() override;
};


/**
 * @brief Tests calculateIndex.
 */
class TrivialCMMTestCalculateIndex : public stest::LeafTest {
public:
    TrivialCMMTestCalculateIndex();
protected:
    void test() override;
};


/**
 * @brief Tests swap.
 */
class TrivialCMMTestSwap : public stest::LeafTest {
public:
    TrivialCMMTestSwap();
protected:
    void test() override;
};


/**
 * @brief Tests copy construction.
 */
class TrivialCMMTestCopyConstruct : public stest::LeafTest {
public:
    TrivialCMMTestCopyConstruct();
protected:
    void test() override;
};


/**
 * @brief Tests copy assignment.
 */
class TrivialCMMTestCopyAssign : public stest::LeafTest {
public:
    TrivialCMMTestCopyAssign();
protected:
    void test() override;
};


/**
 * @brief Tests equality comparison.
 */
class TrivialCMMTestEquals : public stest::LeafTest {
public:
    TrivialCMMTestEquals();
protected:
    void test() override;
};


/**
 * @brief All CompactMemoryTrivialManager leaf tests.
 */
class TrivialCMMTests : public stest::CompositeTest {
public:
    TrivialCMMTests();
};


} // namespace ds::tests

#endif

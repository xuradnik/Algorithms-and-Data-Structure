#ifndef LIBDS_TESTS_MM_COMPACT_MEMORY_OBJECT_MANAGER_TEST_HPP
#define LIBDS_TESTS_MM_COMPACT_MEMORY_OBJECT_MANAGER_TEST_HPP

#include <libstest/test.hpp>

namespace ds::tests {


/**
 * @brief Tests allocation at an arbitrary index.
 */
class ObjectCMMTestAllocateAt : public stest::LeafTest {
public:
    ObjectCMMTestAllocateAt();
protected:
    void test() override;
};


/**
 * @brief Tests deallocation of a pointer.
 */
class ObjectCMMTestReleasePtr : public stest::LeafTest {
public:
    ObjectCMMTestReleasePtr();
protected:
    void test() override;
};


/**
 * @brief Tests copy construction.
 */
class ObjectCMMTestCopyConstruct : public stest::LeafTest {
public:
    ObjectCMMTestCopyConstruct();
protected:
    void test() override;
};


/**
 * @brief Tests copy assignment.
 */
class ObjectCMMTestCopyAssign : public stest::LeafTest {
public:
    ObjectCMMTestCopyAssign();
protected:
    void test() override;
};


/**
 * @brief All CompactMemoryObjectManager leaf tests.
 */
class ObjectCMMTests : public stest::CompositeTest {
public:
    ObjectCMMTests();
};


} // namespace ds::tests

#endif

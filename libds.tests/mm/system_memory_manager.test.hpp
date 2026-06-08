#ifndef LIBDS_TESTS_MM_SYSTEM_MEMORY_MANAGER_TEST_HPP
#define LIBDS_TESTS_MM_SYSTEM_MEMORY_MANAGER_TEST_HPP

#include <libstest/test.hpp>


namespace ds::tests {


class SystemMMTestAllocate : public stest::LeafTest {
public:
    SystemMMTestAllocate();
protected:
    void test() override;
};


class SystemMMTestRelease : public stest::LeafTest {
public:
    SystemMMTestRelease();
protected:
    void test() override;
};


class SystemMMTestCounts : public stest::LeafTest {
public:
    SystemMMTestCounts();
protected:
    void test() override;
};


class SystemMemoryManagerTest : public stest::CompositeTest {
public:
    SystemMemoryManagerTest();
};


} // namespace ds::tests

#endif

#include "system_memory_manager.test.hpp"

#include <libds/mm/system_memory_manager.hpp>

#include <memory>
#include <vector>


namespace ds::tests {


SystemMMTestAllocate::SystemMMTestAllocate() : LeafTest("allocate") {}

void SystemMMTestAllocate::test() {
    mm::SystemMemoryManager<int> manager;
    int *ip = manager.allocateMemory();
    const int data = 42;
    *ip = data;
    this->assert_equals(data, *ip);
    manager.releaseMemory(ip);
}


SystemMMTestRelease::SystemMMTestRelease() : LeafTest("release") {}

void SystemMMTestRelease::test() {
    mm::SystemMemoryManager<int> manager;
    int *ip = manager.allocateMemory();
    const int data = 42;
    *ip = data;
    manager.releaseMemory(ip);
    this->assert_true(true, "Released memory");
}


SystemMMTestCounts::SystemMMTestCounts() : LeafTest("count") {}

void SystemMMTestCounts::test() {
    const std::size_t elemcount = 10;
    std::vector<int *> elems;
    elems.reserve(elemcount);
    mm::SystemMemoryManager<int> manager;
    for (std::size_t i = 0; i < elemcount; ++i) {
        elems.push_back(manager.allocateMemory());
    }
    this->assert_equals(elemcount, manager.getAllocatedBlockCount());
    for (int *elem : elems) {
        manager.releaseMemory(elem);
    }
}


SystemMemoryManagerTest::SystemMemoryManagerTest() :
    CompositeTest("SystemMemoryManager")
{
    this->add_test<SystemMMTestAllocate>();
    this->add_test<SystemMMTestRelease>();
    this->add_test<SystemMMTestCounts>();
}


} // namespace ds::tests

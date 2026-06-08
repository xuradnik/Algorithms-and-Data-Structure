#include "compact_memory_object_manager.test.hpp"

#include <libds/mm/compact_memory_object_manager.hpp>

#include <format>

#include "../other/utils.hpp"


namespace ds::tests {


ObjectCMMTestAllocateAt::ObjectCMMTestAllocateAt() :
    LeafTest("allocateMemoryAt") {
}

void ObjectCMMTestAllocateAt::test() {
    Dummy::reset_counters();

{
    mm::CompactMemoryObjectManager<Dummy> manager;
    const int n = 10;

    for (int i = 0; i < n / 2; ++i) {
        manager.allocateMemoryAt(i)->set_value(2 * i);
    }

    for (int i = 0; i < n / 2; ++i) {
        const int k = 2 * i + 1;
        manager.allocateMemoryAt(k)->set_value(k);
    }

    for (int i = 0; i < n; ++i) {
        this->assert_equals(i, manager.getBlockAt(i).get_value());
    }

    this->assert_equals(
        static_cast<std::size_t>(n),
        manager.getAllocatedBlockCount());

    for (int i = 0; i < n; ++i) {
        this->assert_true(
            manager.getBlockAt(i).is_self(),
            std::format("Self reference preserved for i = {}", i));
    }
}

    this->assert_equals(
        Dummy::get_constructors_called(),
        Dummy::get_destructors_called());
}


ObjectCMMTestReleasePtr::ObjectCMMTestReleasePtr() :
    LeafTest("releaseMemory") {
}

void ObjectCMMTestReleasePtr::test() {
    Dummy::reset_counters();

{
    mm::CompactMemoryObjectManager<Dummy> manager;
    const int n = 10;

    for (int i = 0; i < n; ++i) {
        manager.allocateMemory()->set_value(i);
    }

    std::size_t index = 4;
    const auto oldCapacity = manager.getCapacity();
    manager.releaseMemory(std::addressof(manager.getBlockAt(index)));

    for (int i = 0; i < n - 1; ++i) {
        const int expected = i < static_cast<int>(index) ? i : i + 1;
        this->assert_equals(
            expected,
            manager.getBlockAt(static_cast<std::size_t>(i)).get_value());
    }

    this->assert_equals(
        static_cast<std::size_t>(n - 1),
        manager.getAllocatedBlockCount());
    this->assert_equals(oldCapacity, manager.getCapacity());

    for (int i = 0; i < n - 1; ++i) {
        this->assert_true(
            manager.getBlockAt(i).is_self(),
            std::format("Self reference preserved for i = {}", i));
    }
}

    this->assert_equals(
        Dummy::get_constructors_called(),
        Dummy::get_destructors_called());
}


ObjectCMMTestCopyConstruct::ObjectCMMTestCopyConstruct() :
    LeafTest("copy-construct") {
}

void ObjectCMMTestCopyConstruct::test() {
    const int n = 10;

{
    mm::CompactMemoryObjectManager<Dummy> manager;
    for (int i = 0; i < n; ++i) {
        manager.allocateMemory()->set_value(i);
    }

    {
        mm::CompactMemoryObjectManager<Dummy> copy(manager);
        this->assert_equals(
            manager.getAllocatedBlockCount(),
            copy.getAllocatedBlockCount());

        for (int i = 0; i < n; ++i) {
            this->assert_equals(manager.getBlockAt(i), copy.getBlockAt(i));
        }

        for (int i = 0; i < n - 1; ++i) {
            this->assert_true(
                manager.getBlockAt(i).is_self(),
                std::format("Self reference preserved for i = {}", i));
        }

        for (int i = 0; i < n - 1; ++i) {
            this->assert_true(
                copy.getBlockAt(i).is_self(),
                std::format("Self reference of the copy correct for i = {}", i));
        }

        for (int i = 0; i < n; ++i) {
            copy.getBlockAt(i).set_value(copy.getBlockAt(i).get_value() * -1);
        }

        copy.clear();
        this->assert_equals(
            static_cast<std::size_t>(n),
            manager.getAllocatedBlockCount());

        for (int i = 0; i < n; ++i) {
            this->assert_equals(i, manager.getBlockAt(i).get_value());
        }
    }

    this->assert_equals(
        static_cast<std::size_t>(n),
        manager.getAllocatedBlockCount());
}
    this->assert_equals(
        Dummy::get_constructors_called(),
        Dummy::get_destructors_called());
}


ObjectCMMTestCopyAssign::ObjectCMMTestCopyAssign() :
    LeafTest("copy-assign") {
}

void ObjectCMMTestCopyAssign::test() {
    const int n = 10;

{
    mm::CompactMemoryObjectManager<Dummy> manager;
    for (int i = 0; i < n; ++i) {
        manager.allocateMemory()->set_value(i);
    }

    mm::CompactMemoryObjectManager<Dummy> copy;
    for (int i = 0; i < n / 2; ++i) {
        copy.allocateMemory()->set_value(i);
    }

    copy.assign(manager);
    this->assert_equals(
        copy.getAllocatedBlockCount(),
        manager.getAllocatedBlockCount());

    for (int i = 0; i < n; ++i) {
        this->assert_equals(manager.getBlockAt(i), copy.getBlockAt(i));
    }

    for (int i = 0; i < n; ++i) {
        manager.getBlockAt(i).set_value(manager.getBlockAt(i).get_value() * -1);
    }

    for (int i = 0; i < n; ++i) {
        this->assert_equals(i, copy.getBlockAt(i).get_value());
    }

    copy.clear();

    this->assert_equals(
        static_cast<std::size_t>(n),
        manager.getAllocatedBlockCount());
}
    this->assert_equals(
        Dummy::get_constructors_called(),
        Dummy::get_destructors_called());
}


ObjectCMMTests::ObjectCMMTests() :
    CompositeTest("CompactMemoryObjectManager")
{
    this->add_test<ObjectCMMTestAllocateAt>();
    this->add_test<ObjectCMMTestReleasePtr>();
    this->add_test<ObjectCMMTestCopyConstruct>();
    this->add_test<ObjectCMMTestCopyAssign>();
}


} // namespace ds::tests

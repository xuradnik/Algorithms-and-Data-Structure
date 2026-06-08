#include "compact_memory_trivial_manager.test.hpp"

#include <libds/mm/compact_memory_trivial_manager.hpp>
#include <libds/mm/compact_memory_object_manager.hpp>

#include <memory>


namespace ds::tests {


TrivialCMMTestGetters::TrivialCMMTestGetters() :
    LeafTest("getters") {
}

void TrivialCMMTestGetters::test() {
    const std::size_t capacity = 20;
    const std::size_t n = 10;
    mm::CompactMemoryTrivialManager<int> manager(capacity);

    for (std::size_t i = 0; i < n; ++i) {
        *manager.allocateMemory() = static_cast<int>(i);
    }

    this->assert_equals(n, manager.getAllocatedBlockCount());
    this->assert_equals(capacity, manager.getCapacity());
}


TrivialCMMTestAllocate::TrivialCMMTestAllocate() :
    LeafTest("allocateMemory") {
}

void TrivialCMMTestAllocate::test() {
    mm::CompactMemoryTrivialManager<int> manager;
    const int n = 10;

    for (int i = 0; i < n; ++i) {
        *manager.allocateMemory() = i;
    }

    for (int i = 0; i < n; ++i) {
        int *expectedPtr = &manager.getBlockAt(0) + i;
        int *actualPtr = &manager.getBlockAt(i);
        this->assert_equals(expectedPtr, actualPtr);
        this->assert_equals(i, *actualPtr);
    }

    this->assert_equals(
        static_cast<std::size_t>(n),
        manager.getAllocatedBlockCount());
}


TrivialCMMTestAllocateAt::TrivialCMMTestAllocateAt() :
    LeafTest("allocateMemoryAt") {
}

void TrivialCMMTestAllocateAt::test() {
    mm::CompactMemoryTrivialManager<int> manager;
    const int n = 10;

    for (int i = 0; i < n / 2; ++i) {
        *manager.allocateMemoryAt(i) = 2 * i;
    }

    for (int i = 0; i < n / 2; ++i) {
        const int k = 2 * i + 1;
        *manager.allocateMemoryAt(k) = k;
    }

    for (int i = 0; i < n; ++i) {
        this->assert_equals(i, manager.getBlockAt(i));
    }

    this->assert_equals(
        static_cast<std::size_t>(n),
        manager.getAllocatedBlockCount());
}


TrivialCMMTestExpansion::TrivialCMMTestExpansion() :
    LeafTest("expansion") {
}

void TrivialCMMTestExpansion::test() {
    mm::CompactMemoryTrivialManager<int> manager;
    for (int i = 0; i < 4; ++i) {
        manager.allocateMemory();
    }

    this->assert_equals(
        static_cast<std::size_t>(4),
        manager.getCapacity());

    manager.allocateMemory();

    this->assert_equals(
        static_cast<std::size_t>(8),
        manager.getCapacity());
}


TrivialCMMTestReleaseLast::TrivialCMMTestReleaseLast() :
    LeafTest("releaseLast") {
}

void TrivialCMMTestReleaseLast::test() {
    mm::CompactMemoryTrivialManager<int> manager;
    const int n = 10;

    for (int i = 0; i < n; ++i) {
        *manager.allocateMemory() = i;
    }

    manager.releaseLast();
    manager.releaseLast();
    this->assert_equals(n - 3, manager.getBlockAt(n - 3));
    this->assert_equals(
        static_cast<std::size_t>(n) - 2,
        manager.getAllocatedBlockCount()
    );
}


TrivialCMMTestReleasePtr::TrivialCMMTestReleasePtr() :
    LeafTest("releaseMemory") {
}

void TrivialCMMTestReleasePtr::test() {
    mm::CompactMemoryTrivialManager<int> manager;
    const int n = 10;

    for (int i = 0; i < n; ++i) {
        *manager.allocateMemory() = i;
    }

    std::size_t index = 4;
    const auto oldCapacity = manager.getCapacity();
    manager.releaseMemory(std::addressof(manager.getBlockAt(index)));

    for (int i = 0; i < n - 1; ++i) {
        const int expected = i < static_cast<int>(index) ? i : i + 1;
        this->assert_equals(
            expected,
            manager.getBlockAt(static_cast<std::size_t>(i)));
    }

    this->assert_equals(
        static_cast<std::size_t>(n - 1),
        manager.getAllocatedBlockCount());
    this->assert_equals(oldCapacity, manager.getCapacity());
}


TrivialCMMTestCalculateIndex::TrivialCMMTestCalculateIndex() :
    LeafTest("calculateIndex") {
}

void TrivialCMMTestCalculateIndex::test() {
    const int n = 10;

    mm::CompactMemoryTrivialManager<int> manager;
    for (int i = 0; i < n; ++i) {
        *manager.allocateMemory() = i;
    }

    int &third = manager.getBlockAt(3);
    this->assert_equals(
        static_cast<std::size_t>(3),
        manager.calculateIndex(third));
}


TrivialCMMTestSwap::TrivialCMMTestSwap() :
    LeafTest("swap") {
}

void TrivialCMMTestSwap::test() {
    const int n = 10;

    mm::CompactMemoryTrivialManager<int> manager;
    for (int i = 0; i < n; ++i) {
        *manager.allocateMemory() = i;
    }

    for (int i = 0; i < n / 2; ++i) {
        manager.swap(i, n - i - 1);
    }

    for (int i = 0; i < n / 2; ++i) {
        this->assert_equals(n - i - 1, manager.getBlockAt(i));
    }
}


TrivialCMMTestCopyConstruct::TrivialCMMTestCopyConstruct() :
    LeafTest("copy-construct") {
}

void TrivialCMMTestCopyConstruct::test() {
    const int n = 10;

    mm::CompactMemoryTrivialManager<int> manager;
    for (int i = 0; i < n; ++i) {
        *manager.allocateMemory() = i;
    }

    {
        mm::CompactMemoryTrivialManager<int> copy(manager);
        this->assert_equals(
            manager.getAllocatedBlockCount(),
            copy.getAllocatedBlockCount());

        for (int i = 0; i < n; ++i) {
            this->assert_equals(manager.getBlockAt(i), copy.getBlockAt(i));
        }

        for (int i = 0; i < n; ++i) {
            copy.getBlockAt(i) *= -1;
        }

        copy.clear();
        this->assert_equals(
            static_cast<std::size_t>(n),
            manager.getAllocatedBlockCount());

        for (int i = 0; i < n; ++i) {
            this->assert_equals(i, manager.getBlockAt(i));
        }
    }

    this->assert_equals(
        static_cast<std::size_t>(n),
        manager.getAllocatedBlockCount());
}


TrivialCMMTestCopyAssign::TrivialCMMTestCopyAssign() :
    LeafTest("copy-assign") {
}

void TrivialCMMTestCopyAssign::test() {
    const int n = 10;

    mm::CompactMemoryTrivialManager<int> manager;
    for (int i = 0; i < n; ++i) {
        *manager.allocateMemory() = i;
    }

    mm::CompactMemoryTrivialManager<int> copy;
    for (int i = 0; i < n / 2; ++i) {
        *copy.allocateMemory() = i;
    }

    copy.assign(manager);
    this->assert_equals(
        copy.getAllocatedBlockCount(),
        manager.getAllocatedBlockCount());

    for (int i = 0; i < n; ++i) {
        this->assert_equals(manager.getBlockAt(i), copy.getBlockAt(i));
    }

    for (int i = 0; i < n; ++i) {
        manager.getBlockAt(i) *= -1;
    }

    for (int i = 0; i < n; ++i) {
        this->assert_equals(i, copy.getBlockAt(i));
    }

    copy.clear();

    this->assert_equals(
        static_cast<std::size_t>(n),
        manager.getAllocatedBlockCount());
}


TrivialCMMTestEquals::TrivialCMMTestEquals() :
    LeafTest("equals") {
}

void TrivialCMMTestEquals::test() {
    const int n = 10;

    mm::CompactMemoryTrivialManager<int> first;
    for (int i = 0; i < n; ++i) {
        *first.allocateMemory() = i;
    }

    mm::CompactMemoryTrivialManager<int> second;
    for (int i = 0; i < n; ++i) {
        *second.allocateMemory() = i;
    }

    this->assert_true(first.equals(second), "Managers are equal.");
    second.releaseLast();
    this->assert_false(first.equals(second), "Managers are not equal.");

    mm::CompactMemoryObjectManager<int> third;
    for (int i = 0; i < n; ++i) {
        *third.allocateMemory() = i;
    }

    this->assert_false(
        first.equals(third),
        "Managers of different types are not equal.");
}


TrivialCMMTests::TrivialCMMTests() :
    CompositeTest("CompactMemoryTrivialManager")
{
    this->add_test<TrivialCMMTestGetters>();
    this->add_test<TrivialCMMTestAllocate>();
    this->add_test<TrivialCMMTestAllocateAt>();
    this->add_test<TrivialCMMTestReleaseLast>();
    this->add_test<TrivialCMMTestReleasePtr>();
    this->add_test<TrivialCMMTestCalculateIndex>();
    this->add_test<TrivialCMMTestSwap>();
    this->add_test<TrivialCMMTestCopyConstruct>();
    this->add_test<TrivialCMMTestCopyAssign>();
    this->add_test<TrivialCMMTestEquals>();
}


} // namespace ds::tests

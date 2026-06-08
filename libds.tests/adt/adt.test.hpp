#ifndef LIBDS_TESTS_ADT_ADT_TEST_HPP
#define LIBDS_TESTS_ADT_ADT_TEST_HPP

#include "array.test.hpp"
#include "list.test.hpp"
#include "priority_queue.test.hpp"
#include "queue.test.hpp"
#include "sorts.test.hpp"
#include "stack.test.hpp"
#include "table.test.hpp"

#include <libstest/test.hpp>

namespace ds::tests {

class ADTTest : public stest::CompositeTest {
public:
    ADTTest() : CompositeTest("adt") {
        this->add_test(std::make_unique<ArraysTest>());
        this->add_test(std::make_unique<ListTest>());
        this->add_test(std::make_unique<PriorityQueueTest>());
        this->add_test(std::make_unique<QueueTest>());
        this->add_test(std::make_unique<StackTest>());
        this->add_test(std::make_unique<TableTest>());
        this->add_test(std::make_unique<SortTest>());
    }
};

} // namespace ds::tests

#endif

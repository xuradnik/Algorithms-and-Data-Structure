#ifndef LIBDS_TESTS_AMT_SEQUENCE_TEST_HPP
#define LIBDS_TESTS_AMT_SEQUENCE_TEST_HPP

#include <libds/amt/sequence.hpp>

#include <libstest/test.hpp>


namespace ds::tests {


/**
 * @brief Tests calculation of elements position.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestCalculateIndex : public stest::LeafTest {
public:
    SequenceTestCalculateIndex() : LeafTest("calculateIndex") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n; ++i) {
            seq.emplaceLast().data = i;
        }

        auto *three = seq.access(2);
        const size_t threeIndex = seq.calculateIndex(*three);
        this->assert_equals(static_cast<size_t>(2), threeIndex);
    }
};


/**
 * @brief Tests insertion of the first and the last element.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestInsertEnds : public stest::LeafTest {
public:
    SequenceTestInsertEnds() : LeafTest("emplaceEnds") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n; ++i) {
            seq.emplaceLast().data = i;
            seq.emplaceFirst().data = i;
            this->assert_equals(i, seq.accessFirst()->data);
            this->assert_equals(i, seq.accessLast()->data);
        }
    }
};


/**
 * @brief Tests insertion at random positions.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestInsertRandom : public stest::LeafTest {
public:
    SequenceTestInsertRandom() : LeafTest("emplaceRandom") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n / 2; ++i) {
            seq.emplace(i).data = 2 * i;
        }
        // 0 2 4 6 8

        for (int i = 0; i < n / 2; ++i) {
            const int k = 2 * i + 1;
            seq.emplace(k).data = k;
        }
        //  v v v v v
        // 0 2 4 6 8

        for (int i = 0; i < n; ++i) {
            this->assert_equals(i, seq.access(i)->data);
        }
        // 0 1 2 3 4 5 6 7 8 9
    }
};


/**
 * @brief Tests insertion before a block.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestInsertBefore : public stest::LeafTest {
public:
    SequenceTestInsertBefore() : LeafTest("emplaceBefore") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n / 2; ++i) {
            seq.emplace(i).data = 2 * i;
        }
        // 0 2 4 6 8

        for (int i = 0; i < n / 2 - 1; ++i) {
            const int k = 2 * i + 1;
            auto *block = seq.access(k);
            seq.emplaceBefore(*block).data = k;
        }
        //   v- v- v- v-
        // 0  2  4  6  8

        for (int i = 0; i < n - 1; ++i) {
            this->assert_equals(i, seq.access(i)->data);
        }
        // 0 1 2 3 4 5 6 7 8
    }
};


/**
 * @brief Tests insertion after a block.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestInsertAfter : public stest::LeafTest {
public:
    SequenceTestInsertAfter() : LeafTest("emplaceAfter") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n / 2; ++i) {
            seq.emplace(i).data = 2 * i;
        }
        // 0 2 4 6 8

        for (int i = 0; i < n / 2; ++i) {
            const int k = 2 * i + 1;
            auto *block = seq.access(k - 1);
            seq.emplaceAfter(*block).data = k;
        }
        // -v -v -v -v -v
        // 0  2  4  6  8

        for (int i = 0; i < n; ++i) {
            this->assert_equals(i, seq.access(i)->data);
        }
        // 0 1 2 3 4 5 6 7 8
    }
};


/**
 * @brief Tests access of the first and the last element.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestAccessEnds : public stest::LeafTest {
public:
    SequenceTestAccessEnds() : LeafTest("accessEnds") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;

        this->assert_null(seq.accessFirst());
        this->assert_null(seq.accessLast());

        for (int i = 0; i < n; ++i) {
            seq.emplaceLast().data = i;
        }

        this->assert_equals(0, seq.accessFirst()->data, "accessFirst");
        this->assert_equals(n - 1, seq.accessLast()->data, "accessLast");
    }
};


/**
 * @brief Tests access of elements at an arbitrary position.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestAccessRandom : public stest::LeafTest {
public:
    SequenceTestAccessRandom() : LeafTest("accessRandom") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n; ++i) {
            seq.emplaceLast().data = i;
        }

        this->assert_null(seq.access(n));
        this->assert_null(seq.access(static_cast<size_t>(n) + 10));
        for (int i = 0; i < n; ++i) {
            this->assert_equals(i, seq.access(static_cast<size_t>(i))->data);
        }
    }
};


/**
 * @brief Tests access of elements at an arbitrary position.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestAccessCorners : public stest::LeafTest {
public:
    SequenceTestAccessCorners() : LeafTest("accessCorners") {
    }

protected:
    void test() override {
        SequenceT seq;
        this->assert_null(seq.access(0));
        seq.emplaceLast().data = 0;
        this->assert_equals(0, seq.access(0)->data);
        this->assert_equals(0, seq.accessFirst()->data);
        this->assert_equals(0, seq.accessLast()->data);
    }
};


/**
 * @brief Test access of next and previous to block element.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestAccessPrevNext : public stest::LeafTest {
public:
    SequenceTestAccessPrevNext() : LeafTest("accessPrevNext") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n; ++i) {
            seq.emplaceLast().data = i;
        }

        this->assert_null(seq.accessPrevious(*seq.accessFirst()));
        this->assert_null(seq.accessNext(*seq.accessLast()));
        this->assert_equals(1, seq.accessNext(*seq.accessFirst())->data);
        this->assert_equals(n - 2, seq.accessPrevious(*seq.accessLast())->data);
        this->assert_equals(4, seq.accessNext(*seq.access(3))->data);
        this->assert_equals(7, seq.accessPrevious(*seq.access(8))->data);
    }
};


/**
 * @brief Tests removal of the first and the last element.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestRemoveEnds : public stest::LeafTest {
public:
    SequenceTestRemoveEnds() : LeafTest("removeEnds") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        seq.emplaceFirst().data = 0;
        for (int i = 1; i < n; ++i) {
            seq.emplaceLast().data = i;
            seq.emplaceFirst().data = i;
        }
        // 9 8 .. 2 1 0 1 2 .. 8 9

        for (int i = 0; i < n - 1; ++i) {
            seq.removeFirst();
            seq.removeLast();
            this->assert_equals(n - i - 2, seq.accessFirst()->data);
            this->assert_equals(n - i - 2, seq.accessLast()->data);
        }
        // 8 7 .. 2 1 0 1 2 .. 7 8
        // 7 6 .. 2 1 0 1 2 .. 6 7
        //           ...
        //            0
    }
};


/**
 * @brief Tests removal of element at an arbitrary position.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestRemoveRandom : public stest::LeafTest {
public:
    SequenceTestRemoveRandom() : LeafTest("removeRandom") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n; ++i) {
            seq.emplaceLast().data = i;
        }
        // 0 1 2 3 4 5 6 7 8 9

        for (int i = 1; i <= n / 2; ++i) {
            seq.remove(i);
        }
        // 0 2 4 6 8

        for (int i = 0; i < n / 2; ++i) {
            this->assert_equals(2 * i, seq.access(i)->data);
        }
    }
};


/**
 * @brief Tests removal of next to block element.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestRemoveNext : public stest::LeafTest {
public:
    SequenceTestRemoveNext() : LeafTest("removeNext") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n; ++i) {
            seq.emplaceLast().data = i;
        }
        // 0 1 2 3 4 5 6 7 8 9

        for (int i = 0; i < n / 2; ++i) {
            auto *block = seq.access(i);
            seq.removeNext(*block);
        }
        // --v --v --v --v --v
        // 0 1 2 3 4 5 6 7 8 9
        //   x   x   x   x   x

        for (int i = 0; i < n / 2; ++i) {
            this->assert_equals(2 * i, seq.access(i)->data);
        }
        // 0 2 4 6 8
    }
};


/**
 * @brief Tests removal of previous to block element.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestRemovePrevious : public stest::LeafTest {
public:
    SequenceTestRemovePrevious() : LeafTest("removePrevious") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n; ++i) {
            seq.emplaceLast().data = i;
        }
        // 0 1 2 3 4 5 6 7 8 9

        for (int i = 1; i <= n / 2; ++i) {
            auto *block = seq.access(i);
            seq.removePrevious(*block);
        }
        // v-- v-- v-- v-- v--
        // 0 1 2 3 4 5 6 7 8 9
        // x   x   x   x   x

        for (int i = 0; i < n / 2; ++i) {
            this->assert_equals(2 * i + 1, seq.access(i)->data);
        }
        // 1 3 5 7 9
    }
};


/**
 * @brief Tests processing of all elements in forward order.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestProcessForward : public stest::LeafTest {
public:
    SequenceTestProcessForward() : LeafTest("processForward") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n; ++i) {
            seq.emplaceLast().data = i;
        }
        // 0 1 2 3 4 5 6 7 8 9

        int processed = 0;
        seq.processBlocksForward(
            seq.accessFirst(),
            [this, &processed](const amt::MemoryBlock<int> *block) {
                this->assert_equals(processed, block->data);
                ++processed;
            }
        );
        this->assert_equals(n, processed);
    }
};


/**
 * @brief Tests processing of all elements in backward order.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestProcessBackward : public stest::LeafTest {
public:
    SequenceTestProcessBackward() : LeafTest("processBackward") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n; ++i) {
            seq.emplaceLast().data = i;
        }
        // 0 1 2 3 4 5 6 7 8 9

        int processed = 0;
        seq.processBlocksBackward(
            seq.accessLast(),
            [this, &processed, n](const amt::MemoryBlock<int> *block) {
                this->assert_equals(n - processed - 1, block->data);
                ++processed;
            }
        );
        this->assert_equals(n, processed);
    }
};


/**
 * @brief Tests finding a block with property.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestFindWithProperty : public stest::LeafTest {
public:
    SequenceTestFindWithProperty() : LeafTest("findBlockWithProperty") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n; ++i) {
            seq.emplaceLast().data = i;
        }
        // 0 1 2 3 4 5 6 7 8 9

        constexpr int intToFind = 5;
        amt::MemoryBlock<int> *found = seq.findBlockWithProperty(
            [=](const amt::MemoryBlock<int> *block) {
                return block->data == intToFind;
            }
        );
        this->assert_not_null(found);
        this->assert_equals(intToFind, found->data);

        amt::MemoryBlock<int> *notFound
            = seq.findBlockWithProperty([](amt::MemoryBlock<int> *) {
                  return false;
              });
        this->assert_null(notFound);
    }
};


/**
 * @brief Tests finding block previous to a block with property.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestFindPrevToWithProperty : public stest::LeafTest {
public:
    SequenceTestFindPrevToWithProperty() :
        LeafTest("findPrevToBlockWithProperty") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n; ++i) {
            seq.emplaceLast().data = i;
        }
        // 0 1 2 3 4 5 6 7 8 9

        constexpr int intToFind = 5;
        amt::MemoryBlock<int> *found = seq.findPreviousToBlockWithProperty(
            [=](const amt::MemoryBlock<int> *block) {
                return block->data == intToFind;
            }
        );
        this->assert_not_null(found);
        this->assert_equals(intToFind - 1, found->data);
        this->assert_null(seq.findPreviousToBlockWithProperty(
            [](amt::MemoryBlock<int> *) { return false; }
        ));
        this->assert_null(seq.findPreviousToBlockWithProperty(
            [](const amt::MemoryBlock<int> *block) { return block->data == 0; }
        ));
    }
};


/**
 * @brief Tests iterator of the sequence.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestIterators : public stest::LeafTest {
public:
    SequenceTestIterators() : LeafTest("iterator") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n; ++i) {
            seq.emplaceLast().data = i;
        }
        // 0 1 2 3 4 5 6 7 8 9

        int expectedData = 0;
        auto it = seq.begin();
        auto endIt = seq.end();
        while (it != endIt) {
            if (*it != expectedData) {
                this->fail("Iterator dereferenced to invalid value!");
                break;
            }
            ++expectedData;
            ++it;
        }

        if (expectedData != n) {
            this->fail("Iterator did not access all elements!");
        } else {
            this->pass("Iterator visited all elements");
        }
    }
};


/**
 * @brief Tests the clear operation.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestClear : public stest::LeafTest {
public:
    SequenceTestClear() : LeafTest("clear") {
    }

protected:
    void test() override {
        constexpr int n = 10;

        SequenceT seq;
        for (int i = 0; i < n; ++i) {
            seq.emplaceLast().data = i;
        }

        seq.clear();

        this->assert_true(seq.isEmpty(), "Cleared sequence is empty.");
        this->assert_equals(static_cast<size_t>(0), seq.size());
    }
};


/**
 * @brief Tests copy construction.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class SequenceTestCopyConstruct : public stest::LeafTest {
public:
    SequenceTestCopyConstruct() : LeafTest("copy-construct") {
    }

protected:
    void test() override {
        SequenceT seq1;
        constexpr int n = 10;
        for (int i = 0; i < n; ++i) {
            seq1.emplaceLast().data = i;
        }

        {
            SequenceT seq2(seq1);
            this->assert_equals(seq1.size(), seq2.size());
            for (int i = 0; i < n; ++i) {
                this->assert_equals(seq1.access(i)->data, seq2.access(i)->data);
            }

            for (int i = 0; i < n; ++i) {
                seq2.access(i)->data *= -1;
            }

            seq2.clear();
            this->assert_equals(static_cast<std::size_t>(n), seq1.size());

            for (int i = 0; i < n; ++i) {
                this->assert_equals(i, seq1.access(i)->data);
            }
        }

        this->assert_equals(static_cast<std::size_t>(n), seq1.size());
    }
};


/**
 * @brief All generic sequence leaf tests.
 * @tparam SequenceT Type of the sequence. Either explicit or implicit.
 */
template<class SequenceT>
class GenericSequenceTest : public stest::CompositeTest {
public:
    GenericSequenceTest() : CompositeTest("Sequence") {
        this->add_test<SequenceTestCalculateIndex<SequenceT>>();
        this->add_test<SequenceTestInsertEnds<SequenceT>>();
        this->add_test<SequenceTestInsertRandom<SequenceT>>();
        this->add_test<SequenceTestInsertBefore<SequenceT>>();
        this->add_test<SequenceTestInsertAfter<SequenceT>>();
        this->add_test<SequenceTestAccessEnds<SequenceT>>();
        this->add_test<SequenceTestAccessRandom<SequenceT>>();
        this->add_test<SequenceTestAccessCorners<SequenceT>>();
        this->add_test<SequenceTestAccessPrevNext<SequenceT>>();
        this->add_test<SequenceTestRemoveEnds<SequenceT>>();
        this->add_test<SequenceTestRemoveRandom<SequenceT>>();
        this->add_test<SequenceTestRemoveNext<SequenceT>>();
        this->add_test<SequenceTestRemovePrevious<SequenceT>>();
        this->add_test<SequenceTestProcessForward<SequenceT>>();
        this->add_test<SequenceTestProcessBackward<SequenceT>>();
        this->add_test<SequenceTestFindWithProperty<SequenceT>>();
        this->add_test<SequenceTestFindPrevToWithProperty<SequenceT>>();
        this->add_test<SequenceTestIterators<SequenceT>>();
        this->add_test<SequenceTestClear<SequenceT>>();
        this->add_test<SequenceTestCopyConstruct<SequenceT>>();
    }
};


} // namespace ds::tests

#endif

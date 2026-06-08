#include "implicit_sequence.test.hpp"

#include "sequence.test.hpp"


namespace ds::tests {


ImplicitSequenceTestIndexOfNext::ImplicitSequenceTestIndexOfNext() :
    LeafTest("indexOfNext") {
}

void ImplicitSequenceTestIndexOfNext::test() {
    constexpr int n = 10;

    amt::ImplicitSequence<int> seq;

    for (int i = 0; i < n; ++i) {
        seq.emplaceLast().data = i;
    }

    this->assert_equals(static_cast<std::size_t>(1), seq.indexOfNext(0));
    this->assert_equals(INVALID_INDEX, seq.indexOfNext(n - 1));
}


ImplicitSequenceTestIndexOfPrevious::ImplicitSequenceTestIndexOfPrevious() :
    LeafTest("indexOfPrevious") {
}

void ImplicitSequenceTestIndexOfPrevious::test() {
    constexpr int n = 10;

    amt::ImplicitSequence<int> seq;

    for (int i = 0; i < n; ++i) {
        seq.emplaceLast().data = i;
    }

    this->assert_equals(static_cast<std::size_t>(4), seq.indexOfPrevious(5));
    this->assert_equals(INVALID_INDEX, seq.indexOfPrevious(0));
}


CyclicImplicitSequenceTestIndexOfNext::CyclicImplicitSequenceTestIndexOfNext() :
    LeafTest("indexOfNext") {
}

void CyclicImplicitSequenceTestIndexOfNext::test() {
    constexpr int n = 10;

    amt::CyclicImplicitSequence<int> seq;

    for (int i = 0; i < n; ++i) {
        seq.emplaceLast().data = i;
    }

    this->assert_equals(static_cast<size_t>(1), seq.indexOfNext(0));
    this->assert_equals(static_cast<size_t>(0), seq.indexOfNext(n - 1));
}


CyclicImplicitSequenceTestIndexOfPrevious::CyclicImplicitSequenceTestIndexOfPrevious() :
    LeafTest("indexOfPrevious") {
}

void CyclicImplicitSequenceTestIndexOfPrevious::test() {
    constexpr int n = 10;

    amt::CyclicImplicitSequence<int> seq;

    for (int i = 0; i < n; ++i) {
        seq.emplaceLast().data = i;
    }

    this->assert_equals(static_cast<std::size_t>(4), seq.indexOfPrevious(5));
    this->assert_equals(static_cast<std::size_t>(n - 1), seq.indexOfPrevious(0));
}


ImplicitSequenceTests::ImplicitSequenceTests() :
    CompositeTest("ImplicitSequence")
{
    this->add_test<GenericSequenceTest<amt::ImplicitSequence<int>>>();
    this->add_test<ImplicitSequenceTestIndexOfNext>();
    this->add_test<ImplicitSequenceTestIndexOfPrevious>();
}


CyclicImplicitSequenceTests::CyclicImplicitSequenceTests() :
    CompositeTest("CyclicImplicitSequence")
{
    this->add_test<CyclicImplicitSequenceTestIndexOfNext>();
    this->add_test<CyclicImplicitSequenceTestIndexOfPrevious>();
}


} // namespace ds::tests

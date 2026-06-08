#include "explicit_sequence.test.hpp"

#include <libds/amt/explicit_sequence.hpp>

#include "sequence.test.hpp"

#include <memory>


namespace ds::tests {


SinglyLinkedSequenceTests::SinglyLinkedSequenceTests() :
    CompositeTest("SinglyLinkedSequence")
{
    this->add_test(std::make_unique<
        GenericSequenceTest<amt::SinglyLinkedSequence<int>>>());
}


DoublyLinkedSequenceTests::DoublyLinkedSequenceTests() :
    CompositeTest("DoublyLinkedSequence")
{
    this->add_test(std::make_unique<
        GenericSequenceTest<amt::DoublyLinkedSequence<int>>>());
}


ExplicitSequenceTests::ExplicitSequenceTests() :
    CompositeTest("ExplicitSequence")
{
    this->add_test(std::make_unique<SinglyLinkedSequenceTests>());
    this->add_test(std::make_unique<DoublyLinkedSequenceTests>());
}


} // namespace ds::tests

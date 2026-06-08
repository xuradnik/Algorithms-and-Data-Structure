#include "mm/system_memory_manager.test.hpp"
#include "mm/compact_memory_trivial_manager.test.hpp"
#include "mm/compact_memory_object_manager.test.hpp"
#include "amt/implicit_sequence.test.hpp"
#include "amt/explicit_sequence.test.hpp"
#include "amt/implicit_hierarchy.test.hpp"
#include "amt/explicit_hierarchy.test.hpp"
#include "adt/list.test.hpp"
#include "adt/array.test.hpp"
#include "adt/queue.test.hpp"
#include "adt/stack.test.hpp"
#include "adt/priority_queue.test.hpp"
#include "adt/table.test.hpp"
#include "adt/sort.test.hpp"

#include <libstest/run.hpp>
#include <libstest/test.hpp>


int main(int argc, char *argv[]) {
    using namespace ds::tests;

    stest::CompositeTest tests("Tests");

    tests.add_test<SystemMemoryManagerTest>();
    tests.add_test<TrivialCMMTests>();
    tests.add_test<ObjectCMMTests>();

    tests.add_test<ImplicitSequenceTests>();
    tests.add_test<SinglyLinkedSequenceTests>();
    tests.add_test<DoublyLinkedSequenceTests>();

    tests.add_test<ImplicitHierarchyTest>();
    tests.add_test<BinaryImplicitHierarchyTest>();
    tests.add_test<MultiWayEHTest>();
    tests.add_test<KWayEHTest>();

    tests.add_test<ImplicitListTests>();
    tests.add_test<SinglyLinkedListTests>();
    tests.add_test<DoublyLinkedListTests>();

    tests.add_test<ArrayTests>();
    tests.add_test<CompactMatrixTests>();

    tests.add_test<ImplicitQueueTests>();
    tests.add_test<ExplicitQueueTests>();

    tests.add_test<ImplicitStackTests>();
    tests.add_test<ExplicitStackTests>();

    tests.add_test<UnosortedImplicitSequencePQTests>();
    tests.add_test<UnosortedExplicitSequencePQTests>();
    tests.add_test<SortedImplicitSequencePQTests>();
    tests.add_test<SortedExplicitSequencePQTests>();
    tests.add_test<TwoListsTests>();
    tests.add_test<BinaryHeapTests>();

    tests.add_test<UnsortedImplicitSequenceTableTests>();
    tests.add_test<UnsortedExplicitSequenceTableTests>();
    tests.add_test<SortedSequenceTableTests>();

    tests.add_test<BinarySearchTreeTests>();
    tests.add_test<TreapTests>();
    tests.add_test<HashTableTests>();

    tests.add_test<SelectSortTests>();
    tests.add_test<InsertSortTests>();
    tests.add_test<BubbleSortTests>();
    tests.add_test<QuickSortTests>();
    tests.add_test<HeapSortTests>();
    tests.add_test<ShellSortTests>();
    tests.add_test<RadixSortTests>();
    tests.add_test<MergeSortTests>();

    stest::run(argc, argv, tests);
}

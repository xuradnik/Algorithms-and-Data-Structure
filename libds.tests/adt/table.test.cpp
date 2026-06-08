#include "table.test.hpp"

#include <libds/adt/table.hpp>


namespace ds::tests {


template<template<typename, typename> typename TableType>
TableTests<TableType>::TableTests() : stest::CompositeTest("Table") {

this->add_test("insert_empty", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.insert(1, 10);

    s.assert_false(t.isEmpty(), "Table is not empty.");
    s.assert_equals(size_t{1}, t.size(), "Size is 1.");
    s.assert_true(t.contains(1), "Inserted key is present.");
});

this->add_test("insert_multiple", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.insert(2, 20);
    t.insert(1, 10);
    t.insert(3, 30);

    s.assert_equals(size_t{3}, t.size(), "Size is 3.");
    s.assert_true(t.contains(1), "Key 1 is present.");
    s.assert_true(t.contains(2), "Key 2 is present.");
    s.assert_true(t.contains(3), "Key 3 is present.");
});

this->add_test("insert_duplicateKey", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.insert(1, 10);

    s.assert_throws([&t](){ t.insert(1, 20); }, "Throws on duplicate key.");
});

this->add_test("find_empty", [](stest::LeafTest &s){
    TableType<int, int> t;

    s.assert_throws([&t](){ t.find(1); }, "Throws on missing key.");
});

this->add_test("find_presentKey", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.insert(1, 10);
    t.insert(2, 20);

    s.assert_equals(10, t.find(1), "find(1) returns 10.");
    s.assert_equals(20, t.find(2), "find(2) returns 20.");
});

this->add_test("find_missingKey", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.insert(1, 10);

    s.assert_throws([&t](){ t.find(99); }, "Throws when key is not present.");
});

this->add_test("find_mutable", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.insert(1, 10);

    int &val = t.find(1);
    val = 99;

    s.assert_equals(99, t.find(1), "Value is mutated through find reference.");
});

this->add_test("tryFind_empty", [](stest::LeafTest &s){
    TableType<int, int> t;
    int *data = nullptr;

    s.assert_false(t.tryFind(1, data), "Returns false on empty table.");
});

this->add_test("tryFind_presentKey", [](stest::LeafTest &s){
    TableType<int, int> t;
    int *data = nullptr;

    t.insert(42, 100);

    s.assert_true(t.tryFind(42, data), "Returns true when key exists.");
    s.assert_equals(100, *data, "Data pointer points to correct value.");
});

this->add_test("tryFind_missingKey", [](stest::LeafTest &s){
    TableType<int, int> t;
    int *data = nullptr;

    t.insert(1, 10);

    s.assert_false(t.tryFind(99, data), "Returns false when key is absent.");
});

this->add_test("contains_empty", [](stest::LeafTest &s){
    TableType<int, int> t;

    s.assert_false(t.contains(1), "Empty table contains nothing.");
});

this->add_test("contains_presentKey", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.insert(5, 50);

    s.assert_true(t.contains(5), "Table contains inserted key.");
});

this->add_test("contains_missingKey", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.insert(5, 50);

    s.assert_false(t.contains(99), "Table does not contain absent key.");
});

this->add_test("remove_empty", [](stest::LeafTest &s){
    TableType<int, int> t;

    s.assert_throws([&t](){ t.remove(1); }, "Throws on empty table.");
});

this->add_test("remove_presentKey", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.insert(1, 10);
    t.insert(2, 20);

    const int removed = t.remove(1);

    s.assert_equals(10, removed, "Removed value is returned.");
    s.assert_equals(size_t{1}, t.size(), "Size decremented after remove.");
    s.assert_false(t.contains(1), "Removed key is no longer present.");
    s.assert_true(t.contains(2), "Other key is still present.");
});

this->add_test("remove_missingKey", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.insert(1, 10);

    s.assert_throws([&t](){ t.remove(99); }, "Throws when key is absent.");
});

this->add_test("remove_onlyElement", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.insert(7, 70);
    t.remove(7);

    s.assert_true(t.isEmpty(), "Table is empty after removing the only element.");
    s.assert_equals(size_t{0}, t.size(), "Size is 0.");
});

this->add_test("remove_allElements", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.insert(3, 30);
    t.insert(1, 10);
    t.insert(2, 20);

    t.remove(1);
    t.remove(3);
    t.remove(2);

    s.assert_true(t.isEmpty(), "Table is empty after removing all elements.");
    s.assert_equals(size_t{0}, t.size(), "Size is 0.");
});

this->add_test("clear_empty", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.clear();

    s.assert_true(t.isEmpty(), "Cleared empty table is empty.");
    s.assert_equals(size_t{0}, t.size(), "Size is 0.");
});

this->add_test("clear_notEmpty", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.insert(1, 10);
    t.insert(2, 20);
    t.insert(3, 30);
    t.clear();

    s.assert_true(t.isEmpty(), "Cleared table is empty.");
    s.assert_equals(size_t{0}, t.size(), "Size is 0.");
    s.assert_false(t.contains(1), "Cleared table does not contain old keys.");
});

this->add_test("iterators_empty", [](stest::LeafTest &s){
    TableType<int, int> t;

    auto begin = t.begin();
    auto end = t.end();

    s.assert_true(begin == end, "begin == end for empty table.");
});

this->add_test("iterators_notEmpty", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.insert(1, 10);
    t.insert(2, 20);
    t.insert(3, 30);

    size_t count = 0;
    for (auto it = t.begin(); it != t.end(); ++it) {
        ++count;
    }

    s.assert_equals(size_t{3}, count, "Iterator visits all 3 elements.");
});

this->add_test("iterators_keysPresent", [](stest::LeafTest &s){
    TableType<int, int> t;

    t.insert(10, 100);
    t.insert(20, 200);

    bool found10 = false;
    bool found20 = false;
    for (auto it = t.begin(); it != t.end(); ++it) {
        if ((*it).key == 10) found10 = true;
        if ((*it).key == 20) found20 = true;
    }

    s.assert_true(found10, "Iterator visits key 10.");
    s.assert_true(found20, "Iterator visits key 20.");
});

this->add_test("assign_emptyBoth", [](stest::LeafTest &s){
    TableType<int, int> t1;
    TableType<int, int> t2;

    t2.assign(t1);

    s.assert_equals(t1.size(), t2.size(), "Sizes are equal.");
    s.assert_true(t2.isEmpty(), "Assigned table is empty.");
});

this->add_test("assign_emptyThis", [](stest::LeafTest &s){
    TableType<int, int> t1;

    t1.insert(1, 10);
    t1.insert(2, 20);

    TableType<int, int> t2;
    t2.assign(t1);

    s.assert_equals(t1.size(), t2.size(), "Sizes are equal after assign.");
    s.assert_true(t2.contains(1), "Assigned table contains key 1.");
    s.assert_true(t2.contains(2), "Assigned table contains key 2.");

    t1.find(1) = 999;
    int *data = nullptr;
    t2.tryFind(1, data);
    s.assert_equals(10, *data, "Assign does not share memory.");
});

this->add_test("assign_notEmptyBoth", [](stest::LeafTest &s){
    TableType<int, int> t1;

    t1.insert(1, 10);
    t1.insert(2, 20);

    TableType<int, int> t2;

    t2.insert(3, 30);
    t2.insert(4, 40);
    t2.insert(5, 50);

    t1.assign(t2);

    s.assert_equals(t2.size(), t1.size(), "Sizes are equal after assign.");
    s.assert_true(t1.contains(3), "Assigned table contains key 3.");
    s.assert_true(t1.contains(4), "Assigned table contains key 4.");
    s.assert_false(t1.contains(1), "Old key 1 is gone after assign.");
});

this->add_test("equals_bothEmpty", [](stest::LeafTest &s){
    TableType<int, int> t1;
    TableType<int, int> t2;

    s.assert_true(t1.equals(t2), "Two empty tables are equal.");
});

this->add_test("equals_equal", [](stest::LeafTest &s){
    TableType<int, int> t1;

    t1.insert(1, 10);
    t1.insert(2, 20);
    t1.insert(3, 30);

    TableType<int, int> t2;

    t2.insert(1, 10);
    t2.insert(2, 20);
    t2.insert(3, 30);

    s.assert_true(t1.equals(t2), "Tables with same key-value pairs are equal.");
});

this->add_test("equals_differentValues", [](stest::LeafTest &s){
    TableType<int, int> t1;

    t1.insert(1, 10);
    t1.insert(2, 20);

    TableType<int, int> t2;

    t2.insert(1, 10);
    t2.insert(2, 99);

    s.assert_false(t1.equals(t2), "Tables with different values are not equal.");
});

this->add_test("equals_differentSizes", [](stest::LeafTest &s){
    TableType<int, int> t1;

    t1.insert(1, 10);
    t1.insert(2, 20);

    TableType<int, int> t2;

    t2.insert(1, 10);

    s.assert_false(t1.equals(t2), "Tables of different sizes are not equal.");
});

this->add_test("equals_differentKeys", [](stest::LeafTest &s){
    TableType<int, int> t1;

    t1.insert(1, 10);

    TableType<int, int> t2;

    t2.insert(2, 10);

    s.assert_false(t1.equals(t2), "Tables with different keys are not equal.");
});

} // TableTests::TableTests


UnsortedImplicitSequenceTableTests::UnsortedImplicitSequenceTableTests() :
    stest::CompositeTest("UnsortedImplicitSequenceTable")
{

this->add_test<TableTests<adt::UnsortedImplicitSequenceTable>>();

} // UnsortedImplicitSequenceTableTests::UnsortedImplicitSequenceTableTests


UnsortedExplicitSequenceTableTests::UnsortedExplicitSequenceTableTests() :
    stest::CompositeTest("UnsortedExplicitSequenceTable")
{

this->add_test<TableTests<adt::UnsortedExplicitSequenceTable>>();

} // UnsortedExplicitSequenceTableTests::UnsortedExplicitSequenceTableTests


SortedSequenceTableTests::SortedSequenceTableTests() :
    stest::CompositeTest("SortedSequenceTable")
{

this->add_test<TableTests<adt::SortedSequenceTable>>();

} // SortedSequenceTableTests::SortedSequenceTableTests


BinarySearchTreeTests::BinarySearchTreeTests() :
    stest::CompositeTest("BinarySearchTree")
{

this->add_test<TableTests<adt::BinarySearchTree>>();

} // BinarySearchTreeTests::BinarySearchTreeTests


TreapTests::TreapTests() : stest::CompositeTest("Treap")
{

this->add_test<TableTests<adt::Treap>>();

} // TreapTests::TreapTests


HashTableTests::HashTableTests() : stest::CompositeTest("HashTable")
{

this->add_test<TableTests<adt::HashTable>>();

const auto hasher = [](const int &key) {
    return static_cast<std::size_t>(key % 3);
};

const std::size_t CAPACITY = 3;

this->add_test("collision_insertSameBucket", [&hasher](stest::LeafTest &s){
    adt::HashTable<int, int> t(hasher, CAPACITY);

    t.insert(0, 100);
    t.insert(3, 300);
    t.insert(6, 600);

    s.assert_equals(size_t{3}, t.size(), "Size is 3 after 3 colliding inserts.");
    s.assert_equals(100, t.find(0), "find(0) returns 100.");
    s.assert_equals(300, t.find(3), "find(3) returns 300.");
    s.assert_equals(600, t.find(6), "find(6) returns 600.");
});

this->add_test("collision_duplicateKeyThrows", [&hasher](stest::LeafTest &s){
    adt::HashTable<int, int> t(hasher, CAPACITY);

    t.insert(0, 100);
    t.insert(3, 300);

    s.assert_throws([&t](){ t.insert(3, 999); },
        "Duplicate key in same bucket throws.");
});


this->add_test("collision_findNonFirst", [&hasher](stest::LeafTest &s){
    adt::HashTable<int, int> t(hasher, CAPACITY);

    t.insert(1, 10);
    t.insert(4, 40);
    t.insert(7, 70);

    int *data = nullptr;
    s.assert_true(t.tryFind(7, data), "tryFind returns true for last-inserted colliding key.");
    s.assert_equals(70, *data, "Correct value retrieved from synonym chain.");
});

this->add_test("collision_tryFindMissingInBucket", [&hasher](stest::LeafTest &s){
    adt::HashTable<int, int> t(hasher, CAPACITY);

    t.insert(2, 20);
    t.insert(5, 50);

    int *data = nullptr;
    s.assert_false(t.tryFind(8, data),
        "tryFind returns false for absent key in non-empty bucket.");
});

this->add_test("collision_removeMiddle", [&hasher](stest::LeafTest &s){
    adt::HashTable<int, int> t(hasher, CAPACITY);

    t.insert(0, 100);
    t.insert(3, 300);
    t.insert(6, 600);

    const int removed = t.remove(3);

    s.assert_equals(300, removed, "Correct value returned when removing from chain.");
    s.assert_equals(size_t{2}, t.size(), "Size decremented after synonym remove.");
    s.assert_false(t.contains(3), "Removed key is gone.");
    s.assert_true(t.contains(0), "Remaining key 0 still present.");
    s.assert_true(t.contains(6), "Remaining key 6 still present.");
});

this->add_test("collision_removeAllInBucket_thenReinsert", [&hasher](stest::LeafTest &s){
    adt::HashTable<int, int> t(hasher, CAPACITY);

    t.insert(1, 10);
    t.insert(4, 40);

    t.remove(1);
    t.remove(4);

    s.assert_true(t.isEmpty(), "Table empty after removing all from bucket.");

    t.insert(1, 11);
    s.assert_equals(11, t.find(1), "Reinserted key accessible after bucket cleared.");
});

this->add_test("collision_allBucketsPopulated", [&hasher](stest::LeafTest &s){
    adt::HashTable<int, int> t(hasher, CAPACITY);

    t.insert(0, 100); t.insert(3, 300);
    t.insert(1, 10);  t.insert(4, 40);
    t.insert(2, 20);  t.insert(5, 50);

    s.assert_equals(size_t{6}, t.size(), "All 6 elements inserted across buckets.");

    s.assert_equals(100, t.find(0), "Bucket 0: key 0.");
    s.assert_equals(300, t.find(3), "Bucket 0: key 3 (collision).");
    s.assert_equals(10,  t.find(1), "Bucket 1: key 1.");
    s.assert_equals(40,  t.find(4), "Bucket 1: key 4 (collision).");
    s.assert_equals(20,  t.find(2), "Bucket 2: key 2.");
    s.assert_equals(50,  t.find(5), "Bucket 2: key 5 (collision).");
});

this->add_test("collision_iteratorsVisitAll", [&hasher](stest::LeafTest &s){
    adt::HashTable<int, int> t(hasher, CAPACITY);

    t.insert(0, 100);
    t.insert(3, 300);
    t.insert(1, 10);
    t.insert(4, 40);
    t.insert(2, 20);

    size_t count = 0;
    int sum = 0;
    for (auto it = t.begin(); it != t.end(); ++it) {
        ++count;
        sum += (*it).data;
    }

    s.assert_equals(size_t{5}, count, "Iterator visits all 5 elements.");
    s.assert_equals(470, sum, "Sum of all values matches expected (100+300+10+40+20).");
});

this->add_test("collision_clear", [&hasher](stest::LeafTest &s){
    adt::HashTable<int, int> t(hasher, CAPACITY);

    t.insert(0, 100);
    t.insert(3, 300);
    t.insert(6, 600);
    t.clear();

    s.assert_true(t.isEmpty(), "Table is empty after clear.");
    s.assert_false(t.contains(0), "Key 0 gone after clear.");
    s.assert_false(t.contains(3), "Key 3 gone after clear.");
    s.assert_false(t.contains(6), "Key 6 gone after clear.");

    t.insert(0, 1);
    s.assert_true(t.contains(0), "Bucket 0 usable again after clear.");
});

} // HashTable::HashTable


} // namespace ds::tests

#ifndef LIBDS_TESTS_AMT_HIERARCHY_TEST_HPP
#define LIBDS_TESTS_AMT_HIERARCHY_TEST_HPP

#include <libstest/test.hpp>

#include <vector>


namespace ds::tests {


template<class FixtureType>
class HierarchyTest : public stest::CompositeTest {
public:
    HierarchyTest();
};


template<class FixtureType>
class BinaryHierarchyTest : public stest::CompositeTest {
public:
    BinaryHierarchyTest();
};


template<class FixtureType>
HierarchyTest<FixtureType>::HierarchyTest() :
    stest::CompositeTest("Hierarchy")
{

using HierarchyType = typename FixtureType::HierarchyType;

this->add_test("isEmpty_empty", [](stest::LeafTest &s){
    HierarchyType h;
    s.assert_true(h.isEmpty(), "Hierarchy is empty.");
});

this->add_test("isEmpty_notEmpty", [](stest::LeafTest &s){
    FixtureType fixture;
    HierarchyType &h = fixture.hierarchy;
    s.assert_false(h.isEmpty(), "Hierarchy is not empty.");
});

this->add_test("size_empty", [](stest::LeafTest &s){
    HierarchyType h;
    s.assert_equals(size_t{0}, h.size(), "Size is 0.");
});

this->add_test("size_notEmpty", [](stest::LeafTest &s){
    FixtureType fixture;
    HierarchyType &h = fixture.hierarchy;
    s.assert_equals(fixture.size, h.size(), "Size is 7.");
});

this->add_test("clear_empty", [](stest::LeafTest &s){
    HierarchyType h;
    h.clear();
    s.assert_true(h.isEmpty(), "Hierarchy is empty.");
    s.assert_equals(size_t{0}, h.size(), "Size is 0.");
    s.assert_null(h.accessRoot(), "Root is nullptr.");
});

this->add_test("clear_notEmpty", [](stest::LeafTest &s){
    FixtureType fixture;
    HierarchyType &h = fixture.hierarchy;
    h.clear();
    s.assert_true(h.isEmpty(), "Hierarchy is empty.");
    s.assert_equals(size_t{0}, h.size(), "Size is 0.");
    s.assert_null(h.accessRoot(), "Root is nullptr.");
});

this->add_test("processPreOrder_empty", [](stest::LeafTest &s){
    HierarchyType h;
    int x = 0;
    h.processPreOrder(
        h.accessRoot(),
        [&x, &s](auto *node){
            (void)node;
            ++x;
        });
    s.assert_equals(x, 0, "Function not invoked on empty hierarchy.");
});

this->add_test("processPreOrder_notEmpty", [](stest::LeafTest &s){
    FixtureType fixture;
    HierarchyType &h = fixture.hierarchy;
    const std::vector<int> &order = fixture.preOrder;
    auto it = order.begin();
    h.processPreOrder(
        h.accessRoot(),
        [&it, &s](auto *node){
            s.assert_equals(*it, node->data);
            ++it;
        });
});

this->add_test("processPostOrder_empty", [](stest::LeafTest &s){
    HierarchyType h;
    int x = 0;
    h.processPostOrder(
        h.accessRoot(),
        [&x, &s](auto *node){
            (void)node;
            ++x;
        });
    s.assert_equals(x, 0, "Function not invoked on empty hierarchy.");
});

this->add_test("processPostOrder_notEmpty", [](stest::LeafTest &s){
    FixtureType fixture;
    HierarchyType &h = fixture.hierarchy;
    const std::vector<int> &order = fixture.postOrder;
    auto it = order.begin();
    h.processPostOrder(
        h.accessRoot(),
        [&it, &s](auto *node){
            s.assert_equals(*it, node->data);
            ++it;
        });

});

this->add_test("PreOrderIterator", [](stest::LeafTest &s){
    FixtureType fixture;
    HierarchyType &h = fixture.hierarchy;
    auto gtIt = fixture.preOrder.begin();
    auto hierarchyIt = h.beginPre();
    const int size = static_cast<int>(fixture.size);
    for (int i = 0; i < size; ++i) {
        s.assert_equals(*gtIt, *hierarchyIt);
        ++gtIt;
        ++hierarchyIt;
    }
    s.assert_equals(h.endPre(), hierarchyIt, "Iterator is at the end.");
});

this->add_test("PostOrderIterator", [](stest::LeafTest &s){
    FixtureType fixture;
    HierarchyType &h = fixture.hierarchy;
    auto gtIt = fixture.postOrder.begin();
    auto hierarchyIt = h.beginPost();
    const int size = static_cast<int>(fixture.size);
    for (int i = 0; i < size; ++i) {
        s.assert_equals(*gtIt, *hierarchyIt);
        ++gtIt;
        ++hierarchyIt;
    }
    s.assert_equals(h.endPost(), hierarchyIt, "Iterator is at the end.");
});

this->add_test("begin", [](stest::LeafTest &s){
    HierarchyType h;
    s.assert_equals(h.begin(), h.end(), "Empty hierarchy end.");
});

} // HierarchyTest<FixtureType>::HierarchyTest


template<class FixtureType>
BinaryHierarchyTest<FixtureType>::BinaryHierarchyTest() :
    stest::CompositeTest("BinaryHierarchy")
{

using HierarchyType = typename FixtureType::HierarchyType;

this->add_test("processInOrder_empty", [](stest::LeafTest &s){
    HierarchyType h;
    int x = 0;
    h.processInOrder(
        h.accessRoot(),
        [&x, &s](auto *node){
            (void)node;
            ++x;
        });
    s.assert_equals(x, 0, "Function not invoked on empty hierarchy.");
});

this->add_test("processInOrder_notEmpty", [](stest::LeafTest &s){
    FixtureType fixture;
    HierarchyType &h = fixture.hierarchy;
    const std::vector<int> &order = fixture.inOrder;
    auto it = order.begin();
    h.processInOrder(
        h.accessRoot(),
        [&it, &s](auto *node){
            s.assert_equals(*it, node->data);
            ++it;
        });

});

this->add_test("InOrderIterator", [](stest::LeafTest &s){
    FixtureType fixture;
    HierarchyType &h = fixture.hierarchy;
    auto gtIt = fixture.inOrder.begin();
    auto hierarchyIt = h.begin();
    const int size = static_cast<int>(fixture.size);
    for (int i = 0; i < size; ++i) {
        s.assert_equals(*gtIt, *hierarchyIt);
        ++gtIt;
        ++hierarchyIt;
    }
    s.assert_equals(h.end(), hierarchyIt, "Iterator is at the end.");
});

} // BinaryHierarchyTest<FixtureType>::BinaryHierarchyTest


namespace details {


template<class Hierarchy>
struct HierarchyFixture {
    using HierarchyType = Hierarchy;
    Hierarchy hierarchy;
    std::size_t size;
    std::vector<int> preOrder;
    std::vector<int> postOrder;
    std::vector<int> levelOrder;
    std::vector<int> inOrder;
};


} // namespace details


} // namespace ds::tests

#endif

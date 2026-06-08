#include "explicit_hierarchy.test.hpp"

#include "hierarchy.test.hpp"

#include <libds/amt/explicit_hierarchy.hpp>


namespace ds::tests {


struct MWEHFixture :
    details::HierarchyFixture<ds::amt::MultiWayExplicitHierarchy<int>> {
    MWEHFixture() {
        using NodeType = HierarchyType::BlockType;
        NodeType &root = this->hierarchy.emplaceRoot();
        NodeType &one = this->hierarchy.emplaceSon(root, 0);
        NodeType &two = this->hierarchy.emplaceSon(root, 1);
        root.data = 0;
        one.data = 1;
        two.data = 2;
        this->hierarchy.emplaceSon(one, 0).data = 3;
        this->hierarchy.emplaceSon(one, 1).data = 4;
        this->hierarchy.emplaceSon(one, 2).data = 5;
        this->hierarchy.emplaceSon(two, 0).data = 6;
        this->size = 7;
        this->preOrder   = {0, 1, 3, 4, 5, 2, 6};
        this->postOrder  = {3, 4, 5, 1, 6, 2, 0};
        this->levelOrder = {0, 1, 2, 3, 4, 5, 6};
        /**
         *         0
         *    /         \
         *    1         2
         *  / | \       |
         *  3 4 5       6
         */
    }
};

struct KWEHFixture :
    details::HierarchyFixture<ds::amt::KWayExplicitHierarchy<int, 3>> {
    KWEHFixture() {
        using NodeType = HierarchyType::BlockType;
        NodeType &root = this->hierarchy.emplaceRoot();
        NodeType &one = this->hierarchy.emplaceSon(root, 0);
        NodeType &two = this->hierarchy.emplaceSon(root, 2);
        root.data = 0;
        one.data = 1;
        two.data = 2;
        this->hierarchy.emplaceSon(one, 0).data = 3;
        this->hierarchy.emplaceSon(one, 2).data = 4;
        this->hierarchy.emplaceSon(two, 1).data = 5;
        this->size = 6;
        this->preOrder = {0, 1, 3, 4, 2, 5};
        this->postOrder = {3, 4, 1, 5, 2, 0};
        this->levelOrder = {0, 1, 2, 3, 4, 5};
        /**
         *         0
         *    /    |    \
         *    1    -    2
         *  / | \     / | \
         *  3 - 4     - 5 -
         */
    }
};


template<typename FixtureType>
class ExplicitHierarchyTest : public stest::CompositeTest {
public:
    ExplicitHierarchyTest();
};


template<typename FixtureType>
ExplicitHierarchyTest<FixtureType>::ExplicitHierarchyTest() :
    stest::CompositeTest("ExplicitHierarchy")
{

using HierarchyType = typename FixtureType::HierarchyType;
using NodeType = typename HierarchyType::BlockType;

this->add_test("accessRoot_empty", [](stest::LeafTest &s){
    HierarchyType h;
    s.assert_null(h.accessRoot(), "Root is nullptr.");
});

this->add_test("accessRoot_notEmpty", [](stest::LeafTest &s){
    HierarchyType h;
    h.emplaceRoot().data = 0;
    s.assert_not_null(h.accessRoot(), "Root is not nullptr.");
    s.assert_equals(0, h.accessRoot()->data, "Root contains 0.");
});

this->add_test("accessParent_root", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType *root = &h.emplaceRoot();
    NodeType *parent = h.accessParent(*root);
    s.assert_null(parent, "Root has no parent.");
});

this->add_test("accessParent_notRoot", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType *root = &h.emplaceRoot();
    NodeType *son = &h.emplaceSon(*root, 0);
    NodeType *parent = h.accessParent(*son);
    s.assert_not_null(parent, "Son of root has parent.");
    s.assert_equals(root, parent, "The parent is the root.");
});

this->add_test("emplaceRoot_empty", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType *rootPtr = h.accessRoot();
    s.assert_false(h.isEmpty(), "Hierarchy is not empty.");
    s.assert_equals(size_t{1}, h.size(), "Size is 1.");
    s.assert_not_null(rootPtr, "Root is not nullptr.");
    s.assert_equals(&root, rootPtr, "Acces root is the root.");
});

this->add_test("changeRoot", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    h.changeRoot(nullptr);
    s.assert_null(h.accessRoot(), "Root is temporarily nullptr.");
    h.changeRoot(&root);
    s.assert_equals(&root, h.accessRoot(), "Root is back.");
});

} // ExplicitHierarchyTest<BlockType>::ExplicitHierarchyTest


MultiWayEHTest::MultiWayEHTest() :
    stest::CompositeTest("MultiWayExplicitHierarchy")
{

using FixtureType = MWEHFixture;
using HierarchyType = typename FixtureType::HierarchyType;
using NodeType = typename HierarchyType::BlockType;

this->add_test<ExplicitHierarchyTest<FixtureType>>();

this->add_test("emplaceSon_leaf", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType &son = h.emplaceSon(root, 0);
    s.assert_equals(h.degree(root), size_t{1}, "Degree is 1.");
    s.assert_equals(h.accessSon(root, 0), &son, "Accesible at index 0.");
    s.assert_equals(
        h.accessParent(son),
        &root,
        "Parent pointer is correct.");
});

this->add_test("emplaceSon_last", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType &first = h.emplaceSon(root, 0);
    NodeType &second = h.emplaceSon(root, 1);
    s.assert_equals(h.degree(root), size_t{2}, "Degree is 2.");
    s.assert_equals(h.accessSon(root, 1), &second, "Accesible at index 1.");
    s.assert_equals(
        h.accessParent(first),
        &root,
        "Parent pointer is correct for first.");
    s.assert_equals(
        h.accessParent(second),
        &root,
        "Parent pointer is correct for second.");
});

this->add_test("emplaceSon_middle", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType &first = h.emplaceSon(root, 0);
    NodeType &second = h.emplaceSon(root, 1);
    NodeType &third = h.emplaceSon(root, 2);
    NodeType &newSecond = h.emplaceSon(root, 1);
    s.assert_equals(h.degree(root), size_t{4}, "Degree is 4.");
    s.assert_equals(h.accessSon(root, 0), &first, "First is correct.");
    s.assert_equals(h.accessSon(root, 1), &newSecond, "Nodes moved, second.");
    s.assert_equals(h.accessSon(root, 2), &second, "Nodes moved, third.");
    s.assert_equals(h.accessSon(root, 3), &third, "Nodes moved, fourth.");
    s.assert_equals(
        h.accessParent(first),
        &root,
        "Parent pointer is correct for first.");
    s.assert_equals(
        h.accessParent(second),
        &root,
        "Parent pointer is correct for second.");
    s.assert_equals(
        h.accessParent(third),
        &root,
        "Parent pointer is correct for third.");
    s.assert_equals(
        h.accessParent(newSecond),
        &root,
        "Parent pointer is correct for newSecond.");
});

this->add_test("degree_leaf", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    s.assert_equals(h.degree(root), size_t{0}, "Leaf degree is 0.");
});

this->add_test("degree_internal", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    (void)h.emplaceSon(root, 0);
    (void)h.emplaceSon(root, 1);
    (void)h.emplaceSon(root, 2);
    s.assert_equals(h.degree(root), size_t{3}, "Internal degree is 3.");
});

this->add_test("accessSon_leaf", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType *son = h.accessSon(root, 0);
    s.assert_null(son, "Leaf has no son, nullptr is returned.");
});

this->add_test("accessSon_internalPastDegree", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    (void)h.emplaceSon(root, 0);
    (void)h.emplaceSon(root, 1);
    (void)h.emplaceSon(root, 2);
    NodeType *son = h.accessSon(root, 3);
    s.assert_null(son, "No son at index >= degree, nullptr is returned.");
});

this->add_test("accessSon_internalWithinDegree", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType &first = h.emplaceSon(root, 0);
    NodeType &second = h.emplaceSon(root, 1);
    NodeType &third = h.emplaceSon(root, 2);
    s.assert_equals(h.accessSon(root, 0), &first, "First is correct.");
    s.assert_equals(h.accessSon(root, 1), &second, "Second is correct.");
    s.assert_equals(h.accessSon(root, 2), &third, "Third is correct.");
});

this->add_test("changeSon", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType &first = h.emplaceSon(root, 0);
    NodeType &second = h.emplaceSon(root, 1);
    NodeType &grandFirst = h.emplaceSon(first, 0);
    h.changeSon(root, 1, &grandFirst);
    h.changeSon(first, 0, &second);
    s.assert_equals(h.accessSon(root, 1), &grandFirst, "Sons are swapped.");
    s.assert_equals(h.accessSon(first, 0), &second, "Sons are swapped.");
    s.assert_equals(h.size(), size_t{4}, "Size did not change.");
});

this->add_test("removeSon", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType &first = h.emplaceSon(root, 0);
    NodeType &second = h.emplaceSon(root, 1);
    NodeType &grandFirst = h.emplaceSon(first, 0);
    h.emplaceSon(grandFirst, 0);
    h.removeSon(root, 0);
    s.assert_equals(h.size(), size_t{2}, "Nodes are deleted recursively.");
    s.assert_equals(h.accessSon(root, 0), &second, "Nodes moved, no gap.");
});

this->add_test<HierarchyTest<FixtureType>>();

} // MultiWayEHTest::MultiWayEHTest


KWayEHTest::KWayEHTest() :
    stest::CompositeTest("KWayExplicitHierarchy")
{

using FixtureType = KWEHFixture;
using HierarchyType = typename FixtureType::HierarchyType;
using NodeType = typename HierarchyType::BlockType;

this->add_test<ExplicitHierarchyTest<KWEHFixture>>();

this->add_test("emplaceSon_leaf", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType &son = h.emplaceSon(root, 0);
    s.assert_equals(h.degree(root), size_t{1}, "Degree is 1.");
    s.assert_equals(h.accessSon(root, 0), &son, "Accesible at index 0.");
    s.assert_equals(
        h.accessParent(son),
        &root,
        "Parent pointer is correct.");
});

this->add_test("emplaceSon_last", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType &son = h.emplaceSon(root, 2);
    s.assert_equals(h.degree(root), size_t{1}, "Degree is 1.");
    s.assert_equals(h.accessSon(root, 2), &son, "Accesible at index 2.");
    s.assert_equals(
        h.accessParent(son),
        &root,
        "Parent pointer is correct.");
});

this->add_test("emplaceSon_middle", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType &second = h.emplaceSon(root, 1);
    NodeType &first = h.emplaceSon(root, 0);
    NodeType &third = h.emplaceSon(root, 2);
    s.assert_equals(h.degree(root), size_t{3}, "Degree is 3.");
    s.assert_equals(h.accessSon(root, 0), &first, "First is correct.");
    s.assert_equals(h.accessSon(root, 1), &second, "Nodes moved, third.");
    s.assert_equals(h.accessSon(root, 2), &third, "Nodes moved, fourth.");
    s.assert_equals(
        h.accessParent(first),
        &root,
        "Parent pointer is correct for first.");
    s.assert_equals(
        h.accessParent(second),
        &root,
        "Parent pointer is correct for second.");
    s.assert_equals(
        h.accessParent(third),
        &root,
        "Parent pointer is correct for third.");
});

this->add_test("degree_leaf", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    s.assert_equals(h.degree(root), size_t{0}, "Leaf degree is 0.");
});

this->add_test("degree_internal", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    (void)h.emplaceSon(root, 0);
    (void)h.emplaceSon(root, 2);
    s.assert_equals(h.degree(root), size_t{2}, "Internal degree is 2.");
});

this->add_test("accessSon_leaf", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType *first = h.accessSon(root, 0);
    NodeType *second = h.accessSon(root, 1);
    NodeType *third = h.accessSon(root, 2);
    s.assert_null(first, "Leaf has no son, nullptr is returned.");
    s.assert_null(second, "Leaf has no son, nullptr is returned.");
    s.assert_null(third, "Leaf has no son, nullptr is returned.");
});

this->add_test("accessSon_internalPastDegree", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    (void)h.emplaceSon(root, 0);
    (void)h.emplaceSon(root, 1);
    (void)h.emplaceSon(root, 2);
    NodeType *son = h.accessSon(root, 3);
    s.assert_null(son, "No son at index >= degree, nullptr is returned.");
});

this->add_test("accessSon_internalWithinDegree", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType &first = h.emplaceSon(root, 0);
    NodeType &second = h.emplaceSon(root, 1);
    NodeType &third = h.emplaceSon(root, 2);
    s.assert_equals(h.accessSon(root, 0), &first, "First is correct.");
    s.assert_equals(h.accessSon(root, 1), &second, "Second is correct.");
    s.assert_equals(h.accessSon(root, 2), &third, "Third is correct.");
});

this->add_test("changeSon_replaceNullptr", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType &first = h.emplaceSon(root, 0);
    NodeType &third = h.emplaceSon(root, 2);
    h.changeSon(root, 0, nullptr);
    s.assert_null(h.accessParent(first), "Parent pointer is nullptr.");
    h.changeSon(root, 1, &first);
    s.assert_equals(h.degree(root), size_t{2}, "Degree did not change.");
    s.assert_equals(h.size(), size_t{3}, "Size did not change.");
    s.assert_equals(h.accessSon(root, 1), &first, "First moved.");
    s.assert_equals(h.accessSon(root, 2), &third, "Third did not move.");
});

this->add_test("changeSon_replaceSon", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType &first = h.emplaceSon(root, 0);
    NodeType &second = h.emplaceSon(root, 1);
    NodeType &third = h.emplaceSon(root, 2);
    h.changeSon(root, 1, &first);
    h.changeSon(root, 0, &second);
    s.assert_equals(h.degree(root), size_t{3}, "Degree did not change.");
    s.assert_equals(h.size(), size_t{4}, "Size did not change.");
    s.assert_equals(h.accessSon(root, 0), &second, "Sons are swapped.");
    s.assert_equals(h.accessSon(root, 1), &first, "Sons are swapped.");
    s.assert_equals(h.accessSon(root, 2), &third, "Third did not move.");
});

this->add_test("removeSon", [](stest::LeafTest &s){
    HierarchyType h;
    NodeType &root = h.emplaceRoot();
    NodeType &first = h.emplaceSon(root, 0);
    (void)h.emplaceSon(root, 1);
    NodeType &grandFirst = h.emplaceSon(first, 0);
    h.emplaceSon(grandFirst, 0);
    h.removeSon(root, 0);
    s.assert_equals(h.size(), size_t{2}, "Nodes are deleted recursively.");
    s.assert_null(h.accessSon(root, 0), "Nodes did not move. Gap created.");
});

this->add_test<HierarchyTest<FixtureType>>();

} // KWayEHTest::KWayEHTest


} // namespace ds::tests

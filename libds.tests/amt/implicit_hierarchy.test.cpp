#include "implicit_hierarchy.test.hpp"

#include "hierarchy.test.hpp"

#include <libds/amt/implicit_hierarchy.hpp>


namespace ds::tests {


struct IHFixture :
    details::HierarchyFixture<ds::amt::ImplicitHierarchy<int, 3>> {
    IHFixture() : IHFixture(9) {
    }

    IHFixture(const int size) {
        for (int i = 0; i < size; ++i) {
            this->hierarchy.emplaceLastLeaf().data = i;
        }
        this->size = size;
        this->preOrder   = {0, 1, 4, 5, 6, 2, 7, 8, 3};
        this->postOrder  = {4, 5, 6, 1, 7, 8, 2, 3, 0};
        this->levelOrder = {0, 1, 2, 3, 4, 5, 6, 7, 8};
        /*
         *              0
         *      /       |       \
         *      1       2       3
         *    / | \   / |
         *    4 5 6   7 8
         */
    }
};


struct BIHFixture :
    details::HierarchyFixture<ds::amt::BinaryImplicitHierarchy<int>> {
    BIHFixture() {
        this->size = 6;
        this->hierarchy.emplaceLastLeaf().data = 10;
        this->hierarchy.emplaceLastLeaf().data = 5;
        this->hierarchy.emplaceLastLeaf().data = 15;
        this->hierarchy.emplaceLastLeaf().data = 2;
        this->hierarchy.emplaceLastLeaf().data = 7;
        this->hierarchy.emplaceLastLeaf().data = 12;
        this->preOrder   = {10, 5, 2,  7,  15, 12};
        this->postOrder  = {2,  7, 5,  12, 15, 10};
        this->levelOrder = {10, 5, 15, 2,  7,  12};
        this->inOrder    = {2,  5, 7,  10, 12, 15};
        /*         10
         *    /          \
         *    5          15
         *  /   \      /   \
         *  2   7      -   20
         */
    }
};



ImplicitHierarchyTestThrowing::ImplicitHierarchyTestThrowing() :
    LeafTest("unsupported-operations") {
}

auto ImplicitHierarchyTestThrowing::test() -> void {
    amt::ImplicitHierarchy<int, 3> hierarchy;
    this->assert_throws([&hierarchy]() { hierarchy.emplaceRoot(); });

    this->assert_throws([&hierarchy]() { hierarchy.changeRoot(nullptr); });

    this->assert_throws([&hierarchy]() {
        hierarchy.emplaceSon(*hierarchy.accessRoot(), 0);
    });

    this->assert_throws([&hierarchy]() {
        hierarchy.changeSon(*hierarchy.accessRoot(), 0, nullptr);
    });

    this->assert_throws([&hierarchy]() {
        hierarchy.removeSon(*hierarchy.accessRoot(), 0);
    });
}


ImplicitHierarchyTestInsert::ImplicitHierarchyTestInsert() :
    LeafTest("insertLastLeaf") {
}

void ImplicitHierarchyTestInsert::test() {
    const int n = 9;
    amt::ImplicitHierarchy<int, 3> hierarchy;
    for (int i = 0; i < n; ++i) {
        hierarchy.emplaceLastLeaf().data = i;
        this->assert_not_null(hierarchy.accessLastLeaf());
        this->assert_equals(i, hierarchy.accessLastLeaf()->data);
    }
    /*
     *              0
     *      /       |       \
     *      1       2       3
     *    / | \   / |
     *    4 5 6   7 8
     */

    this->assert_equals(static_cast<size_t>(n), hierarchy.size());
}


ImplicitHierarchyTestAccess::ImplicitHierarchyTestAccess() :
    LeafTest("access") {
}

void ImplicitHierarchyTestAccess::test() {
    using HierarchyType = IHFixture::HierarchyType;
    using BlockType = HierarchyType::BlockType;

    const HierarchyType emptyHierarchy;
    this->assert_null(emptyHierarchy.accessRoot());

    const int n = 9;
    IHFixture fixture(n);
    HierarchyType &hierarchy = fixture.hierarchy;
    /*
     *              0
     *      /       |       \
     *      1       2       3
     *    / | \   / |
     *    4 5 6   7 8
     */

    this->assert_not_null(hierarchy.accessLastLeaf());
    this->assert_equals(n - 1, hierarchy.accessLastLeaf()->data);

    BlockType *root = hierarchy.accessRoot();
    this->assert_not_null(root);
    this->assert_equals(0, root->data);

    BlockType *two = hierarchy.accessSon(*root, 1);
    this->assert_not_null(two);
    this->assert_equals(2, two->data);

    BlockType *eight = hierarchy.accessSon(*two, 1);
    this->assert_not_null(eight);

    BlockType *notnine = hierarchy.accessSon(*two, 2);
    this->assert_null(notnine);

    BlockType *twoagain = hierarchy.accessParent(*eight);
    this->assert_not_null(twoagain);
    this->assert_equals(two, twoagain);
    this->assert_equals(root, hierarchy.accessParent(*twoagain));
}


ImplicitHierarchyTestNodeCount::ImplicitHierarchyTestNodeCount() :
    LeafTest("nodeCount") {
}

void ImplicitHierarchyTestNodeCount::test() {
    using HierarchyType = IHFixture::HierarchyType;

    const int n = 9;
    IHFixture fixture(n);
    HierarchyType &hierarchy = fixture.hierarchy;
    /*
     *              0
     *      /       |       \
     *      1       2       3
     *    / | \   / |
     *    4 5 6   7 8
     */

    this->assert_equals(static_cast<std::size_t>(n), hierarchy.nodeCount());
    this->assert_equals(
        static_cast<std::size_t>(n),
        hierarchy.nodeCount(*hierarchy.accessRoot()));
    this->assert_equals(
        static_cast<std::size_t>(3),
        hierarchy.nodeCount(
            *hierarchy.accessSon(*hierarchy.accessRoot(), 1)));
}


ImplicitHierarchyTestLevel::ImplicitHierarchyTestLevel() :
    LeafTest("level") {
}

void ImplicitHierarchyTestLevel::test() {
    using HierarchyType = IHFixture::HierarchyType;
    using BlockType = HierarchyType::BlockType;

    const int n = 9;
    IHFixture fixture(n);
    HierarchyType &hierarchy = fixture.hierarchy;
    /*
     *              0
     *      /       |       \
     *      1       2       3
     *    / | \   / |
     *    4 5 6   7 8
     */

    BlockType *root = hierarchy.accessRoot();
    BlockType *two = hierarchy.accessSon(*root, 1);
    BlockType *eight = hierarchy.accessSon(*two, 1);

    this->assert_equals(static_cast<std::size_t>(0), hierarchy.level(*root));
    this->assert_equals(static_cast<std::size_t>(1), hierarchy.level(*two));
    this->assert_equals(static_cast<std::size_t>(2), hierarchy.level(*eight));
}


ImplicitHierarchyTestDegree::ImplicitHierarchyTestDegree() :
    LeafTest("degree") {
}

void ImplicitHierarchyTestDegree::test() {
    using HierarchyType = IHFixture::HierarchyType;
    using BlockType = HierarchyType::BlockType;

    const int n = 9;
    IHFixture fixture(n);
    HierarchyType &hierarchy = fixture.hierarchy;
    /*
     *              0
     *      /       |       \
     *      1       2       3
     *    / | \   / |
     *    4 5 6   7 8
     */
    BlockType *root = hierarchy.accessRoot();
    BlockType *two = hierarchy.accessSon(*root, 1);
    BlockType *eight = hierarchy.accessSon(*two, 1);

    this->assert_equals(static_cast<std::size_t>(3), hierarchy.degree(*root));
    this->assert_equals(static_cast<std::size_t>(2), hierarchy.degree(*two));
    this->assert_equals(static_cast<std::size_t>(0), hierarchy.degree(*eight));
}


ImplicitHierarchyTestRemove::ImplicitHierarchyTestRemove() :
    LeafTest("removeLastLeaf") {
}

void ImplicitHierarchyTestRemove::test() {
    using HierarchyType = IHFixture::HierarchyType;

    const int n = 9;
    IHFixture fixture(n);
    HierarchyType &hierarchy = fixture.hierarchy;
    /*
     *              0
     *      /       |       \
     *      1       2       3
     *    / | \   / |
     *    4 5 6   7 8
     *      x x   x x
     */

    for (int i = 0; i < 4; ++i) {
        hierarchy.removeLastLeaf();
        this->assert_equals(n - i - 2, hierarchy.accessLastLeaf()->data);
    }
}


ImplicitHierarchyTestCopyAssign::ImplicitHierarchyTestCopyAssign() :
    LeafTest("copy/assign") {
}

void ImplicitHierarchyTestCopyAssign::test() {
    using HierarchyType = IHFixture::HierarchyType;

    const int n = 9;
    IHFixture fixture(n);
    HierarchyType &hierarchy1 = fixture.hierarchy;

    HierarchyType hierarchy2(hierarchy1);
    this->assert_true(
        hierarchy1.equals(hierarchy2),
        "Copy constructed hierarchy is the same.");
    hierarchy1.removeLastLeaf();
    this->assert_false(
        hierarchy1.equals(hierarchy2),
        "Modified copy is different.");

    HierarchyType hierarchy3;
    hierarchy3.assign(hierarchy1);
    this->assert_true(
        hierarchy1.equals(hierarchy3),
        "Assigned hierarchy is the same.");
    hierarchy1.removeLastLeaf();
    hierarchy1.removeLastLeaf();
    this->assert_false(
        hierarchy1.equals(hierarchy3),
        "Modified assigned hierarchy is different.");
}


ImplicitHierarchyTest::ImplicitHierarchyTest() :
    CompositeTest("ImplicitHierarchy")
{
    this->add_test<ImplicitHierarchyTestThrowing>();
    this->add_test<ImplicitHierarchyTestInsert>();
    this->add_test<ImplicitHierarchyTestAccess>();
    this->add_test<ImplicitHierarchyTestNodeCount>();
    this->add_test<ImplicitHierarchyTestLevel>();
    this->add_test<ImplicitHierarchyTestDegree>();
    this->add_test<ImplicitHierarchyTestRemove>();
    this->add_test<ImplicitHierarchyTestCopyAssign>();

    this->add_test<HierarchyTest<IHFixture>>();
}


BinaryImplicitHierarchyTest::BinaryImplicitHierarchyTest() :
    CompositeTest("BinaryImplicitHierarchy")
{
    this->add_test<BinaryHierarchyTest<BIHFixture>>();
}


} // namespace ds::tests

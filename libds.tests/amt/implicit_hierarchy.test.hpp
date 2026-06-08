#ifndef LIBDS_TESTS_AMT_IMPLICIT_HIERARCHY_TEST_HPP
#define LIBDS_TESTS_AMT_IMPLICIT_HIERARCHY_TEST_HPP


#include <libstest/test.hpp>


namespace ds::tests {


/**
 * @brief Tests unsupported methods that should just throw an exception.
 */
class ImplicitHierarchyTestThrowing : public stest::LeafTest {
public:
    ImplicitHierarchyTestThrowing();

protected:
    void test() override;
};


/**
 * @brief Tests insertion of the last leaf.
 */
class ImplicitHierarchyTestInsert : public stest::LeafTest {
public:
    ImplicitHierarchyTestInsert();

protected:
    void test() override;
};


/**
 * @brief Tests access of root, son, parent.
 */
class ImplicitHierarchyTestAccess : public stest::LeafTest {
public:
    ImplicitHierarchyTestAccess();

protected:
    void test() override;
};


/**
 * @brief Tests method for calculation of node count.
 */
class ImplicitHierarchyTestNodeCount : public stest::LeafTest {
public:
    ImplicitHierarchyTestNodeCount();

protected:
    void test() override;
};


/**
 * @brief Tests method for calculation of level.
 */
class ImplicitHierarchyTestLevel : public stest::LeafTest {
public:
    ImplicitHierarchyTestLevel();

protected:
    void test() override;
};


/**
 * @brief Tests method for calculation of degree.
 */
class ImplicitHierarchyTestDegree : public stest::LeafTest {
public:
    ImplicitHierarchyTestDegree();

protected:
    void test() override;
};


/**
 *  @brief Tests removal of the last leaf.
 */
class ImplicitHierarchyTestRemove : public stest::LeafTest {
public:
    ImplicitHierarchyTestRemove();

protected:
    void test() override;
};


/**
 *  @brief Tests copy constructor and assign.
 */
class ImplicitHierarchyTestCopyAssign : public stest::LeafTest {
public:
    ImplicitHierarchyTestCopyAssign();

protected:
    void test() override;
};


/**
 * @brief All ImplicitHierarchy tests.
 */
class ImplicitHierarchyTest : public stest::CompositeTest {
public:
    ImplicitHierarchyTest();
};


/**
 * @brief All BinaryImplicitHierarchy tests.
 */
class BinaryImplicitHierarchyTest : public stest::CompositeTest {
public:
    BinaryImplicitHierarchyTest();
};


} // namespace ds::tests

#endif

#ifndef LIBDS_TESTS_ADT_ARRAY_TEST_HPP
#define LIBDS_TESTS_ADT_ARRAY_TEST_HPP

#include <libstest/test.hpp>


namespace ds::tests {


class ArrayTests : public stest::CompositeTest {
public:
    ArrayTests();
};


class CompactMatrixTests : public stest::CompositeTest {
public:
    CompactMatrixTests();
};


} // namespace ds::tests

#endif

#ifndef LIBDS_TESTS_AMT_EXPLICIT_HIERARCHY_TEST_HPP
#define LIBDS_TESTS_AMT_EXPLICIT_HIERARCHY_TEST_HPP

#include <libstest/test.hpp>


namespace ds::tests {


class MultiWayEHTest : public stest::CompositeTest {
public:
    MultiWayEHTest();
};

class KWayEHTest : public stest::CompositeTest {
public:
    KWayEHTest();
};

class BinaryEHTest : public stest::CompositeTest {
public:
    BinaryEHTest();
};


} // namespace ds::tests

#endif

#ifndef LIBSTEST_IMPL_LIST_TESTS_HPP
#define LIBSTEST_IMPL_LIST_TESTS_HPP

#include <libstest/impl/visitors.hpp>

#include <string>

namespace stest {


class LeafTest;
class CompositeTest;


class ListTestsVisitor : public IVisitor {
public:
    void visit(LeafTest &leaf);
    void visit(CompositeTest &composite);

private:
    std::string m_prefix;
};


} // namespace stest


#endif


#ifndef LIBSTEST_IMPL_RUN_AND_PRINT_HPP
#define LIBSTEST_IMPL_RUN_AND_PRINT_HPP

#include <libstest/config.hpp>
#include <libstest/impl/visitors.hpp>


namespace stest {


class Test;
class LeafTest;
class CompositeTest;


class RunAndPrintVisitor : public IVisitor {
public:
    RunAndPrintVisitor(const Config &config);

    void visit(LeafTest &leaf);
    void visit(CompositeTest &composite);

private:
    void increaseIndent();
    void increaseIndentNested();
    void decreaseIndent();

private:
    inline static const int INDENT_SIZE = 4;

private:
    const Config *m_config;
    std::string m_prefix;
};


void findRunAndPrint(Test &root, const Config &config);


} // namespace stest


#endif

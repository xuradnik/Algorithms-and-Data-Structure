#include <libstest/impl/list_tests.hpp>

#include <libstest/test.hpp>
#include <libstest/impl/terminal.hpp>


namespace stest {


void ListTestsVisitor::visit(LeafTest &leaf) {
    Terminal::print(m_prefix);
    Terminal::println(leaf.name(), Terminal::Color::Yellow);
}

void ListTestsVisitor::visit(CompositeTest &composite) {
    Terminal::print(m_prefix);
    Terminal::println(composite.name(), Terminal::Color::Yellow);
    m_prefix += composite.name();
    m_prefix += ".";
    for (const std::unique_ptr<Test> &nested : composite.subtests()) {
        nested->accept(*this);
    }
    m_prefix.resize(m_prefix.size() - composite.name().size() - 1);
}


} // namespace stest

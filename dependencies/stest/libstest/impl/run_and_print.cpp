#include <libstest/impl/run_and_print.hpp>

#include <libstest/test.hpp>
#include <libstest/impl/diagnostics.hpp>
#include <libstest/impl/terminal.hpp>
#include <libstest/impl/utils.hpp>
#include <libstest/impl/visitors.hpp>

#include <format>


namespace stest {


class NoSuchTestException : public std::exception {
public:
    NoSuchTestException(const std::string &name) :
        m_message(std::format("Test `{}` not found.", name)) {
    }

    const char *what() const noexcept override{
        return m_message.c_str();
    }

private:
    std::string m_message;
};


static Terminal::Color testResultToColor(const TestResult result) {
    switch (result) {
        case TestResult::Pass:    return Terminal::Color::Green;
        case TestResult::Fail:    return Terminal::Color::Red;
        case TestResult::Partial: return Terminal::Color::Yellow;
        default:                  return Terminal::Color::Default;
    }
}


RunAndPrintVisitor::RunAndPrintVisitor(const Config &config) :
    m_config(&config),
    m_prefix("")
{
    m_prefix.reserve(100);
}

void RunAndPrintVisitor::visit(LeafTest &leaf) {
    saveTestName(leaf.name());

    leaf.run();

    Terminal::println(leaf.name(), testResultToColor(leaf.result()));

    this->increaseIndent();

    if (m_config->getOutputLevel() == Config::OutputLevel::Summary) {
        this->decreaseIndent();
        return;
    }

    for (const stest::TestMessage &r : leaf.output()) {
        Terminal::print(m_prefix);
        switch (r.type) {
            case TestMessageType::Pass:
                Terminal::print("pass", Terminal::Color::Green);
                break;
            case TestMessageType::Fail:
                Terminal::print("fail", Terminal::Color::Red);
                break;
            case TestMessageType::Info:
                Terminal::print("info", Terminal::Color::Blue);
                break;
            default:
                break;
        }
        Terminal::print(" ");
        Terminal::println(r.text);
    }

    this->decreaseIndent();
}

void RunAndPrintVisitor::visit(CompositeTest &composite) {
    Terminal::println(composite.name(), Terminal::Color::Blue);
    for (const std::unique_ptr<Test> &nested : composite.subtests()) {
        Terminal::print(m_prefix);
        Terminal::print("+-> ");
        const bool isLast = &nested == &composite.subtests().back();
        if (isLast) {
            this->increaseIndent();
        } else {
            this->increaseIndentNested();
        }
        nested->accept(*this);
        this->decreaseIndent();
    }
}

void RunAndPrintVisitor::increaseIndent() {
    m_prefix += std::string(INDENT_SIZE, ' ');
}

void RunAndPrintVisitor::increaseIndentNested() {
    m_prefix += "|";
    m_prefix += std::string(INDENT_SIZE - 1, ' ');
}

void RunAndPrintVisitor::decreaseIndent() {
    if (m_prefix.size() >= INDENT_SIZE) {
        m_prefix.resize(m_prefix.size() - INDENT_SIZE);
    }
}


static Test &findTest(Test &root, const std::string &fqName) {
    const std::vector<std::string> names = splitOn(fqName, '.');
    const auto namesEnd = names.end();

    auto namesIt = names.begin();
    Test *current = &root;

    if (current->name() != *namesIt) {
        throw NoSuchTestException(*namesIt + ".");
    }
    ++namesIt;

    while (namesIt != namesEnd) {
        CompositeTest *currentComposite = current->as_composite();
        if (currentComposite == nullptr) {
            throw NoSuchTestException("." + *(namesIt - 1));
        }
        current = currentComposite->subtest(*namesIt);
        if (current == nullptr) {
            throw NoSuchTestException("." + *(namesIt));
        }
        ++namesIt;
    }

    return *current;
}

void findRunAndPrint(Test &root, const Config &config) {
    for (const std::string &fqName : config.getTestsToRun()) {
        Test &test = findTest(root, fqName);
        RunAndPrintVisitor visitor(config);
        test.accept(visitor);
    }
}


} // namespace stest

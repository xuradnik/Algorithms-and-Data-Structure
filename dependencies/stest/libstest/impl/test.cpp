#include <libstest/test.hpp>

#include <algorithm>
#include <exception>
#include <utility>

namespace stest {


// Test:

Test::Test(std::string name) : m_name(std::move(name)) {
}

auto Test::name() const -> std::string_view {
    return m_name;
}

// LeafTest:

namespace {

struct test_failed_exception { };

} // namespace

LeafTest::LeafTest(std::string name, AssertPolicy policy) :
    Test(std::move(name)),
    m_assertPolicy(policy) {
}

auto LeafTest::run() -> void {
    try {
        m_results.clear();
        this->test();
    } catch (test_failed_exception) {
        this->info("Terminated after failed assertion.");
    } catch (const std::exception &e) {
        using namespace std::string_literals;
        this->log_fail("Unhandled exception: "s + e.what());
    } catch (...) {
        this->log_fail("Unhandled exception.");
    }
}

auto LeafTest::result() const -> TestResult {
    auto asserts = std::vector<TestMessage>();
    std::copy_if(
        begin(m_results),
        end(m_results),
        std::back_inserter(asserts),
        [](const TestMessage &m) { return m.type != TestMessageType::Info; }
    );

    const auto is_pass
        = [](const TestMessage &m) { return m.type == TestMessageType::Pass; };

    return m_results.empty() ? TestResult::NotEvaluated
         : std::all_of(begin(asserts), end(asserts), is_pass) ? TestResult::Pass
         : std::none_of(begin(asserts), end(asserts), is_pass)
             ? TestResult::Fail
             : TestResult::Partial;
}

auto LeafTest::reset() -> void {
    m_results.clear();
}

auto LeafTest::output() const -> const std::vector<TestMessage> & {
    return m_results;
}

auto LeafTest::accept(IVisitor &v) -> void {
    v.visit(*this);
}

auto LeafTest::assert_true(const bool b, std::string m) -> void {
    if (b) {
        this->pass(std::move(m));
    } else {
        this->fail(std::move(m));
    }
}

auto LeafTest::assert_false(const bool b, std::string m) -> void {
    this->assert_true(not b, std::move(m));
}

auto LeafTest::assert_null(std::nullptr_t) -> void {
    this->pass("Null literal is indeed null");
}

auto LeafTest::assert_not_null(std::nullptr_t) -> void {
    this->fail("Null literal cannot be not null");
}

auto LeafTest::assert_nullopt(std::nullopt_t) -> void {
    this->pass("Nullopt literal is indeed nullopt");
}

auto LeafTest::assert_has_value(std::nullopt_t) -> void {
    this->fail("Nullopt literal never has value");
}

auto LeafTest::info(std::string m) -> void {
    m_results.emplace_back(TestMessage {TestMessageType::Info, std::move(m)});
}

auto LeafTest::fail(std::string m) -> void {
    this->log_fail(std::move(m));
    if (m_assertPolicy == AssertPolicy::StopAtFirstFail) {
        throw test_failed_exception();
    }
}

auto LeafTest::pass(std::string m) -> void {
    m_results.emplace_back(TestMessage {TestMessageType::Pass, std::move(m)});
}

auto LeafTest::log_fail(std::string m) -> void {
    m_results.emplace_back(TestMessage {TestMessageType::Fail, std::move(m)});
}

auto LeafTest::is_leaf() const -> bool {
    return true;
}

auto LeafTest::is_composite() const -> bool {
    return false;
}

auto LeafTest::as_leaf() -> LeafTest* {
    return this;
}

auto LeafTest::as_leaf() const -> const LeafTest* {
    return this;
}

auto LeafTest::as_composite() -> CompositeTest* {
    return nullptr;
}

auto LeafTest::as_composite() const -> const CompositeTest* {
    return nullptr;
}

// CompositeTest:

CompositeTest::CompositeTest(std::string name) : Test(std::move(name)) {
}

auto CompositeTest::accept(IVisitor &v) -> void {
    v.visit(*this);
}

auto CompositeTest::run() -> void {
    for (auto &t : m_tests) {
        t->run();
    }
}

auto CompositeTest::result() const -> TestResult {
    auto results = std::vector<TestResult>();
    std::transform(
        begin(m_tests),
        end(m_tests),
        std::back_inserter(results),
        [](const std::unique_ptr<Test> &test) { return test->result(); }
    );

    const auto didfail
        = [](const TestResult r) { return r == TestResult::Fail; };
    const auto didpass
        = [](const TestResult r) { return r == TestResult::Pass; };
    const auto didntev
        = [](const TestResult r) { return r == TestResult::NotEvaluated; };

    return std::all_of(begin(results), end(results), didntev)
             ? TestResult::NotEvaluated
         : std::all_of(begin(results), end(results), didfail) ? TestResult::Fail
         : std::all_of(begin(results), end(results), didpass)
             ? TestResult::Pass
             : TestResult::Partial;
}

auto CompositeTest::reset() -> void {
    for (const std::unique_ptr<Test> &subTest : m_tests) {
        subTest->reset();
    }
}

auto CompositeTest::subtests() const
    -> const std::vector<std::unique_ptr<Test>> & {
    return m_tests;
}

auto CompositeTest::subtests() -> std::vector<std::unique_ptr<Test>> & {
    return m_tests;
}

auto CompositeTest::subtest(const std::string &name) -> Test* {
    for (const std::unique_ptr<Test> &t : m_tests) {
        if (t->name() == name) {
            return t.get();
        }
    }
    return nullptr;
}

auto CompositeTest::add_test(std::unique_ptr<Test> t) -> void {
    m_tests.emplace_back(std::move(t));
}

auto CompositeTest::is_leaf() const -> bool {
    return false;
}

auto CompositeTest::is_composite() const -> bool {
    return true;
}

auto CompositeTest::as_leaf() -> LeafTest* {
    return nullptr;
}

auto CompositeTest::as_leaf() const -> const LeafTest* {
    return nullptr;
}

auto CompositeTest::as_composite() -> CompositeTest* {
    return this;
}

auto CompositeTest::as_composite() const -> const CompositeTest* {
    return this;
}


} // namespace stest

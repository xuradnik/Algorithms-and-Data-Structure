#ifndef LIBSTEST_TEST_HPP
#define LIBSTEST_TEST_HPP

#include <libstest/impl/visitors.hpp>

#include <cmath>
#include <cstddef>
#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>


namespace stest {


/**
 * @brief Result of a Test.
 */
enum class TestResult {
    Pass,
    Fail,
    Partial,
    NotEvaluated
};

/**
 * @brief Type of a test message.
 */
enum class TestMessageType {
    Pass,
    Fail,
    Info
};

/**
 * @brief Message logged by a test.
 */
struct TestMessage {
    TestMessageType type;
    std::string text;
};

/**
 * @brief Specifies behavior of failed assertion.
 */
enum class AssertPolicy {
    StopAtFirstFail,
    RunAll
};


class LeafTest;

class CompositeTest;

/**
 * @brief Common base class for tests.
 */
class Test : public IVisitable {
public:
    virtual ~Test() = default;

    /**
     * @brief Runs the test. Implemented by child classes.
     */
    virtual auto run() -> void = 0;

    /**
     * @brief Returns result of the test. Implemented by child classes.
     * @return Result of the test.
     */
    virtual auto result() const -> TestResult = 0;

    /**
     * @brief Returns name of the test.
     * @return Name of the test.
     */
    auto name() const -> std::string_view;

    /**
     * @brief Resets state of the test.
     */
    virtual auto reset() -> void = 0;

    /**
     * @brief Checks if this test is leaf test.
     * @return @c true of the test is LeafTest, @c false otherwise.
     */
    virtual auto is_leaf() const -> bool = 0;

    /**
     * @brief Checks if this test is composite test.
     * @return @c true of the test is CompositeTest, @c false otherwise.
     */
    virtual auto is_composite() const -> bool = 0;

    /**
     * @brief Returns @c this as LeafTest*.
     * @return @c this safely casted to LeafTest*.
     */
    virtual auto as_leaf() -> LeafTest* = 0;

    /**
     * @brief Returns @c this as const LeafTest*.
     * @return @c this safely casted to const LeafTest*.
     */
    virtual auto as_leaf() const -> const LeafTest* = 0;

    /**
     * @brief Returns @c this as CompositeTest*.
     * @return @c this safely casted to CompositeTest*.
     */
    virtual auto as_composite() -> CompositeTest* = 0;

    /**
     * @brief Returns @c this as const CompositeTest*.
     * @return @c this safely casted to const CompositeTest*.
     */
    virtual auto as_composite() const -> const CompositeTest* = 0;

protected:
    /**
     * @brief Initializes the test with @p name .
     * @param name name of the test.
     */
    Test(std::string name);

private:
    std::string m_name;
};


/**
 * @brief Base class for implementation of a single test.
 */
class LeafTest : public Test {
public:
    /**
     * @brief Initializes the test with @p name .
     * @param name name of the test.
     * @param policy specifies behavior after first failed assertion.
     */
    LeafTest(
        std::string name,
        AssertPolicy policy = AssertPolicy::StopAtFirstFail
    );

    /**
     * @brief Runs the test.
     */
    auto run() -> void final;

    /**
     * @brief Returns result of the test.
     * @return Result of the test.
     */
    auto result() const -> TestResult override;

    /**
     * @brief Resets state of the test.
     */
    auto reset() -> void override;

    /**
     * @brief Returns output of the test.
     * @return Vector of messages produced by the test.
     */
    auto output() const -> const std::vector<TestMessage>&;

    /**
     * @brief Implements the visitor design patter.
     * @param visitor visitor.
     */
    auto accept(IVisitor &visitor) -> void override;

    /**
     * @brief Checks if this test is leaf test.
     * @return @c true of the test is LeafTest, @c false otherwise.
     */
    auto is_leaf() const -> bool override;

    /**
     * @brief Checks if this test is composite test.
     * @return @c true of the test is CompositeTest, @c false otherwise.
     */
    auto is_composite() const -> bool override;

    /**
     * @brief Returns @c this as LeafTest*.
     * @return @c this safely casted to LeafTest*.
     */
    auto as_leaf() -> LeafTest* override;

    /**
     * @brief Returns @c this as const LeafTest*.
     * @return @c this safely casted to const LeafTest*.
     */
    auto as_leaf() const -> const LeafTest* override;

    /**
     * @brief Returns @c this as CompositeTest*.
     * @return @c this safely casted to CompositeTest*.
     */
    auto as_composite() -> CompositeTest* override;

    /**
     * @brief Returns @c this as const CompositeTest*.
     * @return @c this safely casted to const CompositeTest*.
     */
    auto as_composite() const -> const CompositeTest* override;

public:
    /**
     * @brief Child classes implements the test in this method.
     */
    virtual auto test() -> void = 0;

    /**
     * @brief Asserts that @p b is true.
     * @param b condition to be checked.
     * @param message message that describes the assertion.
     */
    auto assert_true(bool b, std::string message) -> void;

    /**
     * @brief Asserts that @p b is false.
     * @param b condition to be checked.
     * @param message message that describes the assertion.
     */
    auto assert_false(bool b, std::string message) -> void;

    /**
     * @brief Asserts that @p expected and @p actual are equal.
     *
     *  Tries to print values of compared objects using to_string function,
     *  operator<<, and std::format.
     *
     * @tparam T comparable non floating-point data type.
     * @param expected expected value.
     * @param actual actual value.
     */
    template<class T>
    auto assert_equals(T expected, T actual)
        -> std::enable_if_t<! std::is_floating_point_v<T>>;

    /**
     * @brief Asserts that @p expected and @p actual are equal.
     *
     *  Tries to print values of compared objects using to_string function,
     *  operator<<, and std::format.
     *
     *  @tparam T comparable non floating-point data type.
     * @param expected expected value.
     * @param actual actual value.
     * @param message message that describes the assertion.
     */
    template<class T>
    auto assert_equals(T expected, T actual, std::string message)
        -> std::enable_if_t<! std::is_floating_point_v<T>>;

    /**
     * @brief Asserts that @p expected and @p actual are equal
     *  using precision defined by @p epsilon .
     *
     *  Tries to print values of compared objects using to_string function,
     *  operator<<, and std::format.
     *
     *  @tparam T floating-point data type.
     * @param expected expected value.
     * @param actual actual value.
     * @param epsilon precision used in the comparison.
     */
    template<class T>
    auto assert_equals(T expected, T actual, T epsilon)
        -> std::enable_if_t<std::is_floating_point_v<T>>;


    /**
     * @brief Asserts that @p expected and @p actual are equal
     *  using precision defined by @p epsilon .
     *
     *  Tries to print values of compared objects using to_string function,
     *  operator<<, and std::format.
     *
     *  @tparam T floating-point data type.
     * @param expected expected value.
     * @param actual actual value.
     * @param epsilon precision used in the comparison.
     * @param message message that describes the assertion.
     */
    template<class T>
    auto assert_equals(T expected, T actual, T epsilon, std::string message)
        -> std::enable_if_t<std::is_floating_point_v<T>>;

    /**
     * @brief Asserts that @p expected and @p actual are different.
     *
     *  Tries to print values of compared objects using to_string function,
     *  operator<<, and std::format.
     *
     *  @tparam T comparable non floating-point data type.
     * @param expected expected value.
     * @param actual actual value.
     */
    template<class T>
    auto assert_not_equals(T expected, T actual)
        -> std::enable_if_t<! std::is_floating_point_v<T>>;

    /**
     * @brief Asserts that @p expected and @p actual are different.
     *
     *  Tries to print values of compared objects using to_string function,
     *  operator<<, and std::format.
     *
     *  @tparam T comparable non floating-point data type.
     * @param expected expected value.
     * @param actual actual value.
     * @param message message that describes the assertion.
     */
    template<class T>
    auto assert_not_equals(T expected, T actual, std::string message)
        -> std::enable_if_t<! std::is_floating_point_v<T>>;

    /**
     * @brief Asserts that @p expected and @p actual are different
     *  using precision defined by @p epsilon .
     *
     *  Tries to print values of compared objects using to_string function,
     *  operator<<, and std::format.
     *
     *  @tparam T floating-point data type.
     * @param expected expected value.
     * @param actual actual value.
     * @param epsilon precision used in the comparison.
     */
    template<class T>
    auto assert_not_equals(T expected, T actual, T epsilon)
        -> std::enable_if_t<std::is_floating_point_v<T>>;

    /**
     * @brief Asserts that @p expected and @p actual are different
     *  using precision defined by @p epsilon .
     *
     *  Tries to print values of compared objects using to_string function,
     *  operator<<, and std::format.
     *
     *  @tparam T floating-point data type.
     * @param expected expected value.
     * @param actual actual value.
     * @param epsilon precision used in the comparison.
     * @param message message that describes the assertion.
     */
    template<class T>
    auto assert_not_equals(T expected, T actual, T epsilon, std::string message)
        -> std::enable_if_t<std::is_floating_point_v<T>>;

    /**
     * @brief Asserts that callable object @p f throws something.
     *  @tparam F callable object type.
     * @param f callable object.
     */
    template<class F>
    auto assert_throws(F f) -> std::enable_if_t<std::is_invocable_v<F>>;

    /**
     * @brief Asserts that callable object @p f throws something.
     *  @tparam F callable object type.
     * @param f callable object.
     * @param message message that describes the assertion.
     */
    template<class F>
    auto assert_throws(F f, std::string message)
        -> std::enable_if_t<std::is_invocable_v<F>>;

    /**
     * @brief Asserts that null literal is nullptr which is indeed true.
     */
    auto assert_null(std::nullptr_t) -> void;

    /**
     * @brief Asserts that @p is nullptr.
     * @param p pointer
     */
    template<class T>
    auto assert_null(T *p, std::string message = "") -> void;

    /**
     * @brief Asserts that @p is nullptr.
     * @param p pointer
     */
    template<class T, class D>
    auto assert_null(const std::unique_ptr<T, D> &p, std::string message = "") -> void;

    /**
     * @brief Asserts that @p is nullptr.
     * @param p pointer
     */
    template<class T>
    auto assert_null(const std::shared_ptr<T> &p, std::string message = "") -> void;

    /**
     * @brief Asserts that null literal is not nullptr which is never true.
     */
    auto assert_not_null(std::nullptr_t) -> void;

    /**
     * @brief Asserts that @p p is not nullptr.
     * @param p pointer
     */
    template<class T>
    auto assert_not_null(T *p, std::string message = "") -> void;

    /**
     * @brief Asserts that @p p is not nullptr.
     * @param p pointer
     */
    template<class T, class D>
    auto assert_not_null(const std::unique_ptr<T, D> &p, std::string message = "") -> void;

    /**
     * @brief Asserts that @p p is not nullptr.
     * @param p pointer
     */
    template<class T>
    auto assert_not_null(const std::shared_ptr<T> &p, std::string message = "") -> void;

    /**
     * @brief Asserts that nullopt literal is nullopt which is indeed true.
     */
    auto assert_nullopt(std::nullopt_t) -> void;

    /**
     * @brief Asserts that @p o is nullopt.
     * @param o optional
     */
    template<class T>
    auto assert_nullopt(const std::optional<T> &o, std::string message = "") -> void;

    /**
     * @brief Asserts that nullopt literal has value which is always false.
     */
    auto assert_has_value(std::nullopt_t) -> void;

    /**
     * @brief Asserts that @p o contains value.
     * @param o optional
     */
    template<class T>
    auto assert_has_value(const std::optional<T> &o, std::string message = "") -> void;

    /**
     * @brief Logs informational message.
     * @param message message to be logged.
     */
    auto info(std::string message) -> void;

    /**
     * @brief Fails the test as if @c asert_true(false,message) .
     * @param message message to be logged.
     */
    auto fail(std::string message) -> void;

    /**
     * @brief Logs passed assertion as if @c asert_true(true,message) .
     * @param message message to be logged.
     */
    auto pass(std::string message) -> void;

private:
    /**
     * @brief Logs failed assertion.
     * @param message message to be logged.
     */
    auto log_fail(std::string message) -> void;

private:
    std::vector<TestMessage> m_results;
    AssertPolicy m_assertPolicy;
};


template<typename TestCallable>
class AnonymousLeafTest : public LeafTest {
public:
    AnonymousLeafTest(const std::string &name, TestCallable test);
    void test() override;

private:
    TestCallable m_test;
};


/**
 * @brief Container for tests. Creates hierarchy of tests.
 */
class CompositeTest : public Test {
public:
    /**
     * @brief Initializes the test with @p name .
     * @param name name of the test.
     */
    CompositeTest(std::string name);

    /**
     * @brief Deleted copy constructor.
     *  It is up to the user to create a deep copy if necessary.
     */
    CompositeTest(const CompositeTest &) = delete;

    /**
     * @brief Compiler generated move constructor.
     */
    CompositeTest(CompositeTest &&) = default;

    /**
     * @brief Implements the visitor design patter.
     *  Does NOT proceed with visiting subtests.
     * @param visitor visitor.
     */
    auto accept(IVisitor &visitor) -> void override;

    /**
     *  Runs all substests.
     */
    auto run() -> void final;

    /**
     * @brief Returns result of the test.
     *  Pass if all subtests passed.
     *  Fail if all subtests failed.
     *  Partial if some subtests passed and some failed.
     *  NotEvaluated if there is no output.
     * @return Resuslt of the test.
     */
    auto result() const -> TestResult override;

    /**
     * @brief Resets state of the test.
     */
    auto reset() -> void override;

    /**
     * @brief Returns subtests.
     * @return Vector of subtests.
     */
    auto subtests() const -> const std::vector<std::unique_ptr<Test>>&;

    /**
     * @brief Returns subtests.
     * @return Vector of subtests.
     */
    auto subtests() -> std::vector<std::unique_ptr<Test>>&;

    /**
     * @brief Returns subtest with name @p name.
     * @return Substest with name @p name or @c nullptr if no such test.
     */
    auto subtest(const std::string &name) -> Test*;

    /**
     * @brief Adds new subtest.
     * @param t subtest.
     */
    auto add_test(std::unique_ptr<Test> t) -> void;

    /**
     * @brief Adds new default-constructed test.
     */
    template<typename TestType>
    auto add_test() -> void;

    /**
     * @brief Adds new leaf test name @p name that runs @p test.
     * @param name Name of the leaf test.
     * @param test Callable implementing the test.
     */
    template<typename TestCallable>
    auto add_test(const std::string &name, TestCallable test) -> void;

    /**
     * @brief Checks if this test is leaf test.
     * @return @c true of the test is LeafTest, @c false otherwise.
     */
    auto is_leaf() const -> bool override;

    /**
     * @brief Checks if this test is composite test.
     * @return @c true of the test is CompositeTest, @c false otherwise.
     */
    auto is_composite() const -> bool override;

    /**
     * @brief Returns @c this as LeafTest*.
     * @return @c this safely casted to LeafTest*.
     */
    auto as_leaf() -> LeafTest* override;

    /**
     * @brief Returns @c this as const LeafTest*.
     * @return @c this safely casted to const LeafTest*.
     */
    auto as_leaf() const -> const LeafTest* override;

    /**
     * @brief Returns @c this as CompositeTest*.
     * @return @c this safely casted to CompositeTest*.
     */
    auto as_composite() -> CompositeTest* override;

    /**
     * @brief Returns @c this as const CompositeTest*.
     * @return @c this safely casted to const CompositeTest*.
     */
    auto as_composite() const -> const CompositeTest* override;

private:
    std::vector<std::unique_ptr<Test>> m_tests;
};




template<class T>
auto LeafTest::assert_equals(T expected, T actual)
    -> std::enable_if_t<! std::is_floating_point_v<T>> {
    if constexpr (std::is_arithmetic_v<T>) {
        this->assert_equals(
            expected,
            actual,
            "Expected " + std::to_string(expected) + " got "
                + std::to_string(actual)
        );
    } else if constexpr (std::is_same_v<T, std::string>) {
        this->assert_equals(
            expected,
            actual,
            "Expected " + expected + " got " + actual
        );
    } else {
        this->assert_equals(expected, actual, "Expected == actual");
    }
}

template<class T>
auto LeafTest::assert_equals(T expected, T actual, std::string m)
    -> std::enable_if_t<! std::is_floating_point_v<T>> {
    this->assert_true(expected == actual, std::move(m));
}

template<class T>
auto LeafTest::assert_equals(T expected, T actual, T epsilon)
    -> std::enable_if_t<std::is_floating_point_v<T>> {
    auto ost = std::ostringstream();

    ost.precision(std::numeric_limits<double>::max_digits10);
    ost << "Expected " << expected << " got " << actual << " using presision "
        << epsilon;

    this->assert_equals(expected, actual, epsilon, ost.str());
}

template<class T>
auto LeafTest::assert_equals(T expected, T actual, T epsilon, std::string m)
    -> std::enable_if_t<std::is_floating_point_v<T>> {
    this->assert_true(std::abs(expected - actual) < epsilon, std::move(m));
}

template<class T>
auto LeafTest::assert_not_equals(T expected, T actual)
    -> std::enable_if_t<! std::is_floating_point_v<T>> {
    if constexpr (std::is_arithmetic_v<T>) {
        this->assert_not_equals(
            expected,
            actual,
            "Expected " + std::to_string(expected) + " got "
                + std::to_string(actual)
        );
    } else if constexpr (std::is_same_v<T, std::string>) {
        this->assert_not_equals(
            expected,
            actual,
            "Expected " + expected + " got " + actual
        );
    } else {
        this->assert_not_equals(expected, actual, "Expected/actual match");
    }
}

template<class T>
auto LeafTest::assert_not_equals(T expected, T actual, std::string m)
    -> std::enable_if_t<! std::is_floating_point_v<T>> {
    this->assert_true(expected != actual, std::move(m));
}

template<class T>
auto LeafTest::assert_not_equals(T expected, T actual, T epsilon)
    -> std::enable_if_t<std::is_floating_point_v<T>> {
    auto ost = std::ostringstream();

    ost.precision(std::numeric_limits<double>::max_digits10);
    ost << "Expected " << expected << " and " << actual
        << " to be different using presision " << epsilon;

    this->assert_not_equals(expected, actual, epsilon, ost.str());
}

template<class T>
auto LeafTest::assert_not_equals(T expected, T actual, T epsilon, std::string m)
    -> std::enable_if_t<std::is_floating_point_v<T>> {
    this->assert_true(std::abs(expected - actual) >= epsilon, std::move(m));
}

template<class F>
auto LeafTest::assert_throws(F f) -> std::enable_if_t<std::is_invocable_v<F>> {
    this->assert_throws(f, "Function throws");
}

template<class F>
auto LeafTest::assert_throws(F f, std::string m)
    -> std::enable_if_t<std::is_invocable_v<F>> {
    try {
        std::invoke(f);
        this->fail(std::move(m));
    } catch (...) {
        this->pass(std::move(m));
    }
}

template<class T>
auto LeafTest::assert_null(T *p, std::string message) -> void {
    this->assert_true(
        p == nullptr,
        message.empty()
            ? std::string("Pointer is nullptr")
            : std::move(message));
}

template<class T, class D>
auto LeafTest::assert_null(const std::unique_ptr<T, D> &p, std::string message) -> void {
    this->assert_true(
        p.get() == nullptr,
        message.empty()
            ? std::string("Pointer is nullptr")
            : std::move(message));
}

template<class T>
auto LeafTest::assert_null(const std::shared_ptr<T> &p, std::string message) -> void {
    this->assert_true(
        p.get() == nullptr,
        message.empty()
            ? std::string( "Pointer is nullptr")
            : std::move(message));
}

template<class T>
auto LeafTest::assert_not_null(T *p, std::string message) -> void {
    this->assert_true(
        p != nullptr,
        message.empty()
            ? std::string("Pointer is not nullptr")
            : std::move(message));
}

template<class T, class D>
auto LeafTest::assert_not_null(const std::unique_ptr<T, D> &p, std::string message) -> void {
    this->assert_true(
        p.get() != nullptr,
        message.empty()
            ? std::string("Pointer is not nullptr")
            : std::move(message));
}

template<class T>
auto LeafTest::assert_not_null(const std::shared_ptr<T> &p, std::string message) -> void {
    this->assert_true(
        p.get() != nullptr,
        message.empty()
            ? std::string("Pointer is not nullptr")
            : std::move(message));
}

template<class T>
auto LeafTest::assert_nullopt(const std::optional<T> &o, std::string message) -> void {
    this->assert_true(
        ! o.has_value(),
        message.empty()
            ? std::string( "Optional is nullopt")
            : std::move(message));
}

template<class T>
auto LeafTest::assert_has_value(const std::optional<T> &o, std::string message) -> void {
    this->assert_true(
        o.has_value(),
        message.empty()
            ? std::string( "Optional has value")
            : std::move(message));
}

template<typename TestType>
auto CompositeTest::add_test() -> void {
    this->add_test(std::make_unique<TestType>());
}

template<typename TestCallable>
auto CompositeTest::add_test(
    const std::string &name,
    TestCallable test
) -> void {
    this->add_test(
        std::make_unique<AnonymousLeafTest<TestCallable>>(
            name,
            std::move(test)));
}


template<typename TestCallable>
AnonymousLeafTest<TestCallable>::AnonymousLeafTest(
    const std::string &name,
    TestCallable test
) : LeafTest(name), m_test(std::move(test)) {
}

template<typename TestCallable>
void AnonymousLeafTest<TestCallable>::test() {
    m_test(*this);
}


} // namespace stest


#endif

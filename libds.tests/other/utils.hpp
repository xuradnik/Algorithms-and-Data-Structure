#ifndef LIBDS_TESTS_UTILS_HPP
#define LIBDS_TESTS_UTILS_HPP


namespace ds::tests {


class Dummy {
public:
    Dummy();
    explicit Dummy(int value);
    Dummy(const Dummy &other);
    Dummy(Dummy &&other) noexcept;
    ~Dummy();

    static int get_constructors_called();
    static int get_destructors_called();
    static void reset_counters();

    int get_value() const;
    void set_value(int value);
    bool is_self() const;

private:
    static int sm_constructors_called;
    static int sm_destructors_called;

private:
    Dummy *m_self;
    int m_value;
};


void swap(Dummy &lhs, Dummy &rhs) noexcept;

bool operator==(const Dummy &lhs, const Dummy &rhs);
bool operator!=(const Dummy &lhs, const Dummy &rhs);


} // namespace ds::tests


#endif

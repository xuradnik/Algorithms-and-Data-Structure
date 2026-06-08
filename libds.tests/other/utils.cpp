#include "utils.hpp"


namespace ds::tests {


int Dummy::sm_constructors_called = 0;

int Dummy::sm_destructors_called = 0;


Dummy::Dummy() : m_self{this}, m_value{0} {
    ++sm_constructors_called;
}


Dummy::Dummy(int value) : m_self{this}, m_value{value} {
    ++sm_constructors_called;
}


Dummy::Dummy(const Dummy &other) : m_self{this}, m_value{other.m_value} {
    ++sm_constructors_called;
}


Dummy::Dummy(Dummy &&other) noexcept : m_self{this}, m_value{other.m_value} {
    ++sm_constructors_called;
}


Dummy::~Dummy() {
    ++sm_destructors_called;
}


int Dummy::get_constructors_called() {
    return sm_constructors_called;
}


int Dummy::get_destructors_called() {
    return sm_destructors_called;
}


void Dummy::reset_counters() {
    sm_constructors_called = 0;
    sm_destructors_called = 0;
}


int Dummy::get_value() const {
    return m_value;
}


void Dummy::set_value(int value) {
    m_value = value;
}


bool Dummy::is_self() const {
    return this == m_self;
}


void swap(Dummy &lhs, Dummy &rhs) noexcept {
    const int tmp = lhs.get_value();
    lhs.set_value(rhs.get_value());
    rhs.set_value(tmp);
}



bool operator==(const Dummy &lhs, const Dummy &rhs) {
    return lhs.get_value() == rhs.get_value();
}


bool operator!=(const Dummy &lhs, const Dummy &rhs) {
    return lhs.get_value() != rhs.get_value();
}


} // namespace ds::tests

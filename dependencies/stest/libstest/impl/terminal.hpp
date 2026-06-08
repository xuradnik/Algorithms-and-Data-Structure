#ifndef LIBSTEST_IMPL_TERMINAL_HPP
#define LIBSTEST_IMPL_TERMINAL_HPP

#include <string_view>


namespace stest {


struct Terminal {

    enum class Color {
        Red,
        Green,
        Blue,
        Yellow,
        Default
    };

    static void print(std::string_view msg);
    static void print(std::string_view msg, Color color);
    static void println(std::string_view msg);
    static void println(std::string_view msg, Color color);

};


} // namespace stest

#endif

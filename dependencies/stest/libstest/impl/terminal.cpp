#include <libstest/impl/terminal.hpp>

#include <iostream>


namespace stest {


static std::string_view convert(const Terminal::Color color) {
    switch (color) {
        case Terminal::Color::Red:    return "\x1B[91m";
        case Terminal::Color::Green:  return "\x1B[92m";
        case Terminal::Color::Blue:   return "\x1B[94m";
        case Terminal::Color::Yellow: return "\x1B[93m";
        default:                      return "\x1B[97m";
    }
}

static void set_color(const Terminal::Color color) {
    std::cout << convert(color);
}

static void reset_color() {
    std::cout << "\x1B[0m";
}


void Terminal::print(std::string_view msg) {
    std::cout << msg;
}

void Terminal::print(std::string_view msg, Color color) {
    if (color != Color::Default) {
        set_color(color);
        Terminal::print(msg);
        reset_color();
    } else {
        Terminal::print(msg);
    }

}

void Terminal::println(std::string_view msg) {
    std::cout << msg << "\n";
}

void Terminal::println(std::string_view msg, Color color) {
    if (color != Color::Default) {
        set_color(color);
        Terminal::println(msg);
        reset_color();
    } else {
        Terminal::println(msg);
    }
}


} // namespace stest

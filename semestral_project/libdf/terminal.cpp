/**
 * @file terminal.cpp
 * @brief terminal helpers from terminal.hpp.
 */

#include "terminal.hpp"
#include <iostream>

namespace aus
{
    void Terminal::clear() { std::cout << "\033[0m"; }
    void Terminal::setTextColor(Color c) { std::cout << "\033[" << static_cast<int>(c) << "m"; }
    void Terminal::resetTextColor() { std::cout << "\033[0m"; }
} // namespace aus

#ifndef AUS_TERMINAL
#define AUS_TERMINAL

/**
 * @file terminal.hpp
 * @brief codes for colored terminal output.
 */

#include <iostream>

namespace aus
{
    /** @brief colour codes. */
    enum class Color
    {
        Black   = 30,
        Red     = 31,
        Green   = 32,
        Yellow  = 33,
        Blue    = 34,
        Magenta = 35,
        Cyan    = 36,
        White   = 37
    };

    /** @brief Helper class with static terminal control methods. */
    class Terminal
    {
    public:
        /** @brief Resets all terminal attributes. */
        static void clear();

        /**
         * @brief Sets the text foreground color.
         * @param c Color to apply.
         */
        static void setTextColor(Color c);

        /** @brief Resets the text color to default. */
        static void resetTextColor();
    };

} // namespace aus


#endif

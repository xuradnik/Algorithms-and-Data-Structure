#ifndef AUS_CLI_HPP
#define AUS_CLI_HPP

/**
 * @file cli.hpp
 * @brief Top-level CLI that dispatches user commands.
 *
 * Reads one line from stdin, takes the first word as the command keyword,
 * and forwards the rest to Cli_Command_Logic or Cli_Helper_Printer.
 */

#include <filesystem>
#include <sstream>

#include "../../lib/cli/cli_commands_logic.hpp"
#include "../../lib/cli/cli_helper_printer.hpp"
#include "../../lib/structs/accident.hpp"
#include "../data_handlers/data_frame.hpp"

namespace aus {
    /**
     * @brief Interactive CLI for the accident data explorer.
     *
     * Dispatches typed keywords (help, load, filter, print, ...) to the right helper.
     */
    class CommandLineInterface {
        public:
            /**
             * @brief Builds the CLI and auto loads files from the command line.
             * @param argc Argument count from main().
             * @param args Argument vector from main().
             * @param ostream Output stream for printed text.
             * @param is_user_input_still_running Pointer to the main loop flag.
             * @param data_frame DataFrame storing accident records.
             */
            CommandLineInterface(int argc, char* args[], std::ostream& ostream, bool* is_user_input_still_running, DataFrame<Accident>& data_frame);


            /** @brief Reads one line from stdin and dispatches the command. */
            void handle_user_input();

        private:
            /**
             * @brief Handles the `load` command. Parses the path argument.
             * @param ss Stringstream positioned after `load`.
             */
            void load_data_function(std::stringstream& ss);

            /**
             * @brief Handles the `print` command.
             * @param ss Stringstream positioned after `print`.
             */
            void print_function(std::stringstream& ss);

            /**
             * @brief Handles the `print` command and prints the N accidents.
             * @param ss Stringstream positioned after `print`.
             */
            void print_function_N(std::stringstream& ss) const;

        bool* m_p_is_running;

        Cli_Helper_Printer   m_cli_printer;
        Cli_Command_Logic    m_cli_command_logic;
        std::ostream&        m_os;
        DataFrame<Accident>& m_data_frame;
    };
} // namespace aus

#endif // AUS_CLI_HPP

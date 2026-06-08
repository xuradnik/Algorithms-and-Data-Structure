/**
 * @file main.cpp
 * @brief Entry point of the Road Accident Data Explorer.
 *
 * Sets up UTF-8 console on Windows, builds the DataFrame and the CLI,
 * then runs the input loop until the user types `exit`.
 *
 * @mainpage Road Accident Data Explorer
 *
 * @section overview Overview
 * CLI tool for loading and filtering road accident records from the DTF
 * Road Casualty Statistics CSV dataset.
 *
 * @section usage Quick Start
 * @code
 * ./semestral_project                          # interactive mode
 * ./semestral_project path/to/data.csv         # auto-load on startup
 * @endcode
 *
 * Type `help` inside the prompt to see commands, `flags` for filter flags.
 *
 * @section arch Architecture
 * - Accident / enums (`lib/accident.hpp`): the data record.
 * - DataFrame (`libdf/data_frame.hpp`): generic sequential container.
 * - CSV_Loader (`libdf/csv_loader.hpp`): CSV parser.
 * - Cli_Helper_Printer (`lib/cli_helper_printer.hpp`): output formatting.
 * - Cli_Command_Logic (`lib/cli_commands_logic.hpp`): filtering and queries.
 * - CommandLineInterface (`libdf/cli.hpp`): top-level dispatcher.
 */
#include <filesystem>
#include <iostream>
#include "libdf/cli/cli.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

#include "dependencies/heap/heap/heap-monitor.hpp"
#include "libdf/data_handlers/csv_loader.hpp"
#include "libdf/data_handlers/data_frame.hpp"


#include "lib/structs/accident.hpp"

/**
 * @brief Program entry point. Builds the CLI and runs the input loop.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on clean exit, -1 on fatal error.
 */
int main(int argc, char* argv[])
{

#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif


    bool  is_running   = true;
    bool* p_is_running = &is_running;

    aus::DataFrame<aus::Accident> ac{};
    aus::CommandLineInterface     CLI = aus::CommandLineInterface{argc, argv, std::cout, p_is_running, ac};


    while (is_running)
    {

        try
        {
            CLI.handle_user_input();
        }
        catch (const std::bad_alloc& e)
        {
            std::cerr << "Error with memory allocation: " << e.what() << std::endl;
            return -1;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error while loading data: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Unknown error" << std::endl;
            return -1;
        }
    }


    return 0;
}

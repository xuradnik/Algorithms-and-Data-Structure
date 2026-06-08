/**
 * @file cli.cpp
 * @brief Top-level command dispatcher from cli.hpp.
 *
 * Routes each command keyword to the matching @ref aus::Cli_Command_Logic
 * method. Unknown keywords print "Invalid Input".
 */

#include "cli.hpp"
#include "../terminal.hpp"

namespace aus {
    CommandLineInterface::CommandLineInterface
    (
        const int                  argc,
        char*                      args[],
        std::ostream&              ostream,
        bool*                      is_user_input_still_running,
        DataFrame<Accident>&       data_frame
    ) : m_cli_printer({ostream}),
        m_p_is_running(is_user_input_still_running),
        m_os(ostream),
        m_data_frame(data_frame),
        m_cli_command_logic(argc,args,data_frame,ostream,m_cli_printer) {

        m_cli_command_logic.load_incoming_data_from_arguments();
        Terminal::setTextColor(Color::Yellow);
        this->m_os << "======= USER CLI TERMINAL =======" << std::endl;
        Terminal::resetTextColor();
    };

    void CommandLineInterface::handle_user_input() {
        this->m_os << "> ";

        std::string user_input;
        std::getline
        (
            std::cin,
            user_input
        );

        if (user_input.empty())
            return;

        std::string       command;
        std::stringstream ss(user_input);
        ss >> command;

        if (command == "help")
            this->m_cli_printer.print_help_menu();

        else if (command == "flags")
            this->m_cli_printer.print_filter_flags();

        else if (command == "data_meaning")
            this->m_cli_printer.print_data_meaning();

        else if (command == "exit")
            *m_p_is_running = false;

        else if (command == "load")
            this->load_data_function(ss);

        else if (command == "print")
            this->print_function(ss);

        else if (command == "print_N")
            this->print_function_N(ss);

        else if (command == "filter")
            this->m_cli_command_logic.filter_data_based(ss,false);

        else if (command == "date")
            this->m_cli_command_logic.filter_data_based_date(ss);

        else if (command == "date_table")
            this->m_cli_command_logic.find_accidents_by_date_table(ss);

        else if (command == "save_data")
            this->m_cli_command_logic.save_current_data_to_vector(ss);

        else if (command == "tree")
            this->m_cli_command_logic.tree_navigate(ss);
        else if (command == "change")
            this->m_cli_command_logic.change_doc_state();


        else
            m_os << "Invalid Input" << std::endl;
    }

    void CommandLineInterface::load_data_function(std::stringstream& ss) {
        std::string path;

        if (!(ss >> path))
            this->m_os << "Error load command needs argument" << std::endl;

        else
            this->m_cli_command_logic.load_incoming_data(path,true);
    }

    void CommandLineInterface::print_function(std::stringstream& ss) {
        auto pos = ss.tellg();
        std::string argument;
        if (ss >> argument)
        {
            if (argument == "-d")
            {
                this->m_cli_command_logic.print_filtered_data();
                return;
            }
            ss.seekg(pos);
        }
        this->m_cli_command_logic.print_all_data_filtered(ss);
    }

    void CommandLineInterface::print_function_N(std::stringstream& ss) const
    {
        std::string argument;
        ss >> argument;

        try
        {
            int N = std::stoi(argument);
            this->m_cli_command_logic.print_all_data_N(N);
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << "Argument " << argument << " is not a number." << std::endl;
        }

    }


} // namespace aus

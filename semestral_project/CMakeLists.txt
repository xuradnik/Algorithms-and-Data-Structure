add_executable(semestral_project)


target_include_directories(semestral_project PRIVATE ../dependencies/heap)

target_sources(semestral_project PRIVATE
        main.cpp
        ../dependencies/heap/heap/heap-monitor.cpp
        libdf/data_handlers/csv_loader.hpp
        libdf/data_handlers/data_frame.hpp
        libdf/cli/cli.cpp
        libdf/terminal.cpp
        lib/cli/cli_helper_printer.cpp
        lib/cli/cli_commands_logic.cpp
        lib/data_strucutre/data_structure.hpp
        lib/data_strucutre/data_structure.cpp
        lib/data_strucutre/hash_map_data_structure.cpp
        lib/data_strucutre/Sorter/data_sorter.hpp

)


target_link_libraries(semestral_project PRIVATE ds)

set_target_properties(semestral_project PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)




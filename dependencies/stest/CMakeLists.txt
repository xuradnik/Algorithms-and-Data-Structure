add_library(stest STATIC)

target_sources(stest PRIVATE
    libstest/impl/config.cpp
    libstest/impl/diagnostics.cpp
    libstest/impl/list_tests.cpp
    libstest/impl/run.cpp
    libstest/impl/run_and_print.cpp
    libstest/impl/terminal.cpp
    libstest/impl/test.cpp
    libstest/impl/utils.cpp
)

target_include_directories(stest PUBLIC .)

target_compile_options(stest PRIVATE -Wall -Wextra -Wpedantic)

set_target_properties(stest PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(stest PUBLIC cxxopts)

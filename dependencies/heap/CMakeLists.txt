add_library(heap-monitor STATIC)

target_sources(heap-monitor PRIVATE heap/heap-monitor.cpp)

target_include_directories(heap-monitor PUBLIC .)

set_target_properties(heap-monitor PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

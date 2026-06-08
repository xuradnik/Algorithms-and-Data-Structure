add_library(ds INTERFACE)

target_include_directories(ds INTERFACE ..)

# -fdiagnostics-color=always must be used for ninja to show colors.
# Since ninja buffers output, gcc disables color by default.

target_compile_options(ds INTERFACE -fdiagnostics-color=always)

set_target_properties(ds PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

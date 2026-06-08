#include <libstest/impl/diagnostics.hpp>

#include <csignal>
#include <cstring>

#if defined(_WIN32)
#include "Windows.h"
#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#endif


namespace stest {


static char g_currentTestName[256];

void saveTestName(std::string_view name) {
    std::strncpy(g_currentTestName, name.data(), sizeof(g_currentTestName) - 1);
    g_currentTestName[sizeof(g_currentTestName) - 1] = '\0';
}

void signalHandler(int signal) {
    const char *signalMsg = "";
    switch (signal) {
        case SIGSEGV:
            signalMsg = "Received signal: "
                "\x1B[91mSIGSEGV\x1B[0m -> "
                "invalid memory access (segmentation fault).\n";
            break;
        case SIGABRT:
            signalMsg = "Received signal: "
                "\x1B[91mSIGABRT\x1B[0m -> "
                "abnormal termination condition.\n";
            break;
        case SIGILL:
            signalMsg = "Received signal: "
                "\x1B[91mSIGILL\x1B[0m -> "
                "invalid program image, such as invalid instruction, "
                "often a sign of \x1B[91mmissing return statement\x1B[0m.\n";
            break;
        case SIGFPE:
            signalMsg = "Received signal: "
                "\x1B[91mSIGFPE\x1B[0m -> "
                "erroneous arithmetic operation, such as divide by zero.\n";
            break;
        default:
            signalMsg = "Received unknown singal.\n";
            break;
    }
#if defined(_WIN32)
    HANDLE err = GetStdHandle(STD_ERROR_HANDLE);
    DWORD w;
    WriteFile(err, "Error in `", 10, &w, NULL);
    WriteFile(err, g_currentTestName, std::strlen(g_currentTestName), &w, NULL);
    WriteFile(err, "`.\n", 3, &w, NULL);
    WriteFile(err, signalMsg, std::strlen(signalMsg), &w, NULL);
#elif defined(__linux__) || defined(__APPLE_)
    write(STDERR_FILENO, "Error in `", 10);
    write(STDERR_FILENO, g_currentTestName, std::strlen(g_currentTestName));
    write(STDERR_FILENO, "`.\n", 3);
    write(STDERR_FILENO, signalMsg, std::strlen(signalMsg));
#endif
    std::signal(signal, SIG_DFL);
    std::raise(signal);
}

void registerSignalHandlers() {
    std::signal(SIGSEGV, signalHandler);
    std::signal(SIGABRT, signalHandler);
    std::signal(SIGILL, signalHandler);
    std::signal(SIGFPE, signalHandler);
}


} // namespace stest

#include <libstest/run.hpp>
#include <libstest/impl/diagnostics.hpp>
#include <libstest/impl/list_tests.hpp>
#include <libstest/impl/run_and_print.hpp>

#include <iostream>
#include <stdexcept>


namespace stest {


static void runModeRunTests(Test &root, const Config &config) {
    registerSignalHandlers();
    if (config.getTestsToRun().empty()) {
        RunAndPrintVisitor visitor(config);
        root.accept(visitor);
    } else {
        findRunAndPrint(root, config);
    }
}

static void runModelistTests(Test &root, const Config &config) {
    (void)config;
    ListTestsVisitor visitor;
    root.accept(visitor);
}

static void runModePrintHelp(Test &root, const Config &config) {
    (void)root;
    config.printHelp(std::cout);
}


int run(int argc, char *argv[], Test &root) {
    const Config config = Config::createFromArgs(argc, argv);
    switch (config.getMode()) {
        case Config::Mode::Run:
            runModeRunTests(root, config);
            break;

        case Config::Mode::List:
            runModelistTests(root, config);
            break;

        case Config::Mode::Help:
            runModePrintHelp(root, config);
            break;

        default:
            throw std::invalid_argument("Unknown stest mode.");
    }
    return 0;
}


} // namespace stest

#include <libstest/config.hpp>
#include <libstest/impl/terminal.hpp>
#include <libstest/impl/utils.hpp>

#include <cxxopts/cxxopts.hpp>

#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>


namespace stest {


static cxxopts::Options createCmdOptions() {
    cxxopts::Options options(
        "[test-target]",
        "Simple Test -- library for writing and running tests");
    options.add_options()
        ("l,list", "List available tests")
        ("d,detail", "Enable detailed output")
        ("t,test",
            "Run specific tests; use full names from --list delimited by ,",
            cxxopts::value<std::string>())
        ("h,help", "Print usage");
    return options;
}


Config Config::createFromArgs(int argc, char *argv[]) {
    cxxopts::Options options = createCmdOptions();
    try {
        Config cfg = Config::createDefault();
        const cxxopts::ParseResult args = options.parse(argc, argv);
        const std::optional<bool> helpOpt =
            args.as_optional<bool>("help");
        if (helpOpt && *helpOpt) {
            Terminal::println(options.help());
            std::exit(0);
            return Config::createDefault();
        }
        const std::optional<bool> modeOpt =
            args.as_optional<bool>("list");
        cfg.m_mode = modeOpt && *modeOpt
            ? Config::Mode::List
            : Config::Mode::Run;
        const std::optional<bool> detailedOpt =
            args.as_optional<bool>("detail");
        cfg.m_outputLevel = detailedOpt && *detailedOpt
            ? Config::OutputLevel::Detailed
            : Config::OutputLevel::Summary;
        const std::optional<std::string> testOpt =
            args.as_optional<std::string>("test");
        cfg.m_testToRun = testOpt
            ? std::vector<std::string>{splitOn(*testOpt, ',')}
            : std::vector<std::string>{};
        return cfg;
    } catch (const std::exception &e) {
        Terminal::println("Failed to parse arguments.", Terminal::Color::Red);
        Terminal::println(options.help());
        std::exit(1);
        return Config::createDefault();
    }
}

Config Config::createDefault() {
    Config cfg;
    cfg.m_mode = Config::Mode::Run;
    cfg.m_outputLevel = Config::OutputLevel::Detailed;
    cfg.m_testToRun = {};
    return cfg;
}

Config::Mode Config::getMode() const {
    return m_mode;
}

Config::OutputLevel Config::getOutputLevel() const {
    return m_outputLevel;
}

const std::vector<std::string> Config::getTestsToRun() const {
    return m_testToRun;
}

void Config::printHelp(std::ostream &ost) const {
    ost << createCmdOptions().help() << "\n";
}


} // namespace stest

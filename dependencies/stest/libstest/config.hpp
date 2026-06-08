#ifndef LIBSTEST_CONFIG_HPP
#define LIBSTEST_CONFIG_HPP

#include <ostream>
#include <string>
#include <vector>


namespace stest {


class Config {
public:
    static Config createFromArgs(int argc, char *argv[]);
    static Config createDefault();

public:
    enum class Mode {
        Run,
        List,
        Help
    };

    enum class OutputLevel {
        Detailed,
        Summary
    };

public:
    Config::Mode getMode() const;
    Config::OutputLevel getOutputLevel() const;
    const std::vector<std::string> getTestsToRun() const;

    void printHelp(std::ostream &ost) const;

private:
    Config() = default;

private:
    Mode m_mode;
    OutputLevel m_outputLevel;
    std::vector<std::string> m_testToRun;
};


} // namespace stest

#endif

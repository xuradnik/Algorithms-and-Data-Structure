#ifndef LIBSTEST_RUN_HPP
#define LIBSTEST_RUN_HPP

#include <libstest/config.hpp>
#include <libstest/test.hpp>


namespace stest {


/**
 * @brief Runs stest on @p test using configuration provided by @p argv
 */
int run(int argc, char *argv[], Test &test);

/**
 * @brief Runs stest on @p test using configuration provided by @p config
 */
int run(const Config &config, Test &test);


} // namespace stest

#endif

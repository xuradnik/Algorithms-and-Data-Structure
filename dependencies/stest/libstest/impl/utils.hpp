#ifndef LIBSTEST_IMPL_UTILS_HPP
#define LIBSTEST_IMPL_UTILS_HPP

#include <string>
#include <string_view>
#include <vector>


namespace stest {


std::vector<std::string> splitOn(std::string_view str, char delim);


} // namespace stest

#endif

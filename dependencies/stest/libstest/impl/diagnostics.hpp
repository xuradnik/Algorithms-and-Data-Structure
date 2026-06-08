#ifndef LIBSTEST_IMPL_DIAGNOSTICS_HPP
#define LIBSTEST_IMPL_DIAGNOSTICS_HPP

#include <string_view>


namespace stest {


void saveTestName(std::string_view name);

void registerSignalHandlers();


} // namespace stest


#endif

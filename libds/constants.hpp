#ifndef LIBDS_CONSTANTS_HPP
#define LIBDS_CONSTANTS_HPP

#include <cstddef>
#include <limits>

namespace ds {

/**
 * @brief Constant representing an invalid or non-existent index.
 * Usually returned by search or calculation methods when a block is not found within the managed memory range.
 */
const std::size_t INVALID_INDEX = (std::numeric_limits<size_t>::max)();

} // namespace ds

#endif

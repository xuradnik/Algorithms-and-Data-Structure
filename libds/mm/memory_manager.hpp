#ifndef LIBDS_MM_MEMORY_MANAGER_HPP
#define LIBDS_MM_MEMORY_MANAGER_HPP

#include <cstddef>


namespace ds::mm {


/**
 * @brief Abstract base class for managing memory blocks of a specific type.
 * @tparam BlockType The type of the memory block being managed.
 */
template<typename BlockType>
class MemoryManager {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes.
     */
    virtual ~MemoryManager() = default;

    /**
     * @brief Allocates a new block of memory.
     * @return A pointer to the newly allocated BlockType.
     */
    virtual BlockType *allocateMemory() = 0;

    /**
     * @brief Releases a previously allocated memory block.
     * @param pointer Pointer to the memory block to be released.
     */
    virtual void releaseMemory(BlockType *pointer) = 0;

    /**
     * @brief Retrieves the total number of currently allocated blocks.
     * @return The number of active memory blocks.
     */
    virtual std::size_t getAllocatedBlockCount() const = 0;
};


} // namespace ds::mm

#endif

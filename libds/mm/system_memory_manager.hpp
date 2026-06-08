#ifndef LIBDS_MM_SYSTEM_MEMORY_MANAGER_HPP
#define LIBDS_MM_SYSTEM_MEMORY_MANAGER_HPP

#include "memory_manager.hpp"


namespace ds::mm {


/**
 * @brief Memory manager that uses standard system allocation (new/delete).
 * @tparam BlockType The type of the memory block being managed.
 */
template<typename BlockType>
class SystemMemoryManager : public MemoryManager<BlockType> {
public:
    /**
     * @brief Default constructor. Initializes the allocated block count to zero.
     */
    SystemMemoryManager();

    /**
     * @brief Allocates a new block of memory.
     * @return A pointer to the newly allocated BlockType.
     * @throws std::bad_alloc If memory allocation fails.
     */
    BlockType *allocateMemory() override;

    /**
     * @brief Releases a previously allocated memory block.
     * @param pointer Pointer to the memory block to be released.
     */
    void releaseMemory(BlockType *pointer) override;

    /**
     * @brief Retrieves the total number of currently allocated blocks.
     * @return The number of active memory blocks.
     */
    std::size_t getAllocatedBlockCount() const override;

    /** @brief Copy construction is deleted to prevent multiple management of the same resources. */
    SystemMemoryManager(const SystemMemoryManager<BlockType> &other) = delete;

    /** @brief Copy assignment is deleted. */
    SystemMemoryManager<BlockType> &operator=(const SystemMemoryManager<BlockType>& other) = delete;

protected:
    /** @brief Tracks the number of blocks currently allocated through this manager. */
    std::size_t m_allocatedBlockCount;
};


template<typename BlockType>
SystemMemoryManager<BlockType>::SystemMemoryManager() : m_allocatedBlockCount(0) {
}

template<typename BlockType>
BlockType *SystemMemoryManager<BlockType>::allocateMemory() {
    m_allocatedBlockCount++;
    return new BlockType();
}

template<typename BlockType>
void SystemMemoryManager<BlockType>::releaseMemory(BlockType *pointer) {
    m_allocatedBlockCount--;
    delete pointer;
}

template<typename BlockType>
std::size_t SystemMemoryManager<BlockType>::getAllocatedBlockCount() const {
    return m_allocatedBlockCount;
}


} // namespace ds::mm

#endif


#ifndef LIBDS_MM_COMPACT_OPERATION_MEMORY_MANAGER_HPP
#define LIBDS_MM_COMPACT_OPERATION_MEMORY_MANAGER_HPP

#include "../constants.hpp"
#include "compact_memory_manager.hpp"


namespace ds::mm {


/**
 * @brief Base class for memory managers that provide contiguous or index-based storage in operation memory.
 * @tparam BlockType The type of the memory block, must be equality comparable.
 */
template<typename BlockType>
class CompactOperationMemoryManager : public CompactMemoryManager<BlockType> {
public:
    /**
     * @brief Default constructor. Initializes with INIT_SIZE.
     */
    CompactOperationMemoryManager();

    /**
     * @brief Constructor with specific initial capacity.
     * @param size Initial number of blocks to allocate.
     */
    CompactOperationMemoryManager(std::size_t size);

    /**
     * @brief Copy constructor.
     * @param other Source manager to copy from.
     */
    CompactOperationMemoryManager(
        const CompactOperationMemoryManager<BlockType> &other);

    /**
     * @brief Move constructor.
     * @param other Source manager to move from.
     */
    CompactOperationMemoryManager(
        CompactOperationMemoryManager<BlockType> &&other);

    /**
     * @brief Destructor. Frees the allocated heap memory.
     */
    ~CompactOperationMemoryManager() override;

    /**
     * @brief Retrieves the total number of currently allocated blocks.
     * @return The number of active memory blocks.
     */
    std::size_t getAllocatedBlockCount() const override;

    /**
     * @brief Gets the total capacity of the manager.
     * @return The maximum number of blocks the manager can currently hold.
     */
    std::size_t getCapacity() const override;

    /**
     * @brief Calculates the memory address of a given data block.
     * @param data The data block to locate.
     * @return A pointer to the memory address of the data.
     */
    void *calculateAddress(const BlockType &data) override;

    /**
     * @brief Calculates the index of a given data block.
     * @param data The data block to locate.
     * @return The index of the block.
     */
    std::size_t calculateIndex(const BlockType &data) const override;

    /**
     * @brief Provides access to the block at the specified index.
     * @param index The index of the block.
     * @return Reference to the block.
     */
    BlockType &getBlockAt(std::size_t index) override;

    /**
     * @brief Provides constant access to the block at the specified index.
     * @param index The index of the block.
     * @return Constant reference to the block.
     */
    const BlockType &getBlockAt(std::size_t index) const override;

    /**
     * @brief Swaps the contents of two memory blocks.
     * @param index1 Index of the first block.
     * @param index2 Index of the second block.
     */
    void swap(std::size_t index1, std::size_t index2) override;

    /**
     * @brief Allocates a new block of memory.
     * @return A pointer to the newly allocated BlockType.
     * @throws std::bad_alloc If memory allocation fails.
     */
    BlockType *allocateMemory() override;

protected:
    /**
     * @brief Calculates the next capacity size for expansion.
     * @return The new calculated capacity.
     */
    std::size_t calculateExpandedCapacity() const;

protected:
    /** @brief Default initial size for the manager. */
    static const std::size_t INIT_SIZE = 4;

protected:
    /** @brief Pointer to the start of the allocated memory. */
    BlockType *m_base;
    /** @brief Pointer to the position immediately following the last allocated block. */
    BlockType *m_end;
    /** @brief Pointer to the end of the reserved memory capacity. */
    BlockType *m_limit;
};


template<typename BlockType>
CompactOperationMemoryManager<BlockType>::CompactOperationMemoryManager() :
    CompactOperationMemoryManager<BlockType>(INIT_SIZE) {
}

template<typename BlockType>
CompactOperationMemoryManager<BlockType>::CompactOperationMemoryManager(
    std::size_t size
) :
    m_base(static_cast<BlockType*>(std::malloc(size * sizeof(BlockType)))),
    m_end(m_base),
    m_limit(m_base + size) {
}

template<typename BlockType>
CompactOperationMemoryManager<BlockType>::CompactOperationMemoryManager(
    const CompactOperationMemoryManager<BlockType> &other
) :
    m_base(static_cast<BlockType*>(
        std::malloc(other.getCapacity() * sizeof(BlockType)))),
    m_end(m_base + other.getAllocatedBlockCount()),
    m_limit(m_base + other.getCapacity())
{
}

template<typename BlockType>
CompactOperationMemoryManager<BlockType>::CompactOperationMemoryManager(
    CompactOperationMemoryManager<BlockType> &&other
) :
    m_base(other.m_base),
    m_end(other.m_end),
    m_limit(other.m_limit)
{
    other.m_base = nullptr;
    other.m_end = nullptr;
    other.m_limit = nullptr;
}

template<typename BlockType>
CompactOperationMemoryManager<BlockType>::~CompactOperationMemoryManager() {
    std::free(m_base);
}

template<typename BlockType>
std::size_t CompactOperationMemoryManager<BlockType>::getAllocatedBlockCount(
) const {
    return m_end - m_base;
}

template<typename BlockType>
std::size_t CompactOperationMemoryManager<BlockType>::getCapacity() const {
    return m_limit - m_base;
}

template<typename BlockType>
void *CompactOperationMemoryManager<BlockType>::calculateAddress(
    const BlockType &data
) {
    return const_cast<BlockType*>(&data);
}

template<typename BlockType>
std::size_t CompactOperationMemoryManager<BlockType>::calculateIndex(
    const BlockType &data
) const {
    return &data - m_base;
}

template<typename BlockType>
BlockType &CompactOperationMemoryManager<BlockType>::getBlockAt(
    std::size_t index
) {
    return *(m_base + index);
}

template<typename BlockType>
const BlockType &CompactOperationMemoryManager<BlockType>::getBlockAt(
    std::size_t index
) const {
    return *(m_base + index);
}

template<typename BlockType>
void CompactOperationMemoryManager<BlockType>::swap(
    std::size_t index1,
    std::size_t index2
) {
    using std::swap;
    swap(this->getBlockAt(index1), this->getBlockAt(index2));
}

template<typename BlockType>
BlockType *CompactOperationMemoryManager<BlockType>::allocateMemory() {
    return this->allocateMemoryAt(
        static_cast<std::size_t>(this->m_end - this->m_base));
}

template<typename BlockType>
std::size_t CompactOperationMemoryManager<BlockType>::calculateExpandedCapacity(
) const {
    return 2 * this->getCapacity();
}


} // namespace ds::mm

#endif

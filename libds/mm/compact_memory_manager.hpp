#ifndef LIBDS_MM_COMPACT_MEMORY_MANAGER_HPP
#define LIBDS_MM_COMPACT_MEMORY_MANAGER_HPP

#include "memory_manager.hpp"

#include <concepts>
#include <cstdlib>
#include <cstring>


namespace ds::mm {


/**
 * @brief Base class for memory managers that provide contiguous or index-based storage.
 * @tparam BlockType The type of the memory block, must be equality comparable.
 */
template<typename BlockType>
requires(std::equality_comparable<BlockType>)
class CompactMemoryManager : public MemoryManager<BlockType> {
public:
    /**
     * @brief Allocates a memory block at a specific index.
     * @param index The target position for allocation.
     * @return Pointer to the allocated block.
     */
    virtual BlockType *allocateMemoryAt(std::size_t index) = 0;

    /**
     * @brief Releases the last allocated block in the manager.
     */
    virtual void releaseLast() = 0;

    /**
     * @brief Gets the total capacity of the manager.
     * @return The maximum number of blocks the manager can currently hold.
     */
    virtual std::size_t getCapacity() const = 0;

    /**
     * @brief Increases the capacity to a specific value if it exceeds current capacity.
     * @param newCapacity The desired minimum capacity.
     */
    void expandCapacity(std::size_t newCapacity);

    /**
     * @brief Increases the capacity by a specified amount.
     * @param howMuch The amount to add to current capacity.
     */
    void expandCapacityBy(std::size_t howMuch);

    /**
     * @brief Reduces capacity to match the current number of allocated blocks.
     */
    void shrinkToFit();

    /**
     * @brief Clears all allocated memory and resets the manager.
     */
    virtual void clear() = 0;

    /**
     * @brief Calculates the memory address of a given data block.
     * @param data The data block to locate.
     * @return A pointer to the memory address of the data.
     */
    virtual void *calculateAddress(const BlockType &data) = 0;

    /**
     * @brief Calculates the index of a given data block.
     * @param data The data block to locate.
     * @return The index of the block.
     */
    virtual std::size_t calculateIndex(const BlockType &data) const = 0;

    /**
     * @brief Provides access to the block at the specified index.
     * @param index The index of the block.
     * @return Reference to the block.
     * @throws std::out_of_range If index is invalid.
     */
    virtual BlockType &getBlockAt(std::size_t index) = 0;

    /**
     * @brief Provides constant access to the block at the specified index.
     * @param index The index of the block.
     * @return Constant reference to the block.
     * @throws std::out_of_range If index is invalid.
     */
    virtual const BlockType &getBlockAt(std::size_t index) const = 0;

    /**
     * @brief Swaps the contents of two memory blocks.
     * @param index1 Index of the first block.
     * @param index2 Index of the second block.
     */
    virtual void swap(std::size_t index1, std::size_t index2) = 0;

    /**
     * @brief Assigns the content of another manager to this one.
     * @param other The source manager.
     * @return Reference to this manager.
     */
    virtual CompactMemoryManager<BlockType> &assign(
        const CompactMemoryManager<BlockType> &other) = 0;

    /**
     * @brief Compares this manager with another for equality.
     * @param other The manager to compare with.
     * @return True if managers are equal, false otherwise.
     */
    virtual bool equals(const CompactMemoryManager<BlockType> &other) const = 0;

    /**
     * @brief Assignment operator.
     * @param other The source manager.
     * @return Reference to this manager.
     */
    CompactMemoryManager<BlockType> &operator=(
        const CompactMemoryManager<BlockType> &other);

protected:
    /**
     * @brief Internal method to change the storage capacity.
     * @param newCapacity The new capacity to set.
     */
    virtual void changeCapacity(std::size_t newCapacity) = 0;
};


template<typename BlockType>
requires(std::equality_comparable<BlockType>)
CompactMemoryManager<BlockType> &CompactMemoryManager<BlockType>::operator=(
    const CompactMemoryManager<BlockType> &other
) {
    this->assign(other);
    return *this;
}

template<typename BlockType>
requires(std::equality_comparable<BlockType>)
void CompactMemoryManager<BlockType>::expandCapacity(std::size_t newCapacity) {
    if (newCapacity > this->getCapacity()) {
        this->changeCapacity(newCapacity);
    }
}

template<typename BlockType>
requires(std::equality_comparable<BlockType>)
void CompactMemoryManager<BlockType>::expandCapacityBy(std::size_t howMuch) {
    this->changeCapacity(this->getCapacity() + howMuch);
}

template<typename BlockType>
requires(std::equality_comparable<BlockType>)
void CompactMemoryManager<BlockType>::shrinkToFit() {
    this->changeCapacity(this->getAllocatedBlockCount());
}


} // namespace ds::mm

#endif

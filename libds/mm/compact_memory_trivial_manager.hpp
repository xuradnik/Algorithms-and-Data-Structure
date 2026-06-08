#ifndef LIBDS_MM_COMPACT_MEMORY_TRIVIAL_MANAGER_HPP
#define LIBDS_MM_COMPACT_MEMORY_TRIVIAL_MANAGER_HPP

#include "compact_operation_memory_manager.hpp"

#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <new>


namespace ds::mm {


/**
 * @brief Implementation of a compact memory manager in operation memory for trivially copyable types.
 * @tparam BlockType The type of the memory block.
 */
template<typename BlockType>
class CompactMemoryTrivialManager :
    public CompactOperationMemoryManager<BlockType> {
public:
    /**
     * @brief Default constructor.
     */
    CompactMemoryTrivialManager() = default;

    /**
     * @brief Constructor with specific initial capacity.
     * @param size Initial number of blocks to allocate.
     */
    CompactMemoryTrivialManager(std::size_t size);

    /**
     * @brief Copy constructor.
     * @param other Source manager to copy from.
     */
    CompactMemoryTrivialManager(
        const CompactMemoryTrivialManager<BlockType> &other);

    /**
     * @brief Move constructor.
     * @param other Source manager to move from.
     */
    CompactMemoryTrivialManager(
        CompactMemoryTrivialManager<BlockType> &&other) = default;

    /**
     * @brief Allocates a memory block at a specific index.
     * @param index The target position for allocation.
     * @return Pointer to the allocated block.
     */
    BlockType *allocateMemoryAt(std::size_t index) override;

    /**
     * @brief Releases a previously allocated memory block.
     * @param pointer Pointer to the memory block to be released.
     */
    void releaseMemory(BlockType *pointer) override;

    /**
     * @brief Releases the last allocated block in the manager.
     */
    void releaseLast() override;

    /**
     * @brief Clears all allocated memory and resets the manager.
     */
    void clear() override;

    /**
     * @brief Assigns the content of another manager to this one.
     * @param other The source manager.
     * @return Reference to this manager.
     * @throws std::runtime_error If types do not match.
     */
    CompactMemoryManager<BlockType> &assign(
        const CompactMemoryManager<BlockType> &other) override;

    /**
     * @brief Compares this manager with another for equality.
     * @param other The manager to compare with.
     * @return True if managers are equal, false otherwise.
     */
    bool equals(const CompactMemoryManager<BlockType> &other) const override;

protected:
    /**
     * @brief Internal method to change the storage capacity.
     * @param newCapacity The new capacity to set.
     * @throws std::bad_alloc If reallocation fails.
     */
    void changeCapacity(std::size_t newCapacity) override;
};


template<typename BlockType>
CompactMemoryTrivialManager<BlockType>::CompactMemoryTrivialManager(
    std::size_t size
) :
    CompactOperationMemoryManager<BlockType>(size)
{
}

template<typename BlockType>
CompactMemoryTrivialManager<BlockType>::CompactMemoryTrivialManager(
    const CompactMemoryTrivialManager<BlockType> &other
) :
    CompactOperationMemoryManager<BlockType>(other)
{
    std::memcpy(
        this->m_base,
        other.m_base,
        other.getAllocatedBlockCount() * sizeof(BlockType));
}

template<typename BlockType>
BlockType *CompactMemoryTrivialManager<BlockType>::allocateMemoryAt(
    std::size_t index
) {
    if (this->m_end == this->m_limit) {
        this->changeCapacity(this->calculateExpandedCapacity());
    }

    std::memmove(
        this->m_base + index + 1,
        this->m_base + index,
        (this->m_end - this->m_base - index) * sizeof(BlockType));
    ++this->m_end;
    return this->m_base + index;
}

template<typename BlockType>
void CompactMemoryTrivialManager<BlockType>::releaseMemory(BlockType *pointer) {
    std::memmove(
        pointer,
        pointer + 1,
        sizeof(BlockType) * (this->m_end - pointer));
    --this->m_end;
}

template<typename BlockType>
void CompactMemoryTrivialManager<BlockType>::releaseLast() {
    --this->m_end;
}

template<typename BlockType>
void CompactMemoryTrivialManager<BlockType>::clear() {
    this->m_end = this->m_base;
}

template<typename BlockType>
CompactMemoryManager<BlockType> &CompactMemoryTrivialManager<BlockType>::assign(
    const CompactMemoryManager<BlockType> &other
) {
    if (this != &other) {
        auto *const otherCmtm =
            dynamic_cast<const CompactMemoryTrivialManager<BlockType>*>(&other);

        if (otherCmtm == nullptr) {
            throw std::runtime_error("Invalid assign. Type missmatch.");
        }

        this->changeCapacity(otherCmtm->getCapacity());

        std::memcpy(
            this->m_base,
            otherCmtm->m_base,
            otherCmtm->getAllocatedBlockCount() * sizeof(BlockType));
        this->m_end = this->m_base + otherCmtm->getAllocatedBlockCount();
    }
    return *this;
}

template<typename BlockType>
bool CompactMemoryTrivialManager<BlockType>::equals(
    const CompactMemoryManager<BlockType> &other
) const
{
    if (this->getAllocatedBlockCount() != other.getAllocatedBlockCount()) {
        return false;
    }

    if (this == &other) {
        return true;
    }

    const CompactMemoryTrivialManager<BlockType> *otherCmtm =
        dynamic_cast<const CompactMemoryTrivialManager<BlockType>*>(&other);
    if (otherCmtm == nullptr) {
        return false;
    }

    return 0 == std::memcmp(
        this->m_base,
        otherCmtm->m_base,
        this->getAllocatedBlockCount() * sizeof(BlockType));
}

template<typename BlockType>
void CompactMemoryTrivialManager<BlockType>::changeCapacity(
    std::size_t newCapcaity
) {
    BlockType *const newBase = static_cast<BlockType*>(
        std::realloc(this->m_base, newCapcaity * sizeof(BlockType)));

    if (newBase == nullptr) {
        throw std::bad_alloc();
    }

    this->m_end = newBase + (this->m_end - this->m_base);
    this->m_base = newBase;
    this->m_limit = this->m_base + newCapcaity;
}


} // namespace ds::mm

#endif

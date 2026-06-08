#ifndef LIBDS_MM_COMPACT_MEMORY_OBJECT_MANAGER_HPP
#define LIBDS_MM_COMPACT_MEMORY_OBJECT_MANAGER_HPP

#include "../utils.hpp"
#include "compact_operation_memory_manager.hpp"

#include <stdexcept>
#include <new>

namespace ds::mm {


/**
 * @brief Implementation of a compact memory manager in operation memory for complex objects requiring explicit construction/destruction.
 * @tparam BlockType The type of the memory block.
 */
template<typename BlockType>
class CompactMemoryObjectManager :
    public CompactOperationMemoryManager<BlockType> {
public:
    /**
     * @brief Default constructor.
     */
    CompactMemoryObjectManager() = default;

    /**
     * @brief Constructor with specific initial capacity.
     * @param size Initial number of blocks to allocate.
     */
    CompactMemoryObjectManager(std::size_t size);

    /**
     * @brief Copy constructor. Performs deep copy of objects.
     * @param other Source manager to copy from.
     */
    CompactMemoryObjectManager(
        const CompactMemoryObjectManager<BlockType> &other);

    /**
     * @brief Move constructor.
     * @param other Source manager to move from.
     */
    CompactMemoryObjectManager(
        CompactMemoryObjectManager<BlockType> &&other) = default;

    /**
     * @brief Destructor. Explicitly destroys all allocated objects.
     */
    ~CompactMemoryObjectManager() override;

    /**
     * @brief Allocates a memory block at a specific index.
     * @param index The target position for allocation.
     * @return Pointer to the newly constructed BlockType.
     * @throws std::bad_alloc If memory reallocation fails during expansion.
     */
    BlockType *allocateMemoryAt(std::size_t index) override;

    /**
     * @brief Releases a previously allocated memory block and calls its destructor.
     * @param pointer Pointer to the memory block to be released.
     */
    void releaseMemory(BlockType *pointer) override;

    /**
     * @brief Releases the last allocated block in the manager and calls its destructor.
     */
    void releaseLast() override;

    /**
     * @brief Clears all allocated memory, calling destructors for all objects.
     */
    void clear() override;

    /**
     * @brief Assigns the content of another manager to this one.
     * @param other The source manager.
     * @return Reference to this manager.
     * @throws std::runtime_error If types do not match.
     * @throws std::bad_alloc If memory allocation fails.
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
     * @brief Internal method to change the storage capacity while relocating objects.
     * @param newCapacity The new capacity to set.
     * @throws std::bad_alloc If allocation fails.
     */
    void changeCapacity(std::size_t newCapacity) override;

private:
    /**
     * @brief Handles allocation when a capacity expansion is required.
     * @param index The target position.
     * @return Pointer to the newly constructed BlockType.
     * @throws std::bad_alloc If allocation fails.
     */
    BlockType *allocateMemoryAtExpanding(std::size_t index);

    /**
     * @brief Handles allocation within existing capacity.
     * @param index The target position.
     * @return Pointer to the newly constructed BlockType.
     */
    BlockType *allocateMemoryAtNotExpanding(std::size_t index);
};


template<typename BlockType>
CompactMemoryObjectManager<BlockType>::CompactMemoryObjectManager(
    std::size_t size
) :
    CompactOperationMemoryManager<BlockType>(size)
{
}

template<typename BlockType>
CompactMemoryObjectManager<BlockType>::CompactMemoryObjectManager(
    const CompactMemoryObjectManager<BlockType> &other
) :
    CompactOperationMemoryManager<BlockType>(other)
{
    utils::copyObjects(this->m_base, other.m_base, other.m_end);
}

template<typename BlockType>
CompactMemoryObjectManager<BlockType>::~CompactMemoryObjectManager() {
    utils::destroyObjects(this->m_base, this->m_end);
}

template<typename BlockType>
BlockType *CompactMemoryObjectManager<BlockType>::allocateMemoryAt(
    std::size_t index
) {
    return this->m_end == this->m_limit
        ? this->allocateMemoryAtExpanding(index)
        : this->allocateMemoryAtNotExpanding(index);
}

template<typename BlockType>
void CompactMemoryObjectManager<BlockType>::releaseMemory(BlockType *pointer) {
    pointer->~BlockType();
    utils::rellocateObjects(pointer, pointer + 1, this->m_end);
    --this->m_end;
}

template<typename BlockType>
void CompactMemoryObjectManager<BlockType>::releaseLast() {
    --this->m_end;
    this->m_end->~BlockType();
}

template<typename BlockType>
BlockType *CompactMemoryObjectManager<BlockType>::allocateMemoryAtExpanding(
    std::size_t index
) {
    const std::size_t newCapacity = this->calculateExpandedCapacity();
    BlockType *const newBase = static_cast<BlockType*>(
        std::malloc(sizeof(BlockType) * newCapacity));

    if (newBase == nullptr) {
        throw std::bad_alloc();
    }

    utils::rellocateObjects(
        newBase,
        this->m_base,
        this->m_base + index);
    utils::rellocateObjects(
        newBase + index + 1,
        this->m_base + index,
        this->m_end);

    std::free(this->m_base);

    this->m_end = newBase + (this->m_end - this->m_base + 1);
    this->m_base = newBase;
    this->m_limit = newBase + newCapacity;
    return new (newBase + index) BlockType();
}

template<typename BlockType>
BlockType *CompactMemoryObjectManager<BlockType>::allocateMemoryAtNotExpanding(
    std::size_t index
) {
    utils::rellocateObjectsBackwards(
        this->m_end,
        this->m_end - 1,
        this->m_base + index - 1);
    ++this->m_end;
    return new (this->m_base + index) BlockType();
}

template<typename BlockType>
void CompactMemoryObjectManager<BlockType>::clear() {
    utils::destroyObjects(this->m_base, this->m_end);
    this->m_end = this->m_base;
}

template<typename BlockType>
CompactMemoryManager<BlockType> &CompactMemoryObjectManager<BlockType>::assign(
    const CompactMemoryManager<BlockType> &other
) {
    if (this != &other) {
        auto *const otherCmom =
            dynamic_cast<const CompactMemoryObjectManager<BlockType>*>( &other);

        if (otherCmom == nullptr) {
            throw std::runtime_error("Invalid assign. Type missmatch.");
        }

        BlockType *const newBase = static_cast<BlockType*>(std::malloc(
            otherCmom->getCapacity()  * sizeof(BlockType)));
        if (newBase == nullptr) {
            throw std::bad_alloc();
        }

        utils::destroyObjects(this->m_base, this->m_end);
        std::free(this->m_base);
        utils::copyObjects(newBase, otherCmom->m_base, otherCmom->m_end);

        this->m_base = newBase;
        this->m_end = newBase + otherCmom->getAllocatedBlockCount();
        this->m_limit = newBase + otherCmom->getCapacity();
    }
    return *this;
}

template<typename BlockType>
bool CompactMemoryObjectManager<BlockType>::equals(
    const CompactMemoryManager<BlockType> &other
) const
{
    if (this->getAllocatedBlockCount() != other.getAllocatedBlockCount()) {
        return false;
    }

    if (this == &other) {
        return true;
    }

    const CompactMemoryObjectManager<BlockType> *otherCmom =
        dynamic_cast<const CompactMemoryObjectManager<BlockType>*>(&other);
    if (otherCmom == nullptr) {
        return false;
    }

    BlockType *selfCurrent = this->m_base;
    BlockType *otherCurrent = otherCmom->m_base;
    while (selfCurrent != this->m_end) {
        if (!(*selfCurrent == *otherCurrent)) {
            return false;
        }
        ++selfCurrent;
        ++otherCurrent;
    }

    return true;
}

template<typename BlockType>
void CompactMemoryObjectManager<BlockType>::changeCapacity(
    std::size_t newCapacity
) {
    BlockType *const newBase = static_cast<BlockType*>(
        std::malloc(newCapacity * sizeof(BlockType)));

    if (newBase == nullptr) {
        throw std::bad_alloc();
    }

    utils::rellocateObjects(newBase, this->m_base, this->m_end);
    std::free(this->m_base);

    this->m_end = newBase + (this->m_end - this->m_base);
    this->m_base = newBase;
    this->m_limit = newBase + newCapacity;
}


} // namespace ds::mm

#endif

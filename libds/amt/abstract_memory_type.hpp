#ifndef LIBDS_AMT_ABSTRACT_MEMORY_TYPE_HPP
#define LIBDS_AMT_ABSTRACT_MEMORY_TYPE_HPP

#include "../mm/system_memory_manager.hpp"
#include "../mm/compact_memory_trivial_manager.hpp"
#include "../mm/compact_memory_object_manager.hpp"

#include <cstddef>
#include <type_traits>


namespace ds::amt {


/**
 * @brief A plain aggregate wrapping a single data value stored in a memory structure.
 * Base for other memory blocks.
 *
 * @tparam DataType The type of the value held by the block.
 */
template<typename DataType>
struct MemoryBlock {
    using DataT = DataType;

    DataType data;

    bool operator==(const MemoryBlock<DataType> &other) const = default;
    bool operator!=(const MemoryBlock<DataType> &other) const = default;
};

//----------

/**
 * @brief Base class for abstract memory types. Defines their interface.
 */
class AbstractMemoryType {
public:
    /**
     * @brief Virtual destructor ensuring correct cleanup of derived types.
     */
    virtual ~AbstractMemoryType() = default;

    /**
     * @brief Returns the number of currently allocated blocks.
     */
    virtual std::size_t size() const = 0;

    /**
     * @brief Returns @c true when no blocks are allocated.
     */
    virtual bool isEmpty() const = 0;

    /**
     * @brief Releases all allocated elements, leaving the structure empty.
     */
    virtual void clear() = 0;

    /**
     * @brief Replaces the contents of this structure with a copy of @p other.
     * @param other The source structure to copy from.
     * @return Reference to this structure.
     */
    virtual AbstractMemoryType &assign(const AbstractMemoryType &other) = 0;

    /**
     * @brief Returns @c true if this structure is element-wise equal to @p other.
     * @param other The structure to compare against.
     */
    virtual bool equals(const AbstractMemoryType &other) const = 0;
};

using AMT = AbstractMemoryType;

//----------

/**
 * @brief Base for memory structures that own a @c MemoryManager instance.
 *
 * Manages the lifetime of the underlying @c MemoryManager and delegates
 * @c size() and @c isEmpty() queries to it.
 *
 * @tparam BlockType The type of block managed by the memory manager.
 */
template<typename BlockType>
class AbstractMemoryStructure : virtual public AMT {
public:
    /**
     * @brief Returns the number of currently allocated blocks.
     */
    std::size_t size() const override;

    /**
     * @brief Returns @c true when no blocks are allocated.
     */
    bool isEmpty() const override;

    /**
     * @brief Copy-assigns @p other to this structure by delegating to @c assign.
     * @param other The source structure to copy from.
     * @return Reference to this structure.
     */
    AbstractMemoryStructure &operator=(
        const AbstractMemoryStructure &other);

protected:
    /**
     * @brief Constructs the structure, taking ownership of @p memoryManager.
     * @param memoryManager Pointer to the memory manager to adopt; must not be @c nullptr.
     */
    explicit AbstractMemoryStructure(
        mm::MemoryManager<BlockType> *memoryManager);

    /**
     * @brief Destructor that deletes the owned memory manager.
     */
    ~AbstractMemoryStructure() override;

protected:
    /** @brief Pointer to the memory manager that allocates and releases blocks. */
    mm::MemoryManager<BlockType> *m_memoryManager;
};

template<typename BlockType>
using AMS = AbstractMemoryStructure<BlockType>;

//----------

/**
 * @brief Base class for memory structures backed by contiguous (implicit) storage.
 *
 * Selects between @c CompactMemoryTrivialManager and @c CompactMemoryObjectManager
 * at compile time depending on whether @c DataType is trivial. Provides capacity
 * management and safe/unsafe assign and equals helpers for derived types.
 *
 * @tparam DataType The type of data stored in each @c MemoryBlock.
 */
template<typename DataType>
class ImplicitAbstractMemoryStructure : public AMS<MemoryBlock<DataType>> {
public:
    using BlockType = MemoryBlock<DataType>;
    using MemoryManagerType = std::conditional_t<
        std::is_trivial_v<DataType>,
        mm::CompactMemoryTrivialManager<BlockType>,
        mm::CompactMemoryObjectManager<BlockType>>;

public:
    /**
     * @brief Releases all allocated elements, leaving the structure empty.
     */
    void clear() override;

    /**
     * @brief Returns the total number of blocks that can be held without reallocation.
     */
    std::size_t getCapacity() const;

    /**
     * @brief Ensures the structure can hold at least @p newCapacity blocks without reallocation.
     * @param newCapacity The minimum desired capacity.
     */
    void reserveCapacity(std::size_t newCapacity);

    /**
     * @brief Copy-assigns the contents of @p other to this structure.
     * @param other The source structure to copy from.
     * @return Reference to this structure.
     */
    ImplicitAbstractMemoryStructure &operator=(
        const ImplicitAbstractMemoryStructure &other);

protected:
    /**
     * @brief Constructs the structure with a default initial capacity and no initialised blocks.
     */
    ImplicitAbstractMemoryStructure();

    /**
     * @brief Constructs the structure with the given capacity, optionally allocating all blocks.
     * @param capacity Number of blocks to reserve.
     * @param initBlocks If @c true, all @p capacity blocks are allocated immediately.
     */
    ImplicitAbstractMemoryStructure(size_t capacity, bool initBlocks);

    /**
     * @brief Copy-constructs the structure by duplicating the memory manager of @p other.
     * @param other The source structure to copy from.
     */
    ImplicitAbstractMemoryStructure(
        const ImplicitAbstractMemoryStructure<DataType> &other);

    /**
     * @brief Move-constructs the structure by transferring the memory manager from @p other.
     * @param other The source structure to move from.
     */
    ImplicitAbstractMemoryStructure(
        ImplicitAbstractMemoryStructure<DataType> &&other);

    /**
     * @brief Copies the memory manager state from @p other without type-safety checks.
     * @param other Pointer to the source structure; silently does nothing if @c nullptr.
     */
    void assignUnsafe(const ImplicitAbstractMemoryStructure *other);

    /**
     * @brief Copies the memory manager state from @p other, throwing on type mismatch.
     * @param other Pointer to the source structure.
     * @throws std::runtime_error If @p other is @c nullptr.
     */
    void assignSafe(const ImplicitAbstractMemoryStructure *other);

    /**
     * @brief Compares this structure to @p other without type-safety checks.
     * @param other Pointer to the structure to compare; returns @c false if @c nullptr.
     * @return @c true if both managers hold equivalent blocks.
     */
    bool equalsUnsafe(const ImplicitAbstractMemoryStructure *other) const;

    /**
     * @brief Compares this structure to @p other, throwing on type mismatch.
     * @param other Pointer to the structure to compare.
     * @return @c true if both managers hold equivalent blocks.
     * @throws std::runtime_error If @p other is @c nullptr.
     */
    bool equalsSafe(const ImplicitAbstractMemoryStructure *other) const;

    /**
     * @brief Returns a typed pointer to the underlying memory manager.
     * @return Pointer to the @c MemoryManagerType managing this structure's blocks.
     */
    MemoryManagerType *getMemoryManager() const;

private:
    /** @brief Default capacity used when no size is specified at construction. */
    static const int INIT_CAPACITY = 10;
};

template<typename DataType>
using ImplicitAMS = ImplicitAbstractMemoryStructure<DataType>;

//----------

/**
 * @brief Base class for memory structures backed by dynamic (explicit) allocation.
 *
 * Uses a @c SystemMemoryManager to allocate each block individually from the heap,
 * supporting non-contiguous storage layouts.
 *
 * @tparam BlockType The type of block allocated by the system memory manager.
 */
template<typename BlockType>
class ExplicitAbstractMemoryStructure : public AMS<BlockType> {
public:
    /**
     * @brief Constructs the structure with a default-constructed @c SystemMemoryManager.
     */
    ExplicitAbstractMemoryStructure();
};

template<typename BlockType>
using ExplicitAMS = ExplicitAbstractMemoryStructure<BlockType>;

//----------

template<typename BlockType>
AbstractMemoryStructure<BlockType>::AbstractMemoryStructure(
    mm::MemoryManager<BlockType> *memoryManager
) :
    m_memoryManager(memoryManager)
{
}

template<typename BlockType>
AbstractMemoryStructure<BlockType>::~AbstractMemoryStructure() {
    delete m_memoryManager;
}

template<typename BlockType>
size_t AbstractMemoryStructure<BlockType>::size() const {
    return m_memoryManager->getAllocatedBlockCount();
}

template<typename BlockType>
bool AbstractMemoryStructure<BlockType>::isEmpty() const {
    return this->size() == 0;
}

template<typename BlockType>
AbstractMemoryStructure<BlockType> &AbstractMemoryStructure<BlockType>::operator=(
    const AbstractMemoryStructure &other
) {
    this->assign(other);
    return *this;
}

//----------

template<typename DataType>
ImplicitAbstractMemoryStructure<DataType>::ImplicitAbstractMemoryStructure() :
    ImplicitAMS<DataType>(INIT_CAPACITY, false) {
}

template<typename DataType>
ImplicitAbstractMemoryStructure<DataType>::ImplicitAbstractMemoryStructure(
    std::size_t capacity,
    bool initBlocks
) :
    AMS<MemoryBlock<DataType>>(new MemoryManagerType(capacity))
{
    if (initBlocks) {
        for (std::size_t i = 0; i < capacity; i++) {
            this->m_memoryManager->allocateMemory();
        }
    }
}

template<typename DataType>
ImplicitAbstractMemoryStructure<DataType>::ImplicitAbstractMemoryStructure(
    const ImplicitAMS<DataType> &other
) :
    AMS<MemoryBlock<DataType>>(new MemoryManagerType(*other.getMemoryManager()))
{
}

template<typename DataType>
ImplicitAbstractMemoryStructure<DataType>::ImplicitAbstractMemoryStructure(
    ImplicitAMS<DataType> &&other
) :
    AMS<MemoryBlock<DataType>>(
        new MemoryManagerType(std::move(*other.getMemoryManager())))
{
}

template<typename DataType>
void ImplicitAbstractMemoryStructure<DataType>::clear() {
    this->getMemoryManager()->clear();
}

template<typename DataType>
std::size_t ImplicitAbstractMemoryStructure<DataType>::getCapacity() const {
    return this->getMemoryManager()->getCapacity();
}

template<typename DataType>
void ImplicitAbstractMemoryStructure<DataType>::reserveCapacity(
    std::size_t newCapacity
) {
    this->getMemoryManager()->expandCapacity(newCapacity);
}

template<typename DataType>
ImplicitAbstractMemoryStructure<DataType>&
ImplicitAbstractMemoryStructure<DataType>::operator=(
    const ImplicitAbstractMemoryStructure &other
) {
    this->assignUnsafe(&other);
    return *this;
}

template<typename DataType>
void ImplicitAbstractMemoryStructure<DataType>::assignUnsafe(
    const ImplicitAbstractMemoryStructure *other
) {
    if (other == nullptr) {
        return;
    }
    this->getMemoryManager()->assign(*other->getMemoryManager());
}

template<typename DataType>
void ImplicitAbstractMemoryStructure<DataType>::assignSafe(
    const ImplicitAbstractMemoryStructure *other
) {
    if (other == nullptr) {
        throw std::runtime_error("Invalid assignment. Type missmatch.");
    }
    this->getMemoryManager()->assign(*other->getMemoryManager());
}

template<typename DataType>
bool ImplicitAbstractMemoryStructure<DataType>::equalsUnsafe(
    const ImplicitAbstractMemoryStructure *other
) const {
    if (other == nullptr) {
        return false;
    }
    return this->getMemoryManager()->equals(*other->getMemoryManager());
}

template<typename DataType>
bool ImplicitAbstractMemoryStructure<DataType>::equalsSafe(
    const ImplicitAbstractMemoryStructure *other
) const {
    if (other == nullptr) {
        throw std::runtime_error("Invalid comparison. Type missmatch.");
    }
    return this->getMemoryManager()->equals(*other->getMemoryManager());
}

template<typename DataType>
typename ImplicitAbstractMemoryStructure<DataType>::MemoryManagerType*
ImplicitAbstractMemoryStructure<DataType>::getMemoryManager() const {
    return static_cast<MemoryManagerType*>(AMS<BlockType>::m_memoryManager);
}

//----------

template<typename BlockType>
ExplicitAbstractMemoryStructure<BlockType>::ExplicitAbstractMemoryStructure() :
    AMS<BlockType>(new mm::SystemMemoryManager<BlockType>()) {
}


} // namespace ds::amt

#endif

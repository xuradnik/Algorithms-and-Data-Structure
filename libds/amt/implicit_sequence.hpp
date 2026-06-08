#ifndef LIBDS_AMT_IMPLICIT_SEQUENCE_HPP
#define LIBDS_AMT_IMPLICIT_SEQUENCE_HPP

#include "../constants.hpp"
#include "abstract_memory_type.hpp"
#include "sequence.hpp"


namespace ds::amt {

/**
 * @brief A sequence container backed by implicit (contiguous) memory storage.
 *
 * Combines the @c Sequence and @c ImplicitAMS bases to provide index-based
 * access and in-place element management over a flat memory block.
 *
 * @tparam DataType The type of data stored in each memory block.
 */
template<typename DataType>
class ImplicitSequence :
    public Sequence<MemoryBlock<DataType>>,
    public ImplicitAMS<DataType> {
public:
    using BlockType = MemoryBlock<DataType>;

public:
    /**
     * @brief Constructs an empty sequence with no allocated memory.
     */
    ImplicitSequence() = default;

    /**
     * @brief Constructs a sequence with a given initial capacity.
     * @param capacity Number of blocks to pre-allocate.
     * @param initBlocks Whether to initialise the allocated blocks immediately.
     */
    ImplicitSequence(std::size_t capacity, bool initBlocks);

    /**
     * @brief Copy-constructs the sequence from @p other.
     */
    ImplicitSequence(const ImplicitSequence &other) = default;

    /**
     * @brief Move-constructs the sequence from @p other.
     */
    ImplicitSequence(ImplicitSequence &&other) = default;

    /**
     * @brief Returns the logical index of the given block within the sequence.
     * Assumes @p block is from this sequence.
     * Otherwise, the result is undefined.
     * @param block The block whose index is to be determined.
     * @return Zero-based index of @p block.
     */
    std::size_t calculateIndex(const BlockType &block) const override;

    /**
     * @brief Returns a pointer to the first block, or @c nullptr if empty.
     */
    BlockType *accessFirst() const override;

    /**
     * @brief Returns a pointer to the last block, or @c nullptr if empty.
     */
    BlockType *accessLast() const override;

    /**
     * @brief Returns a pointer to the block at @p index, or @c nullptr if out of range.
     * @param index Zero-based position to access.
     */
    BlockType *access(std::size_t index) const override;

    /**
     * @brief Returns a pointer to the block that follows @p block, or @c nullptr if none.
     * @param block The reference block.
     */
    BlockType *accessNext(const BlockType &block) const override;

    /**
     * @brief Returns a pointer to the block that precedes @p block, or @c nullptr if none.
     * @param block The reference block.
     */
    BlockType *accessPrevious(const BlockType &block) const override;

    /**
     * @brief Allocates and inserts a new block at the front of the sequence.
     */
    BlockType &emplaceFirst() override;

    /**
     * @brief Allocates and appends a new block at the back of the sequence.
     */
    BlockType &emplaceLast() override;

    /**
     * @brief Allocates and inserts a new block at position @p index.
     * @param index Zero-based position at which to insert.
     */
    BlockType &emplace(std::size_t index) override;

    /**
     * @brief Allocates and inserts a new block immediately after @p block.
     * @param block The block after which to insert.
     */
    BlockType &emplaceAfter(BlockType &block) override;

    /**
     * @brief Allocates and inserts a new block immediately before @p block.
     * @param block The block before which to insert.
     */
    BlockType &emplaceBefore(BlockType &block) override;

    /**
     * @brief Removes and releases the first block in the sequence.
     */
    void removeFirst() override;

    /**
     * @brief Removes and releases the last block in the sequence.
     */
    void removeLast() override;

    /**
     * @brief Removes and releases the block at position @p index.
     * @param index Zero-based position of the block to remove.
     */
    void remove(std::size_t index) override;

    /**
     * @brief Removes and releases the block that follows @p block.
     * @param block The reference block whose successor is to be removed.
     */
    void removeNext(BlockType &block) override;

    /**
     * @brief Removes and releases the block that precedes @p block.
     * @param block The reference block whose predecessor is to be removed.
     */
    void removePrevious(BlockType &block) override;

    /**
     * @brief Assigns the contents of @p other to this sequence.
     * @param other The source memory structure to copy from.
     * @return Reference to this sequence.
     */
    AbstractMemoryType &assign(const AbstractMemoryType &other) override;

    /**
     * @brief Checks whether this sequence is equal to @p other.
     * @param other The memory structure to compare against.
     * @return @c true if both sequences contain equivalent data.
     */
    bool equals(const AbstractMemoryType &other) const override;

    /**
     * @brief Copy-assigns @p other to this sequence.
     */
    ImplicitSequence &operator=(const ImplicitSequence &other) = default;

    /**
     * @brief Returns the index of the element after @p currentIndex.
     * @param currentIndex The current position in the sequence.
     * @return The next index, or @c INVALID_INDEX if there is no successor.
     */
    virtual std::size_t indexOfNext(std::size_t currentIndex) const;

    /**
     * @brief Returns the index of the element before @p currentIndex.
     * @param currentIndex The current position in the sequence.
     * @return The previous index, or @c INVALID_INDEX if there is no predecessor.
     */
    virtual std::size_t indexOfPrevious(std::size_t currentIndex) const;

public:
    /**
     * @brief Forward iterator over the data elements of an @c ImplicitSequence.
     *
     * Advances by incrementing the underlying memory-block pointer directly,
     * exploiting the contiguous storage layout.
     */
    class ImplicitSequenceIterator {
    public:
        /**
         * @brief Constructs an iterator pointing to the given memory block.
         * @param block Pointer to the initial memory block.
         */
        explicit ImplicitSequenceIterator(MemoryBlock<DataType> *block);

        /**
         * @brief Pre-increment: advances the iterator to the next element.
         */
        ImplicitSequenceIterator &operator++();

        /**
         * @brief Post-increment: returns a copy before advancing.
         */
        ImplicitSequenceIterator operator++(int);

        /**
         * @brief Returns @c true if both iterators point to the same block.
         * @param other Iterator to compare against.
         */
        bool operator==(const ImplicitSequenceIterator &other) const;

        /**
         * @brief Returns @c true if the iterators point to different blocks.
         * @param other Iterator to compare against.
         */
        bool operator!=(const ImplicitSequenceIterator &other) const;

        /**
         * @brief Dereferences the iterator to obtain a mutable reference to the data.
         */
        DataType &operator*();

        /**
         * @brief Dereferences the iterator to obtain a const reference to the data.
         */
        const DataType &operator*() const;

    private:
        /** @brief Pointer to the current memory block. */
        MemoryBlock<DataType> *m_block;
    };

public:
    using IteratorType = ImplicitSequenceIterator;

    /**
     * @brief Returns an iterator to the first element of the sequence.
     */
    IteratorType begin() const;

    /**
     * @brief Returns a past-the-end iterator for the sequence.
     */
    IteratorType end() const;
};

template<typename DataType>
using IS = ImplicitSequence<DataType>;

//----------

template<typename DataType>
class CyclicImplicitSequence : public IS<DataType> {
public:
    /**
     * @brief Constructs an empty cyclic sequence.
     */
    CyclicImplicitSequence() = default;

    /**
     * @brief Constructs a cyclic sequence with the given initial size.
     * @param initSize Number of blocks to pre-allocate.
     * @param initBlocks Whether to initialise the allocated blocks immediately.
     */
    CyclicImplicitSequence(std::size_t initSize, bool initBlocks);

    /**
     * @brief Returns the index after @p currentIndex, wrapping to 0 at the end.
     * @param currentIndex The current position in the sequence.
     * @return Wrapped next index, or @c INVALID_INDEX if the sequence is empty.
     */
    std::size_t indexOfNext(std::size_t currentIndex) const override;

    /**
     * @brief Returns the index before @p currentIndex, wrapping to the last element.
     * @param currentIndex The current position in the sequence.
     * @return Wrapped previous index, or @c INVALID_INDEX if the sequence is empty.
     */
    std::size_t indexOfPrevious(std::size_t currentIndex) const override;

    /**
     * @brief Assigns the contents of @p other to this cyclic sequence.
     * @param other The source memory structure to copy from.
     * @return Reference to this sequence.
     */
    AbstractMemoryType &assign(const AbstractMemoryType &other) override;

    /**
     * @brief Checks whether this cyclic sequence is equal to @p other.
     * @param other The memory structure to compare against.
     * @return @c true if both sequences contain equivalent data.
     */
    bool equals(const AbstractMemoryType &other) const override;

};

template<typename DataType>
using CIS = CyclicImplicitSequence<DataType>;

//----------

template<typename DataType>
ImplicitSequence<DataType>::ImplicitSequence(
    std::size_t initialSize,
    bool initBlocks
) :
    ImplicitAMS<DataType>(initialSize, initBlocks) {
}

template<typename DataType>
std::size_t ImplicitSequence<DataType>::calculateIndex(
    const BlockType &block
) const {
    return this->getMemoryManager()->calculateIndex(block);
}

template<typename DataType>
typename ImplicitSequence<DataType>::BlockType*
ImplicitSequence<DataType>::accessFirst() const {
    return this->size() > 0
        ? &this->getMemoryManager()->getBlockAt(0)
        : nullptr;
}

template<typename DataType>
typename ImplicitSequence<DataType>::BlockType*
ImplicitSequence<DataType>::accessLast() const {
    const std::size_t size = this->size();
    return size > 0 ? &this->getMemoryManager()->getBlockAt(size - 1) : nullptr;
}

template<typename DataType>
typename ImplicitSequence<DataType>::BlockType*
ImplicitSequence<DataType>::access(std::size_t index) const {
    return index < this->size() ? &this->getMemoryManager()->getBlockAt(index)
                                : nullptr;
}

template<typename DataType>
typename ImplicitSequence<DataType>::BlockType*
ImplicitSequence<DataType>::accessNext(const BlockType &block) const {
    const std::size_t index = this->indexOfNext(this->calculateIndex(block));
    return index != INVALID_INDEX ? &this->getMemoryManager()->getBlockAt(index)
                                : nullptr;
}

template<typename DataType>
typename ImplicitSequence<DataType>::BlockType*
ImplicitSequence< DataType>::accessPrevious(const BlockType &block) const {
    const std::size_t index = this->indexOfPrevious(this->calculateIndex(block));
    return index != INVALID_INDEX ? &this->getMemoryManager()->getBlockAt(index)
                                  : nullptr;
}

template<typename DataType>
typename ImplicitSequence<DataType>::BlockType&
ImplicitSequence<DataType>::emplaceFirst() {
    return *this->getMemoryManager()->allocateMemoryAt(0);
}

template<typename DataType>
typename ImplicitSequence<DataType>::BlockType&
ImplicitSequence<DataType>::emplaceLast() {
    return *this->getMemoryManager()->allocateMemory();
}

template<typename DataType>
typename ImplicitSequence<DataType>::BlockType&
ImplicitSequence<DataType>::emplace(std::size_t index) {
    return *this->getMemoryManager()->allocateMemoryAt(index);
}

template<typename DataType>
typename ImplicitSequence<DataType>::BlockType&
ImplicitSequence<DataType>::emplaceAfter(BlockType &block)
{
    return *this->getMemoryManager()->allocateMemoryAt(
        this->calculateIndex(block) + 1);
}

template<typename DataType>
typename ImplicitSequence<DataType>::BlockType&
ImplicitSequence<DataType>::emplaceBefore(BlockType &block) {
    return *this->getMemoryManager()->allocateMemoryAt(
        this->calculateIndex(block));
}

template<typename DataType>
void ImplicitSequence<DataType>::removeFirst() {
    this->getMemoryManager()->releaseMemory(
        &this->getMemoryManager()->getBlockAt(0));
}

template<typename DataType>
void ImplicitSequence<DataType>::removeLast() {
    this->getMemoryManager()->releaseLast();
}

template<typename DataType>
void ImplicitSequence<DataType>::remove(std::size_t index) {
    this->getMemoryManager()->releaseMemory(
        &this->getMemoryManager()->getBlockAt(index));
}

template<typename DataType>
void ImplicitSequence<DataType>::removeNext(BlockType &block) {
    this->remove(
        this->indexOfNext(this->calculateIndex(block)));
}

template<typename DataType>
void ImplicitSequence<DataType>::removePrevious(BlockType &block) {
    this->remove(
        this->indexOfPrevious(this->calculateIndex(block)));
}

template<typename DataType>
AbstractMemoryType &ImplicitSequence<DataType>::assign(
    const AbstractMemoryType &other
) {
    this->assignSafe(dynamic_cast<const ImplicitSequence<DataType>*>(&other));
    return *this;
}

template<typename DataType>
bool ImplicitSequence<DataType>::equals(const AbstractMemoryType &other) const {
    return this->equalsUnsafe(
        dynamic_cast<const ImplicitSequence<DataType>*>(&other));
}

template<typename DataType>
std::size_t ImplicitSequence<DataType>::indexOfNext(
    std::size_t currentIndex
) const {
    return currentIndex >= this->size() - 1 ? INVALID_INDEX : currentIndex + 1;
}

template<typename DataType>
std::size_t ImplicitSequence<DataType>::indexOfPrevious(
    std::size_t currentIndex
) const {
    return currentIndex <= 0 ? INVALID_INDEX : currentIndex - 1;
}

//----------

template<typename DataType>
ImplicitSequence<DataType>::ImplicitSequenceIterator::ImplicitSequenceIterator(
    MemoryBlock<DataType> *block
) :
    m_block(block) {
}

template<typename DataType>
typename ImplicitSequence<DataType>::ImplicitSequenceIterator&
ImplicitSequence<DataType>::ImplicitSequenceIterator::operator++() {
    ++m_block;
    return *this;
}

template<typename DataType>
typename ImplicitSequence<DataType>::ImplicitSequenceIterator
ImplicitSequence<DataType>::ImplicitSequenceIterator::operator++(int) {
    ImplicitSequenceIterator tmp(*this);
    ++(*this);
    return tmp;
}

template<typename DataType>
bool ImplicitSequence<DataType>::ImplicitSequenceIterator::operator==(
    const ImplicitSequenceIterator &other
) const {
    return m_block == other.m_block;
}

template<typename DataType>
bool ImplicitSequence<DataType>::ImplicitSequenceIterator::operator!=(
    const ImplicitSequenceIterator &other
) const {
    return ! (*this == other);
}

template<typename DataType>
DataType &ImplicitSequence<DataType>::ImplicitSequenceIterator::operator*() {
    return m_block->data;
}

template<typename DataType>
const DataType&
ImplicitSequence<DataType>::ImplicitSequenceIterator::operator*() const {
    return m_block->data;
}

//----------

template<typename DataType>
typename ImplicitSequence<DataType>::ImplicitSequenceIterator
ImplicitSequence<DataType>::begin() const {
    return ImplicitSequenceIterator(this->accessFirst());
}

template<typename DataType>
typename ImplicitSequence<DataType>::ImplicitSequenceIterator
ImplicitSequence<DataType>::end() const {
    return ImplicitSequenceIterator(this->accessFirst() + this->size());
}

//----------

template<typename DataType>
CyclicImplicitSequence<DataType>::CyclicImplicitSequence(
    std::size_t initCapacity,
    bool initBlocks
) :
    IS<DataType>(initCapacity, initBlocks) {
}

template<typename DataType>
std::size_t CyclicImplicitSequence<DataType>::indexOfNext(
    std::size_t currentIndex
) const {
    const std::size_t size = this->size();
    return size != 0 ? currentIndex >= size - 1 ? 0 : currentIndex + 1
                     : INVALID_INDEX;
}

template<typename DataType>
std::size_t CyclicImplicitSequence<DataType>::indexOfPrevious(
    std::size_t currentIndex
) const {
    const std::size_t size = this->size();
    return size != 0 ? currentIndex <= 0 ? size - 1 : currentIndex - 1
                     : INVALID_INDEX;
}

template<typename DataType>
AbstractMemoryType &CyclicImplicitSequence<DataType>::assign(
    const AbstractMemoryType &other
) {
    this->assignSafe(
        dynamic_cast<const CyclicImplicitSequence<DataType>*>(&other));
    return *this;
}

template<typename DataType>
bool CyclicImplicitSequence<DataType>::equals(
    const AbstractMemoryType &other
) const {
    return ImplicitAbstractMemoryStructure<DataType>::equalsUnsafe(
        dynamic_cast<const CyclicImplicitSequence<DataType>*>(&other));
}


} // namespace ds::amt

#endif

#ifndef LIBDS_AMT_EXPLICIT_SEQUENCE_HPP
#define LIBDS_AMT_EXPLICIT_SEQUENCE_HPP

#include "../constants.hpp"
#include "abstract_memory_type.hpp"
#include "sequence.hpp"


namespace ds::amt {


/**
 * @brief Base class for sequence containers backed by explicitly linked (non-contiguous) storage.
 *
 * Each block is individually heap-allocated and linked to its neighbours via pointers.
 * Provides the full @c Sequence interface and maintains @c m_first and @c m_last pointers.
 *
 * @tparam BlockType The block type used for storage.
 */
template<typename BlockType>
class ExplicitSequence :
    public Sequence<BlockType>,
    public ExplicitAMS<BlockType> {
public:
    /**
     * @brief Constructs an empty sequence with no allocated blocks.
     */
    ExplicitSequence();

    /**
     * @brief Destructor that releases all remaining allocated blocks.
     */
    ~ExplicitSequence() override;

    /**
     * @brief Releases all allocated blocks, leaving the sequence empty.
     */
    void clear() override;

    /**
     * @brief Returns the logical index of @p data within the sequence.
     * @param data The block whose position is to be determined.
     * @return Zero-based index of the block, or @c INVALID_INDEX if not found.
     */
    std::size_t calculateIndex(const BlockType &data) const override;

    /**
     * @brief Returns a pointer to the first block, or @c nullptr if the sequence is empty.
     */
    BlockType *accessFirst() const override;

    /**
     * @brief Returns a pointer to the last block, or @c nullptr if the sequence is empty.
     */
    BlockType *accessLast() const override;

    /**
     * @brief Returns a pointer to the block at @p index, or @c nullptr if out of range.
     * @param index Zero-based position to access.
     */
    BlockType *access(std::size_t index) const override;

    /**
     * @brief Returns a pointer to the block following @p block via its @c next pointer.
     * @param block The reference block.
     */
    BlockType *accessNext(const BlockType &block) const override;

    /**
     * @brief Returns a pointer to the block preceding @p block by scanning from the front.
     * @param block The reference block.
     */
    BlockType *accessPrevious(const BlockType &block) const override;

    /**
     * @brief Allocates and inserts a new block at the front of the sequence.
     * @return Reference to the newly inserted block.
     */
    BlockType &emplaceFirst() override;

    /**
     * @brief Allocates and appends a new block at the back of the sequence.
     * @return Reference to the newly inserted block.
     */
    BlockType &emplaceLast() override;

    /**
     * @brief Allocates and inserts a new block at position @p index.
     * @param index Zero-based position at which to insert.
     * @return Reference to the newly inserted block.
     */
    BlockType &emplace(std::size_t index) override;

    /**
     * @brief Allocates and inserts a new block immediately after @p block.
     * @param block The block after which to insert.
     * @return Reference to the newly inserted block.
     */
    BlockType &emplaceAfter(BlockType &block) override;

    /**
     * @brief Allocates and inserts a new block immediately before @p block.
     * @param block The block before which to insert.
     * @return Reference to the newly inserted block.
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
     * @brief Copy-assigns @p other to this sequence.
     */
    ExplicitSequence &operator=(const ExplicitSequence &other) = default;

protected:
    /**
     * @brief Links @p previous and @p next together in the chain.
     * @param previous Block that should precede @p next; may be @c nullptr.
     * @param next Block that should follow @p previous; may be @c nullptr.
     */
    virtual void connectBlocks(BlockType *previous, BlockType *next);

    /**
     * @brief Unlinks @p block from its neighbours without releasing its memory.
     * @param block The block to detach from the chain.
     */
    virtual void disconnectBlock(BlockType *block);

    /**
     * @brief Copies all blocks from @p other.
     * @param other Pointer to the source sequence; silently does nothing if @c this == @p other or @p other is nullptr.
     */
    void assignUnsafe(const ExplicitSequence *other);

    /**
     * @brief Copies all blocks from @p other, throwing on type mismatch.
     * @param other Pointer to the source sequence.
     * @throws std::runtime_error If @p other is @c nullptr.
     */
    void assignSafe(const ExplicitSequence *other);

    /**
     * @brief Compares this sequence to @p other element-wise without type-safety checks.
     * @param other Pointer to the sequence to compare; returns @c true if both point to @c this.
     * @return @c true if both sequences contain equivalent data in the same order.
     */
    bool equalsUnsafe(const ExplicitSequence *other) const;

    /**
     * @brief Compares this sequence to @p other element-wise, throwing on type mismatch.
     * @param other Pointer to the sequence to compare.
     * @return @c true if both sequences contain equivalent data in the same order.
     * @throws std::runtime_error If @p other is @c nullptr.
     */
    bool equalsSafe(const ExplicitSequence *other) const;

protected:
    /** @brief Pointer to the first block in the linked chain, or @c nullptr if empty. */
    BlockType *m_first;

/** @brief Pointer to the last block in the linked chain, or @c nullptr if empty. */
    BlockType *m_last;

public:
    using DataType = typename BlockType::DataT;

    /**
     * @brief Forward iterator over the data elements of an @c ExplicitSequence.
     *
     * Advances by following each block's @c next pointer, terminating at @c nullptr.
     */
    class ExplicitSequenceIterator {
    public:
        /**
         * @brief Constructs an iterator pointing at the given block.
         * @param position Pointer to the initial block, or @c nullptr for the end iterator.
         */
        explicit ExplicitSequenceIterator(BlockType *position);

        /**
         * @brief Pre-increment: advances the iterator to the next block.
         */
        ExplicitSequenceIterator &operator++();

        /**
         * @brief Post-increment: returns a copy before advancing.
         */
        ExplicitSequenceIterator operator++(int);

        /**
         * @brief Returns @c true if both iterators point to the same block.
         * @param other The iterator to compare against.
         */
        bool operator==(const ExplicitSequenceIterator &other) const;

        /**
         * @brief Returns @c true if the iterators point to different blocks.
         * @param other The iterator to compare against.
         */
        bool operator!=(const ExplicitSequenceIterator &other) const;

        /**
         * @brief Dereferences the iterator to obtain a mutable reference to the data.
         */
        DataType &operator*();

        /**
         * @brief Dereferences the iterator to obtain a const reference to the data.
         */
        const DataType &operator*() const;

    private:
        /** @brief Pointer to the block the iterator currently refers to. */
        BlockType *m_position;
    };

public:
    using IteratorType = ExplicitSequenceIterator;

    /**
     * @brief Returns an iterator to the first element of the sequence.
     */
    ExplicitSequenceIterator begin() const;

    /**
     * @brief Returns a past-the-end iterator for the sequence.
     */
    ExplicitSequenceIterator end() const;
};

template<typename BlockType>
using ES = ExplicitSequence<BlockType>;

//----------

/**
 * @brief Memory block for singly linked sequences, extending @c MemoryBlock with a @c next pointer.
 *
 * @tparam DataType The type of data stored in the block.
 */
template<typename DataType>
struct SinglyLinkedSequenceBlock : public MemoryBlock<DataType> {
    SinglyLinkedSequenceBlock<DataType> *next {nullptr};
};

template<typename DataType>
using SLSBlock = SinglyLinkedSequenceBlock<DataType>;

//----------

/**
 * @brief A singly linked explicit sequence where each block carries a single @c next pointer.
 *
 * @tparam DataType The type of data stored in each block.
 */
template<typename DataType>
class SinglyLinkedSequence : public ES<SLSBlock<DataType>> {
public:
    using BlockType = SinglyLinkedSequenceBlock<DataType>;

public:
    /**
     * @brief Constructs an empty singly linked sequence.
     */
    SinglyLinkedSequence() = default;

    /**
     * @brief Copy-constructs the sequence by appending a copy of each block from @p other.
     * @param other The source sequence to copy from.
     */
    SinglyLinkedSequence(const SinglyLinkedSequence &other);

    /**
     * @brief Replaces the contents of this structure with a copy of @p other.
     * @param other The source structure to copy from.
     * @return Reference to this structure.
     */
    AbstractMemoryType &assign(const AbstractMemoryType &other) override;

    /**
     * @brief Returns @c true if this structure is element-wise equal to @p other.
     * @param other The structure to compare against.
     */
    bool equals(const AbstractMemoryType &other) const override;

    /**
     * @brief Copy-assigns @p other to this sequence.
     */
    SinglyLinkedSequence &operator=(const SinglyLinkedSequence &other) = default;
};

template<typename DataType>
using SinglyLS = SinglyLinkedSequence<DataType>;

//----------

template<typename DataType>
class SinglyCyclicLinkedSequence : public SinglyLS<DataType> { };

template<typename DataType>
using SinglyCLS = SinglyCyclicLinkedSequence<DataType>;

//----------

/**
 * @brief Memory block for doubly linked sequences, extending @c MemoryBlock with @c next and @c previous pointers.
 *
 * @tparam DataType The type of data stored in the block.
 */
template<typename DataType>
struct DoublyLinkedSequenceBlock : public MemoryBlock<DataType> {
    DoublyLinkedSequenceBlock<DataType> *next {nullptr};
    DoublyLinkedSequenceBlock<DataType> *previous {nullptr};
};

template<typename DataType>
using DLSBlock = DoublyLinkedSequenceBlock<DataType>;

//----------

/**
 * @brief A doubly linked explicit sequence where each block carries both @c next and @c previous pointers.
 *
 * @tparam DataType The type of data stored in each block.
 */
template<typename DataType>
class DoublyLinkedSequence : public ES<DLSBlock<DataType>> {
public:
    using BlockType = DLSBlock<DataType>;

public:
    /**
     * @brief Constructs an empty doubly linked sequence.
     */
    DoublyLinkedSequence() = default;

    /**
     * @brief Copy-constructs the sequence by appending a copy of each block from @p other.
     * @param other The source sequence to copy from.
     */
    DoublyLinkedSequence(const DoublyLinkedSequence &other);

    /**
     * @brief Returns a pointer to the block at @p index using a bidirectional search.
     *
     * Approaches from the front or back depending on which half @p index falls in,
     * halving the worst-case traversal compared to the singly linked variant.
     *
     * @param index Zero-based position to access.
     */
    BlockType *access(std::size_t index) const override;

    /**
     * @brief Returns a pointer to the predecessor of @p block via its @c previous pointer.
     * @param block The reference block.
     */
    BlockType *accessPrevious(const BlockType &block) const override;

    /**
     * @brief Removes the first block and clears the new head's @c previous pointer.
     */
    void removeFirst() override;

    /**
     * @brief Replaces the contents of this structure with a copy of @p other.
     * @param other The source structure to copy from.
     * @return Reference to this structure.
     */
    AbstractMemoryType &assign(const AbstractMemoryType &other) override;

    /**
     * @brief Returns @c true if this structure is element-wise equal to @p other.
     * @param other The structure to compare against.
     */
    bool equals(const AbstractMemoryType &other) const override;

    /**
     * @brief Copy-assigns @p other to this sequence.
     */
    DoublyLinkedSequence &operator=(const DoublyLinkedSequence &other) = default;

protected:
    /**
     * @brief Links @p previous and @p next bidirectionally.
     * @param previous Block that should precede @p next; may be @c nullptr.
     * @param next Block that should follow @p previous; may be @c nullptr.
     */
    void connectBlocks(BlockType *previous, BlockType *next) override;

    /**
     * @brief Unlinks @p block from both its predecessor and successor, clearing its own pointers.
     * @param block The block to detach from the chain.
     */
    void disconnectBlock(BlockType *block) override;
};

template<typename DataType>
using DoublyLS = DoublyLinkedSequence<DataType>;

//----------

template<typename DataType>
class DoublyCyclicLinkedSequence : public DoublyLS<DataType> { };

template<typename DataType>
using DoublyCLS = DoublyCyclicLinkedSequence<DataType>;

//----------

template<typename BlockType>
ExplicitSequence<BlockType>::ExplicitSequence() :
    m_first(nullptr),
    m_last(nullptr) {
}

template<typename BlockType>
ExplicitSequence<BlockType>::~ExplicitSequence() {
    ExplicitSequence<BlockType>::clear();
}

template<typename BlockType>
void ExplicitSequence<BlockType>::clear() {
    BlockType *current = m_first;
    while (current != nullptr) {
        BlockType *next = this->accessNext(*current);
        this->m_memoryManager->releaseMemory(current);
        current = next;
    }
    m_first = nullptr;
    m_last = nullptr;
}

template<typename BlockType>
std::size_t ExplicitSequence<BlockType>::calculateIndex(
    const BlockType &data
) const {
    std::size_t result = 0;
    BlockType *block = this->findBlockWithProperty([&](BlockType *b) {
        result++;
        return &data == b;
    });
    return block != nullptr ? result - 1 : INVALID_INDEX;
}

template<typename BlockType>
BlockType *ExplicitSequence<BlockType>::accessFirst() const {
    return m_first;
}

template<typename BlockType>
BlockType *ExplicitSequence<BlockType>::accessLast() const {
    return m_last;
}

template<typename BlockType>
BlockType *ExplicitSequence<BlockType>::access(std::size_t index) const {
    BlockType *result = nullptr;

    if (index < this->size()) {
        result = m_first;
        for (std::size_t i = 0; i < index; i++) {
            result = this->accessNext(*result);
        }
    }

    return result;
}

template<typename BlockType>
BlockType *ExplicitSequence<BlockType>::accessNext(
    const BlockType &block
) const {
    return block.next;
}

template<typename BlockType>
BlockType *ExplicitSequence<BlockType>::accessPrevious(
    const BlockType &block
) const {
    return this->findBlockWithProperty([&](BlockType *b) {
        return b->next == &block;
    });
}

template<typename BlockType>
BlockType &ExplicitSequence<BlockType>::emplaceFirst() {
    if (this->size() == 0) {
        m_first = m_last = this->m_memoryManager->allocateMemory();
        return *m_first;
    } else {
        return this->emplaceBefore(*m_first);
    }
}

template<typename BlockType>
BlockType &ExplicitSequence<BlockType>::emplaceLast() {
    if (this->size() == 0) {
        m_first = m_last = this->m_memoryManager->allocateMemory();
        return *m_last;
    } else {
        return this->emplaceAfter(*m_last);
    }
}

template<typename BlockType>
BlockType &ExplicitSequence<BlockType>::emplace(std::size_t index) {
    return index == 0            ? this->emplaceFirst()
         : index == this->size() ? this->emplaceLast()
                                 : this->emplaceAfter(*this->access(index - 1));
}

template<typename BlockType>
BlockType &ExplicitSequence<BlockType>::emplaceAfter(BlockType &block) {
    BlockType *nextBlock = this->accessNext(block);
    BlockType *newBlock = this->m_memoryManager->allocateMemory();

    this->connectBlocks(&block, newBlock);
    this->connectBlocks(newBlock, nextBlock);

    if (m_last == &block) {
        m_last = newBlock;
    }
    return *newBlock;
}

template<typename BlockType>
BlockType &ExplicitSequence<BlockType>::emplaceBefore(BlockType &block) {
    BlockType *prevBlock = this->accessPrevious(block);
    BlockType *newBlock = this->m_memoryManager->allocateMemory();

    this->connectBlocks(prevBlock, newBlock);
    this->connectBlocks(newBlock, &block);

    if (m_first == &block) {
        m_first = newBlock;
    }
    return *newBlock;
}

template<typename BlockType>
void ExplicitSequence<BlockType>::removeFirst() {
    if (m_first == m_last) {
        this->m_memoryManager->releaseMemory(m_first);
        m_first = m_last = nullptr;
    } else {
        BlockType *newFirst = this->accessNext(*m_first);
        this->m_memoryManager->releaseMemory(m_first);
        m_first = newFirst;
    }
}

template<typename BlockType>
void ExplicitSequence<BlockType>::removeLast() {
    if (m_first == m_last) {
        this->m_memoryManager->releaseMemory(m_last);
        m_first = m_last = nullptr;
    } else {
        BlockType *newLast = this->accessPrevious(*m_last);
        this->m_memoryManager->releaseMemory(m_last);
        m_last = newLast;
        m_last->next = nullptr;
    }
}

template<typename BlockType>
void ExplicitSequence<BlockType>::remove(std::size_t index) {
    if (index == 0) {
        this->removeFirst();
    } else {
        this->removeNext(*this->access(index - 1));
    }
}

template<typename BlockType>
void ExplicitSequence<BlockType>::removeNext(BlockType &block) {
    BlockType *deletedBlock = this->accessNext(block);
    if (deletedBlock == m_last) {
        this->removeLast();
    } else {
        this->disconnectBlock(deletedBlock);
        this->m_memoryManager->releaseMemory(deletedBlock);
    }
}

template<typename BlockType>
void ExplicitSequence<BlockType>::removePrevious(BlockType &block) {
    BlockType *deletedBlock = this->accessPrevious(block);
    if (deletedBlock == m_first) {
        this->removeFirst();
    } else {
        this->disconnectBlock(deletedBlock);
        this->m_memoryManager->releaseMemory(deletedBlock);
    }
}

template<typename BlockType>
void ExplicitSequence<BlockType>::connectBlocks(
    BlockType *previous,
    BlockType *next
) {
    if (previous != nullptr) {
        previous->next = next;
    }
}

template<typename BlockType>
void ExplicitSequence<BlockType>::disconnectBlock(BlockType *block) {
    this->connectBlocks(this->accessPrevious(*block), this->accessNext(*block));
    block->next = nullptr;
}

template<typename BlockType>
void ExplicitSequence<BlockType>::assignUnsafe(const ExplicitSequence *other) {
    if (this == other) {
        return;
    }
    this->clear();
    other->processAllBlocksForward([&](const BlockType *b) {
        this->emplaceLast().data = b->data;
    });
}

template<typename BlockType>
void ExplicitSequence<BlockType>::assignSafe(const ExplicitSequence *other) {
    if (other == nullptr) {
        throw std::runtime_error("Invalid assignment. Type missmatch.");
    }
    this->assignUnsafe(other);
}

template<typename BlockType>
bool ExplicitSequence<BlockType>::equalsUnsafe(
    const ExplicitSequence *other
) const {
    if (this == other) {
        return true;
    }

    if (this->size() != other->size()) {
        return false;
    }

    BlockType *myCurrent = m_first;
    BlockType *otherCurrent = other->m_first;
    while (myCurrent != nullptr) {
        if (! (myCurrent->data == otherCurrent->data)) {
            return false;
        } else {
            myCurrent = this->accessNext(*myCurrent);
            otherCurrent = other->accessNext(*otherCurrent);
        }
    }

    return true;
}

template<typename BlockType>
bool ExplicitSequence<BlockType>::equalsSafe(
    const ExplicitSequence *other
) const {
    if (other == nullptr) {
        throw std::runtime_error("Invalid comparison. Type missmatch.");
    }
    return this->equalsUnsafe(other);
}

//----------

template<typename BlockType>
ExplicitSequence<BlockType>::ExplicitSequenceIterator::ExplicitSequenceIterator(
    BlockType *position
) :
    m_position(position) {
}

template<typename BlockType>
typename ExplicitSequence<BlockType>::ExplicitSequenceIterator&
ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator++() {
    m_position = m_position->next;
    return *this;
}

template<typename BlockType>
typename ExplicitSequence<BlockType>::ExplicitSequenceIterator
ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator++(int) {
    ExplicitSequenceIterator tmp(*this);
    this->operator++ ();
    return tmp;
}

template<typename BlockType>
bool ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator==(
    const ExplicitSequenceIterator &other
) const {
    return m_position == other.m_position;
}

template<typename BlockType>
bool ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator!=(
    const ExplicitSequenceIterator &other
) const {
    return m_position != other.m_position;
}

template<typename BlockType>
typename ExplicitSequence<BlockType>::DataType&
ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator*() {
    return m_position->data;
}

template<typename BlockType>
const typename ExplicitSequence<BlockType>::DataType&
ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator*() const {
    return m_position->data;
}

//----------

template<typename BlockType>
typename ExplicitSequence<BlockType>::ExplicitSequenceIterator
ExplicitSequence<BlockType>::begin() const {
    return ExplicitSequenceIterator(m_first);
}

template<typename BlockType>
typename ExplicitSequence<BlockType>::ExplicitSequenceIterator
ExplicitSequence<BlockType>::end() const {
    return ExplicitSequenceIterator(nullptr);
}

//----------

template<typename DataType>
SinglyLinkedSequence<DataType>::SinglyLinkedSequence(
    const SinglyLinkedSequence &other
) :
    ExplicitSequence<SLSBlock<DataType>>()
{
    other.processAllBlocksForward([&](const BlockType *b) {
        this->emplaceLast().data = b->data;
    });
}

template<typename DataType>
AbstractMemoryType &SinglyLinkedSequence<DataType>::assign(
    const AbstractMemoryType &other
) {
    this->assignSafe(
        dynamic_cast<const SinglyLinkedSequence<DataType>*>(&other));
    return *this;
}

template<typename DataType>
bool SinglyLinkedSequence<DataType>::equals(
    const AbstractMemoryType &other
) const {
    return this->equalsUnsafe(
        dynamic_cast<const SinglyLinkedSequence<DataType>*>(&other));
}

template<typename DataType>
DoublyLinkedSequence<DataType>::DoublyLinkedSequence(
    const DoublyLinkedSequence<DataType> &other
)
{
    other.processAllBlocksForward([&](const BlockType* b) {
        this->emplaceLast().data = b->data;
    });
}

template<typename DataType>
typename DoublyLinkedSequence<DataType>::BlockType*
DoublyLinkedSequence<DataType>::access(std::size_t index) const {
    if (index >= this->size()) {
        return nullptr;
    }

    if (index < this->size() / 2) {
        BlockType* result = this->m_first;
        for (size_t i = 0; i < index; ++i) {
            result = result->next;
        }
        return result;
    } else {
        BlockType* result = this->m_last;
        for (size_t i = 0; i < this->size() - index - 1; ++i) {
            result = result->previous;
        }
        return result;
    }
}

template<typename DataType>
typename DoublyLinkedSequence<DataType>::BlockType*
DoublyLinkedSequence<DataType>::accessPrevious(const BlockType &block) const {
    return block.previous;
}

template<typename DataType>
void DoublyLinkedSequence<DataType>::removeFirst() {
    ExplicitSequence<BlockType>::removeFirst();
    if (this->m_first) {
        this->m_first->previous = nullptr;
    }
}

template<typename DataType>
void DoublyLinkedSequence<DataType>::connectBlocks(
    BlockType *previous,
    BlockType *next
) {
    ExplicitSequence<BlockType>::connectBlocks(previous, next);
    if (next) {
        next->previous = previous;
    }
}

template<typename DataType>
void DoublyLinkedSequence<DataType>::disconnectBlock(BlockType *block) {
    ExplicitSequence<BlockType>::disconnectBlock(block);
    block->previous = nullptr;
}

template<typename DataType>
AbstractMemoryType &DoublyLinkedSequence<DataType>::assign(
    const AbstractMemoryType &other
) {
    this->assignSafe(dynamic_cast<const DoublyLinkedSequence<DataType>*>(&other));
    return *this;
}

template<typename DataType>
bool DoublyLinkedSequence<DataType>::equals(
    const AbstractMemoryType &other
) const {
    return this->equalsUnsafe(dynamic_cast<const DoublyLinkedSequence<DataType>*>(&other));
}


} // namespace ds::amt

#endif

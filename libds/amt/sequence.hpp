#ifndef LIBDS_AMT_SEQUENCE_HPP
#define LIBDS_AMT_SEQUENCE_HPP

#include <libds/amt/abstract_memory_type.hpp>

#include <functional>


namespace ds::amt {


/**
 * @brief Abstract base class interface for all sequence memory types.
 *
 * @tparam BlockType The type of memory block used to store individual elements.
 */
template<typename BlockType>
class Sequence : virtual public AMT {
public:
    /**
     * @brief Returns the logical index of the given block within the sequence.
     * Assumes @p block is from this sequence.
     * Otherwise, the result is undefined.
     * @param block The block whose index is to be determined.
     * @return Zero-based index of @p block.
     */
    virtual std::size_t calculateIndex(const BlockType &block) const = 0;

    /**
     * @brief Returns a pointer to the first block, or @c nullptr if empty.
     */
    virtual BlockType *accessFirst() const = 0;

    /**
     * @brief Returns a pointer to the last block, or @c nullptr if empty.
     */
    virtual BlockType *accessLast() const = 0;

    /**
     * @brief Returns a pointer to the block at @p index, or @c nullptr if out of range.
     * @param index Zero-based position to access.
     */
    virtual BlockType *access(std::size_t index) const = 0;

    /**
     * @brief Returns a pointer to the block that follows @p block, or @c nullptr if none.
     * @param block The reference block.
     */
    virtual BlockType *accessNext(const BlockType &block) const = 0;

    /**
     * @brief Returns a pointer to the block that precedes @p block, or @c nullptr if none.
     * @param block The reference block.
     */
    virtual BlockType *accessPrevious(const BlockType &block) const = 0;

    /**
     * @brief Allocates and inserts a new block at the front of the sequence.
     */
    virtual BlockType &emplaceFirst() = 0;

    /**
     * @brief Allocates and appends a new block at the back of the sequence.
     */
    virtual BlockType &emplaceLast() = 0;

    /**
     * @brief Allocates and inserts a new block at position @p index.
     * @param index Zero-based position at which to insert.
     */
    virtual BlockType &emplace(std::size_t index) = 0;

    /**
     * @brief Allocates and inserts a new block immediately after @p block.
     * @param block The block after which to insert.
     */
    virtual BlockType &emplaceAfter(BlockType &block) = 0;

    /**
     * @brief Allocates and inserts a new block immediately before @p block.
     * @param block The block before which to insert.
     */
    virtual BlockType &emplaceBefore(BlockType &block) = 0;

    /**
     * @brief Removes and releases the first block in the sequence.
     */
    virtual void removeFirst() = 0;

    /**
     * @brief Removes and releases the last block in the sequence.
     */
    virtual void removeLast() = 0;

    /**
     * @brief Removes and releases the block at position @p index.
     * @param index Zero-based position of the block to remove.
     */
    virtual void remove(std::size_t index) = 0;

    /**
     * @brief Removes and releases the block that follows @p block.
     * @param block The reference block whose successor is to be removed.
     */
    virtual void removeNext(BlockType &block) = 0;

    /**
     * @brief Removes and releases the block that precedes @p block.
     * @param block The reference block whose predecessor is to be removed.
     */
    virtual void removePrevious(BlockType &block) = 0;

    /**
     * @brief Applies @p operation to every block in forward order, starting from the first.
     * @param operation Callable invoked with a pointer to each block in turn.
     */
    virtual void processAllBlocksForward(
        std::function<void(BlockType *)> operation
    ) const;

    /**
     * @brief Applies @p operation to every block in reverse order, starting from the last.
     * @param operation Callable invoked with a pointer to each block in turn.
     */
    virtual void processAllBlocksBackward(
        std::function<void(BlockType *)> operation
    ) const;

    /**
     * @brief Applies @p operation to @p block and all successors in forward order.
     * @param block The block at which traversal begins.
     * @param operation Callable invoked with a pointer to each visited block.
     */
    virtual void processBlocksForward(
        BlockType *block,
        std::function<void(BlockType *)> operation
    ) const;

    /**
     * @brief Applies @p operation to @p block and all predecessors in reverse order.
     * @param block The block at which traversal begins.
     * @param operation Callable invoked with a pointer to each visited block.
     */
    virtual void processBlocksBackward(
        BlockType *block,
        std::function<void(BlockType *)> operation
    ) const;

    /**
     * @brief Returns the first block for which @p predicate returns @c true.
     * @param predicate Callable that tests each block; traversal stops at the first match.
     * @return Pointer to the matching block, or @c nullptr if no block satisfies the predicate.
     */
    BlockType *findBlockWithProperty(
        std::function<bool(BlockType *)> predicate
    ) const;

    /**
     * @brief Returns the block immediately before the first block satisfying @p predicate.
     * @param predicate Callable that identifies the target block.
     * @return Pointer to the predecessor of the matching block, @c nullptr if the first block
     *         matches or no block satisfies the predicate.
     */
    BlockType *findPreviousToBlockWithProperty(
        std::function<bool(BlockType *)> predicate
    ) const;
};

template<typename BlockType>
void Sequence<BlockType>::processAllBlocksForward(
    std::function<void(BlockType *)> operation
) const {
    this->processBlocksForward(accessFirst(), operation);
}

template<typename BlockType>
void Sequence<BlockType>::processAllBlocksBackward(
    std::function<void(BlockType *)> operation
) const {
    this->processBlocksBackward(accessLast(), operation);
}

template<typename BlockType>
void Sequence<BlockType>::processBlocksForward(
    BlockType *block,
    std::function<void(BlockType *)> operation
) const {
    while (block != nullptr) {
        operation(block);
        block = this->accessNext(*block);
    }
}

template<typename BlockType>
void Sequence<BlockType>::processBlocksBackward(
    BlockType *block,
    std::function<void(BlockType *)> operation
) const {
    while (block != nullptr) {
        operation(block);
        block = this->accessPrevious(*block);
    }
}

template<typename BlockType>
BlockType *Sequence<BlockType>::findBlockWithProperty(
    std::function<bool(BlockType *)> predicate
) const {
    BlockType *block = accessFirst();

    while (block != nullptr && ! predicate(block)) {
        block = this->accessNext(*block);
    }

    return block;
}

template<typename BlockType>
BlockType *Sequence<BlockType>::findPreviousToBlockWithProperty(
    std::function<bool(BlockType *)> predicate
) const {
    BlockType *blockWithProperty = accessFirst();

    if (predicate(blockWithProperty)) {
        return nullptr;
    } else {
        BlockType *prevToBlockWithProperty = blockWithProperty;
        blockWithProperty = this->accessNext(*blockWithProperty);

        while (blockWithProperty != nullptr && ! predicate(blockWithProperty)) {
            prevToBlockWithProperty = blockWithProperty;
            blockWithProperty = this->accessNext(*blockWithProperty);
        }

        return blockWithProperty != nullptr ? prevToBlockWithProperty : nullptr;
    }
}

} // namespace ds::amt

#endif

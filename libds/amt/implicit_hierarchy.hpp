#ifndef LIBDS_AMT_IMPLICIT_HIERARCHY_HPP
#define LIBDS_AMT_IMPLICIT_HIERARCHY_HPP

#include "../utils.hpp"
#include "abstract_memory_type.hpp"
#include "hierarchy.hpp"

#include <cmath>


namespace ds::amt {


/**
 * @brief A K-way hierarchy backed by implicit (contiguous) memory storage.
 *
 * Nodes are stored in a compact memory and their parent/son relationships are
 * computed arithmetically from indices, eliminating the need for
 * explicit pointer fields. Modifications (emplaceRoot, emplaceSon, etc.)
 * are not supported and will throw; use @c emplaceLastLeaf and @c removeLastLeaf
 * to grow or shrink the hierarchy.
 *
 * @tparam DataType The type of data stored in each node block.
 * @tparam K The maximum number of sons per node.
 */
template<typename DataType, std::size_t K>
class ImplicitHierarchy :
    virtual public KWayHierarchy<MemoryBlock<DataType>, K>,
    public ImplicitAMS<DataType> {
public:
    using BlockType = MemoryBlock<DataType>;

public:
    /**
     * @brief Constructs an empty implicit hierarchy.
     */
    ImplicitHierarchy() = default;

    /**
     * @brief Copy-constructs the hierarchy from @p other.
     */
    ImplicitHierarchy(const ImplicitHierarchy &other) = default;

    /**
     * @brief Returns the depth of @p node, where the root has level 0.
     * @param node The node whose depth is queried.
     * @return Number of "jumps" on the path from the root to @p node.
     */
    std::size_t level(const BlockType &node) const override;

    /**
     * @brief Returns the depth of the node at the given @p index.
     * @param index Zero-based position of the node in the underlying memory.
     * @return Depth level of the node at @p index, where the root is at level 0.
     */
    std::size_t level(std::size_t index) const;

    /**
     * @brief Returns the number of sons of @p node.
     * @param node The node whose degree is queried.
     * @return Degree of @p node.
     */
    std::size_t degree(const BlockType &node) const override;

    /**
     * @brief Returns the number of sons of the node at the given flat array @p index.
     * @param index Zero-based position of the node in the underlying array.
     * @return Number of direct sons of the node at @p index.
     */
    std::size_t degree(std::size_t index) const;

    /**
     * @brief Returns the total number of nodes in the hierarchy.
     * @return 0 if the hierarchy is empty, otherwise the count from the root.
     */
    std::size_t nodeCount() const override;

    /**
     * @brief Returns the number of nodes in the sub-hierarchy rooted at @p node.
     * @param node The root of the sub-hierarchy to count.
     * @return the number of nodes in the sub-hierarchy rooted at @p node.
     */
    std::size_t nodeCount(const BlockType &node) const override;

    /**
     * @brief Returns a pointer to the root node, or @c nullptr if the hierarchy is empty.
     * @return Pointer to the root node or @c nullptr if empty.
     */
    BlockType *accessRoot() const override;

    /**
     * @brief Returns a pointer to the parent of @p node, or @c nullptr if @p node is the root.
     * @param node The node whose parent is requested.
     * @return Pointer to the parent of @p node or @c nullptr if @p node is root.
     */
    BlockType *accessParent(const BlockType &node) const override;

    /**
     * @brief Returns a pointer to the @p sonOrder-th son of @p node, or @c nullptr if absent.
     * @param node The parent node.
     * @param sonOrder Zero-based index of the son to access.
     * @return Pointer to the @p sonOrder-th son of @p node, or @c nullptr if absent.
     */
    BlockType *accessSon(
        const BlockType &node,
        std::size_t sonOrder
    ) const override;

    /**
     * @brief Returns a pointer to the last allocated node (the rightmost leaf), or @c nullptr if empty.
     * @return Pointer to the block at the highest index, or @c nullptr if the hierarchy is empty.
     */
    BlockType *accessLastLeaf() const;

    /**
     * @brief Allocates and installs a new root node in the hierarchy.
     * @return Reference to the newly created root block.
     */
    BlockType &emplaceRoot() override;

    /**
     * @brief Replaces the root of the hierarchy with @p newRoot.
     * @param newRoot Pointer to the new root block; may be @c nullptr to clear the root.
     */
    void changeRoot(BlockType *newRoot) override;

    /**
     * @brief Allocates and inserts a new son at position @p sonOrder under @p parent.
     * @param parent The node that will become the parent.
     * @param sonOrder Zero-based index at which to insert the son.
     * @return Reference to the newly created son block.
     */
    BlockType &emplaceSon(BlockType &parent, std::size_t sonOrder) override;

    /**
     * @brief Replaces the @p sonOrder-th son of @p parent with @p newSon.
     * @param parent The parent node.
     * @param sonOrder Zero-based index of the son slot to update.
     * @param newSon Pointer to the replacement son; may be @c nullptr to remove the slot.
     */
    void changeSon(
        BlockType &parent,
        std::size_t sonOrder,
        BlockType *newSon
    ) override;

    /**
     * @brief Replaces the @p sonOrder-th son of @p parent with @p newSon.
     * @param parent The parent node.
     * @param sonOrder Zero-based index of the son slot to update.
     * @param newSon Pointer to the replacement son; may be @c nullptr to remove the slot.
     */
    void removeSon(BlockType &parent, std::size_t sonOrder) override;

    /**
     * @brief Allocates a new block appended at the end of the underlying memory.
     *
     * The new block becomes the last leaf in the implicit hierarchy layout.
     *
     * @return Reference to the newly allocated block.
     */
    BlockType &emplaceLastLeaf();

    /**
     * @brief Releases the last allocated block, removing the current last leaf.
     */
    void removeLastLeaf();

    /**
     * @brief Returns the index of the parent of @p node.
     * @param node The node whose parent index is requested.
     * @return Index of the parent block, or @c INVALID_INDEX if @p node is the root.
     */
    std::size_t indexOfParent(const BlockType &node) const;

    /**
     * @brief Returns the index of the parent of the node at @p index.
     * @param index Zero-based position of the node whose parent index is requested.
     * @return Index of the parent block, or @c INVALID_INDEX if @p index is 0 (the root).
     */
    std::size_t indexOfParent(std::size_t index) const;

    /**
     * @brief Returns the index of the @p sonOrder-th son of @p node.
     * @param node The parent node.
     * @param sonOrder Zero-based index of the son slot.
     * @return Flat array index of the requested son block.
     */
    std::size_t indexOfSon(const BlockType &node, std::size_t sonOrder) const;

    /**
     * @brief Returns the index of the @p sonOrder-th son of the node at @p parentIndex.
     * @param parentIndex Zero-based flat array index of the parent node.
     * @param sonOrder Zero-based index of the son slot.
     * @return Flat array index of the requested son block.
     */
    std::size_t indexOfSon(std::size_t parentIndex, std::size_t sonOrder) const;

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
    ImplicitHierarchy &operator=(const ImplicitHierarchy &other) = default;
};


/**
 * @brief A binary implicit hierarchy specialising @c ImplicitHierarchy with K = 2.
 *
 * Combines @c BinaryHierarchy and @c ImplicitHierarchy to provide the named
 * left/right son interface over contiguous memory storage.
 *
 * @tparam DataType The type of data stored in each node block.
 */
template<typename DataType>
class BinaryImplicitHierarchy :
    public BinaryHierarchy<MemoryBlock<DataType>>,
    public ImplicitHierarchy<DataType, 2> {
};

//----------

template<typename DataType, std::size_t K>
std::size_t ImplicitHierarchy<DataType, K>::level(const BlockType &node) const {
    return this->level(this->getMemoryManager()->calculateIndex(node));
}

template<typename DataType, std::size_t K>
std::size_t ImplicitHierarchy<DataType, K>::level(std::size_t index) const {
    return static_cast<std::size_t>(
        std::floor(std::log((K - 1) * (index + 1)) / std::log(K)));
}

template<typename DataType, std::size_t K>
std::size_t ImplicitHierarchy<DataType, K>::degree(
    const BlockType &node
) const {
    return this->degree(this->getMemoryManager()->calculateIndex(node));
}

template<typename DataType, std::size_t K>
std::size_t ImplicitHierarchy<DataType, K>::degree(std::size_t index) const {
    const std::size_t currentLevel = this->level(index);
    const std::size_t indexOfLast = this->size() - 1;
    const std::size_t depth = this->level(indexOfLast);

    if (currentLevel == depth) {
        return 0;
    } else if (currentLevel == depth - 1) {
        const std::size_t indexOfLastsParent = indexOfParent(indexOfLast);
        if (index < indexOfLastsParent) {
            return K;
        } else if (index > indexOfLastsParent) {
            return 0;
        } else {
            const std::size_t mod = (this->size() - 1) % K;
            return mod == 0 ? K : mod;
        }
    } else {
        return K;
    }
}

template<typename DataType, std::size_t K>
std::size_t ImplicitHierarchy<DataType, K>::nodeCount() const {
    return this->getMemoryManager()->getAllocatedBlockCount();
}

template<typename DataType, std::size_t K>
std::size_t ImplicitHierarchy<DataType, K>::nodeCount(
    const BlockType &node
) const {
    return this->getMemoryManager()->calculateIndex(node) == 0
             ? this->nodeCount()
             : Hierarchy<BlockType>::nodeCount(node);
}

template<typename DataType, std::size_t K>
typename ImplicitHierarchy<DataType, K>::BlockType*
ImplicitHierarchy<DataType, K>::accessRoot() const {
    return this->size() > 0 ? &this->getMemoryManager()->getBlockAt(0)
                            : nullptr;
}

template<typename DataType, std::size_t K>
typename ImplicitHierarchy<DataType, K>::BlockType*
ImplicitHierarchy<DataType, K>::accessParent(const BlockType &node) const {
    const std::size_t index = this->indexOfParent(node);
    return INVALID_INDEX != index ? &this->getMemoryManager()->getBlockAt(index)
                                  : nullptr;
}

template<typename DataType, std::size_t K>
typename ImplicitHierarchy<DataType, K>::BlockType*
ImplicitHierarchy<DataType, K>::accessSon(
    const BlockType &node,
    std::size_t sonOrder
) const {
    const std::size_t index = this->indexOfSon(node, sonOrder);
    return index < this->size() ? &this->getMemoryManager()->getBlockAt(index)
                                : nullptr;
}

template<typename DataType, std::size_t K>
typename ImplicitHierarchy<DataType, K>::BlockType*
ImplicitHierarchy<DataType, K>::accessLastLeaf() const {
    const std::size_t size = this->size();
    return size != 0 ? &this->getMemoryManager()->getBlockAt(size - 1)
                     : nullptr;
}

template<typename DataType, std::size_t K>
typename ImplicitHierarchy<DataType, K>::BlockType&
ImplicitHierarchy<DataType, K>::emplaceRoot() {
    throw utils::unavailable_function_call();
}

template<typename DataType, std::size_t K>
void ImplicitHierarchy<DataType, K>::changeRoot(BlockType *newRoot) {
    (void)newRoot;
    throw utils::unavailable_function_call();
}

template<typename DataType, std::size_t K>
typename ImplicitHierarchy<DataType, K>::BlockType&
ImplicitHierarchy<DataType, K>::emplaceSon(
    BlockType &parent,
    std::size_t sonOrder
) {
    (void)parent;
    (void)sonOrder;
    throw utils::unavailable_function_call();
}

template<typename DataType, std::size_t K>
void ImplicitHierarchy<DataType, K>::changeSon(
    BlockType &parent,
    std::size_t sonOrder,
    BlockType *newSon
) {
    (void)parent;
    (void)sonOrder;
    (void)newSon;
    throw utils::unavailable_function_call();
}

template<typename DataType, std::size_t K>
void ImplicitHierarchy<DataType, K>::removeSon(
    BlockType &parent,
    std::size_t sonOrder
) {
    (void)parent;
    (void)sonOrder;
    throw utils::unavailable_function_call();
}

template<typename DataType, std::size_t K>
typename ImplicitHierarchy<DataType, K>::BlockType&
ImplicitHierarchy<DataType, K>::emplaceLastLeaf() {
    return *this->getMemoryManager()->allocateMemory();
}

template<typename DataType, std::size_t K>
void ImplicitHierarchy<DataType, K>::removeLastLeaf() {
    this->getMemoryManager()->releaseLast();
}

template<typename DataType, std::size_t K>
std::size_t ImplicitHierarchy<DataType, K>::indexOfParent(
    const BlockType &node
) const {
    return this->indexOfParent(this->getMemoryManager()->calculateIndex(node));
}

template<typename DataType, std::size_t K>
std::size_t ImplicitHierarchy<DataType, K>::indexOfParent(
    std::size_t index
) const {
    return 0 == index ? INVALID_INDEX : (index - 1) / K;
}

template<typename DataType, std::size_t K>
std::size_t ImplicitHierarchy<DataType, K>::indexOfSon(
    const BlockType &node,
    std::size_t sonOrder
) const {
    return this->indexOfSon(
        this->getMemoryManager()->calculateIndex(node),
        sonOrder);
}

template<typename DataType, std::size_t K>
std::size_t ImplicitHierarchy<DataType, K>::indexOfSon(
    std::size_t indexOfParent,
    std::size_t sonOrder
) const {
    return K * indexOfParent + sonOrder + 1;
}

template<typename DataType, std::size_t K>
AbstractMemoryType &ImplicitHierarchy<DataType, K>::assign(
    const AbstractMemoryType &other
) {
    this->assignUnsafe(
        dynamic_cast<const ImplicitHierarchy<DataType, K>*>(&other));
    return *this;
}

template<typename DataType, std::size_t K>
bool ImplicitHierarchy<DataType, K>::equals(
    const AbstractMemoryType &other
) const {
    return this->equalsUnsafe(
        dynamic_cast<const ImplicitHierarchy<DataType, K>*>(&other));
}


} // namespace ds::amt

#endif

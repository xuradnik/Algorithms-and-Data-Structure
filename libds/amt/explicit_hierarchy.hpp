#ifndef LIBDS_AMT_EXPLICIT_HIERARCHY_HPP
#define LIBDS_AMT_EXPLICIT_HIERARCHY_HPP

#include <libds/amt/abstract_memory_type.hpp>
#include <libds/amt/hierarchy.hpp>
#include <libds/amt/implicit_sequence.hpp>

#include <functional>

namespace ds::amt {

/**
 * @brief Base class for hierarchy structures backed by explicitly linked (non-contiguous) storage.
 *
 * Each node is individually heap-allocated and linked to its parent via a pointer
 * stored in the block. Provides root management, parent access, and safe/unsafe
 * assign and equals helpers for derived types.
 *
 * @tparam BlockType The node block type; must expose a @c parent pointer member.
 */
template<typename BlockType>
class ExplicitHierarchy :
    virtual public Hierarchy<BlockType>,
    public ExplicitAMS<BlockType> {
public:
    /**
     * @brief Constructs an empty hierarchy with no root.
     */
    ExplicitHierarchy();

    /**
     * @brief Copy construction is explicitly disabled.
     * Child classes provide copy constructors that call default constructor
     * of this class.
     */
    ExplicitHierarchy(const ExplicitHierarchy &other) = delete;

    /**
     * @brief Releases all nodes in post-order, leaving the hierarchy empty.
     */
    void clear() override;

    /**
     * @brief Returns @c true if the hierarchy has no root node.
     * @return @c true if hierarchy is empty.
     */
    bool isEmpty() const override;

    /**
     * @brief Returns a pointer to the root node, or @c nullptr if the hierarchy is empty.
     * @return Pointer to the root block, or @c nullptr if empty.
     */
    BlockType *accessRoot() const override;

    /**
     * @brief Returns a pointer to the parent of @p node, or @c nullptr if @p node is the root.
     * @param node The node whose parent is requested.
     * @return Pointer to the parent block, or @c nullptr if @p node is the root.
     */
    BlockType *accessParent(const BlockType &node) const override;

    /**
     * @brief Emplaces root to an empty hierarchy.
     *
     * Hierarchy must be empty before emplacing.
     * Otherwise, the behavior is undefined.
     *
     * @return Reference to the newly created root.
     */
    BlockType &emplaceRoot() override;

    /**
     * @brief Chages root to @p newRoot.
     *
     * @p newRoot must be a node from this hierarchy and relationships in the
     * rest of the hierarchy must be correctly adjusted. Otherwise, the bavaior
     * is undefined.
     *
     * @param newRoot New hierarchy root.
     */
    void changeRoot(BlockType *newRoot) override;

    /**
     * @brief Copy-assigns @p other to this hierarchy.
     */
    ExplicitHierarchy &operator=(const ExplicitHierarchy &other) = default;

protected:
    /**
     * @brief Copies all nodes from @p other, throwing on type mismatch.
     * @param other Pointer to the source hierarchy.
     * @throws std::runtime_error If @p other is @c nullptr.
     */
    void assignSafe(const ExplicitHierarchy *other);

    /**
     * @brief Copies all nodes from @p other without type-safety checks.
     * @param other Pointer to the source hierarchy; silently does nothing if @c nullptr.
     */
    void assignUnsafe(const ExplicitHierarchy *other);

    /**
     * @brief Compares this hierarchy to @p other recursively, throwing on type mismatch.
     * @param other Pointer to the hierarchy to compare.
     * @return @c true if both hierarchies contain equivalent data in the same structure.
     * @throws std::runtime_error If @p other is @c nullptr.
     */
    bool equalsSafe(const ExplicitHierarchy *other) const;

    /**
     * @brief Compares this hierarchy to @p other recursively without type-safety checks.
     * @param other Pointer to the hierarchy to compare; returns @c false if @c nullptr.
     * @return @c true if both hierarchies contain equivalent data in the same structure.
     */
    bool equalsUnsafe(const ExplicitHierarchy *other) const;

protected:
    /** @brief Pointer to the root, or @c nullptr if empty. */
    BlockType *m_root;
};

template<typename BlockType>
using EH = ExplicitHierarchy<BlockType>;

//----------

/**
 * @brief Node block for multi-way explicit hierarchies.
 *
 * Extends @c MemoryBlock with a @c parent pointer and a sequence of
 * son pointers that can grow to accommodate any number of sons.
 *
 * @tparam DataType The type of data stored in the block.
 */
template<typename DataType>
struct MultiWayExplicitHierarchyBlock : MemoryBlock<DataType> {
    /** @brief Pointer to the parent node, or @c nullptr if this block is the root. */
    MultiWayExplicitHierarchyBlock *parent {nullptr};

    /** @brief Sequence holding pointers to each son of this node. */
    IS<MultiWayExplicitHierarchyBlock<DataType> *> sons {};
};

/**
 * @brief An explicit hierarchy in which each node may have an arbitrary number of sons.
 *
 * Sons are stored in a dynamic @c ImplicitSequence, allowing nodes to grow and shrink
 * their son list freely at runtime.
 *
 * @tparam DataType The type of data stored in each node block.
 */
template<typename DataType>
class MultiWayExplicitHierarchy :
    public ExplicitHierarchy<MultiWayExplicitHierarchyBlock<DataType>> {
public:
    using BlockType = MultiWayExplicitHierarchyBlock<DataType>;

public:
    /**
     * @brief Constructs an empty multi-way explicit hierarchy.
     */
    MultiWayExplicitHierarchy() = default;

    /**
     * @brief Copy-constructs the hierarchy by performing a deep copy of @p other.
     * @param other The source hierarchy to copy from.
     */
    MultiWayExplicitHierarchy(const MultiWayExplicitHierarchy &other);

    /**
     * @brief Destructor that releases all allocated nodes.
     */
    ~MultiWayExplicitHierarchy() override;

    /**
     * @brief Returns the number of sons of @p node.
     * @param node The node whose degree is queried.
     * @return Number of sons currently in @p node's son sequence.
     */
    std::size_t degree(const BlockType &node) const override;

    /**
     * @brief Returns a pointer to the @p sonOrder-th son of @p node, or @c nullptr if absent.
     * @param node The parent node.
     * @param sonOrder Zero-based index of the son to access.
     * @return Pointer to the son block, or @c nullptr if that slot is out of range.
     */
    BlockType *accessSon(
        const BlockType &node,
        std::size_t sonOrder
    ) const override;

    /**
     * @brief Allocates and inserts a new son at position @p sonOrder under @p parent.
     *
     * Existing sons from @p sonOrder onwards are shifted to create space
     * for the new node -- just like when inserting into an implicit sequence.
     * The caller is reponsible of ensuring that @p sonOrder
     * is valid for @p parent.
     *
     * @param parent The node that will become the parent.
     * @param sonOrder Zero-based index at which to insert the new son.
     * @return Reference to the newly created son block.
     */
    BlockType &emplaceSon(BlockType &parent, std::size_t sonOrder) override;

    /**
     * @brief Replaces the @p sonOrder-th son of @p parent with @p newSon.
     *
     * The caller must ensure that the old son is properly placed at some
     * other place in this hierarchy. Otherwise, the behavior is undefined.
     *
     * @param parent The parent node.
     * @param sonOrder Zero-based index of the son slot to update.
     * @param newSon Pointer to the replacement son; may be @c nullptr.
     */
    void changeSon(
        BlockType &parent,
        std::size_t sonOrder,
        BlockType *newSon
    ) override;

    /**
     * @brief Removes and recursively releases the @p sonOrder-th son of @p parent.
     * @param parent The parent node.
     * @param sonOrder Zero-based index of the son to remove.
     */
    void removeSon(BlockType &parent, std::size_t sonOrder) override;

    /**
     * @brief Assigns the contents of @p other to this hierarchy.
     * @param other The source memory structure to copy from.
     * @return Reference to this hierarchy.
     */
    AbstractMemoryType &assign(const AbstractMemoryType &other) override;

    /**
     * @brief Checks whether this hierarchy is equal to @p other.
     * @param other The memory structure to compare against.
     * @return @c true if both hierarchies contain equivalent data.
     */
    bool equals(const AbstractMemoryType &other) const override;

    /**
     * @brief Copy-assigns @p other to this hierarchy.
     */
    MultiWayExplicitHierarchy &operator=(
        const MultiWayExplicitHierarchy &other) = default;
};

template<typename DataType>
using MultiWayEH = MultiWayExplicitHierarchy<DataType>;

//----------

/**
 * @brief Node block for K-way explicit hierarchies.
 *
 * Extends @c MemoryBlock with a @c parent pointer and a fixed-capacity sequence
 * of @c K son pointers, pre-allocated and initialised at construction.
 *
 * @tparam DataType The type of data stored in the block.
 * @tparam K The fixed number of son slots per node.
 */
template<typename DataType, std::size_t K>
struct KWayExplicitHierarchyBlock : public MemoryBlock<DataType> {
    /** @brief Pointer to the parent node, or @c nullptr if this block is the root. */
    KWayExplicitHierarchyBlock *parent {nullptr};

    /** @brief Fixed-size sequence of @c K son pointers, all initialised to @c nullptr. */
    IS<KWayExplicitHierarchyBlock<DataType, K> *> sons {K, true};

    /**
     * @brief Initializes all son pointers to @c nullptr.
     */
    KWayExplicitHierarchyBlock();
};

/**
 * @brief An explicit hierarchy in which each node has exactly @c K son slots.
 *
 * Son slots are pre-allocated at node creation; slots not yet occupied hold
 * @c nullptr. This gives O(1) son access by index at the cost of a fixed
 * memory footprint per node regardless of actual son count.
 *
 * @tparam DataType The type of data stored in each node block.
 * @tparam K The fixed number of son slots per node.
 */
template<typename DataType, std::size_t K>
class KWayExplicitHierarchy :
    public KWayHierarchy<KWayExplicitHierarchyBlock<DataType, K>, K>,
    public ExplicitHierarchy<KWayExplicitHierarchyBlock<DataType, K>> {
public:
    using BlockType = KWayExplicitHierarchyBlock<DataType, K>;

public:
    /**
     * @brief Constructs an empty K-way explicit hierarchy.
     */
    KWayExplicitHierarchy() = default;

    /**
     * @brief Copy-constructs the hierarchy by performing a deep copy of @p other.
     * @param other The source hierarchy to copy from.
     */
    KWayExplicitHierarchy(const KWayExplicitHierarchy &other);

    /**
     * @brief Destructor that releases all allocated nodes.
     */
    ~KWayExplicitHierarchy() override;

    /**
     * @brief Returns the number of non-null sons of @p node.
     * @param node The node whose degree is queried.
     * @return Count of occupied (non-null) son slots in @p node.
     */
    std::size_t degree(const BlockType &node) const override;

    /**
     * @brief Returns a pointer to the @p sonOrder-th son of @p node, or @c nullptr if absent.
     * @param node The parent node.
     * @param sonOrder Zero-based index of the son slot to access.
     * @return Pointer to the son block, or @c nullptr if that slot is empty or out of range.
     */
    BlockType *accessSon(
        const BlockType &node,
        std::size_t sonOrder
    ) const override;

    /**
     * @brief Allocates a new son and places it into slot @p sonOrder of @p parent.
     *
     * Silently overwrites the pointer to the old son at index @p sonOrder
     * (if present). The caller is responsible for placing the old son at some
     * other place in the hierarchy. Otherwise, the behavior is undefined.
     *
     * @param parent The node that will become the parent.
     * @param sonOrder Zero-based index of the son slot to fill.
     * @return Reference to the newly created son block.
     */
    BlockType &emplaceSon(BlockType &parent, std::size_t sonOrder) override;

    /**
     * @brief Replaces the @p sonOrder-th son of @p parent with @p newSon.
     *
     * The caller must ensure that the old son is properly placed at some
     * other place in this hierarchy. Otherwise, the behavior is undefined.
     *
     * @param parent The parent node.
     * @param sonOrder Zero-based index of the son slot to update.
     * @param newSon Pointer to the replacement son; may be @c nullptr to clear the slot.
     */
    void changeSon(
        BlockType &parent,
        std::size_t sonOrder,
        BlockType *newSon
    ) override;

    /**
     * @brief Recursively releases the @p sonOrder-th son sub-hierarchy of @p parent and clears the slot.
     * @param parent The parent node.
     * @param sonOrder Zero-based index of the son to remove.
     */
    void removeSon(BlockType &parent, std::size_t sonOrder) override;

    /**
     * @brief Assigns the contents of @p other to this hierarchy.
     * @param other The source memory structure to copy from.
     * @return Reference to this hierarchy.
     */
    AbstractMemoryType &assign(const AbstractMemoryType &other) override;

    /**
     * @brief Checks whether this hierarchy is equal to @p other.
     * @param other The memory structure to compare against.
     * @return @c true if both hierarchies contain equivalent data.
     */
    bool equals(const AbstractMemoryType &other) const override;
};

template<typename DataType, std::size_t K>
using KWayEH = KWayExplicitHierarchy<DataType, K>;

//----------

/**
 * @brief Node block for binary explicit hierarchies.
 *
 * Extends @c MemoryBlock with explicit @c parent, @c left, and @c right pointers,
 * enabling O(1) access in all three directions.
 *
 * @tparam DataType The type of data stored in the block.
 */
template<typename DataType>
struct BinaryExplicitHierarchyBlock : public MemoryBlock<DataType> {
    /** @brief Pointer to the parent node, or @c nullptr if this block is the root. */
    BinaryExplicitHierarchyBlock *parent {nullptr};

    /** @brief Pointer to the left son, or @c nullptr if absent. */
    BinaryExplicitHierarchyBlock *left {nullptr};

    /** @brief Pointer to the right son, or @c nullptr if absent. */
    BinaryExplicitHierarchyBlock *right {nullptr};
};

/**
 * @brief An explicit binary hierarchy where each node carries direct left and right son pointers.
 *
 * All binary operations (@c accessLeftSon, @c emplaceLeftSon, etc.) are O(1) through
 * the @c left and @c right members of @c BinaryExplicitHierarchyBlock. The generic
 * @c accessSon and @c emplaceSon dispatch to the named left/right variants by @p sonOrder.
 *
 * @tparam DataType The type of data stored in each node block.
 */
template<typename DataType>
class BinaryExplicitHierarchy :
    public BinaryHierarchy<BinaryExplicitHierarchyBlock<DataType>>,
    public ExplicitHierarchy<BinaryExplicitHierarchyBlock<DataType>> {
public:
    using BlockType = BinaryExplicitHierarchyBlock<DataType>;

public:
    /**
     * @brief Constructs an empty binary explicit hierarchy.
     */
    BinaryExplicitHierarchy() = default;

    /**
     * @brief Copy-constructs the hierarchy by performing a deep copy of @p other.
     * @param other The source hierarchy to copy from.
     */
    BinaryExplicitHierarchy(const BinaryExplicitHierarchy &other);

    /**
     * @brief Destructor that releases all allocated nodes.
     */
    ~BinaryExplicitHierarchy() override;

    /**
     * @brief Returns the number of non-null sons of @p node (0, 1, or 2).
     * @param node The node whose degree is queried.
     * @return Count of non-null son pointers among @c left and @c right.
     */
    std::size_t degree(const BlockType &node) const override;

    /**
     * @brief Returns a pointer to the @p sonOrder-th son of @p node, or @c nullptr if absent.
     *
     * Dispatches to @c left for @c LEFT_SON_INDEX and @c right for @c RIGHT_SON_INDEX.
     *
     * @param node The parent node.
     * @param sonOrder Zero-based son index; only 0 (left) and 1 (right) are valid.
     * @return Pointer to the son block, or @c nullptr if absent or @p sonOrder is invalid.
     */
    BlockType *accessSon(
        const BlockType &node,
        std::size_t sonOrder
    ) const override;

    /**
     * @brief Allocates and inserts a new son at position @p sonOrder under @p parent.
     *
     * Dispatches to @c emplaceLeftSon or @c emplaceRightSon based on @p sonOrder.
     *
     * @param parent The node that will become the parent.
     * @param sonOrder Zero-based son index; 0 for left, 1 for right.
     * @return Reference to the newly created son block.
     */
    BlockType &emplaceSon(BlockType &parent, std::size_t sonOrder) override;

    /**
     * @brief Replaces the @p sonOrder-th son of @p parent with @p newSon.
     *
     * Dispatches to @c changeLeftSon or @c changeRightSon based on @p sonOrder.
     * The caller must ensure that the old son is properly placed at some
     * other place in this hierarchy. Otherwise, the behavior is undefined.
     *
     * @param parent The parent node.
     * @param sonOrder Zero-based son index; 0 for left, 1 for right.
     * @param newSon Pointer to the replacement son; may be @c nullptr to clear the slot.
     */
    void changeSon(
        BlockType &parent,
        std::size_t sonOrder,
        BlockType *newSon
    ) override;

    /**
     * @brief Recursively releases the @p sonOrder-th son sub-hierarchy of @p parent.
     *
     * Dispatches to @c removeLeftSon or @c removeRightSon based on @p sonOrder.
     *
     * @param parent The parent node.
     * @param sonOrder Zero-based son index; 0 for left, 1 for right.
     */
    void removeSon(BlockType &parent, std::size_t sonOrder) override;

    /**
     * @brief Returns a pointer to the left son of @p node via its @c left field.
     * @param node The parent node.
     * @return Pointer to the left son block, or @c nullptr if @p node has no left son.
     */
    BlockType *accessLeftSon(const BlockType &node) const override;

    /**
     * @brief Returns a pointer to the right son of @p node via its @c right field.
     * @param node The parent node.
     * @return Pointer to the right son block, or @c nullptr if @p node has no right son.
     */
    BlockType *accessRightSon(const BlockType &node) const override;

    /**
     * @brief Returns @c true if @p node is the left son of its parent.
     * @param node The node to test.
     * @return @c true if @p node has a parent and equals its parent's @c left pointer.
     */
    bool isLeftSon(const BlockType &node) const override;

    /**
     * @brief Returns @c true if @p node is the right son of its parent.
     * @param node The node to test.
     * @return @c true if @p node has a parent and equals its parent's @c right pointer.
     */
    bool isRightSon(const BlockType &node) const override;

    /**
     * @brief Returns @c true if @p node has a left son.
     * @param node The node to test.
     * @return @c true if the @c left pointer of @p node is non-null.
     */
    bool hasLeftSon(const BlockType &node) const override;

    /**
     * @brief Returns @c true if @p node has a right son.
     * @param node The node to test.
     * @return @c true if the @c right pointer of @p node is non-null.
     */
    bool hasRightSon(const BlockType &node) const override;

    /**
     * @brief Allocates a new node and sets it as the left son of @p parent.
     * @param parent The node that will become the parent.
     * @return Reference to the newly created left son block.
     */
    BlockType &emplaceLeftSon(BlockType &parent) override;

    /**
     * @brief Allocates a new node and sets it as the right son of @p parent.
     * @param parent The node that will become the parent.
     * @return Reference to the newly created right son block.
     */
    BlockType &emplaceRightSon(BlockType &parent) override;

    /**
     * @brief Replaces the left son of @p parent with @p newSon, updating parent pointers.
     *
     * The caller must ensure that the old son is properly placed at some
     * other place in this hierarchy. Otherwise, the behavior is undefined.
     *
     * @param parent The parent node.
     * @param newSon Pointer to the replacement node; may be @c nullptr to clear the slot.
     */
    void changeLeftSon(BlockType &parent, BlockType *newSon) override;

    /**
     * @brief Replaces the right son of @p parent with @p newSon, updating parent pointers.
     *
     * The caller must ensure that the old son is properly placed at some
     * other place in this hierarchy. Otherwise, the behavior is undefined.
     *
     * @param parent The parent node.
     * @param newSon Pointer to the replacement node; may be @c nullptr to clear the slot.
     */
    void changeRightSon(BlockType &parent, BlockType *newSon) override;

    /**
     * @brief Recursively releases the entire left sub-hierarchy of @p parent and clears its @c left pointer.
     * @param parent The parent node whose left son sub-hierarchy is to be removed.
     */
    void removeLeftSon(BlockType &parent) override;

    /**
     * @brief Recursively releases the entire right sub-hierarchy of @p parent and clears its @c right pointer.
     * @param parent The parent node whose right son sub-hierarchy is to be removed.
     */
    void removeRightSon(BlockType &parent) override;

    /**
     * @brief Assigns the contents of @p other to this hierarchy.
     * @param other The source memory structure to copy from.
     * @return Reference to this hierarchy.
     */
    AbstractMemoryType &assign(const AbstractMemoryType &other) override;

    /**
     * @brief Checks whether this hierarchy is equal to @p other.
     * @param other The memory structure to compare against.
     * @return @c true if both hierarchies contain equivalent data.
     */
    bool equals(const AbstractMemoryType &other) const override;
};

template<typename DataType>
using BinaryEH = BinaryExplicitHierarchy<DataType>;

//----------

template<typename BlockType>
ExplicitHierarchy<BlockType>::ExplicitHierarchy() : m_root(nullptr) {
}

template<typename BlockType>
void ExplicitHierarchy<BlockType>::clear() {
    Hierarchy<BlockType>::processPostOrder(m_root, [&](BlockType *b) {
        AMS<BlockType>::m_memoryManager->releaseMemory(b);
    });
    m_root = nullptr;
}

template<typename BlockType>
bool ExplicitHierarchy<BlockType>::isEmpty() const {
    return m_root == nullptr;
}

template<typename BlockType>
BlockType *ExplicitHierarchy<BlockType>::accessRoot() const {
    return m_root;
}

template<typename BlockType>
BlockType *ExplicitHierarchy<BlockType>::accessParent(
    const BlockType &node
) const {
    return node.parent;
}

template<typename BlockType>
BlockType &ExplicitHierarchy<BlockType>::emplaceRoot() {
    m_root = AMS<BlockType>::m_memoryManager->allocateMemory();
    return *m_root;
}

template<typename BlockType>
void ExplicitHierarchy<BlockType>::changeRoot(BlockType *newRoot) {
    if (newRoot != nullptr) {
        newRoot->parent = nullptr;
    }
    m_root = newRoot;
}

template<typename BlockType>
void ExplicitHierarchy<BlockType>::assignSafe(
    const ExplicitHierarchy *other
) {
    if (other == nullptr) {
        throw std::runtime_error("Invalid assignment. Type missmatch.");
    }
    this->assignUnsafe(other);
}

template<typename BlockType>
void ExplicitHierarchy<BlockType>::assignUnsafe(
    const ExplicitHierarchy *other
) {
    if (other == nullptr) {
        return;
    }

    std::function<void(BlockType *, BlockType *)> copy;
    copy = [this, other, &copy](BlockType *myBlock, BlockType *otherBlock) {
        myBlock->data = otherBlock->data;

        const std::size_t sonCount =other->degree(*otherBlock);
        std::size_t copiedSonCount = 0;
        std::size_t sonIndex = 0;
        while (copiedSonCount < sonCount) {
            BlockType *otherSon
                = other->accessSon(*otherBlock, sonIndex);
            if (otherSon != nullptr) {
                copy(&this->emplaceSon(*myBlock, sonIndex), otherSon);
                copiedSonCount++;
            }
            sonIndex++;
        }
    };

    this->clear();
    if (other->m_root != nullptr) {
        this->emplaceRoot();
        copy(m_root, other->m_root);
    }
}

template<typename BlockType>
bool ExplicitHierarchy<BlockType>::equalsSafe(
    const ExplicitHierarchy *other
) const {
    if (other == nullptr) {
        throw std::runtime_error("Invalid comparison. Type missmatch.");
    }
    return this->equalsUnsafe(other);
}

template<typename BlockType>
bool ExplicitHierarchy<BlockType>::equalsUnsafe(
    const ExplicitHierarchy *other
) const {
    if (other == nullptr) {
        return false;
    }

    std::function<bool(BlockType *, BlockType *)> compare;
    compare = [this, other, &compare](
        BlockType *myBlock, BlockType *otherBlock
    ) -> bool {
        if (myBlock == nullptr && otherBlock == nullptr) {
            return true;
        }

        if (myBlock == nullptr || otherBlock == nullptr) {
            return false;
        }

        if (this->degree(*myBlock) != other->degree(*otherBlock)) {
            return false;
        }

        if (!(myBlock->data == otherBlock->data)) {
            return false;
        }

        const std::size_t sonCount = this->degree(*myBlock);
        std::size_t sonsProcessed = 0;
        std::size_t i = 0;
        while (sonsProcessed < sonCount) {
            BlockType *mySon = this->accessSon(*myBlock, i);
            BlockType *otherSon = other->accessSon(*otherBlock, i);
            if (mySon != nullptr) {
                ++sonsProcessed;
            }
            if (! compare(mySon, otherSon)) {
                return false;
            }
            ++i;
        }
        return true;
    };

    return compare(m_root, other->m_root);
}

//----------

template<typename DataType>
MultiWayExplicitHierarchy<DataType>::MultiWayExplicitHierarchy(
    const MultiWayExplicitHierarchy &other
) :
    ExplicitHierarchy<MultiWayExplicitHierarchyBlock<DataType>>() {
    this->assignUnsafe(&other);
}

template<typename DataType>
MultiWayExplicitHierarchy<DataType>::~MultiWayExplicitHierarchy() {
    this->clear();
}

template<typename DataType>
std::size_t MultiWayExplicitHierarchy<DataType>::degree(
    const BlockType &node
) const {
    return node.sons.size();
}

template<typename DataType>
typename MultiWayExplicitHierarchy<DataType>::BlockType*
MultiWayExplicitHierarchy<DataType>::accessSon(
    const BlockType &node,
    std::size_t sonOrder
) const {
    MemoryBlock<BlockType *> *sonBlock = node.sons.access(sonOrder);
    return sonBlock != nullptr ? sonBlock->data : nullptr;
}

template<typename DataType>
typename MultiWayExplicitHierarchy<DataType>::BlockType&
MultiWayExplicitHierarchy<DataType>::emplaceSon(
    BlockType &parent,
    std::size_t sonOrder
) {
    BlockType *newSon
        = AbstractMemoryStructure<BlockType>::m_memoryManager->allocateMemory();
    parent.sons.emplace(sonOrder).data = newSon;
    newSon->parent = &parent;
    return *newSon;
}

template<typename DataType>
void MultiWayExplicitHierarchy<DataType>::changeSon(
    BlockType &parent,
    std::size_t sonOrder,
    BlockType *newSon
) {
    MemoryBlock<BlockType*> *sonBlock = parent.sons.access(sonOrder);
    BlockType *oldSon = sonBlock->data;
    sonBlock->data = newSon;

    if (oldSon != nullptr) {
        oldSon->parent = nullptr;
    }

    if (newSon != nullptr) {
        newSon->parent = &parent;
    }
}

template<typename DataType>
void MultiWayExplicitHierarchy<DataType>::removeSon(
    BlockType &parent,
    std::size_t sonOrder
) {
    MemoryBlock<BlockType *> *sonBlock = parent.sons.access(sonOrder);

    BlockType *removedSon = sonBlock->data;

    Hierarchy<BlockType>::processPostOrder(removedSon, [&](BlockType *b) {
        AbstractMemoryStructure<BlockType>::m_memoryManager->releaseMemory(b);
    });

    parent.sons.remove(sonOrder);
}

template<typename DataType>
AbstractMemoryType &MultiWayExplicitHierarchy<DataType>::assign(
    const AbstractMemoryType &other
) {
    this->assignUnsafe(
        dynamic_cast<const MultiWayExplicitHierarchy*>(&other));
    return *this;
}

template<typename DataType>
bool MultiWayExplicitHierarchy<DataType>::equals(
    const AbstractMemoryType &other
) const {
    return this->equalsUnsafe(
        dynamic_cast<const MultiWayExplicitHierarchy*>(&other));
}

//----------

template<typename DataType, std::size_t K>
KWayExplicitHierarchyBlock<DataType, K>::KWayExplicitHierarchyBlock() {
    for (std::size_t i = 0; i < K; ++i) {
        sons.access(i)->data = nullptr;
    }
}

template<typename DataType, std::size_t K>
KWayExplicitHierarchy<DataType, K>::KWayExplicitHierarchy(
    const KWayExplicitHierarchy &other
) :
    ExplicitHierarchy<KWayExplicitHierarchyBlock<DataType, K>>() {
    this->assignUnsafe(&other);
}

template<typename DataType, std::size_t K>
KWayExplicitHierarchy<DataType, K>::~KWayExplicitHierarchy() {
    this->clear();
}

template<typename DataType, std::size_t K>
std::size_t KWayExplicitHierarchy<DataType, K>::degree(
    const BlockType &node
) const {
    std::size_t result = 0;
    for (BlockType *b : node.sons) {
        if (b != nullptr) {
            result++;
        }
    }
    return result;
}

template<typename DataType, std::size_t K>
typename KWayExplicitHierarchy<DataType, K>::BlockType*
KWayExplicitHierarchy<DataType, K>::accessSon(
    const BlockType &node,
    std::size_t sonOrder
) const {
    MemoryBlock<BlockType *> *sonBlock = node.sons.access(sonOrder);
    return sonBlock != nullptr ? sonBlock->data : nullptr;
}

template<typename DataType, std::size_t K>
typename KWayExplicitHierarchy<DataType, K>::BlockType&
KWayExplicitHierarchy<DataType, K>::emplaceSon(
    BlockType &parent,
    std::size_t sonOrder
) {
    BlockType *newSon
        = AbstractMemoryStructure<BlockType>::m_memoryManager->allocateMemory();
    parent.sons.access(sonOrder)->data = newSon;
    newSon->parent = &parent;
    return *newSon;
}

template<typename DataType, std::size_t K>
void KWayExplicitHierarchy<DataType, K>::changeSon(
    BlockType &parent,
    std::size_t sonOrder,
    BlockType *newSon
) {
    MemoryBlock<BlockType *> *sonBlock = parent.sons.access(sonOrder);

    BlockType *oldSon = sonBlock->data;
    sonBlock->data = newSon;

    if (oldSon != nullptr) {
        oldSon->parent = nullptr;
    }

    if (newSon != nullptr) {
        newSon->parent = &parent;
    }
}

template<typename DataType, std::size_t K>
void KWayExplicitHierarchy<DataType, K>::removeSon(
    BlockType &parent,
    std::size_t sonOrder
) {
    MemoryBlock<BlockType *> *sonBlock = parent.sons.access(sonOrder);

    BlockType *removedSon = sonBlock->data;

    Hierarchy<BlockType>::processPostOrder(removedSon, [&](BlockType *b) {
        AbstractMemoryStructure<BlockType>::m_memoryManager->releaseMemory(b);
    });

    sonBlock->data = nullptr;
}

template<typename DataType, std::size_t K>
AbstractMemoryType &KWayExplicitHierarchy<DataType, K>::assign(
    const AbstractMemoryType &other
) {
    this->assignUnsafe(
        dynamic_cast<const KWayExplicitHierarchy*>(&other));
    return *this;
}

template<typename DataType, std::size_t K>
bool KWayExplicitHierarchy<DataType, K>::equals(
    const AbstractMemoryType &other
) const {
    return this->equalsUnsafe(
        dynamic_cast<const KWayExplicitHierarchy*>(&other));
}

//----------

template<typename DataType>
BinaryExplicitHierarchy<DataType>::BinaryExplicitHierarchy(
    const BinaryExplicitHierarchy &other
) :
    ExplicitHierarchy<BinaryExplicitHierarchyBlock<DataType>>() {
    this->assignUnsafe(&other);
}

template<typename DataType>
BinaryExplicitHierarchy<DataType>::~BinaryExplicitHierarchy() {
    this->clear();
}

template<typename DataType>
std::size_t BinaryExplicitHierarchy<DataType>::degree(
    const BlockType &node
) const {
    std::size_t result = 0;
    if (node.left != nullptr) {
        ++result;
    }
    if (node.right != nullptr) {
        ++result;
    }
    return result;
}

template<typename DataType>
BinaryExplicitHierarchyBlock<DataType>*
BinaryExplicitHierarchy<DataType>::accessSon(
    const BlockType &node,
    std::size_t sonOrder
) const {
    switch (sonOrder) {
    case BinaryHierarchy<BlockType>::LEFT_SON_INDEX:
        return node.left;
    case BinaryHierarchy<BlockType>::RIGHT_SON_INDEX:
        return node.right;
    default:
        return nullptr;
    }
}

template<typename DataType>
BinaryExplicitHierarchyBlock<DataType>&
BinaryExplicitHierarchy<DataType>::emplaceSon(
    BlockType &parent,
    std::size_t sonOrder
) {
    if (sonOrder == BinaryHierarchy<BlockType>::LEFT_SON_INDEX) {
        return this->emplaceLeftSon(parent);
    } else {
        return this->emplaceRightSon(parent);
    }
}

template<typename DataType>
void BinaryExplicitHierarchy<DataType>::changeSon(
    BlockType &parent,
    std::size_t sonOrder,
    BlockType *newSon
) {
    if (sonOrder == BinaryHierarchy<BlockType>::LEFT_SON_INDEX) {
        this->changeLeftSon(parent, newSon);
    } else {
        this->changeRightSon(parent, newSon);
    }
}

template<typename DataType>
void BinaryExplicitHierarchy<DataType>::removeSon(
    BlockType &parent,
    std::size_t sonOrder
) {
    if (sonOrder == BinaryHierarchy<BlockType>::LEFT_SON_INDEX) {
        this->removeLeftSon(parent);
    } else {
        this->removeRightSon(parent);
    }
}

template<typename DataType>
auto BinaryExplicitHierarchy<DataType>::accessLeftSon(
    const BlockType &node
) const -> BlockType * {
    return node.left;
}

template<typename DataType>
auto BinaryExplicitHierarchy<DataType>::accessRightSon(
    const BlockType &node
) const -> BlockType * {
    return node.right;
}

template<typename DataType>
bool BinaryExplicitHierarchy<DataType>::isLeftSon(const BlockType &node) const {
    return node.parent != nullptr
        && this->accessLeftSon(*this->accessParent(node)) == &node;
}

template<typename DataType>
bool BinaryExplicitHierarchy<DataType>::isRightSon(
    const BlockType &node
) const {
    return node.parent != nullptr
        && this->accessRightSon(*this->accessParent(node)) == &node;
}

template<typename DataType>
bool BinaryExplicitHierarchy<DataType>::hasLeftSon(
    const BlockType &node
) const {
    return node.left != nullptr;
}

template<typename DataType>
bool BinaryExplicitHierarchy<DataType>::hasRightSon(
    const BlockType &node
) const {
    return node.right != nullptr;
}

template<typename DataType>
BinaryExplicitHierarchyBlock<DataType>&
BinaryExplicitHierarchy<DataType>::emplaceLeftSon(BlockType &parent) {
    BlockType *newSon
        = AbstractMemoryStructure<BlockType>::m_memoryManager->allocateMemory();
    parent.left = newSon;
    newSon->parent = &parent;
    return *newSon;
}

template<typename DataType>
BinaryExplicitHierarchyBlock<DataType>&
BinaryExplicitHierarchy<DataType>::emplaceRightSon(BlockType &parent) {
    BlockType *newSon
        = AbstractMemoryStructure<BlockType>::m_memoryManager->allocateMemory();
    parent.right = newSon;
    newSon->parent = &parent;
    return *newSon;
}

template<typename DataType>
void BinaryExplicitHierarchy<DataType>::changeLeftSon(
    BlockType &parent,
    BlockType *newSon
) {
    BlockType *oldSon = parent.left;
    parent.left = newSon;
    if (oldSon != nullptr) {
        oldSon->parent = nullptr;
    }
    if (newSon != nullptr) {
        newSon->parent = &parent;
    }
}

template<typename DataType>
void BinaryExplicitHierarchy<DataType>::changeRightSon(
    BlockType &parent,
    BlockType *newSon
) {
    BlockType *oldSon = parent.right;
    parent.right = newSon;
    if (oldSon != nullptr) {
        oldSon->parent = nullptr;
    }
    if (newSon != nullptr) {
        newSon->parent = &parent;
    }
}

template<typename DataType>
void BinaryExplicitHierarchy<DataType>::removeLeftSon(BlockType &parent) {
    BlockType *removedSon = parent.left;

    Hierarchy<BlockType>::processPostOrder(removedSon, [&](BlockType *b) {
        AbstractMemoryStructure<BlockType>::m_memoryManager->releaseMemory(b);
    });

    parent.left = nullptr;
}

template<typename DataType>
void BinaryExplicitHierarchy<DataType>::removeRightSon(BlockType &parent) {
    BlockType *removedSon = parent.right;

    Hierarchy<BlockType>::processPostOrder(removedSon, [&](BlockType *b) {
        AbstractMemoryStructure<BlockType>::m_memoryManager->releaseMemory(b);
    });

    parent.right = nullptr;
}

template<typename DataType>
AbstractMemoryType &BinaryExplicitHierarchy<DataType>::assign(
    const AbstractMemoryType &other
) {
    this->assignUnsafe(
        dynamic_cast<const BinaryExplicitHierarchy*>(&other));
    return *this;
}

template<typename DataType>
bool BinaryExplicitHierarchy<DataType>::equals(
    const AbstractMemoryType &other
) const {
    return this->equalsUnsafe(
        dynamic_cast<const BinaryExplicitHierarchy*>(&other));
}


} // namespace ds::amt

#endif

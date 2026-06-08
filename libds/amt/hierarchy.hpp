#ifndef LIBDS_AMT_HIERARCHY_HPP
#define LIBDS_AMT_HIERARCHY_HPP

#include "abstract_memory_type.hpp"
#include "explicit_sequence.hpp"

#include <functional>


namespace ds::amt {


 /**
 * @brief Abstract base class for all hierarchical memory structures.
 *
 * Defines interface of a hierarchy and provides general implemenetation
 * of some of the functions.
 *
 * @tparam BlockType Type of the blocks used by the hierarchy.
 */
template<typename BlockType>
class Hierarchy : virtual public AMT {
public:
    using DataType = typename BlockType::DataT;

public:
    /**
     * @brief Returns the depth of @p node, where the root has level 0.
     * @param node The node whose depth is queried.
     * @return Number of "jumps" on the path from the root to @p node.
     */
    virtual std::size_t level(const BlockType &node) const;

    /**
     * @brief Returns the number of sons of @p node.
     * @param node The node whose degree is queried.
     * @return Degree of @p node.
     */
    virtual std::size_t degree(const BlockType &node) const = 0;

    /**
     * @brief Returns the total number of nodes in the hierarchy.
     * @return 0 if the hierarchy is empty, otherwise the count from the root.
     */
    virtual std::size_t nodeCount() const;

    /**
     * @brief Returns the number of nodes in the sub-hierarchy rooted at @p node.
     * @param node The root of the sub-hierarchy to count.
     * @return the number of nodes in the sub-hierarchy rooted at @p node.
     */
    virtual std::size_t nodeCount(const BlockType &node) const;

    /**
     * @brief Returns a pointer to the root node, or @c nullptr if the hierarchy is empty.
     * @return Pointer to the root node or @c nullptr if empty.
     */
    virtual BlockType *accessRoot() const = 0;

    /**
     * @brief Returns a pointer to the parent of @p node, or @c nullptr if @p node is the root.
     * @param node The node whose parent is requested.
     * @return Pointer to the parent of @p node or @c nullptr if @p node is root.
     */
    virtual BlockType *accessParent(const BlockType &node) const = 0;

    /**
     * @brief Returns a pointer to the @p sonOrder-th son of @p node, or @c nullptr if absent.
     * @param node The parent node.
     * @param sonOrder Zero-based index of the son to access.
     * @return Pointer to the @p sonOrder-th son of @p node, or @c nullptr if absent.
     */
    virtual BlockType *accessSon(
        const BlockType &node,
        std::size_t sonOrder
    ) const = 0;

    /**
     * @brief Returns @c true if @p node has no parent (i.e. it is the root).
     * @param node The node to test.
     * @return @c true of @p node is the root, @c false otherwise.
     */
    virtual bool isRoot(const BlockType &node) const;

    /**
     * @brief Returns @c true if @p node is the @p sonOrder-th son of its parent.
     * @param node The node to test.
     * @param sonOrder The son index to check against.
     * @return @c true if @p node occupies son slot @p sonOrder of its parent.
     */
    virtual bool isNthSon(const BlockType &node, std::size_t sonOrder) const;

    /**
     * @brief Returns @c true if @p node has no sons.
     * @param node The node to test.
     * @return @c true if the degree of @p node is zero, @c false otherwise.
     */
    virtual bool isLeaf(const BlockType &node) const;

    /**
     * @brief Returns @c true if the @p sonOrder-th son of @p node exists.
     * @param node The parent node.
     * @param sonOrder Zero-based son index to check.
     * @return @c true if son slot @p sonOrder of @p node is non-null.
     */
    virtual bool hasNthSon(const BlockType &node, std::size_t sonOrder) const;

    /**
     * @brief Allocates and installs a new root node in the hierarchy.
     * @return Reference to the newly created root block.
     */
    virtual BlockType &emplaceRoot() = 0;

    /**
     * @brief Replaces the root of the hierarchy with @p newRoot.
     * @param newRoot Pointer to the new root block; may be @c nullptr to clear the root.
     */
    virtual void changeRoot(BlockType *newRoot) = 0;

    /**
     * @brief Allocates and inserts a new son at position @p sonOrder under @p parent.
     * @param parent The node that will become the parent.
     * @param sonOrder Zero-based index at which to insert the son.
     * @return Reference to the newly created son block.
     */
    virtual BlockType &emplaceSon(BlockType &parent, std::size_t sonOrder) = 0;

    /**
     * @brief Replaces the @p sonOrder-th son of @p parent with @p newSon.
     * @param parent The parent node.
     * @param sonOrder Zero-based index of the son slot to update.
     * @param newSon Pointer to the replacement son; may be @c nullptr to remove the slot.
     */
    virtual void changeSon(
        BlockType &parent,
        std::size_t sonOrder,
        BlockType *newSon
    ) = 0;

    /**
     * @brief Removes and releases the @p sonOrder-th son of @p parent.
     * @param parent The parent node.
     * @param sonOrder Zero-based index of the son to remove.
     */
    virtual void removeSon(BlockType &parent, std::size_t sonOrder) = 0;

    /**
     * @brief Applies @p operation to every node in the sub-hierarchy rooted at @p node in pre-order.
     *
     * Each node is visited before any of its sons.
     *
     * @param node Root of the sub-hierarchy to traverse; does nothing if @c nullptr.
     * @param operation Callable invoked with a const pointer to each visited node.
     */
    void processPreOrder(
        const BlockType *node,
        std::function<void(const BlockType *)> operation
    ) const;

    /**
     * @brief Applies @p operation to every node in the sub-hierarchy rooted at @p node in post-order.
     *
     * Each node is visited after all of its descendants.
     *
     * @param node Root of the sub-hierarchy to traverse; does nothing if @c nullptr.
     * @param operation Callable invoked with a pointer to each visited node.
     */
    void processPostOrder(
        BlockType *node,
        std::function<void(BlockType *)> operation
    ) const;

    /**
     * @brief Applies @p operation to every node level by level, starting from @p node.
     *
     * Uses an internal queue so nodes at depth d are all visited before any node at depth d+1.
     *
     * @param node Root of the sub-hierarchy to traverse; does nothing if @c nullptr.
     * @param operation Callable invoked with a pointer to each visited node.
     */
    void processLevelOrder(
        BlockType *node,
        std::function<void(BlockType *)> operation
    ) const;

protected:
    /**
     * @brief Base class for depth-first iterators over a @c Hierarchy.
     *
     * Maintains a pointer to the owning hierarchy and the currently visited node.
     * Provides shared navigation helpers used by both pre-order and post-order subclasses.
     */
    class DepthFirstIterator {
    public:
        /**
         * @brief Constructs the iterator anchored to @p hierarchy, initially pointing at @p root.
         * @param hierarchy The hierarchy being iterated; must not be @c nullptr.
         * @param root The starting node, or @c nullptr to create an end iterator.
         */
        DepthFirstIterator(
            const Hierarchy<BlockType> *hierarchy,
            BlockType *root
        );

        /**
         * @brief Returns @c true if both iterators point to the same node.
         * @param other The iterator to compare against.
         * @return @c true if both iterators refer to the same block.
         */
        bool operator==(const DepthFirstIterator &other) const;

        /**
         * @brief Returns @c true if the iterators point to different nodes.
         * @param other The iterator to compare against.
         * @return @c true if the iterators refer to different blocks.
         */
        bool operator!=(const DepthFirstIterator &other) const;

        /**
         * @brief Dereferences the iterator to obtain a mutable reference to the current node's data.
         * @return Mutable reference to the data stored in the current block.
         */
        DataType &operator*();

        /**
         * @brief Dereferences the iterator to obtain a const reference to the current node's data.
         * @return Const reference to the data stored in the current block.
         */
        const DataType &operator*() const;

    protected:
        /**
         * @brief Returns the first right sibling of @p node, or @c nullptr if none exists.
         *
         * Scans the parent's sons to find @p node and then returns the next non-null son.
         *
         * @param node The node whose right sibling is requested.
         * @return Pointer to the first right sibling, or @c nullptr if @p node is the rightmost son or the root.
         */
        BlockType *accessRightSibling(BlockType *node) const;

        /**
         * @brief Returns the deepest leftmost descendant reachable from @p node.
         * @param node Starting node for the descent.
         * @return Pointer to the leftmost leaf in the sub-hierarchy rooted at @p node.
         */
        BlockType *accessLeftmost(BlockType *node) const;

        /**
         * @brief Returns the first (leftmost) son of @p node, or @c nullptr if @p node is a leaf.
         * @param node The node whose leftmost son is requested.
         * @return Pointer to the leftmost son block, or @c nullptr if @p node has no sons.
         */
        BlockType *accessLeftmostSon(BlockType *node) const;

    protected:
        /** @brief Pointer to the hierarchy being traversed. */
        const Hierarchy<BlockType> *m_hierarchy;

       /** @brief Pointer to the node the iterator currently refers to. */
        BlockType *m_current;
    };

public:
    /**
     * @brief Forward iterator that traverses a @c Hierarchy in pre-order (node before sons).
     */
    class PreOrderHierarchyIterator : public DepthFirstIterator {
    public:
        /**
         * @brief Constructs the iterator starting at @p root within @p hierarchy.
         * @param hierarchy The hierarchy to iterate.
         * @param root The first node to visit, or @c nullptr for an end iterator.
         */
        PreOrderHierarchyIterator(
            const Hierarchy<BlockType> *hierarchy,
            BlockType *root
        );

        /**
         * @brief Pre-increment: advances to the next node in pre-order.
         * @return Reference to this iterator after advancing.
         */
        PreOrderHierarchyIterator &operator++();

        /**
         * @brief Post-increment: returns a copy before advancing.
         * @return Copy of this iterator taken before the increment.
         */
        PreOrderHierarchyIterator operator++(int);
    };

    /**
     * @brief Forward iterator that traverses a @c Hierarchy in post-order (sons before node).
     */
    class PostOrderHierarchyIterator : public DepthFirstIterator {
    public:
        /**
         * @brief Constructs the iterator, positioning it at the leftmost leaf under @p root.
         * @param hierarchy The hierarchy to iterate.
         * @param root Subtree root to begin from, or @c nullptr for an end iterator.
         */
        PostOrderHierarchyIterator(
            const Hierarchy<BlockType> *hierarchy,
            BlockType *root
        );

        /**
         * @brief Pre-increment: advances to the next node in post-order.
         * @return Reference to this iterator after advancing.
         */
        PostOrderHierarchyIterator &operator++();

        /**
         * @brief Post-increment: returns a copy before advancing.
         * @return Copy of this iterator taken before the increment.
         */
        PostOrderHierarchyIterator operator++(int);
    };

public:
    using IteratorType = PreOrderHierarchyIterator;

    /**
     * @brief Returns a pre-order iterator to the root of the hierarchy.
     * @return Pre-order iterator positioned at the root.
     */
    IteratorType begin() const;

    /**
     * @brief Returns a past-the-end pre-order iterator.
     * @return Pre-order iterator representing the end sentinel.
     */
    IteratorType end() const;

    /**
     * @brief Returns a pre-order iterator to the root of the hierarchy.
     * @return Pre-order iterator positioned at the root.
     */
    PreOrderHierarchyIterator beginPre() const;

    /**
     * @brief Returns a past-the-end pre-order iterator.
     * @return Pre-order iterator representing the end sentinel.
     */
    PreOrderHierarchyIterator endPre() const;

    /**
     * @brief Returns a post-order iterator positioned at the leftmost leaf.
     * @return Post-order iterator positioned at the first node in post-order traversal.
     */
    PostOrderHierarchyIterator beginPost() const;

    /**
     * @brief Returns a past-the-end post-order iterator.
     * @return Post-order iterator representing the end sentinel.
     */
    PostOrderHierarchyIterator endPost() const;
};


/**
 * @brief Base class for hierarchies in which every node has at most @p K sons.
 *
 * Serves as a compile-time marker for the arity constraint.
 *
 * @tparam BlockType The node block type.
 * @tparam K Maximum number of sons per node.
 */
template<typename BlockType, std::size_t K>
class KWayHierarchy : virtual public Hierarchy<BlockType> {
};


/**
 * @brief Abstract base class for binary hierarchy structures where each node has at most two sons.
 *
 * Specialises @c KWayHierarchy<BlockType, 2> with named left/right son accessors,
 * predicates, emplace helpers, and an in-order traversal algorithm and iterator.
 *
 * @tparam BlockType The node block type.
 */
template<typename BlockType>
class BinaryHierarchy : virtual public KWayHierarchy<BlockType, 2> {
public:
    /** @brief Son index used to identify the left son slot. */
    static const std::size_t LEFT_SON_INDEX = 0;

    /** @brief Son index used to identify the right son slot. */
    static const std::size_t RIGHT_SON_INDEX = 1;

public:
    /**
     * @brief Returns a pointer to the left son of @p node, or @c nullptr if absent.
     * @param node The parent node.
     * @return Pointer to the left son block, or @c nullptr if @p node has no left son.
     */
    virtual BlockType *accessLeftSon(const BlockType &node) const;

    /**
     * @brief Returns a pointer to the right son of @p node, or @c nullptr if absent.
     * @param node The parent node.
     * @return Pointer to the right son block, or @c nullptr if @p node has no right son.
     */
    virtual BlockType *accessRightSon(const BlockType &node) const;

    /**
     * @brief Returns @c true if @p node is the left son of its parent.
     * @param node The node to test.
     * @return @c true if @p node occupies the left son slot of its parent.
     */
    virtual bool isLeftSon(const BlockType &node) const;

    /**
     * @brief Returns @c true if @p node is the right son of its parent.
     * @param node The node to test.
     * @return @c true if @p node occupies the right son slot of its parent.
     */
    virtual bool isRightSon(const BlockType &node) const;

    /**
     * @brief Returns @c true if @p node has a left son.
     * @param node The node to test.
     * @return @c true if the left son slot of @p node is non-null.
     */
    virtual bool hasLeftSon(const BlockType &node) const;

    /**
     * @brief Returns @c true if @p node has a right son.
     * @param node The node to test.
     * @return @c true if the right son slot of @p node is non-null.
     */
    virtual bool hasRightSon(const BlockType &node) const;

    /**
     * @brief Allocates and inserts a new left son under @p parent.
     * @param parent The node that will become the parent.
     * @return Reference to the newly created left son block.
     */
    virtual BlockType &emplaceLeftSon(BlockType &parent);

    /**
     * @brief Allocates and inserts a new right son under @p parent.
     * @param parent The node that will become the parent.
     * @return Reference to the newly created right son block.
     */
    virtual BlockType &emplaceRightSon(BlockType &parent);

    /**
     * @brief Replaces the left son of @p parent with @p newSon.
     * @param parent The parent node.
     * @param newSon Pointer to the replacement node; may be @c nullptr to clear the slot.
     */
    virtual void changeLeftSon(BlockType &parent, BlockType *newSon);

    /**
     * @brief Replaces the right son of @p parent with @p newSon.
     * @param parent The parent node.
     * @param newSon Pointer to the replacement node; may be @c nullptr to clear the slot.
     */
    virtual void changeRightSon(BlockType &parent, BlockType *newSon);

    /**
     * @brief Removes and releases the left son of @p parent.
     * @param parent The parent node whose left son is to be removed.
     */
    virtual void removeLeftSon(BlockType &parent);

    /**
     * @brief Removes and releases the right son of @p parent.
     * @param parent The parent node whose right son is to be removed.
     */
    virtual void removeRightSon(BlockType &parent);

    /**
     * @brief Applies @p operation to every node in the sub-hierarchy rooted at @p node in in-order.
     *
     * Visits the left sub-hierarchy then the node itself, then the right sub-hirarchy.
     *
     * @param node Root of the sub-hierarchy to traverse; does nothing if @c nullptr.
     * @param operation Callable invoked with a const pointer to each visited node.
     */
    void processInOrder(
        const BlockType *node,
        std::function<void(const BlockType *)> operation
    ) const;

public:
    /**
     * @brief Forward iterator that traverses a @c BinaryHierarchy in in-order.
     *
     * Starts at the leftmost leaf and advances by moving to the leftmost node of the
     * right sub-hierarchy, or climbing up past right-son ancestors when no right sub-hierarchy exists.
     */
    class InOrderHierarchyIterator :
        public Hierarchy<BlockType>::DepthFirstIterator {
    public:
        /**
         * @brief Constructs the iterator, positioning it at the leftmost leaf under @p root.
         * @param hierarchy The binary hierarchy to iterate.
         * @param root Subtree root to begin from, or @c nullptr for an end iterator.
         */
        InOrderHierarchyIterator(
            const BinaryHierarchy<BlockType> *hierarchy,
            BlockType *root
        );

        /**
         * @brief Pre-increment: advances to the next node in in-order.
         * @return Reference to this iterator after advancing.
         */
        InOrderHierarchyIterator &operator++();

        /**
         * @brief Post-increment: returns a copy before advancing.
         * @return Copy of this iterator taken before the increment.
         */
        InOrderHierarchyIterator operator++(int);

    private:
        /**
         * @brief Returns @c true if @p node is the right son of its parent.
         * @param node The node to test; returns @c false if @p node has no parent.
         * @return @c true if @p node occupies the right son slot of its parent.
         */
        bool isRightSon(BlockType *node) const;

        /**
         * @brief Returns the leftmost node by following left son pointers.
         * @param node Node where to start.
         * @return The leftmost node.
         */
        BlockType *followLeft(BlockType *node) const;

        /**
         * @brief Returns a reference to the owning hierarchy cast to @c BinaryHierarchy.
         * @return Reference to the underlying @c BinaryHierarchy being iterated.
         */
        const BinaryHierarchy<BlockType> *getBinaryHierarchy() const;
    };

public:
    using IteratorType = InOrderHierarchyIterator;

    /**
     * @brief Returns an in-order iterator positioned at the leftmost leaf.
     * @return In-order iterator positioned at the first node in in-order traversal.
     */
    IteratorType begin() const;

    /**
     * @brief Returns a past-the-end in-order iterator.
     * @return In-order iterator representing the end sentinel.
     */
    IteratorType end() const;
};

//----------

template<typename BlockType>
std::size_t Hierarchy<BlockType>::level(const BlockType &node) const {
    std::size_t level = 0;
    BlockType *parent = this->accessParent(node);
    while (parent != nullptr) {
        level++;
        parent = this->accessParent(*parent);
    }
    return level;
}

template<typename BlockType>
std::size_t Hierarchy<BlockType>::nodeCount() const {
    BlockType *root = this->accessRoot();
    return root != nullptr ? this->nodeCount(*root) : 0;
}

template<typename BlockType>
std::size_t Hierarchy<BlockType>::nodeCount(const BlockType &node) const {
    std::size_t result = 0;
    this->processPreOrder(&node, [&result](const BlockType *) { result++; });
    return result;
}

template<typename BlockType>
bool Hierarchy<BlockType>::isRoot(const BlockType &node) const {
    return this->accessParent(node) == nullptr;
}

template<typename BlockType>
bool Hierarchy<BlockType>::isNthSon(
    const BlockType &node,
    std::size_t sonOrder
) const {
    BlockType *parent = this->accessParent(node);
    return parent != nullptr && this->accessSon(*parent, sonOrder) == &node;
}

template<typename BlockType>
bool Hierarchy<BlockType>::isLeaf(const BlockType &node) const {
    return this->degree(node) == 0;
}

template<typename BlockType>
bool Hierarchy<BlockType>::hasNthSon(
    const BlockType &node,
    std::size_t sonOrder
) const {
    return this->accessSon(node, sonOrder) != nullptr;
}

template<typename BlockType>
void Hierarchy<BlockType>::processPreOrder(
    const BlockType *node,
    std::function<void(const BlockType *)> operation
) const {
    if (node != nullptr) {
        operation(node);

        std::size_t deg = this->degree(*node);
        std::size_t sonsProcessed = 0;
        std::size_t n = 0;
        while (sonsProcessed < deg) {
            BlockType *son = this->accessSon(*node, n);
            if (son != nullptr) {
                this->processPreOrder(son, operation);
                ++sonsProcessed;
            }
            ++n;
        }
    }
}

template<typename BlockType>
void Hierarchy<BlockType>::processPostOrder(
    BlockType *node,
    std::function<void(BlockType *)> operation
) const {
    if (node != nullptr) {
        std::size_t deg = this->degree(*node);
        std::size_t sonsProcessed = 0;
        std::size_t n = 0;
        while (sonsProcessed < deg) {
            BlockType *son = this->accessSon(*node, n);
            if (son != nullptr) {
                this->processPostOrder(son, operation);
                ++sonsProcessed;
            }
            ++n;
        }

        operation(node);
    }
}

template<typename BlockType>
void Hierarchy<BlockType>::processLevelOrder(
    BlockType *node,
    std::function<void(BlockType *)> operation
) const {
    if (node != nullptr) {
        SinglyLinkedSequence<BlockType *> sequence;
        sequence.emplaceFirst().data = node;
        while (! sequence.isEmpty()) {
            BlockType *current = sequence.accessFirst()->data;
            sequence.removeFirst();
            if (current != nullptr) {
                operation(current);
                std::size_t nodeDegree = this->degree(*current);
                std::size_t n = 0;
                std::size_t sonsProcessed = 0;
                while (sonsProcessed < nodeDegree) {
                    BlockType *son = this->accessSon(*current, n);
                    if (son != nullptr) {
                        sequence.emplaceLast().data = son;
                        ++sonsProcessed;
                    }
                    ++n;
                }
            }
        }
    }
}

template<typename BlockType>
void BinaryHierarchy<BlockType>::processInOrder(
    const BlockType *node,
    std::function<void(const BlockType *)> operation
) const {
    if (node != nullptr) {
        this->processInOrder(this->accessLeftSon(*node), operation);
        operation(node);
        this->processInOrder(this->accessRightSon(*node), operation);
    }
}

//----------

template<typename BlockType>
Hierarchy<BlockType>::DepthFirstIterator::DepthFirstIterator(
    const Hierarchy<BlockType> *hierarchy,
    BlockType *root
) :
    m_hierarchy(hierarchy),
    m_current(root) {
}

template<typename BlockType>
bool Hierarchy<BlockType>::DepthFirstIterator::operator== (
    const DepthFirstIterator &other
) const {
    return m_current == other.m_current;
}

template<typename BlockType>
bool Hierarchy<BlockType>::DepthFirstIterator::operator!= (
    const DepthFirstIterator &other
) const {
    return ! (*this == other);
}

template<typename BlockType>
typename Hierarchy<BlockType>::DataType&
Hierarchy<BlockType>::DepthFirstIterator::operator*() {
    return m_current->data;
}

template<typename BlockType>
const typename Hierarchy<BlockType>::DataType&
Hierarchy<BlockType>::DepthFirstIterator::operator*() const {
    return m_current->data;
}

template<typename BlockType>
BlockType *Hierarchy<BlockType>::DepthFirstIterator::accessRightSibling(
    BlockType *node
) const
{
    BlockType *parent = m_hierarchy->accessParent(*node);
    if (parent == nullptr) {
        // Node is root => does not have sibling.
        return nullptr;
    }

    std::size_t deg = m_hierarchy->degree(*parent);
    std::size_t sonsProcessed = 0;
    std::size_t i = 0;
    BlockType *son = nullptr;

    // Find position of the node.
    while (son != node && sonsProcessed < deg) {
        son = m_hierarchy->accessSon(*parent, i);
        if (son != nullptr) {
            ++sonsProcessed;
        }
        ++i;
    }

    if (sonsProcessed == deg) {
        // Node is the rightmost son of its parent
        // => does not have right sibling.
        return nullptr;
    }

    // Find the first right sibling.
    do {
        son = m_hierarchy->accessSon(*parent, i);
        ++i;
    } while (son == nullptr);
    return son;
}

template<typename BlockType>
BlockType* Hierarchy<BlockType>::DepthFirstIterator::accessLeftmost(
    BlockType *node
) const
{
    BlockType *current = node;
    BlockType *next = this->accessLeftmostSon(node);
    while (next != nullptr) {
        current = next;
        next = this->accessLeftmostSon(next);
    }
    return current;
}

template<typename BlockType>
BlockType* Hierarchy<BlockType>::DepthFirstIterator::accessLeftmostSon(
    BlockType *node
) const
{
    if (m_hierarchy->degree(*node) == 0) {
        return nullptr;
    }
    std::size_t i = 0;
    BlockType *son = nullptr;
    do {
        son = m_hierarchy->accessSon(*node, i);
        ++i;
    }
    while (son == nullptr);
    return son;
}

//----------

template<typename BlockType>
Hierarchy<BlockType>::PreOrderHierarchyIterator::PreOrderHierarchyIterator(
    const Hierarchy<BlockType> *hierarchy,
    BlockType *root
) :
    Hierarchy<BlockType>::DepthFirstIterator::DepthFirstIterator(
        hierarchy,
        root)
{
}

template<typename BlockType>
typename Hierarchy<BlockType>::PreOrderHierarchyIterator&
Hierarchy<BlockType>::PreOrderHierarchyIterator::operator++() {
    // Try go to the leftmost son.
    if (this->m_hierarchy->degree(*this->m_current) != 0) {
        this->m_current = this->accessLeftmostSon(this->m_current);
        return *this;
    }

    // Try go to the right sibling.
    BlockType *next = this->accessRightSibling(this->m_current);
    if (next != nullptr) {
        this->m_current = next;
        return *this;
    }

    // Try go to the leftmost right uncle.
    while (next == nullptr) {
        this->m_current = this->m_hierarchy->accessParent(*this->m_current);
        if (this->m_current == nullptr) {
            // We are the end iterator now.
            return *this;
        }
        next = this->accessRightSibling(this->m_current);
    }
    this->m_current = next;

    return *this;
}

template<typename BlockType>
typename Hierarchy<BlockType>::PreOrderHierarchyIterator
Hierarchy<BlockType>::PreOrderHierarchyIterator::operator++(int) {
    PreOrderHierarchyIterator copy(*this);
    ++(*this);
    return copy;
}

//----------

template<typename BlockType>
Hierarchy<BlockType>::PostOrderHierarchyIterator::PostOrderHierarchyIterator(
    const Hierarchy<BlockType> *hierarchy,
    BlockType *root
) :
    Hierarchy<BlockType>::DepthFirstIterator::DepthFirstIterator(
        hierarchy,
        nullptr)
{
    if (root != nullptr) {
        this->m_current = this->accessLeftmost(root);
    }
}

template<typename BlockType>
typename Hierarchy<BlockType>::PostOrderHierarchyIterator&
Hierarchy<BlockType>::PostOrderHierarchyIterator::operator++() {
    // Access right sibling.
    BlockType* next = this->accessRightSibling(this->m_current);

    // Access leftmost node in the right sibling sub-hierarchy.
    if (next != nullptr) {
        this->m_current = this->accessLeftmost(next);
        return *this;
    }

    // Try to go to parent.
    this->m_current = this->m_hierarchy->accessParent(*this->m_current);

    // We may be the end iterator now, if current was root.
    return *this;
}

template<typename BlockType>
typename Hierarchy<BlockType>::PostOrderHierarchyIterator
Hierarchy<BlockType>::PostOrderHierarchyIterator::operator++(int) {
    PostOrderHierarchyIterator copy(*this);
    ++(*this);
    return copy;
}

//----------

template<typename BlockType>
typename Hierarchy<BlockType>::PreOrderHierarchyIterator
Hierarchy<BlockType>::begin() const {
    return PreOrderHierarchyIterator(this, this->accessRoot());
}

template<typename BlockType>
typename Hierarchy<BlockType>::PreOrderHierarchyIterator
Hierarchy<BlockType>::end() const {
    return PreOrderHierarchyIterator(this, nullptr);
}

template<typename BlockType>
typename Hierarchy<BlockType>::PreOrderHierarchyIterator
Hierarchy<BlockType>::beginPre() const {
    return PreOrderHierarchyIterator(this, this->accessRoot());
}

template<typename BlockType>
typename Hierarchy<BlockType>::PreOrderHierarchyIterator
Hierarchy<BlockType>::endPre() const {
    return PreOrderHierarchyIterator(this, nullptr);
}

template<typename BlockType>
typename Hierarchy<BlockType>::PostOrderHierarchyIterator
Hierarchy<BlockType>::beginPost() const {
    return PostOrderHierarchyIterator(this, this->accessRoot());
}

template<typename BlockType>
typename Hierarchy<BlockType>::PostOrderHierarchyIterator
Hierarchy<BlockType>::endPost() const {
    return PostOrderHierarchyIterator(this, nullptr);
}

//----------

template<typename BlockType>
BlockType *BinaryHierarchy<BlockType>::accessLeftSon(
    const BlockType &node
) const {
    return this->accessSon(node, LEFT_SON_INDEX);
}

template<typename BlockType>
BlockType *BinaryHierarchy<BlockType>::accessRightSon(
    const BlockType &node
) const {
    return this->accessSon(node, RIGHT_SON_INDEX);
}

template<typename BlockType>
bool BinaryHierarchy<BlockType>::isLeftSon(const BlockType &node) const {
    return this->isNthSon(node, LEFT_SON_INDEX);
}

template<typename BlockType>
bool BinaryHierarchy<BlockType>::isRightSon(const BlockType &node) const {
    return this->isNthSon(node, RIGHT_SON_INDEX);
}

template<typename BlockType>
bool BinaryHierarchy<BlockType>::hasLeftSon(const BlockType &node) const {
    return this->hasNthSon(node, LEFT_SON_INDEX);
}

template<typename BlockType>
bool BinaryHierarchy<BlockType>::hasRightSon(const BlockType &node) const {
    return this->hasNthSon(node, RIGHT_SON_INDEX);
}

template<typename BlockType>
BlockType &BinaryHierarchy<BlockType>::emplaceLeftSon(BlockType &parent) {
    return this->emplaceSon(parent, LEFT_SON_INDEX);
}

template<typename BlockType>
BlockType &BinaryHierarchy<BlockType>::emplaceRightSon(BlockType &parent) {
    return this->emplaceSon(parent, RIGHT_SON_INDEX);
}

template<typename BlockType>
void BinaryHierarchy<BlockType>::changeLeftSon(
    BlockType &parent,
    BlockType *newSon
) {
    this->changeSon(parent, LEFT_SON_INDEX, newSon);
}

template<typename BlockType>
void BinaryHierarchy<BlockType>::changeRightSon(
    BlockType &parent,
    BlockType *newSon
) {
    this->changeSon(parent, RIGHT_SON_INDEX, newSon);
}

template<typename BlockType>
void BinaryHierarchy<BlockType>::removeLeftSon(BlockType &parent) {
    this->removeSon(parent, LEFT_SON_INDEX);
}

template<typename BlockType>
void BinaryHierarchy<BlockType>::removeRightSon(BlockType &parent) {
    this->removeSon(parent, RIGHT_SON_INDEX);
}

//----------

template<typename BlockType>
BinaryHierarchy<BlockType>::InOrderHierarchyIterator::InOrderHierarchyIterator(
    const BinaryHierarchy<BlockType> *hierarchy,
    BlockType *root
) :
    Hierarchy<BlockType>::DepthFirstIterator::DepthFirstIterator(
        hierarchy,
        nullptr)
{
    if (root != nullptr) {
        this->m_current = this->followLeft(root);
    }
}

template<typename BlockType>
typename BinaryHierarchy<BlockType>::InOrderHierarchyIterator&
BinaryHierarchy<BlockType>::InOrderHierarchyIterator::operator++() {
    const BinaryHierarchy<BlockType> &bh = *this->getBinaryHierarchy();

    // Access right son.
    BlockType* right = bh.accessRightSon(*this->m_current);

    // Access leftmost node in the right son sub-hierarchy.
    if (right != nullptr) {
        this->m_current = this->followLeft(right);
        return *this;
    }

    // While the current node is right son of its parent, move to the parent.
    while (this->isRightSon(this->m_current)) {
        this->m_current = bh.accessParent(*this->m_current);
    }

    // Move to the parent once more.
    this->m_current = bh.accessParent(*this->m_current);

    // We may be the end iterator now, if current was root.
    return *this;
}

template<typename BlockType>
typename BinaryHierarchy<BlockType>::InOrderHierarchyIterator
BinaryHierarchy<BlockType>::InOrderHierarchyIterator::operator++(int) {
    InOrderHierarchyIterator copy(*this);
    ++(*this);
    return copy;
}


template<typename BlockType>
bool BinaryHierarchy<BlockType>::InOrderHierarchyIterator::isRightSon(
    BlockType *node
) const {
    const BinaryHierarchy<BlockType> &bh = *this->getBinaryHierarchy();
    BlockType *parent = bh.accessParent(*node);
    if (parent == nullptr) {
        return false;
    }
    return bh.accessRightSon(*parent) == node;
}

template<typename BlockType>
BlockType *BinaryHierarchy<BlockType>::InOrderHierarchyIterator::followLeft(
    BlockType *const node
) const {
    const BinaryHierarchy<BlockType> &bh = *this->getBinaryHierarchy();
    BlockType *current = node;
    BlockType *next = bh.accessLeftSon(*node);
    while (next != nullptr) {
        current = next;
        next = bh.accessLeftSon(*next);
    }
    return current;
}

template<typename BlockType>
const BinaryHierarchy<BlockType>*
BinaryHierarchy<BlockType>::InOrderHierarchyIterator::getBinaryHierarchy(
) const {
    return dynamic_cast<const BinaryHierarchy<BlockType>*>(this->m_hierarchy);
}

//----------

template<typename BlockType>
typename BinaryHierarchy<BlockType>::InOrderHierarchyIterator
BinaryHierarchy<BlockType>::begin() const {
    return InOrderHierarchyIterator(this, this->accessRoot());
}

template<typename BlockType>
typename BinaryHierarchy<BlockType>::InOrderHierarchyIterator
BinaryHierarchy<BlockType>::end() const {
    return InOrderHierarchyIterator(this, nullptr);
}


} // namespace ds::amt

#endif

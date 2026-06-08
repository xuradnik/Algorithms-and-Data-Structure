#ifndef LIBDS_ADT_PRIORITY_QUEUE_HPP
#define LIBDS_ADT_PRIORITY_QUEUE_HPP

#include <libds/adt/abstract_data_type.hpp>
#include <libds/amt/explicit_sequence.hpp>
#include <libds/amt/implicit_hierarchy.hpp>
#include <libds/amt/implicit_sequence.hpp>

#include <cmath>

namespace ds::adt {


/**
 * @brief Holds a single element together with its associated priority.
 * @tparam P Priority type; must support less-than comparison.
 * @tparam T Data type stored in the queue.
 */
template<typename P, typename T>
struct PriorityQueueItem {
    P priority;
    T data;

    bool operator==(const PriorityQueueItem &other) const = default;
    bool operator!=(const PriorityQueueItem &other) const = default;
};

//----------

/**
 * @brief Abstract interface for a min-priority queue.
 * @tparam P Priority type; lower values indicate higher priority.
 * @tparam T Data type stored in the queue.
 */
template<typename P, typename T>
class PriorityQueue : virtual public ADT {
public:
    /**
     * @brief Inserts a new element with the given priority.
     * @param priority Priority associated with the element.
     * @param data     Element to insert.
     */
    virtual void push(const P &priority, const T &data) = 0;

    /**
     * @brief Returns a reference to the element with the highest priority.
     * @return Reference to the highest-priority element.
     * @throws std::out_of_range If the queue is empty.
     */
    virtual T &peek() = 0;

    /**
     * @brief Returns a const reference to the element with the highest priority.
     * @return Const reference to the highest-priority element.
     * @throws std::out_of_range If the queue is empty.
     */
    virtual const T &peek() const = 0;

    /**
     * @brief Removes the element with the highest priority.
     * @throws std::out_of_range If the queue is empty.
     */
    virtual void pop() = 0;
};

//----------

/**
 * @brief Base priority queue backed by an arbitrary sequence type.
 * @tparam P            Priority type.
 * @tparam T            Data type.
 * @tparam SequenceType Underlying sequence structure used for storage.
 */
template<typename P, typename T, typename SequenceType>
class SequencePriorityQueue :
    public PriorityQueue<P, T>,
    public ADS<PriorityQueueItem<P, T>> {
public:
    /**
     * @brief Constructs an empty sequence-backed priority queue.
     */
    SequencePriorityQueue();

    /**
     * @brief Copy-constructs a sequence-backed priority queue from another instance.
     * @param other Source queue to copy.
     */
    SequencePriorityQueue(const SequencePriorityQueue &other);

    /**
     * @brief Equality comparison; always throws because it is not supported.
     * @param other ADT to compare against.
     * @return Never returns normally.
     * @throws utils::unavailable_function_call Always.
     */
    bool equals(const ADT &other) const override;

protected:
    /**
     * @brief Assigns the contents of @p other to this queue after a type-safety check.
     * @param other Pointer to the source queue; must not be @c nullptr and must match this type.
     * @throws utils::type_missmatch If @p other is @c nullptr.
     */
    void assignSafe(const SequencePriorityQueue *other);

    /**
     * @brief Returns a typed pointer to the underlying sequence structure.
     * @return Pointer to the internal @p SequenceType instance.
     */
    SequenceType *getSequence() const;
};

//----------

/**
 * @brief Priority queue backed by an unsorted sequence; O(n) peek and pop.
 * @tparam P            Priority type.
 * @tparam T            Data type.
 * @tparam SequenceType Underlying sequence structure used for storage.
 */
template<typename P, typename T, typename SequenceType>
class UnsortedSequencePriorityQueue :
    public SequencePriorityQueue<P, T, SequenceType> {
public:
    /**
     * @brief Returns a reference to the highest-priority element by linear scan.
     * @return Reference to the highest-priority element.
     * @throws std::out_of_range If the queue is empty.
     */
    T &peek() override;

    /**
     * @brief Returns a const reference to the highest-priority element by linear scan.
     * @return Const reference to the highest-priority element.
     * @throws std::out_of_range If the queue is empty.
     */
    const T &peek() const override;

protected:
    using BlockType = typename SequenceType::BlockType;

protected:
    /**
     * @brief Finds the sequence block that holds the element with the lowest priority value.
     * @return Pointer to the block with the highest priority.
     */
    BlockType *findHighestPriorityBlock() const;
};

//----------

/**
 * @brief Priority queue backed by a sorted sequence; O(1) peek and pop, O(n) push.
 * @tparam P            Priority type.
 * @tparam T            Data type.
 * @tparam SequenceType Underlying sequence structure used for storage.
 */
template<typename P, typename T, typename SequenceType>
class SortedSequencePriorityQueue :
    public SequencePriorityQueue<P, T, SequenceType> {
public:
    /**
     * @brief Returns a reference to the highest-priority element.
     * @return Reference to the highest-priority element.
     * @throws std::out_of_range If the queue is empty.
     */
    T &peek() override;

    /**
     * @brief Returns a const reference to the highest-priority element.
     * @return Const reference to the highest-priority element.
     * @throws std::out_of_range If the queue is empty.
     */
    const T &peek() const override;

    /**
     * @brief Removes the highest-priority element from the sorted sequence.
     * @throws std::out_of_range If the queue is empty.
     */
    void pop() override;

protected:
    /**
     * @brief Returns the index of the block containing the highest-priority element.
     * @return Zero-based index of the highest-priority block within the sequence.
     */
    virtual std::size_t indexOfHighestPriorityBlock() const = 0;

private:
    using BlockType = typename SequenceType::BlockType;
};

//----------

/**
 * @brief Unsorted priority queue backed by an implicit sequence.
 * @tparam P Priority type.
 * @tparam T Data type.
 */
template<typename P, typename T>
class UnsortedImplicitSequencePriorityQueue :
    public UnsortedSequencePriorityQueue<
        P,
        T,
        amt::ImplicitSequence<PriorityQueueItem<P, T>>
    > {
public:
    /**
     * @brief Appends a new element at the back of the implicit sequence.
     * @param priority Priority of the new element.
     * @param data     Data of the new element.
     */
    void push(const P &priority, const T &data) override;

    /**
     * @brief Removes the highest-priority element by swapping it to the back and popping.
     * @throws std::out_of_range If the queue is empty.
     */
    void pop() override;

    /**
     * @brief Assigns the contents of @p other to this queue.
     * @param other Source ADT; must be of the same concrete type.
     * @return Reference to this queue after assignment.
     * @throws utils::type_missmatch If @p other is not an UnsortedImplicitSequencePriorityQueue.
     */
    ADT &assign(const ADT &other) override;

private:
    using SequenceType = amt::ImplicitSequence<PriorityQueueItem<P, T>>;
    using BlockType = typename SequenceType::BlockType;
};

//----------

/**
 * @brief Unsorted priority queue backed by a singly-linked explicit sequence.
 * @tparam P Priority type.
 * @tparam T Data type.
 */
template<typename P, typename T>
class UnsortedExplicitSequencePriorityQueue :
    public UnsortedSequencePriorityQueue<
        P,
        T,
        amt::SinglyLinkedSequence<PriorityQueueItem<P, T>>
    > {
public:
    /**
     * @brief Prepends a new element at the front of the linked sequence.
     * @param priority Priority of the new element.
     * @param data     Data of the new element.
     */
    void push(const P &priority, const T &data) override;

    /**
     * @brief Removes the highest-priority element by swapping it to the front and popping.
     * @throws std::out_of_range If the queue is empty.
     */
    void pop() override;

    /**
     * @brief Assigns the contents of @p other to this queue.
     * @param other Source ADT; must be of the same concrete type.
     * @return Reference to this queue after assignment.
     * @throws utils::type_missmatch If @p other is not an UnsortedExplicitSequencePriorityQueue.
     */
    ADT &assign(const ADT &other) override;

private:
    using SequenceType = amt::SinglyLinkedSequence<PriorityQueueItem<P, T>>;
    using BlockType = typename SequenceType::BlockType;
};

//----------

/**
 * @brief Sorted priority queue backed by an implicit sequence.
 * Elements are kept in descending priority order; the highest-priority element
 * is always at the back.
 * @tparam P Priority type.
 * @tparam T Data type.
 */
template<typename P, typename T>
class SortedImplicitSequencePriorityQueue :
    public SortedSequencePriorityQueue<
        P,
        T,
        amt::ImplicitSequence<PriorityQueueItem<P, T>>
    > {
public:
    /**
     * @brief Inserts a new element at its sorted position within the implicit sequence.
     * @param priority Priority of the new element.
     * @param data     Data of the new element.
     */
    void push(const P &priority, const T &data) override;

    /**
     * @brief Assigns the contents of @p other to this queue.
     * @param other Source ADT; must be of the same concrete type.
     * @return Reference to this queue after assignment.
     * @throws utils::type_missmatch If @p other is not a SortedImplicitSequencePriorityQueue.
     */
    ADT &assign(const ADT &other) override;

protected:
    /**
     * @brief Returns the index of the last element, which holds the highest priority.
     * @return Index of the highest-priority block (always @c size - 1).
     */
    std::size_t indexOfHighestPriorityBlock() const override;

private:
    using SequenceType = amt::ImplicitSequence<PriorityQueueItem<P, T>>;
    using BlockType = typename SequenceType::BlockType;
};

//----------

/**
 * @brief Sorted priority queue backed by a singly-linked explicit sequence.
 * Elements are kept in ascending priority order; the highest-priority element
 * is always at the front.
 * @tparam P Priority type.
 * @tparam T Data type.
 */
template<typename P, typename T>
class SortedExplicitSequencePriorityQueue :
    public SortedSequencePriorityQueue<
        P,
        T,
        amt::SinglyLinkedSequence<PriorityQueueItem<P, T>>
    > {
public:
    /**
     * @brief Inserts a new element at its sorted position within the linked sequence.
     * @param priority Priority of the new element.
     * @param data     Data of the new element.
     */
    void push(const P &priority, const T &data) override;

    /**
     * @brief Assigns the contents of @p other to this queue.
     * @param other Source ADT; must be of the same concrete type.
     * @return Reference to this queue after assignment.
     * @throws utils::type_missmatch If @p other is not a SortedExplicitSequencePriorityQueue.
     */
    ADT &assign(const ADT &other) override;

protected:
    /**
     * @brief Returns index 0, since the highest-priority element is always at the front.
     * @return Always returns 0.
     */
    std::size_t indexOfHighestPriorityBlock() const override;

private:
    using SequenceType = amt::SinglyLinkedSequence<PriorityQueueItem<P, T>>;
    using BlockType = typename SequenceType::BlockType;
};

//----------

/**
 * @brief Two-list priority queue that splits elements between a sorted short list
 * and an unsorted long list to amortise insertion cost.
 * The short list has capacity ceil(sqrt(expectedSize)) and is kept sorted; the long list
 * holds overflow elements.
 * @tparam P Priority type.
 * @tparam T Data type.
 */
template<typename P, typename T>
class TwoLists :
    public ADMS<PriorityQueueItem<P, T>>,
    public PriorityQueue<P, T>
{
public:
    /**
     * @brief Constructs a TwoLists queue sized for @p expectedSize elements.
     * @param expectedSize Expected total number of elements; determines short-list capacity.
     */
    TwoLists(std::size_t expectedSize);

    /**
     * @brief Copy-constructs a TwoLists queue, duplicating both internal sequences.
     * @param other Source queue to copy.
     */
    TwoLists(const TwoLists &other);

    /**
     * @brief Destroys the queue and releases both internal sequences.
     */
    ~TwoLists();

    /**
     * @brief Inserts a new element, routing it to the short or long list as appropriate.
     * @param priority Priority of the new element.
     * @param data     Data of the new element.
     */
    void push(const P &priority, const T &data) override;

    /**
     * @brief Returns a reference to the highest-priority element from the short list.
     * @return Reference to the highest-priority element.
     * @throws std::out_of_range If the queue is empty.
     */
    T &peek() override;

    /**
     * @brief Returns a const reference to the highest-priority element from the short list.
     * @return Const reference to the highest-priority element.
     * @throws std::out_of_range If the queue is empty.
     */
    const T &peek() const override;

    /**
     * @brief Removes the highest-priority element; rebuilds the short list from the long
     * list if the short list becomes empty after removal.
     * @throws std::out_of_range If the queue is empty.
     */
    void pop() override;

    /**
     * @brief Assigns the contents of @p other to this queue.
     * @param other Source ADT; must be of the same concrete type.
     * @return Reference to this queue after assignment.
     * @throws utils::type_missmatch If @p other is not a TwoLists instance.
     */
    ADT &assign(const ADT &other) override;

    /**
     * @brief Removes all elements from both internal sequences.
     */
    void clear() override;

    /**
     * @brief Returns the total number of elements across both internal sequences.
     * @return Combined element count.
     */
    std::size_t size() const override;

    /**
     * @brief Checks whether the queue contains no elements.
     * @return @c true if the short list is empty (implying the whole queue is empty).
     */
    bool isEmpty() const override;

    /**
     * @brief Equality comparison; always throws because it is not supported.
     * @param other ADT to compare against.
     * @return Never returns normally.
     * @throws utils::unavailable_function_call Always.
     */
    bool equals(const ADT &other) const override;

private:
    using ItemType = PriorityQueueItem<P, T>;
    using ShortSequenceType = amt::ImplicitSequence<ItemType>;
    using LongSequenceType = amt::SinglyLinkedSequence<ItemType>;
    using ShortBlockType = typename ShortSequenceType::BlockType;
    using LongBlockType = typename LongSequenceType::BlockType;

private:
    ShortSequenceType *m_shortSequence;
    LongSequenceType  *m_longSequence;
};

//----------

/**
 * @brief Min-priority queue implemented as a binary heap stored in an implicit hierarchy.
 * Provides O(log n) push and pop, and O(1) peek.
 * @tparam P Priority type.
 * @tparam T Data type.
 */
template<typename P, typename T>
class BinaryHeap : public PriorityQueue<P, T>, public ADS<PriorityQueueItem<P, T>> {
public:
    /**
     * @brief Constructs an empty binary heap.
     */
    BinaryHeap();

    /**
     * @brief Copy-constructs a binary heap from another instance.
     * @param other Source heap to copy.
     */
    BinaryHeap(const BinaryHeap &other);

    /**
     * @brief Inserts a new element and restores the heap property by sifting up.
     * @param priority Priority of the new element.
     * @param data     Data of the new element.
     */
    void push(const P &priority, const T &data) override;

    /**
     * @brief Returns a reference to the root element, which has the highest priority.
     * @return Reference to the minimum-priority element.
     * @throws std::out_of_range If the heap is empty.
     */
    T &peek() override;

    /**
     * @brief Returns a const reference to the root element, which has the highest priority.
     * @return Const reference to the minimum-priority element.
     * @throws std::out_of_range If the heap is empty.
     */
    const T &peek() const override;

    /**
     * @brief Removes the root element and restores the heap property by sifting down.
     * @throws std::out_of_range If the heap is empty.
     */
    void pop() override;

    /**
     * @brief Assigns the contents of @p other to this heap.
     * @param other Source ADT; must be of the same concrete type.
     * @return Reference to this heap after assignment.
     * @throws utils::type_missmatch If @p other is not a BinaryHeap instance.
     */
    ADT &assign(const ADT &other) override;

    /**
     * @brief Equality comparison; always throws because it is not supported.
     * @param other ADT to compare against.
     * @return Never returns normally.
     * @throws utils::type_missmatch Always.
     */
    bool equals(const ADT &other) const override;

private:
    using HierarchyType = amt::BinaryImplicitHierarchy<PriorityQueueItem<P, T>>;
    using BlockType = typename HierarchyType::BlockType;

private:
    /**
     * @brief Returns a typed pointer to the underlying binary implicit hierarchy.
     * @return Pointer to the internal @c HierarchyType instance.
     */
    HierarchyType *getHierarchy() const;
};

//----------

template<typename P, typename T, typename SequenceType>
SequencePriorityQueue<P, T, SequenceType>::SequencePriorityQueue() :
    ADS<PriorityQueueItem<P, T>>(new SequenceType()) {
}

template<typename P, typename T, typename SequenceType>
SequencePriorityQueue<P, T, SequenceType>::SequencePriorityQueue(
    const SequencePriorityQueue &other
) :
    ADS<PriorityQueueItem<P, T>>(new SequenceType(&other.getSequence())) {
}

template<typename P, typename T, typename SequenceType>
bool SequencePriorityQueue<P, T, SequenceType>::equals(const ADT &) const {
    throw utils::unavailable_function_call();
}

template<typename P, typename T, typename SequenceType>
void SequencePriorityQueue<P, T, SequenceType>::assignSafe(
    const SequencePriorityQueue *other
) {
    if (other == nullptr) {
        throw utils::type_missmatch();
    }

    if (this == other) {
        return;
    }

    this->getSequence()->assign(*other->getSequence());
}

template<typename P, typename T, typename SequenceType>
SequenceType *SequencePriorityQueue<P, T, SequenceType>::getSequence() const {
    return dynamic_cast<SequenceType*>(this->m_memoryStructure);
}

//----------

template<typename P, typename T, typename SequenceType>
T &UnsortedSequencePriorityQueue<P, T, SequenceType>::peek() {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    return this->findHighestPriorityBlock()->data.data;
}

template<typename P, typename T, typename SequenceType>
const T &UnsortedSequencePriorityQueue<P, T, SequenceType>::peek() const {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    return this->findHighestPriorityBlock()->data.data;
}

template<typename P, typename T, typename SequenceType>
typename SequenceType::BlockType*
UnsortedSequencePriorityQueue<P, T, SequenceType>::findHighestPriorityBlock() const {
    BlockType *bestBlock = this->getSequence()->accessFirst();
    this->getSequence()->processBlocksForward(
        this->getSequence()->accessNext(*bestBlock),
        [&](BlockType *b) {
            if (bestBlock->data.priority > b->data.priority) {
                bestBlock = b;
            }
        }
    );
    return bestBlock;
}

//----------

template<typename P, typename T, typename SequenceType>
T &SortedSequencePriorityQueue<P, T, SequenceType>::peek() {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    return this->getSequence()
        ->access(this->indexOfHighestPriorityBlock())
        ->data.data;
}

template<typename P, typename T, typename SequenceType>
const T &SortedSequencePriorityQueue<P, T, SequenceType>::peek() const {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    return this->getSequence()
        ->access(this->indexOfHighestPriorityBlock())
        ->data.data;
}

template<typename P, typename T, typename SequenceType>
void SortedSequencePriorityQueue<P, T, SequenceType>::pop() {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    this->getSequence()->remove(this->indexOfHighestPriorityBlock());
}

//----------

template<typename P, typename T>
void UnsortedImplicitSequencePriorityQueue<P, T>::push(
    const P &priority,
    const T &data
) {
    PriorityQueueItem<P, T> &item = this->getSequence()->emplaceLast().data;
    item.priority = priority;
    item.data = data;
}

template<typename P, typename T>
void UnsortedImplicitSequencePriorityQueue<P, T>::pop() {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    BlockType *bestBlock = this->findHighestPriorityBlock();
    BlockType *lastBlock = this->getSequence()->accessLast();
    if (bestBlock != lastBlock) {
        std::swap(bestBlock->data, lastBlock->data);
    }
    this->getSequence()->removeLast();
}

template<typename P, typename T>
ADT &UnsortedImplicitSequencePriorityQueue<P, T>::assign(
    const ADT &other
) {
    this->assignSafe(dynamic_cast<const UnsortedImplicitSequencePriorityQueue*>(&other));
    return *this;
}

//----------

template<typename P, typename T>
void UnsortedExplicitSequencePriorityQueue<P, T>::push(
    const P &priority,
    const T &data
) {
    PriorityQueueItem<P, T> &item = this->getSequence()->emplaceFirst().data;
    item.priority = priority;
    item.data = data;
}

template<typename P, typename T>
void UnsortedExplicitSequencePriorityQueue<P, T>::pop() {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    BlockType *bestBlock = this->findHighestPriorityBlock();
    BlockType *firstBlock = this->getSequence()->accessFirst();
    if (bestBlock != firstBlock) {
        std::swap(bestBlock->data, firstBlock->data);
    }
    this->getSequence()->removeFirst();
}

template<typename P, typename T>
ADT &UnsortedExplicitSequencePriorityQueue<P, T>::assign(
    const ADT &other
) {
    this->assignSafe(dynamic_cast<const UnsortedExplicitSequencePriorityQueue*>(&other));
    return *this;
}

//----------

template<typename P, typename T>
void SortedImplicitSequencePriorityQueue<P, T>::push(
    const P &priority,
    const T &data
) {
    PriorityQueueItem<P, T> *item = nullptr;

    if (this->isEmpty() || priority <= this->getSequence()->accessLast()->data.priority) {
        item = &this->getSequence()->emplaceLast().data;
    } else if (priority >= this->getSequence()->accessFirst()->data.priority) {
        item = &this->getSequence()->emplaceFirst().data;
    } else {
        BlockType *block = this->getSequence()->findBlockWithProperty(
            [&priority](BlockType *b) -> bool {
                return b->data.priority <= priority;
            }
        );
        item = &this->getSequence()->emplaceBefore(*block).data;
    }

    item->priority = priority;
    item->data = data;
}

template<typename P, typename T>
ADT &SortedImplicitSequencePriorityQueue<P, T>::assign(
    const ADT &other
) {
    this->assignSafe(dynamic_cast<const SortedImplicitSequencePriorityQueue*>(&other));
    return *this;
}

template<typename P, typename T>
std::size_t
SortedImplicitSequencePriorityQueue<P, T>::indexOfHighestPriorityBlock() const {
    return this->size() - 1;
}

//----------

template<typename P, typename T>
void SortedExplicitSequencePriorityQueue<P, T>::push(
    const P &priority,
    const T &data
) {
    PriorityQueueItem<P, T> *item = nullptr;

    if (this->isEmpty() || priority <= this->getSequence()->accessFirst()->data.priority) {
        item = &this->getSequence()->emplaceFirst().data;
    } else if (priority >= this->getSequence()->accessLast()->data.priority) {
        item = &this->getSequence()->emplaceLast().data;
    } else {
        BlockType *block = this->getSequence()->findPreviousToBlockWithProperty(
            [priority](BlockType *b) -> bool {
                return b->data.priority >= priority;
            }
        );
        item = &this->getSequence()->emplaceAfter(*block).data;
    }

    item->priority = priority;
    item->data = data;
}

template<typename P, typename T>
ADT &SortedExplicitSequencePriorityQueue<P, T>::assign(
    const ADT &other
) {
    this->assignSafe(dynamic_cast<const SortedExplicitSequencePriorityQueue*>(&other));
    return *this;
}

template<typename P, typename T>
std::size_t
SortedExplicitSequencePriorityQueue<P, T>::indexOfHighestPriorityBlock() const {
    return 0;
}

//----------

template<typename P, typename T>
TwoLists<P, T>::TwoLists(std::size_t expectedSize) :
    m_shortSequence(new ShortSequenceType(
        static_cast<std::size_t>(std::ceil(std::sqrt(expectedSize))),
        false)),
    m_longSequence(new LongSequenceType()) {
}

template<typename P, typename T>
TwoLists<P, T>::TwoLists(const TwoLists<P, T> &other) :
    m_shortSequence(new ShortSequenceType(*other.m_shortSequence)),
    m_longSequence(new LongSequenceType(*other.m_longSequence)) {
}

template<typename P, typename T>
TwoLists<P, T>::~TwoLists() {
    delete m_shortSequence;
    delete m_longSequence;
}

template<typename P, typename T>
void TwoLists<P, T>::push(const P &priority, const T &data) {
    bool pushToShort = false;
    if (m_shortSequence->size() < m_shortSequence->getCapacity()) {
        pushToShort = true;
    } else if (!m_shortSequence->isEmpty() && priority < m_shortSequence->accessFirst()->data.priority) {
        pushToShort = true;
        ItemType worst = m_shortSequence->accessFirst()->data;
        m_shortSequence->removeFirst();
        m_longSequence->emplaceLast().data = worst;
    }

    if (pushToShort) {
        ItemType *item = nullptr;
        if (m_shortSequence->isEmpty() || priority <= m_shortSequence->accessLast()->data.priority) {
            item = &m_shortSequence->emplaceLast().data;
        } else if (priority >= m_shortSequence->accessFirst()->data.priority) {
            item = &m_shortSequence->emplaceFirst().data;
        } else {
            ShortBlockType *block = m_shortSequence->findBlockWithProperty(
                [&priority](ShortBlockType *b) -> bool {
                    return b->data.priority <= priority;
                }
            );
            item = &m_shortSequence->emplaceBefore(*block).data;
        }
        item->priority = priority;
        item->data = data;
    } else {
        ItemType &item = m_longSequence->emplaceLast().data;
        item.priority = priority;
        item.data = data;
    }
}

template<typename P, typename T>
T &TwoLists<P, T>::peek() {
    if (m_shortSequence->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    return m_shortSequence->accessLast()->data.data;
}

template<typename P, typename T>
const T &TwoLists<P, T>::peek() const {
    if (m_shortSequence->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    return m_shortSequence->accessLast()->data.data;
}

template<typename P, typename T>
void TwoLists<P, T>::pop() {
    if (m_shortSequence->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }

    m_shortSequence->removeLast();

    if (m_shortSequence->isEmpty() && !m_longSequence->isEmpty()) {
        size_t elementsToMove = std::min(m_shortSequence->getCapacity(), m_longSequence->size());
        for (size_t i = 0; i < elementsToMove; ++i) {
            LongBlockType *bestBlock = m_longSequence->accessFirst();
            m_longSequence->processBlocksForward(
                m_longSequence->accessNext(*bestBlock),
                [&](LongBlockType *b) {
                    if (b->data.priority < bestBlock->data.priority) {
                        bestBlock = b;
                    }
                }
            );

            ItemType bestData = bestBlock->data;
            if (m_longSequence->accessFirst() == bestBlock) {
                m_longSequence->removeFirst();
            } else {
                m_longSequence->removeNext(*m_longSequence->accessPrevious(*bestBlock));
            }

            m_shortSequence->emplaceFirst().data = bestData;
        }
    }
}

template<typename P, typename T>
ADT &TwoLists<P, T>::assign(const ADT &other) {
    if (this == &other) {
        return *this;
    }

    const TwoLists *o = dynamic_cast<const TwoLists*>(&other);
    if (o == nullptr) {
        throw utils::type_missmatch();
    }

    m_shortSequence->assign(*o->m_shortSequence);
    m_longSequence->assign(*o->m_longSequence);

    return *this;
}

template<typename P, typename T>
void TwoLists<P, T>::clear() {
    m_shortSequence->clear();
    m_longSequence->clear();
}

template<typename P, typename T>
std::size_t TwoLists<P, T>::size() const {
    return m_shortSequence->size() + m_longSequence->size();
}

template<typename P, typename T>
bool TwoLists<P, T>::isEmpty() const {
    return m_shortSequence->isEmpty();
}

template<typename P, typename T>
bool TwoLists<P, T>::equals(const ADT &other) const {
    (void)other;
    throw utils::unavailable_function_call();
}

//----------

template<typename P, typename T>
BinaryHeap<P, T>::BinaryHeap() :
    ADS<PriorityQueueItem<P, T>>(
        new amt::BinaryImplicitHierarchy<PriorityQueueItem<P, T>>()) {
}

template<typename P, typename T>
BinaryHeap<P, T>::BinaryHeap(const BinaryHeap &other) :
    ADS<PriorityQueueItem<P, T>>(
        new amt::BinaryImplicitHierarchy<PriorityQueueItem<P, T>>(
            *other.getHierarchy())) {
}

template<typename P, typename T>
void BinaryHeap<P, T>::push(const P &priority, const T &data) {
    PriorityQueueItem<P, T> &item = this->getHierarchy()->emplaceLastLeaf().data;
    item.priority = priority;
    item.data = data;

    BlockType *current = this->getHierarchy()->accessLastLeaf();
    BlockType *parent = this->getHierarchy()->accessParent(*current);

    while (parent != nullptr && current->data.priority < parent->data.priority) {
        std::swap(current->data, parent->data);
        current = parent;
        parent = this->getHierarchy()->accessParent(*current);
    }
}

template<typename P, typename T>
T &BinaryHeap<P, T>::peek() {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    return this->getHierarchy()->accessRoot()->data.data;
}

template<typename P, typename T>
const T &BinaryHeap<P, T>::peek() const {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    return this->getHierarchy()->accessRoot()->data.data;
}

template<typename P, typename T>
void BinaryHeap<P, T>::pop() {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }

    HierarchyType *hierarchy = this->getHierarchy();
    BlockType *current = hierarchy->accessRoot();
    std::swap(current->data, hierarchy->accessLastLeaf()->data);
    hierarchy->removeLastLeaf();

    if (this->isEmpty()) {
        return;
    }

    while (true) {
        BlockType *left = hierarchy->accessLeftSon(*current);
        BlockType *right = hierarchy->accessRightSon(*current);
        BlockType *better = left;

        if (left == nullptr) {
            break;
        }

        if (right != nullptr && right->data.priority < left->data.priority) {
            better = right;
        }

        if (better->data.priority >= current->data.priority) {
            break;
        }

        std::swap(current->data, better->data);
        current = better;
    }
}

template<typename P, typename T>
ADT &BinaryHeap<P, T>::assign(const ADT &other) {
    if (this == &other) {
        return *this;
    }

    const BinaryHeap *o = dynamic_cast<const BinaryHeap*>(&other);
    if (o == nullptr) {
        throw utils::type_missmatch();
    }

    this->getHierarchy()->assign(*o->getHierarchy());

    return *this;
}

template<typename P, typename T>
bool BinaryHeap<P, T>::equals(const ADT &other) const {
    (void)other;
    throw utils::type_missmatch();
}

template<typename P, typename T>
typename BinaryHeap<P, T>::HierarchyType*
BinaryHeap<P, T>::getHierarchy() const {
    return dynamic_cast<HierarchyType*>(this->m_memoryStructure);
}


} // namespace ds::adt

#endif

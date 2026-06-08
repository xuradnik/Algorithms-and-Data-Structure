#ifndef LIBDS_ADT_QUEUE_HPP
#define LIBDS_ADT_QUEUE_HPP

#include <libds/adt/abstract_data_type.hpp>
#include <libds/amt/explicit_sequence.hpp>
#include <libds/amt/implicit_sequence.hpp>

#include <libds/utils.hpp>


namespace ds::adt {


/**
 * @brief Abstract base interface for a generic FIFO queue.
 *
 * @tparam T Type of elements stored in the queue.
 */
template<typename T>
class Queue : virtual public ADT {
public:
    /**
     * @brief Inserts an element at the back of the queue.
     *
     * @param element The element to insert.
     */
    virtual void push(const T &element) = 0;

    /**
     * @brief Returns a reference to the front element without removing it.
     *
     * @return Reference to the front element.
     */
    virtual T &peek() = 0;

    /**
     * @brief Returns a const reference to the front element without removing it.
     *
     * @return Const reference to the front element.
     */
    virtual const T &peek() const = 0;

    /**
     * @brief Removes the front element from the queue.
     */
    virtual void pop() = 0;
};

//----------

/**
 * @brief Queue implementation backed by a fixed-capacity cyclic implicit sequence.
 *
 * Uses a circular buffer of pre-allocated memory; capacity must be reserved upfront
 * or defaults to INIT_CAPACITY.
 *
 * @tparam T Type of elements stored in the queue.
 */
template<typename T>
class ImplicitQueue : public Queue<T>, public ADS<T> {
public:
    /**
     * @brief Constructs an ImplicitQueue with default capacity.
     */
    ImplicitQueue();

    /**
     * @brief Copy-constructs an ImplicitQueue from another instance.
     *
     * @param other The source ImplicitQueue to copy from.
     */
    ImplicitQueue(const ImplicitQueue &other);

    /**
     * @brief Constructs an ImplicitQueue with a specified capacity.
     *
     * @param capacity Maximum number of elements the queue can hold.
     */
    ImplicitQueue(std::size_t capacity);

    /**
     * @brief Inserts an element at the back of the queue.
     *
     * @param element The element to insert.
     * @throws std::runtime_error If the queue has reached its capacity.
     */
    void push(const T &element) override;

    /**
     * @brief Returns a reference to the front element without removing it.
     *
     * @return Reference to the front element.
     * @throws std::out_of_range If the queue is empty.
     */
    T &peek() override;

    /**
     * @brief Returns a const reference to the front element without removing it.
     *
     * @return Const reference to the front element.
     * @throws std::out_of_range If the queue is empty.
     */
    const T &peek() const override;

    /**
     * @brief Removes the front element from the queue.
     *
     * @throws std::out_of_range If the queue is empty.
     */
    void pop() override;

    /**
     * @brief Assigns the contents of another ADT to this queue.
     *
     * The other ADT must be an ImplicitQueue and must not exceed this queue's capacity.
     *
     * @param other The source ADT to assign from.
     * @return Reference to this queue after assignment.
     * @throws std::invalid_argument If the other queue exceeds this queue's capacity.
     * @throws ds::utils::type_missmatch If the other ADT is not an ImplicitQueue.
     */
    ADT &assign(const ADT &other) override;

    /**
     * @brief Removes all elements from the queue without releasing underlying memory.
     */
    void clear() override;

    /**
     * @brief Returns the number of elements currently in the queue.
     *
     * @return Current number of elements.
     */
    std::size_t size() const override;

    /**
     * @brief Checks whether this queue is equal to another ADT.
     *
     * Two ImplicitQueues are equal if they have the same size and identical element order.
     *
     * @param other The ADT to compare against.
     * @return True if both queues are equal, false otherwise.
     */
    bool equals(const ADT &other) const override;

    /**
     * @brief Returns the total capacity of the underlying cyclic buffer.
     *
     * @return Maximum number of elements the queue can hold.
     */
    std::size_t getCapacity() const;

    /**
     * @brief Reserves a new capacity for the underlying cyclic buffer.
     *
     * @param newCapacity The desired new capacity.
     */
    void reserveCapacity(std::size_t newCapacity);

private:
    /**
     * @brief Returns a typed pointer to the underlying cyclic implicit sequence.
     *
     * @return Pointer to the internal CyclicImplicitSequence.
     */
    amt::CyclicImplicitSequence<T> *getSequence() const;

private:
    /** @brief Default initial capacity when none is specified. */
    static const int INIT_CAPACITY = 100;

private:
    /** @brief Index in the cyclic buffer where the next element will be inserted. */
    std::size_t m_insertionIndex;

    /** @brief Index in the cyclic buffer of the next element to be removed. */
    std::size_t m_removalIndex;

    /** @brief Current number of elements in the queue. */
    std::size_t m_size;
};

//----------

/**
 * @brief Queue implementation backed by a singly linked explicit sequence.
 *
 * Grows dynamically by allocating nodes on demand, with no fixed capacity limit.
 *
 * @tparam T Type of elements stored in the queue.
 */
template<typename T>
class ExplicitQueue : public Queue<T>, public ADS<T> {
public:
    /**
     * @brief Constructs an empty ExplicitQueue.
     */
    ExplicitQueue();

    /**
     * @brief Copy-constructs an ExplicitQueue from another instance.
     *
     * @param other The source ExplicitQueue to copy from.
     */
    ExplicitQueue(const ExplicitQueue &other);

    /**
     * @brief Inserts an element at the back of the queue.
     *
     * @param element The element to insert.
     */
    void push(const T &element) override;

    /**
     * @brief Returns a reference to the front element without removing it.
     *
     * @return Reference to the front element.
     * @throws std::out_of_range If the queue is empty.
     */
    T &peek() override;

    /**
     * @brief Returns a const reference to the front element without removing it.
     *
     * @return Const reference to the front element.
     * @throws std::out_of_range If the queue is empty.
     */
    const T &peek() const override;

    /**
     * @brief Removes the front element from the queue.
     *
     * @throws std::out_of_range If the queue is empty.
     */
    void pop() override;

    /**
     * @brief Assigns the contents of another ADT to this queue.
     *
     * @param other The source ADT to assign from.
     * @return Reference to this queue after assignment.
     * @throws ds::utils::type_missmatch If the other ADT is not an ExplicitQueue.
     */
    ADT &assign(const ADT &other) override;

    /**
     * @brief Checks whether this queue is equal to another ADT.
     *
     * Delegates element-wise comparison to the underlying sequence.
     *
     * @param other The ADT to compare against.
     * @return True if both queues are equal, false otherwise.
     */
    bool equals(const ADT &other) const override;

private:
    /**
     * @brief Returns a typed pointer to the underlying singly linked sequence.
     *
     * @return Pointer to the internal SinglyLinkedSequence.
     */
    amt::SinglyLinkedSequence<T> *getSequence() const;
};

//----------

template<typename T>
ImplicitQueue<T>::ImplicitQueue() : ImplicitQueue(INIT_CAPACITY) {
}

template<typename T>
ImplicitQueue<T>::ImplicitQueue(std::size_t capacity) :
    ADS<T>(new amt::CyclicImplicitSequence<T>(capacity, true)),
    m_insertionIndex(0),
    m_removalIndex(0),
    m_size(0) {
}

template<typename T>
ImplicitQueue<T>::ImplicitQueue(const ImplicitQueue &other) :
    ADS<T>(new amt::CyclicImplicitSequence<T>(*other.getSequence())),
    m_insertionIndex(other.m_insertionIndex),
    m_removalIndex(other.m_removalIndex),
    m_size(other.m_size) {
}

template<typename T>
void ImplicitQueue<T>::push(const T &element) {
    if (this->size() == this->getSequence()->size()) {
        throw std::runtime_error("Implicit queue is full.");
    }
    this->getSequence()->access(m_insertionIndex)->data = element;
    m_insertionIndex = this->getSequence()->indexOfNext(m_insertionIndex);
    ++m_size;
}

template<typename T>
T &ImplicitQueue<T>::peek() {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    return this->getSequence()->access(m_removalIndex)->data;
}

template<typename T>
const T &ImplicitQueue<T>::peek() const {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    return this->getSequence()->access(m_removalIndex)->data;
}

template<typename T>
void ImplicitQueue<T>::pop() {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    m_removalIndex = this->getSequence()->indexOfNext(m_removalIndex);
    --m_size;
}

template<typename T>
ADT &ImplicitQueue<T>::assign(const ADT &other) {
    if (this == &other) {
        return *this;
    }

    if (this->getSequence()->size() < other.size()) {
        throw std::invalid_argument("Not enough capacity!");
    }

    const ImplicitQueue *o = dynamic_cast<const ImplicitQueue*>(&other);
    if (o == nullptr) {
        throw utils::type_missmatch();
    }

    this->getSequence()->assign(*o->getSequence());

    m_insertionIndex = o->m_insertionIndex;
    m_removalIndex = o->m_removalIndex;
    m_size = o->m_size;

    return *this;
}

template<typename T>
void ImplicitQueue<T>::clear() {
    m_insertionIndex = m_removalIndex;
    m_size = 0;
}

template<typename T>
std::size_t ImplicitQueue<T>::size() const {
    return m_size;
}

template<typename T>
bool ImplicitQueue<T>::equals(const ADT &other) const {
    if (this == &other) {
        return true;
    }

    if (this->size() != other.size()) {
        return false;
    }

    const ImplicitQueue *o = dynamic_cast<const ImplicitQueue*>(&other);
    if (o == nullptr) {
        return false;
    }

    std::size_t myIndex = m_removalIndex;
    std::size_t otherIndex = o->m_removalIndex;
    for (std::size_t i = 0; i < m_size; ++i) {
        if (this->getSequence()->access(myIndex)->data
            != o->getSequence()->access(otherIndex)->data) {
            return false;
        }
        myIndex = this->getSequence()->indexOfNext(myIndex);
        otherIndex = o->getSequence()->indexOfNext(otherIndex);
    }

    return true;
}

template<typename T>
std::size_t ImplicitQueue<T>::getCapacity() const {
    return this->getSequence()->size();
}

template<typename T>
void ImplicitQueue<T>::reserveCapacity(std::size_t newCapacity) {
    return this->getSequence()->reserveCapacity(newCapacity);
}

template<typename T>
amt::CIS<T> *ImplicitQueue<T>::getSequence() const {
    return dynamic_cast<amt::CyclicImplicitSequence<T>*>(
        this->m_memoryStructure);
}

//----------

template<typename T>
ExplicitQueue<T>::ExplicitQueue() : ADS<T>(new amt::SinglyLinkedSequence<T>()) {
}

template<typename T>
ExplicitQueue<T>::ExplicitQueue(const ExplicitQueue &other) :
    ADS<T>(new amt::SinglyLinkedSequence<T>(*other.getSequence())) {
}

template<typename T>
void ExplicitQueue<T>::push(const T &element) {
    this->getSequence()->emplaceLast().data = element;
}

template<typename T>
T &ExplicitQueue<T>::peek() {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    return this->getSequence()->accessFirst()->data;
}

template<typename T>
const T &ExplicitQueue<T>::peek() const {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    return this->getSequence()->accessFirst()->data;
}

template<typename T>
void ExplicitQueue<T>::pop() {
    if (this->isEmpty()) {
        throw std::out_of_range("Queue is empty.");
    }
    this->getSequence()->removeFirst();
}

template<typename T>
ADT &ExplicitQueue<T>::assign(const ADT &other) {
    const ExplicitQueue *o = dynamic_cast<const ExplicitQueue*>(&other);
    if (o == nullptr) {
        throw utils::type_missmatch();
    }
    this->getSequence()->assign(*o->getSequence());
    return *this;
}

template<typename T>
bool ExplicitQueue<T>::equals(const ADT &other) const {
    const ExplicitQueue *o = dynamic_cast<const ExplicitQueue*>(&other);
    if (o == nullptr) {
        return false;
    }
    return this->getSequence()->equals(*o->getSequence());
}

template<typename T>
amt::SinglyLS<T> *ExplicitQueue<T>::getSequence() const {
    return dynamic_cast<amt::SinglyLinkedSequence<T>*>(this->m_memoryStructure);
}


} // namespace ds::adt

#endif

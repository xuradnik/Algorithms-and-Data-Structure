#ifndef LIBDS_ADT_LIST_HPP
#define LIBDS_ADT_LIST_HPP

#include "../adt/abstract_data_type.hpp"
#include "../amt/explicit_sequence.hpp"
#include "../amt/implicit_sequence.hpp"

namespace ds::adt {


/**
 * @brief List interface.
 *
 * @tparam T The type of elements stored in the list.
 */
template<typename T>
class List : virtual public ADT {
public:
    /**
     * @brief Returns the zero-based index of the first occurrence of @p element.
     * @param element The element to search for.
     * @return Zero-based index of @p element, or @c INVALID_INDEX if not found.
     */
    virtual std::size_t calculateIndex(const T &element) const = 0;

    /**
     * @brief Returns @c true if @p element is present in the list.
     * @param element The element to search for.
     * @return @c true if at least one occurrence of @p element exists.
     */
    virtual bool contains(const T &element) const = 0;

    /**
     * @brief Returns a mutable reference to the first element.
     * @return Reference to the first element in the list.
     */
    virtual T &accessFirst() = 0;

    /**
     * @brief Returns a mutable reference to the last element.
     * @return Reference to the last element in the list.
     */
    virtual T &accessLast() = 0;

    /**
     * @brief Returns a mutable reference to the element at position @p index.
     * @param index Zero-based position to access.
     * @return Reference to the element at @p index.
     */
    virtual T &access(std::size_t index) = 0;

    /**
     * @brief Returns a const reference to the first element.
     * @return Const reference to the first element in the list.
     */
    virtual const T &accessFirst() const = 0;

    /**
     * @brief Returns a const reference to the last element.
     * @return Const reference to the last element in the list.
     */
    virtual const T &accessLast() const = 0;

    /**
     * @brief Returns a const reference to the element at position @p index.
     * @param index Zero-based position to access.
     * @return Const reference to the element at @p index.
     */
    virtual const T &access(std::size_t index) const = 0;

    /**
     * @brief Inserts @p element at the front of the list.
     * @param element The value to insert.
     */
    virtual void insertFirst(const T &element) = 0;

    /**
     * @brief Appends @p element at the back of the list.
     * @param element The value to insert.
     */
    virtual void insertLast(const T &element) = 0;

    /**
     * @brief Inserts @p element at position @p index, shifting existing elements.
     * @param element The value to insert.
     * @param index Zero-based position at which to insert.
     */
    virtual void insert(const T &element, std::size_t index) = 0;

    /**
     * @brief Removes the first element from the list.
     */
    virtual void removeFirst() = 0;

    /**
     * @brief Removes the last element from the list.
     */
    virtual void removeLast() = 0;

    /**
     * @brief Removes the element at position @p index.
     * @param index Zero-based position of the element to remove.
     */
    virtual void remove(std::size_t index) = 0;
};

//----------

/**
 * @brief A specific list implementation backed by a given sequence type.
 *
 * Delegates all operations to an owned @c SequenceType instance,
 * adapting its block-based interface to the element-based @c List interface.
 * All specific list variants are strong aliases of this template.
 *
 * @tparam T The type of elements stored in the list.
 * @tparam SequenceType The underlying sequence used for storage.
 */
template<typename T, typename SequenceType>
class GeneralList : public List<T>, public ADS<T> {
public:
    using Iterator = typename SequenceType::IteratorType;
    using BlockType = typename SequenceType::BlockType;

public:
    /**
     * @brief Constructs an empty list with a default-constructed sequence.
     */
    GeneralList();

    /**
     * @brief Copy-constructs the list by deep-copying the underlying sequence.
     * @param other The source list to copy from.
     */
    GeneralList(const GeneralList &other);

    /**
     * @brief Returns the zero-based index of the first occurrence of @p element.
     * @param element The element to search for.
     * @return Zero-based index of @p element, or @c INVALID_INDEX if not found.
     */
    std::size_t calculateIndex(const T &element) const override;

    /**
     * @brief Returns @c true if @p element is present in the list.
     * @param element The element to search for.
     * @return @c true if at least one occurrence of @p element exists.
     */
    bool contains(const T &element) const override;

    /**
     * @brief Returns a mutable reference to the first element.
     * @return Reference to the first element in the list.
     * @throws std::out_of_range If the list is empty.
     */
    T &accessFirst() override;

    /**
     * @brief Returns a mutable reference to the last element.
     * @return Reference to the last element in the list.
     * @throws std::out_of_range If the list is empty.
     */
    T &accessLast() override;

    /**
     * @brief Returns a mutable reference to the element at position @p index.
     * @param index Zero-based position to access.
     * @return Reference to the element at @p index.
     * @throws std::out_of_range If @p index is out of range.
     */
    T &access(std::size_t index) override;

    /**
     * @brief Returns a const reference to the first element.
     * @return Const reference to the first element in the list.
     * @throws std::out_of_range If the list is empty.
     */
    const T &accessFirst() const override;

    /**
     * @brief Returns a const reference to the last element.
     * @return Const reference to the last element in the list.
     * @throws std::out_of_range If the list is empty.
     */
    const T &accessLast() const override;

    /**
     * @brief Returns a const reference to the element at position @p index.
     * @param index Zero-based position to access.
     * @return Const reference to the element at @p index.
     * @throws std::out_of_range If @p index is out of range.
     */
    const T &access(std::size_t index) const override;

    /**
     * @brief Inserts @p element at the front of the list.
     * @param element The value to insert.
     */
    void insertFirst(const T &element) override;

    /**
     * @brief Appends @p element at the back of the list.
     * @param element The value to insert.
     */
    void insertLast(const T &element) override;

    /**
     * @brief Inserts @p element at position @p index, shifting existing elements.
     * @param element The value to insert.
     * @param index Zero-based position at which to insert.
     * @throws std::out_of_range If @p index is greater than the current size.
     */
    void insert(const T &element, std::size_t index) override;

    /**
     * @brief Removes the first element from the list.
     * @throws std::out_of_range If the list is empty.
     */
    void removeFirst() override;

    /**
     * @brief Removes the last element from the list.
     * @throws std::out_of_range If the list is empty.
     */
    void removeLast() override;

    /**
     * @brief Removes the element at position @p index.
     * @param index Zero-based position of the element to remove.
     * @throws std::out_of_range If @p index is out of range.
     */
    void remove(std::size_t index) override;

    /**
     * @brief Assigns the contents of @p other to this list.
     * @param other The source data structure to copy from.
     * @return Reference to this list.
     */
    AbstractDataType &assign(const AbstractDataType &other) override;

    /**
     * @brief Returns @c true if this list is element-wise equal to @p other.
     * @param other The data structure to compare against.
     * @return @c true if both lists contain equivalent elements in the same order.
     */
    bool equals(const AbstractDataType &other) const override;

    /**
     * @brief Returns an iterator to the first element of the list.
     * @return Iterator positioned at the first element.
     */
    Iterator begin() const;

    /**
     * @brief Returns a past-the-end iterator for the list.
     * @return Iterator representing the end sentinel.
     */
    Iterator end() const;

    /**
     * @brief Assigns the contents of @p other to this list.
     * @param other The source data structure to copy from.
     * @return Reference to this list.
     */
    GeneralList &operator=(const GeneralList &other) = default;

protected:
    /**
     * @brief Returns a typed pointer to the underlying sequence.
     * @return Pointer to the @c SequenceType instance owned by this list.
     */
    SequenceType *getSequence() const;

    /**
     * @brief Returns the block at @p index, throwing if it does not exist.
     * @param index Zero-based position of the block to retrieve.
     * @return Pointer to the block at @p index.
     * @throws std::out_of_range If @p index is out of range or the block is @c nullptr.
     */
    BlockType *getBlockSafe(std::size_t index) const;
};

//----------

/**
 * @brief A list backed by an @c ImplicitSequence.
 * @tparam T The type of elements stored in the list.
 */
template<typename T>
class ImplicitList : public GeneralList<T, amt::IS<T>> {
public:
    /**
     * @brief Returns the total number of elements that can be held without reallocation.
     */
    std::size_t getCapacity() const;

    /**
     * @brief Ensures the list can hold at least @p newCapacity elements without reallocation.
     * @param newCapacity The minimum desired capacity.
     */
    void reserveCapacity(std::size_t newCapacity);
};

//----------

/**
 * @brief A list backed by a @c CyclicImplicitSequence.
 * @tparam T The type of elements stored in the list.
 */
template<typename T>
class ImplicitCyclicList : public GeneralList<T, amt::CIS<T>> { };

//----------

/**
 * @brief A list backed by a @c SinglyLinkedSequence.
 * @tparam T The type of elements stored in the list.
 */
template<typename T>
class SinglyLinkedList : public GeneralList<T, amt::SinglyLS<T>> { };

//----------

/**
 * @brief A list backed by a @c SinglyCyclicLinkedSequence.
 * @tparam T The type of elements stored in the list.
 */
template<typename T>
class SinglyCyclicLinkedList : public GeneralList<T, amt::SinglyCLS<T>> { };

//----------

/**
 * @brief A list backed by a @c DoublyLinkedSequence.
 * @tparam T The type of elements stored in the list.
 */
template<typename T>
class DoublyLinkedList : public GeneralList<T, amt::DoublyLS<T>> { };

//----------

/**
 * @brief A list backed by a @c DoublyCyclicLinkedSequence.
 * @tparam T The type of elements stored in the list.
 */
template<typename T>
class DoublyCyclicLinkedList : public GeneralList<T, amt::DoublyCLS<T>> { };

//----------

template<typename T, typename SequenceType>
GeneralList<T, SequenceType>::GeneralList() : ADS<T>(new SequenceType()) {
}

template<typename T, typename SequenceType>
GeneralList<T, SequenceType>::GeneralList(const GeneralList &other) :
    ADS<T>(new SequenceType(*other.getSequence())) {
}

template<typename T, typename SequenceType>
std::size_t GeneralList<T, SequenceType>::calculateIndex(
    const T &element
) const {
    std::size_t result = 0;
    BlockType *block = this->getSequence()->findBlockWithProperty(
            [&](BlockType *b) {
                if (b->data == element) {
                    return true;
                } else {
                    result++;
                    return false;
                }
            }
        );
    return block != nullptr ? result : INVALID_INDEX;
}

template<typename T, typename SequenceType>
bool GeneralList<T, SequenceType>::contains(const T &element) const {
    return this->calculateIndex(element) != INVALID_INDEX;
}

template<typename T, typename SequenceType>
T &GeneralList<T, SequenceType>::accessFirst() {
    return this->getBlockSafe(0)->data;
}

template<typename T, typename SequenceType>
T &GeneralList<T, SequenceType>::accessLast() {
    return this->getBlockSafe(this->size() - 1)->data;
}

template<typename T, typename SequenceType>
T &GeneralList<T, SequenceType>::access(std::size_t index) {
    return this->getBlockSafe(index)->data;
}

template<typename T, typename SequenceType>
const T &GeneralList<T, SequenceType>::accessFirst() const {
    return this->getBlockSafe(0)->data;
}

template<typename T, typename SequenceType>
const T &GeneralList<T, SequenceType>::accessLast() const {
    return this->getBlockSafe(this->size() - 1)->data;
}

template<typename T, typename SequenceType>
const T &GeneralList<T, SequenceType>::access(std::size_t index) const {
    return this->getBlockSafe(index)->data;
}

template<typename T, typename SequenceType>
void GeneralList<T, SequenceType>::insertFirst(const T &element) {
    this->getSequence()->emplaceFirst().data = element;
}

template<typename T, typename SequenceType>
void GeneralList<T, SequenceType>::insertLast(const T &element) {
    this->getSequence()->emplaceLast().data = element;
}

template<typename T, typename SequenceType>
void GeneralList<T, SequenceType>::insert(const T &element, std::size_t index) {
    if (index > this->size()) {
        throw std::out_of_range("Invalid index.");
    }

    this->getSequence()->emplace(index).data = element;
}

template<typename T, typename SequenceType>
void GeneralList<T, SequenceType>::removeFirst() {
    if (this->isEmpty()) {
        throw std::out_of_range("List is empty.");
    }
    this->getSequence()->removeFirst();
}

template<typename T, typename SequenceType>
void GeneralList<T, SequenceType>::removeLast() {
    if (this->isEmpty()) {
        throw std::out_of_range("List is empty.");
    }
    this->getSequence()->removeLast();
}

template<typename T, typename SequenceType>
void GeneralList<T, SequenceType>::remove(std::size_t index) {
    if (index >= this->size()) {
        throw std::out_of_range("Invalid index.");
    }
    this->getSequence()->remove(index);
}

template<typename T, typename SequenceType>
AbstractDataType &GeneralList<T, SequenceType>::assign(
    const AbstractDataType &other
) {
    const GeneralList *o = dynamic_cast<const GeneralList*>(&other);
    if (o == nullptr) {
        throw std::runtime_error("Invalid assign: type missmatch.");
    }
    this->getSequence()->assign(*o->getSequence());
    return *this;
}

template<typename T, typename SequenceType>
bool GeneralList<T, SequenceType>::equals(
    const AbstractDataType &other
) const {
    const GeneralList *o = dynamic_cast<const GeneralList*>(&other);
    if (o == nullptr) {
        return false;
    }
    return this->getSequence()->equals(*o->getSequence());
}

template<typename T, typename SequenceType>
typename GeneralList<T, SequenceType>::Iterator
GeneralList<T, SequenceType>::begin() const {
    return this->getSequence()->begin();
}

template<typename T, typename SequenceType>
typename GeneralList<T, SequenceType>::Iterator
GeneralList<T, SequenceType>::end() const {
    return this->getSequence()->end();
}

template<typename T, typename SequenceType>
SequenceType *GeneralList<T, SequenceType>::getSequence() const {
    return dynamic_cast<SequenceType*>(this->m_memoryStructure);
}

template<typename T, typename SequenceType>
typename GeneralList<T, SequenceType>::BlockType*
GeneralList<T, SequenceType>::getBlockSafe(std::size_t index) const {
    BlockType *block = this->getSequence()->access(index);
    if (block == nullptr) {
        throw std::out_of_range("Invalid index.");
    }
    return block;
}

//----------

template<typename T>
std::size_t ImplicitList<T>::getCapacity() const {
    return this->getSequence()->getCapacity();
}

template<typename T>
void ImplicitList<T>::reserveCapacity(std::size_t newCapacity) {
    this->getSequence()->reserveCapacity(newCapacity);
}


} // namespace ds::adt

#endif

#ifndef LIBDS_ADT_ABSTRACT_DATA_TYPE_HPP
#define LIBDS_ADT_ABSTRACT_DATA_TYPE_HPP

#include "../amt/abstract_memory_type.hpp"

namespace ds::adt {


/**
 * @brief Top-level abstract interface shared by all data types.
 */
class AbstractDataType {
public:
    /**
     * @brief Virtual destructor ensuring correct cleanup of derived types.
     */
    virtual ~AbstractDataType() = default;

    /**
     * @brief Replaces the contents of this structure with a copy of @p other.
     * @param other The source data type to copy from.
     * @return Reference to this data type.
     */
    virtual AbstractDataType &assign(const AbstractDataType &other) = 0;

    /**
     * @brief Removes all elements, leaving the structure empty.
     */
    virtual void clear() = 0;

    /**
     * @brief Returns the number of elements currently stored.
     * @return Current element count.
     */
    virtual std::size_t size() const = 0;

    /**
     * @brief Returns @c true when no elements are stored.
     * @return @c true if the structure contains no elements.
     */
    virtual bool isEmpty() const = 0;

    /**
     * @brief Returns @c true if this structure is element-wise equal to @p other.
     * @param other The data type to compare against.
     * @return @c true if both structures contain equivalent elements.
     */
    virtual bool equals(const AbstractDataType &other) const = 0;
};

/** @brief Convenience alias for @c AbstractDataType. */
using ADT = AbstractDataType;

//----------

/**
 * @brief Base class for data structures that own a single @c AMT memory structure.
 *
 * Manages the lifetime of the underlying @c AMT instance and delegates
 * @c size(), @c isEmpty(), and @c clear() to it. Copy construction is disabled;
 * derived classes are expected to implement @c assign for deep-copy semantics.
 *
 * @tparam T The type of elements stored in this data structure.
 */
template<typename T>
class AbstractDataStructure : virtual public ADT {
public:
    using ValueType = T;
    using Reference = T&;
    using ConstReference = const T&;
    using SizeType = std::size_t;
    using DifferenceType = std::ptrdiff_t;

public:
    /**
     * @brief Constructs the structure, taking ownership of @p memoryStructure.
     * @param memoryStructure Pointer to the AMT instance to adopt; must not be @c nullptr.
     */
    explicit AbstractDataStructure(amt::AMT *memoryStructure);

    /**
     * @brief Copy construction is disabled; use @c assign for deep-copy semantics.
     */
    AbstractDataStructure(const AbstractDataStructure &other) = delete;

    /**
     * @brief Destructor that deletes the owned memory structure.
     */
    ~AbstractDataStructure() override;

    /**
     * @brief Removes all elements by forwarding to the underlying memory structure.
     */
    void clear() override;

    /**
     * @brief Returns the number of elements by querying the underlying memory structure.
     * @return Current element count as reported by @c m_memoryStructure.
     */
    std::size_t size() const override;

    /**
     * @brief Returns @c true when the structure contains no elements.
     * @return @c true if @c size() equals zero.
     */
    bool isEmpty() const override;

    /**
     * @brief Copy-assigns @p other to this structure by delegating to @c assign.
     * @param other The source structure to copy from.
     * @return Reference to this structure.
     */
    AbstractDataStructure &operator=(const AbstractDataStructure &other);

protected:
    /** @brief Pointer to the owned memory structure that stores all elements. */
    amt::AMT *m_memoryStructure;
};

/** @brief Convenience alias for @c AbstractDataStructure. */
template<typename T>
using ADS = AbstractDataStructure<T>;

//----------

/**
 * @brief Base class for data structures that may own multiple underlying memory structures.
 * @tparam T The type of elements stored in this data multistructure.
 */
template<typename T>
class AbstractDataMultistructure : virtual public ADT {
public:
    /**
     * @brief Copy-assigns @p other to this multistructure by delegating to @c assign.
     * @param other The source multistructure to copy from.
     * @return Reference to this multistructure.
     */
    AbstractDataMultistructure &operator=(
        const AbstractDataMultistructure &other);
};

/** @brief Convenience alias for @c AbstractDataMultistructure. */
template<typename T>
using ADMS = AbstractDataMultistructure<T>;

//----------

template<typename T>
AbstractDataMultistructure<T> &AbstractDataMultistructure<T>::operator=(
    const AbstractDataMultistructure<T> &other
) {
    this->assign(other);
    return *this;
}

//----------

template<typename T>
AbstractDataStructure<T>::AbstractDataStructure(amt::AMT *memoryStructure) :
    m_memoryStructure(memoryStructure) {
}

template<typename T>
AbstractDataStructure<T>::~AbstractDataStructure() {
    delete m_memoryStructure;
}

template<typename T>
void AbstractDataStructure<T>::clear() {
    m_memoryStructure->clear();
}

template<typename T>
std::size_t AbstractDataStructure<T>::size() const {
    return m_memoryStructure->size();
}

template<typename T>
bool AbstractDataStructure<T>::isEmpty() const {
    return this->size() == 0;
}

template<typename T>
AbstractDataStructure<T> &AbstractDataStructure<T>::operator=(
    const AbstractDataStructure &other
) {
    this->assign(other);
    return *this;
}


} // namespace ds::adt

#endif

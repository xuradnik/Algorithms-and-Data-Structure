#ifndef LIBDS_ADT_ARRAY_HPP
#define LIBDS_ADT_ARRAY_HPP

#include <libds/adt/abstract_data_type.hpp>
#include <libds/amt/implicit_sequence.hpp>

namespace ds::adt {


/**
 * @brief Describes a single array dimension with a base index and a size.
 *
 * Allows arrays and matrices whose valid index range starts at any integer,
 * not just zero.
 */
struct Dimension {
    /** @brief The lowest valid index. */
    long long base;

    /** @brief The number of elements. */
    std::size_t size;

    /**
     * @brief Returns @c true if both dimensions have the same base and size.
     * @param rhs The dimension to compare against.
     * @return @c true if @c base and @c size are equal.
     */
    bool operator==(const Dimension &rhs) const = default;

    /**
     * @brief Returns @c true if the dimensions differ in base or size.
     * @param rhs The dimension to compare against.
     * @return @c true if @c base or @c size differ.
     */
    bool operator!=(const Dimension &rhs) const = default;
};


//----------

/**
 * @brief A fixed-size one-dimensional array with a configurable base index.
 *
 * Backed by an @c ImplicitSequence, elements are stored contiguously. The valid
 * index range is @c [base, base + size), where @c base defaults to 0 when
 * constructed from a plain size. The array cannot be cleared or resized after
 * construction.
 *
 * @tparam T The type of elements stored in the array.
 */
template<typename T>
class Array : public ADS<T> {
public:
    using Iterator = typename amt::IS<T>::IteratorType;

public:
    /**
     * @brief Constructs an array of @p size elements with base index 0.
     * @param size Number of elements to allocate.
     */
    Array(std::size_t size);

    /**
     * @brief Constructs an array described by @p dimension.
     * @param dimension Dimension providing both base index and element count.
     */
    Array(Dimension dimension);

    /**
     * @brief Copy-constructs the array, duplicating all elements and the base index.
     * @param other The source array to copy from.
     */
    Array(const Array<T> &other);

    /**
     * @brief Not supported; always throws.
     * @throws std::logic_error Always -- arrays have a fixed size and cannot be cleared.
     */
    void clear() override;

    /**
     * @brief Returns @c false; arrays are never considered empty.
     * @return @c false always.
     */
    bool isEmpty() const override;

    /**
     * @brief Returns the number of elements in the array.
     * @return Total element count, equal to the @c size member of the @c Dimension.
     */
    std::size_t size() const override;

    /**
     * @brief Returns the base (lowest valid) index of the array.
     * @return The base index supplied at construction.
     */
    long long getBase() const;

    /**
     * @brief Returns a mutable reference to the element at logical index @p index.
     * @param index Logical index in the range @c [base, base + size).
     * @return Reference to the element at @p index.
     * @throws std::out_of_range If @p index is outside the valid range.
     */
    T &access(long long index);

    /**
     * @brief Returns a const reference to the element at logical index @p index.
     * @param index Logical index in the range @c [base, base + size).
     * @return Const reference to the element at @p index.
     * @throws std::out_of_range If @p index is outside the valid range.
     */
    const T &access(long long index) const;

    /**
     * @brief Assigns the contents of @p other to this array.
     *
     * Both arrays must have the same base and size; otherwise an exception is thrown.
     *
     * @param other The source data structure to copy from.
     * @return Reference to this array.
     * @throws std::runtime_error If @p other is not an @c Array<T>.
     * @throws std::logic_error If the base index or size of @p other differs.
     */
    ADT &assign(const ADT &other) override;

    /**
     * @brief Returns @c true if this array is element-wise equal to @p other.
     *
     * Two arrays are equal only if they share the same base, size, and element values.
     *
     * @param other The data structure to compare against.
     * @return @c true if both arrays have equal dimensions and elements.
     */
    bool equals(const ADT &other) const override;

    /**
     * @brief Returns an iterator to the first element of the array.
     * @return Iterator positioned at the first element.
     */
    Iterator begin() const;

    /**
     * @brief Returns a past-the-end iterator for the array.
     * @return Iterator representing the end sentinel.
     */
    Iterator end() const;

    /**
     * @brief Copy-assigns @p other to this array.
     * @return Reference to this array.
     */
    Array &operator=(const Array &other) = default;

private:
    /**
     * @brief Returns @c true if @p index falls within the valid logical range.
     * @param index The logical index to validate.
     * @return @c true if @p index is in @c [base, base + size).
     */
    bool validateIndex(long long index) const;

    /**
     * @brief Maps a logical index to a zero-based physical index, throwing if invalid.
     * @param index The logical index to map.
     * @return Zero-based physical index into the underlying sequence.
     * @throws std::out_of_range If @p index is outside the valid range.
     */
    std::size_t mapIndexSafe(long long index) const;

    /**
     * @brief Returns a typed pointer to the underlying implicit sequence.
     * @return Pointer to the @c amt::IS<T> instance owned by this array.
     */
    amt::IS<T> *getSequence() const;

private:
    /** @brief The lowest valid logical index; set at construction and immutable thereafter. */
    long long m_base;
};

//----------

/**
 * @brief Abstract interface for two-dimensional matrix containers.
 *
 * Provides a single pure virtual element accessor indexed by two logical indices.
 *
 * @tparam T The type of elements stored in the matrix.
 */
template<typename T>
class Matrix : virtual public ADT {
public:
    /**
     * @brief Returns mutable reference to the element at logical position (@p index1, @p index2).
     * @param index1 Logical row index.
     * @param index2 Logical column index.
     * @return Mutable reference to the element at the given position.
     */
    virtual T &access(long long index1, long long index2) = 0;

    /**
     * @brief Returns const reference to the element at logical position (@p index1, @p index2).
     * @param index1 Logical row index.
     * @param index2 Logical column index.
     * @return Const reference the element at the given position.
     */
    virtual const T &access(long long index1, long long index2) const = 0;
};

//----------

/**
 * @brief A fixed-size two-dimensional matrix backed by contiguous (row-major) storage.
 *
 * Elements are stored in a single @c ImplicitSequence of size
 * @c dimension1.size * @c dimension2.size. Each axis has an independent base index,
 * allowing the valid index ranges to start at any integer. The matrix cannot be
 * cleared or resized after construction.
 *
 * @tparam T The type of elements stored in the matrix.
 */
template<typename T>
class CompactMatrix : public Matrix<T>, public ADS<T> {
public:
    /**
     * @brief Constructs a matrix of @p size1 × @p size2 elements, both axes base-0.
     * @param size1 Number of rows.
     * @param size2 Number of columns.
     */
    CompactMatrix(std::size_t size1, std::size_t size2);

    /**
     * @brief Constructs a matrix described by two @c Dimension axes.
     * @param dimension1 Row axis descriptor providing base index and row count.
     * @param dimension2 Column axis descriptor providing base index and column count.
     */
    CompactMatrix(Dimension dimension1, Dimension dimension2);

    /**
     * @brief Copy-constructs the matrix, duplicating all elements and both dimensions.
     * @param other The source matrix to copy from.
     */
    CompactMatrix(const CompactMatrix &other);

    /**
     * @brief Assigns the contents of @p other to this matrix.
     *
     * Both matrices must share the same dimensions; otherwise an exception is thrown.
     *
     * @param other The source data structure to copy from.
     * @return Reference to this matrix.
     * @throws std::logic_error If the dimensions of @p other differ from this matrix.
     */
    ADT &assign(const ADT &other) override;

    /**
     * @brief Not supported; always throws.
     * @throws std::logic_error Always — compact matrices have a fixed size.
     */
    void clear() override;

    /**
     * @brief Returns @c true if this matrix is element-wise equal to @p other.
     *
     * Two matrices are equal only if they share the same dimensions and element values.
     *
     * @param other The data structure to compare against.
     * @return @c true if both matrices have equal dimensions and elements.
     */
    bool equals(const ADT &other) const override;

    /**
     * @brief Returns @c false; compact matrices are never considered empty.
     * @return @c false always.
     */
    bool isEmpty() const override;

    /**
     * @brief Returns the total number of elements in the matrix.
     * @return Product of @c dimension1.size and @c dimension2.size.
     */
    std::size_t size() const override;

    /**
     * @brief Returns the row axis descriptor.
     * @return A copy of @c m_dimension1 describing the row base and count.
     */
    Dimension getDimension1() const;

    /**
     * @brief Returns the column axis descriptor.
     * @return A copy of @c m_dimension2 describing the column base and count.
     */
    Dimension getDimension2() const;

    /**
     * @brief Returns a mutable reference to the element at (@p index1, @p index2).
     * @param index1 Logical row index in @c [dimension1.base, dimension1.base + dimension1.size).
     * @param index2 Logical column index in @c [dimension2.base, dimension2.base + dimension2.size).
     * @return Reference to the element at the given position.
     * @throws std::out_of_range If either index is outside its valid range.
     */
    T &access(long long index1, long long index2) override;

    /**
     * @brief Returns a const reference to the element at (@p index1, @p index2).
     * @param index1 Logical row index in @c [dimension1.base, dimension1.base + dimension1.size).
     * @param index2 Logical column index in @c [dimension2.base, dimension2.base + dimension2.size).
     * @return Const reference to the element at the given position.
     * @throws std::out_of_range If either index is outside its valid range.
     */
    const T &access(long long index1, long long index2) const override;

    /**
     * @brief Copy-assigns @p other to this matrix.
     * @return Reference to this matrix.
     */
    CompactMatrix &operator=(const CompactMatrix &other) = default;

private:
    /**
     * @brief Returns @c true if both indices fall within their respective valid ranges.
     * @param index1 Logical row index to validate.
     * @param index2 Logical column index to validate.
     * @return @c true if both indices are within their dimension bounds.
     */
    bool validateIndices(long long index1, long long index2) const;

    /**
     * @brief Maps a pair of logical indices to a zero-based physical index in row-major order.
     * @param index1 Logical row index.
     * @param index2 Logical column index.
     * @return Zero-based physical index into the underlying flat sequence.
     * @throws std::out_of_range If either index is outside its valid range.
     */
    std::size_t mapIndicesSafe(long long index1, long long index2) const;

    /**
     * @brief Returns a typed pointer to the underlying implicit sequence.
     * @return Pointer to the @c amt::IS<T> instance owned by this matrix.
     */
    amt::IS<T> *getSequence() const;

private:
    /** @brief Row axis descriptor, holding base index and row count. */
    Dimension m_dimension1;

    /** @brief Column axis descriptor, holding base index and column count. */
    Dimension m_dimension2;
};

//----------

template<typename T>
Array<T>::Array(std::size_t size) : Array(Dimension(0, size)) {
}

template<typename T>
Array<T>::Array(Dimension dimension) :
    ADS<T>(new amt::IS<T>(dimension.size, true)),
    m_base(dimension.base) {
}

template<typename T>
Array<T>::Array(const Array<T> &other) :
    ADS<T>(new amt::IS<T>(*other.getSequence())),
    m_base(other.m_base) {
}

template<typename T>
void Array<T>::clear() {
    throw std::logic_error("Array can't be cleared.");
}

template<typename T>
bool Array<T>::isEmpty() const {
    return false;
}

template<typename T>
std::size_t Array<T>::size() const {
    return this->getSequence()->size();
}

template<typename T>
long long Array<T>::getBase() const {
    return m_base;
}

template<typename T>
T &Array<T>::access(long long index) {
    return this->getSequence()->access(this->mapIndexSafe(index))->data;
}

template<typename T>
const T &Array<T>::access(long long index) const {
    return this->getSequence()->access(this->mapIndexSafe(index))->data;
}

template<typename T>
ADT &Array<T>::assign(const ADT &other) {
    if (this == &other) {
        return *this;
    }

    const Array *o = dynamic_cast<const Array*>(&other);

    if (o == nullptr) {
        throw std::runtime_error("Invalid assignment. Type missmatch.");
    }

    if (m_base != o->m_base || this->size() != o->size()) {
        throw std::logic_error("Invalid assignment. Dimension missmatch.");
    }

    this->getSequence()->assign(*o->getSequence());

    return *this;
}

template<typename T>
bool Array<T>::equals(const ADT &other) const {
    const Array *o = dynamic_cast<const Array*>(&other);
    return
        o != nullptr &&
        m_base == o->m_base &&
        this->getSequence()->equals(*o->getSequence());
}

template<typename T>
typename Array<T>::Iterator Array<T>::begin() const {
    return this->getSequence()->begin();
}

template<typename T>
typename Array<T>::Iterator Array<T>::end() const {
    return this->getSequence()->end();
}

template<typename T>
amt::IS<T> *Array<T>::getSequence() const {
    return dynamic_cast<amt::IS<T> *>(this->m_memoryStructure);
}

template<typename T>
bool Array<T>::validateIndex(long long index) const {
    return index >= m_base
        && index < m_base + static_cast<long long>(this->size());
}

template<typename T>
std::size_t Array<T>::mapIndexSafe(long long index) const {
    if (!this->validateIndex(index)) {
        throw std::out_of_range("Invalid index.");
    }
    return index - m_base;
}

//----------

template<typename T>
CompactMatrix<T>::CompactMatrix(std::size_t size1, std::size_t size2) :
    CompactMatrix<T>(Dimension{0, size1}, Dimension{0, size2}) {
}

template<typename T>
CompactMatrix<T>::CompactMatrix(Dimension dimension1, Dimension dimension2) :
    ADS<T>(new amt::IS<T>(dimension1.size * dimension2.size, true)),
    m_dimension1(dimension1),
    m_dimension2(dimension2) {
}

template<typename T>
CompactMatrix<T>::CompactMatrix(const CompactMatrix<T> &other) :
    ADS<T>(new amt::IS<T>(*other.getSequence())),
    m_dimension1(other.m_dimension1),
    m_dimension2(other.m_dimension2) {
}

template<typename T>
ADT &CompactMatrix<T>::assign(const ADT &other) {
    if (this == &other) {
        return *this;
    }

    const CompactMatrix *o = dynamic_cast<const CompactMatrix*>(&other);

    if (o == nullptr) {
        throw std::runtime_error("Invalid assignment. Type missmatch.");
    }

    if (m_dimension1 != o->m_dimension1 || m_dimension2 != o->m_dimension2) {
        throw std::logic_error("CompactMatrix dimensions are different.");
    }

    this->getSequence()->assign(*o->getSequence());

    return *this;
}

template<typename T>
void CompactMatrix<T>::clear() {
    throw std::logic_error("CompactMatrix can't be cleared.");
}

template<typename T>
bool CompactMatrix<T>::equals(const ADT &other) const {
    const CompactMatrix *o = dynamic_cast<const CompactMatrix *>(&other);
    return
        o != nullptr &&
        m_dimension1 == o->m_dimension1 &&
        m_dimension2 == o->m_dimension2 &&
        this->getSequence()->equals(*o->getSequence());
}

template<typename T>
bool CompactMatrix<T>::isEmpty() const {
    return false;
}

template<typename T>
std::size_t CompactMatrix<T>::size() const {
    return m_dimension1.size * m_dimension2.size;
}

template<typename T>
Dimension CompactMatrix<T>::getDimension1() const {
    return m_dimension1;
}

template<typename T>
Dimension CompactMatrix<T>::getDimension2() const {
    return m_dimension2;
}

template<typename T>
T &CompactMatrix<T>::access(long long index1, long long index2) {
    return this->getSequence()->access(this->mapIndicesSafe(index1, index2))->data;
}

template<typename T>
const T &CompactMatrix<T>::access(long long index1, long long index2) const {
    return this->getSequence()->access(this->mapIndicesSafe(index1, index2))->data;
}

template<typename T>
bool CompactMatrix<T>::validateIndices(
    long long index1,
    long long index2
) const {
    return
        index1 >= m_dimension1.base &&
        index2 >= m_dimension2.base &&
        index1 < m_dimension1.base + static_cast<long long>(m_dimension1.size) &&
        index2 < m_dimension2.base + static_cast<long long>(m_dimension2.size);
}

template<typename T>
std::size_t CompactMatrix<T>::mapIndicesSafe(
    long long index1,
    long long index2
) const {
    if (!this->validateIndices(index1, index2)) {
        throw std::out_of_range("Invalid index.");
    }
    return (index1 - m_dimension1.base) * m_dimension2.size
         + (index2 - m_dimension2.base);
}

template<typename T>
amt::IS<T> *CompactMatrix<T>::getSequence() const {
    return dynamic_cast<amt::IS<T>*>(this->m_memoryStructure);
}


} // namespace ds::adt

#endif

#ifndef LIBDS_ADT_STACK_HPP
#define LIBDS_ADT_STACK_HPP

#include <libds/adt/abstract_data_type.hpp>
#include <libds/amt/explicit_sequence.hpp>
#include <libds/amt/implicit_sequence.hpp>


namespace ds::adt {


/**
 * @brief Abstract base interface for a last-in, first-out (LIFO) stack data structure.
 *
 * @tparam T The type of elements stored in the stack.
 */
template<typename T>
class Stack : virtual public ADT {
public:
    /**
     * @brief Pushes a copy of the given element onto the top of the stack.
     *
     * @param element The element to push onto the stack.
     */
    virtual void push(const T &element) = 0;

    /**
     * @brief Returns a reference to the top element without removing it.
     *
     * @return A reference to the top element of the stack.
     */
    virtual T &peek() = 0;

    /**
     * @brief Returns a const reference to the top element without removing it.
     *
     * @return A const reference to the top element of the stack.
     */
    virtual const T &peek() const = 0;

    /**
     * @brief Removes the top element from the stack.
     */
    virtual void pop() = 0;
};

//----------

/**
 * @brief Stack implementation backed by an implicit sequence.
 *
 * Elements are pushed and popped from the end of the underlying ImplicitSequence,
 * giving amortized O(1) push and O(1) pop.
 *
 * @tparam T The type of elements stored in the stack.
 */
template<typename T>
class ImplicitStack : public Stack<T>, public ADS<T> {
public:
    /**
     * @brief Initializes an empty ImplicitStack.
     */
    ImplicitStack();

    /**
     * @brief Copy-constructs an ImplicitStack as a deep copy of @p other.
     *
     * @param other The ImplicitStack to copy from.
     */
    ImplicitStack(const ImplicitStack &other);

    /**
     * @brief Pushes a copy of @p element onto the top of the stack.
     *
     * @param element The element to push.
     */
    void push(const T &element) override;

    /**
     * @brief Returns a reference to the top element without removing it.
     *
     * @return A reference to the top element.
     * @throws std::out_of_range If the stack is empty.
     */
    T &peek() override;

    /**
     * @brief Returns a const reference to the top element without removing it.
     *
     * @return A const reference to the top element.
     * @throws std::out_of_range If the stack is empty.
     */
    const T &peek() const override;

    /**
     * @brief Removes the top element from the stack.
     *
     * @throws std::out_of_range If the stack is empty.
     */
    void pop() override;

    /**
     * @brief Assigns the contents of @p other to this stack, replacing all existing elements.
     *
     * @param other The ADT to assign from; must be an ImplicitStack of the same type.
     * @return A reference to this stack after assignment.
     * @throws utils::type_missmatch If @p other is not an ImplicitStack.
     */
    ADT &assign(const ADT &other) override;

    /**
     * @brief Checks whether this stack is equal to @p other by comparing element sequences.
     *
     * @param other The ADT to compare against.
     * @return @c true if @p other is an ImplicitStack with identical elements in the same order, @c false otherwise.
     */
    bool equals(const ADT &other) const override;

private:
    /**
     * @brief Returns a typed pointer to the underlying ImplicitSequence.
     *
     * @return A pointer to the ImplicitSequence backing this stack.
     */
    amt::ImplicitSequence<T> *getSequence() const;
};

//----------

/**
 * @brief Stack implementation backed by an explicit singly-linked sequence.
 *
 * Elements are pushed and popped from the front of the underlying SinglyLinkedSequence,
 * giving O(1) push and O(1) pop with no reallocation.
 *
 * @tparam T The type of elements stored in the stack.
 */
template<typename T>
class ExplicitStack : public Stack<T>, public ADS<T> {
public:
    /**
     * @brief Constructs an empty ExplicitStack, allocating a new SinglyLinkedSequence.
     */
    ExplicitStack();

    /**
     * @brief Copy-constructs an ExplicitStack as a deep copy of @p other.
     *
     * @param other The ExplicitStack to copy from.
     */
    ExplicitStack(const ExplicitStack &other);

    /**
     * @brief Pushes a copy of @p element onto the top of the stack.
     *
     * @param element The element to push.
     */
    void push(const T &element) override;

    /**
     * @brief Returns a reference to the top element without removing it.
     *
     * @return A reference to the top element.
     * @throws std::out_of_range If the stack is empty.
     */
    T &peek() override;

    /**
     * @brief Returns a const reference to the top element without removing it.
     *
     * @return A const reference to the top element.
     * @throws std::out_of_range If the stack is empty.
     */
    const T &peek() const override;

    /**
     * @brief Removes the top element from the stack.
     *
     * @throws std::out_of_range If the stack is empty.
     */
    void pop() override;

    /**
     * @brief Assigns the contents of @p other to this stack, replacing all existing elements.
     *
     * @param other The ADT to assign from; must be an ExplicitStack of the same type.
     * @return A reference to this stack after assignment.
     * @throws utils::type_missmatch If @p other is not an ExplicitStack.
     */
    ADT &assign(const ADT &other) override;

    /**
     * @brief Checks whether this stack is equal to @p other by comparing element sequences.
     *
     * @param other The ADT to compare against.
     * @return @c true if @p other is an ExplicitStack with identical elements in the same order, @c false otherwise.
     */
    bool equals(const ADT &other) const override;

private:
    /**
     * @brief Returns a typed pointer to the underlying SinglyLinkedSequence.
     *
     * @return A pointer to the SinglyLinkedSequence backing this stack.
     */
    amt::SinglyLinkedSequence<T> *getSequence() const;
};

//----------

template<typename T>
ImplicitStack<T>::ImplicitStack() : ADS<T>(new amt::ImplicitSequence<T>()) {
}

template<typename T>
ImplicitStack<T>::ImplicitStack(const ImplicitStack &other) :
    ADS<T>(new amt::ImplicitSequence<T>(*other.getSequence())) {
}

template<typename T>
void ImplicitStack<T>::push(const T &element) {
    this->getSequence()->emplaceLast().data = element;
}

template<typename T>
T &ImplicitStack<T>::peek() {
    if (this->isEmpty()) {
        throw std::out_of_range("Stack is empty.");
    }
    return this->getSequence()->accessLast()->data;
}

template<typename T>
const T &ImplicitStack<T>::peek() const {
    if (this->isEmpty()) {
        throw std::out_of_range("Stack is empty.");
    }
    return this->getSequence()->accessLast()->data;
}

template<typename T>
void ImplicitStack<T>::pop() {
    if (this->isEmpty()) {
        throw std::out_of_range("Stack is empty.");
    }
    this->getSequence()->removeLast();
}

template<typename T>
ADT &ImplicitStack<T>::assign(const ADT &other) {
    const ImplicitStack *o = dynamic_cast<const ImplicitStack*>(&other);
    if (o == nullptr) {
        throw utils::type_missmatch();
    }
    this->getSequence()->assign(*o->getSequence());
    return *this;
}

template<typename T>
bool ImplicitStack<T>::equals(const ADT &other) const {
    const ImplicitStack *o = dynamic_cast<const ImplicitStack*>(&other);
    if (o == nullptr) {
        return false;
    }
    return this->getSequence()->equals(*o->getSequence());
}

template<typename T>
amt::IS<T> *ImplicitStack<T>::getSequence() const {
    return dynamic_cast<amt::ImplicitSequence<T>*>(this->m_memoryStructure);
}

//----------

template<typename T>
ExplicitStack<T>::ExplicitStack() : ADS<T>(new amt::SinglyLinkedSequence<T>()) {
}

template<typename T>
ExplicitStack<T>::ExplicitStack(const ExplicitStack &other) :
    ADS<T>(new amt::SinglyLinkedSequence<T>(*other.getSequence())) {
}

template<typename T>
void ExplicitStack<T>::push(const T &element) {
    this->getSequence()->emplaceFirst().data = element;
}

template<typename T>
T &ExplicitStack<T>::peek() {
    if (this->isEmpty()) {
        throw std::out_of_range("Stack is empty.");
    }
    return this->getSequence()->accessFirst()->data;
}

template<typename T>
const T &ExplicitStack<T>::peek() const {
    if (this->isEmpty()) {
        throw std::out_of_range("Stack is empty.");
    }
    return this->getSequence()->accessFirst()->data;
}

template<typename T>
void ExplicitStack<T>::pop() {
    if (this->isEmpty()) {
        throw std::out_of_range("Stack is empty.");
    }
    this->getSequence()->removeFirst();
}

template<typename T>
ADT &ExplicitStack<T>::assign(const ADT &other) {
    const ExplicitStack *o = dynamic_cast<const ExplicitStack*>(&other);
    if (o == nullptr) {
        throw utils::type_missmatch();
    }
    this->getSequence()->assign(*o->getSequence());
    return *this;
}

template<typename T>
bool ExplicitStack<T>::equals(const ADT &other) const {
    const ExplicitStack *o = dynamic_cast<const ExplicitStack*>(&other);
    if (o == nullptr) {
        return false;
    }
    return this->getSequence()->equals(*o->getSequence());
}

template<typename T>
amt::SinglyLS<T> *ExplicitStack<T>::getSequence() const {
    return dynamic_cast<amt::SinglyLS<T> *>(this->m_memoryStructure);
}


} // namespace ds::adt

#endif

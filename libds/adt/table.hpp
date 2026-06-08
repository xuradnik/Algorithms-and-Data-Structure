#ifndef LIBDS_ADT_TABLE_HPP
#define LIBDS_ADT_TABLE_HPP

#include <libds/adt/abstract_data_type.hpp>
#include <libds/amt/explicit_hierarchy.hpp>
#include <libds/amt/implicit_sequence.hpp>

#include <functional>
#include <limits>
#include <random>

namespace ds::adt {


/**
 * @brief A key-data pair stored as a single element in a table.
 * @tparam K The type of the key used for lookup.
 * @tparam T The type of the associated data value.
 */
template<typename K, typename T>
struct TableItem {
    /** @brief The lookup key that uniquely identifies this item. */
    K key;

    /** @brief The data value associated with the key. */
    T data;

    /**
     * @brief Compares two TableItems for equality by key and data.
     * @param other The item to compare against.
     * @return True if both key and data are equal, false otherwise.
     */
    bool operator== (const TableItem<K, T> &other) const = default;

    /**
     * @brief Compares two TableItems for inequality by key and data.
     * @param other The item to compare against.
     * @return True if key or data differ, false otherwise.
     */
    bool operator!= (const TableItem<K, T> &other) const = default;
};

//----------

/**
 * @brief Abstract base for all tables.
 * @tparam K The key type used to identify elements.
 * @tparam T The value type stored in the table.
 */
template<typename K, typename T>
class Table : virtual public ADT {
public:
    /**
     * @brief Inserts a new key-data pair into the table.
     * @param key   The unique key to associate with the data.
     * @param data  The data value to store.
     * @throws std::logic_error If the key already exists in the table.
     */
    virtual void insert(const K &key, const T &data) = 0;

    /**
     * @brief Retrieves a reference to the data associated with the given key.
     * @param key The key whose associated data is to be returned.
     * @return A reference to the data mapped to @p key.
     * @throws std::out_of_range If no element with the given key exists.
     */
    virtual T &find(const K &key);

    /**
     * @brief Attempts to find data by key without throwing on failure.
     * @param key  The key to search for.
     * @param data Output pointer set to the found data on success.
     * @return True if the key was found and @p data was set, false otherwise.
     */
    virtual bool tryFind(const K &key, T *&data) const = 0;

    /**
     * @brief Checks whether a given key exists in the table.
     * @param key The key to look up.
     * @return True if the key is present, false otherwise.
     */
    virtual bool contains(const K &key) const;

    /**
     * @brief Removes and returns the data associated with the given key.
     * @param key The key of the element to remove.
     * @return The data value that was stored under @p key.
     * @throws std::out_of_range If no element with the given key exists.
     */
    virtual T remove(const K &key) = 0;

protected:
    /**
     * @brief Performs a type-safe structural equality check against another ADT.
     * @tparam SelfType The concrete type of this table used for the dynamic cast.
     * @param self  Reference to this table instance.
     * @param other The ADT to compare against.
     * @return True if both tables contain identical key-data pairs, false otherwise.
     */
    template<class SelfType>
    bool equalsUnsafe(SelfType &self, const ADT &other) const;
};

//----------

/**
 * @brief A table implementation backed by a sequential memory structure.
 * @tparam K            The key type used to identify elements.
 * @tparam T            The value type stored in the table.
 * @tparam SequenceType The underlying sequence type used for storage.
 */
template<typename K, typename T, typename SequenceType>
class SequenceTable : public Table<K, T>, public ADS<TableItem<K, T>> {
public:
    using Iterator = typename SequenceType::IteratorType;

public:
    /**
     * @brief Constructs an empty SequenceTable with a new underlying sequence.
     */
    SequenceTable();

    /**
     * @brief Copy-constructs a SequenceTable from another instance.
     * @param other The SequenceTable to copy.
     */
    SequenceTable(const SequenceTable &other);

    /**
     * @brief Attempts to find data by key using the underlying sequence.
     * @param key  The key to search for.
     * @param data Output pointer set to the found data on success.
     * @return True if the key was found and @p data was set, false otherwise.
     */
    bool tryFind(const K &key, T *&data) const override;

    /**
     * @brief Returns an iterator to the first element in the sequence.
     * @return An iterator pointing to the first element.
     */
    Iterator begin() const;

    /**
     * @brief Returns an iterator past the last element in the sequence.
     * @return An iterator representing the end sentinel.
     */
    Iterator end() const;

    /**
     * @brief Assigns the contents of another sequence table to this one.
     * @param other The sequence table to copy from.
     * @throws utils::type_missmatch if @p other is nullptr.
     */
    void assignSafe(const SequenceTable *other);

    /**
     * @brief Assigns the contents of another sequence table to this one.
     * @param other The sequence table to copy from.
     */
    SequenceTable &operator=(const SequenceTable &other);

protected:
    using BlockType = typename SequenceType::BlockType;

protected:
    /**
     * @brief Locates and returns the block whose key matches the given key.
     * @param key The key to search for.
     * @return Pointer to the matching block, or nullptr if not found.
     */
    virtual BlockType *findBlockWithKey(const K &key) const = 0;

    /**
     * @brief Provides access to the underlying sequence memory structure.
     * @return Pointer to the sequence cast to @p SequenceType.
     */
    SequenceType *getSequence() const;
};

//----------

/**
 * @brief A sequence table that stores elements in arbitrary insertion order.
 * @tparam K            The key type used to identify elements.
 * @tparam T            The value type stored in the table.
 * @tparam SequenceType The underlying sequence type used for storage.
 */
template<typename K, typename T, typename SequenceType>
class UnsortedSequenceTable : public SequenceTable<K, T, SequenceType> {
protected:
    using BlockType = typename SequenceType::BlockType;

protected:
    /**
     * @brief Performs a linear search to find the block matching the given key.
     * @param key The key to search for.
     * @return Pointer to the matching block, or nullptr if not found.
     */
    BlockType *findBlockWithKey(const K &key) const override;
};

//----------

/**
 * @brief An unsorted table backed by an sequence.
 * @tparam K The key type used to identify elements.
 * @tparam T The value type stored in the table.
 */
template<typename K, typename T>
class UnsortedImplicitSequenceTable :
    public UnsortedSequenceTable<
        K,
        T,
        amt::ImplicitSequence<TableItem<K, T>>
    > {
public:
    /**
     * @brief Appends a new key-data pair at the end of the sequence.
     * @param key   The unique key to associate with the data.
     * @param data  The data value to store.
     * @throws std::logic_error If an element with the given key already exists.
     */
    void insert(const K &key, const T &data) override;

    /**
     * @brief Removes the element with the given key by swapping it with the last element.
     * @param key The key of the element to remove.
     * @return The data value that was stored under @p key.
     * @throws std::out_of_range If no element with the given key exists.
     */
    T remove(const K &key) override;

    /**
     * @brief Assigns the contents of another ADT to this table.
     * @param other The ADT to copy from.
     * @return A reference to this table after assignment.
     * @throws utils::type_missmatch if @p other is of different type.
     */
    ADT &assign(const ADT &other) override;

    /**
     * @brief Checks structural equality with another ADT instance.
     * @param other The ADT to compare against.
     * @return True if both tables contain identical key-data pairs, false otherwise.
     */
    bool equals(const ADT &other) const override;

private:
    using BlockType = typename amt::ImplicitSequence<TableItem<K, T>>::BlockType;
};

//----------

/**
 * @brief An unsorted table backed by a singly-linked sequence.
 * @tparam K The key type used to identify elements.
 * @tparam T The value type stored in the table.
 */
template<typename K, typename T>
class UnsortedExplicitSequenceTable :
    public UnsortedSequenceTable<
        K,
        T,
        amt::SinglyLinkedSequence<TableItem<K, T>>
    > {
public:
    /**
     * @brief Prepends a new key-data pair at the front of the sequence.
     * @param key   The unique key to associate with the data.
     * @param data  The data value to store.
     * @throws std::logic_error If an element with the given key already exists.
     */
    void insert(const K &key, const T &data) override;

    /**
     * @brief Removes the element with the given key by swapping it with the first element.
     * @param key The key of the element to remove.
     * @return The data value that was stored under @p key.
     * @throws std::out_of_range If no element with the given key exists.
     */
    T remove(const K &key) override;

    /**
     * @brief Assigns the contents of another ADT to this table.
     * @param other The ADT to copy from.
     * @return A reference to this table after assignment.
     * @throws utils::type_missmatch if @p other is of different type.
     */
    ADT &assign(const ADT &other) override;

    /**
     * @brief Checks structural equality with another ADT instance.
     * @param other The ADT to compare against.
     * @return True if both tables contain identical key-data pairs, false otherwise.
     */
    bool equals(const ADT &other) const override;

    /**
     * @brief Equality operator comparing two tables of the same concrete type.
     * @param other The table to compare against.
     * @return True if both tables contain identical key-data pairs, false otherwise.
     */
    bool operator==(const UnsortedExplicitSequenceTable &other) const;

    /**
     * @brief Inequality operator comparing two tables of the same concrete type.
     * @param other The table to compare against.
     * @return True if the tables differ in any key-data pair, false otherwise.
     */
    bool operator!=(const UnsortedExplicitSequenceTable &other) const;

private:
    using BlockType = typename amt::SinglyLinkedSequence<TableItem<K, T>>::BlockType;
};

//----------

/**
 * @brief A table backed by a sorted implicit sequence, enabling binary search lookups.
 * @tparam K The key type used to identify elements; must support comparison operators.
 * @tparam T The value type stored in the table.
 */
template<typename K, typename T>
class SortedSequenceTable :
    public SequenceTable<K, T, amt::ImplicitSequence<TableItem<K, T>>> {
public:
    /**
     * @brief Inserts a new key-data pair in sorted key order.
     * @param key   The unique key to insert.
     * @param data  The data value to associate with the key.
     * @throws std::logic_error If an element with the given key already exists.
     */
    void insert(const K &key, const T &data) override;

    /**
     * @brief Removes and returns the element associated with the given key.
     * @param key The key of the element to remove.
     * @return The data value that was stored under @p key.
     * @throws std::out_of_range If no element with the given key exists.
     */
    T remove(const K &key) override;

    /**
     * @brief Assigns the contents of another ADT to this table.
     * @param other The ADT to copy from.
     * @return A reference to this table after assignment.
     * @throws utils::type_missmatch if @p other is of different type.
     */
    ADT &assign(const ADT &other) override;

    /**
     * @brief Checks structural equality with another ADT instance.
     * @param other The ADT to compare against.
     * @return True if both tables contain identical key-data pairs, false otherwise.
     */
    bool equals(const ADT &other) const override;

protected:
    using BlockType = typename amt::ImplicitSequence<TableItem<K, T>>::BlockType;

protected:
    /**
     * @brief Finds the block exactly matching the given key via binary search.
     * @param key The key to search for.
     * @return Pointer to the matching block, or nullptr if not found.
     */
    BlockType *findBlockWithKey(const K &key) const override;

private:
    /**
     * @brief Performs a binary search and sets @p lastBlock to the closest block found.
     * @param key       The key to search for.
     * @param lastBlock Output set to the last examined block during the search.
     * @return True if an exact match was found, false otherwise.
     */
    bool tryFindBlockWithKey(const K &key, BlockType *&lastBlock) const;
};

//----------

/**
 * @brief A hash table using separate chaining via synonym tables.
 * @tparam K The key type; must be compatible with the supplied or default hash function.
 * @tparam T The value type stored in the table.
 */
template<typename K, typename T>
class  HashTable : public Table<K, T>, public ADMS<TableItem<K, T>> {
public:
    using HashFunctionType = std::function<std::size_t(const K&)>;

public:
    /**
     * @brief Constructs a HashTable with the default std::hash function and default capacity.
     */
    HashTable();

    /**
     * @brief Copy-constructs a HashTable, duplicating all elements and the hash function.
     * @param other The HashTable to copy.
     */
    HashTable(const HashTable &other);

    /**
     * @brief Constructs a HashTable with a custom hash function and explicit capacity.
     * @param hashFunction The function used to compute bucket indices from keys.
     * @param capacity     The number of primary buckets to allocate.
     */
    HashTable(HashFunctionType hashFunction, std::size_t capacity);

    /**
     * @brief Assigns the contents of another ADT to this hash table.
     * @param other The ADT to copy from; must be a HashTable of the same type.
     * @throws utils::type_missmatch if @p other is of different type.
     * @return A reference to this table after assignment.
     */
    ADT &assign(const ADT &other) override;

    /**
     * @brief Checks structural equality with another ADT instance.
     * @param other The ADT to compare against.
     * @return True if both tables contain identical key-data pairs, false otherwise.
     */
    bool equals(const ADT &other) const override;

    /**
     * @brief Removes all elements from every synonym table in the primary region.
     */
    void clear() override;

    /**
     * @brief Returns the total number of elements stored across all buckets.
     * @return The number of key-data pairs currently in the table.
     */
    std::size_t size() const override;

    /**
     * @brief Checks whether the hash table contains no elements.
     * @return True if the table is empty, false otherwise.
     */
    bool isEmpty() const override;

    /**
     * @brief Inserts a new key-data pair into the appropriate synonym bucket.
     * @param key   The unique key to associate with the data.
     * @param data  The data value to store.
     * @throws std::logic_error If an element with the given key already exists in its bucket.
     */
    void insert(const K &key, const T &data) override;

    /**
     * @brief Attempts to find data by key within the hashed bucket.
     * @param key  The key to search for.
     * @param data Output pointer set to the found data on success.
     * @return True if the key was found and @p data was set, false otherwise.
     */
    bool tryFind(const K &key, T *&data) const override;

    /**
     * @brief Removes and returns the element associated with the given key.
     * @param key The key of the element to remove.
     * @return The data value that was stored under @p key.
     * @throws std::out_of_range If no element with the given key exists.
     */
    T remove(const K &key) override;

private:
    using SynonymTable = UnsortedExplicitSequenceTable<K, T>;
    using SynonymTableIterator = typename SynonymTable::Iterator;
    using PrimaryRegionIterator =
        typename amt::ImplicitSequence<SynonymTable>::IteratorType;

public:
    /**
     * @brief Forward iterator that traverses all elements across all non-empty buckets.
     */
    class HashTableIterator {
    public:
        /**
         * @brief Constructs an iterator starting at the first non-empty synonym bucket.
         * @param tablesFirst Iterator to the first bucket in the primary region.
         * @param tablesLast  Iterator past the last bucket in the primary region.
         */
        HashTableIterator(
            PrimaryRegionIterator tablesFirst,
            PrimaryRegionIterator tablesLast);

        /**
         * @brief Advances the iterator to the next element (pre-increment).
         * @return A reference to this iterator after advancing.
         */
        HashTableIterator &operator++();

        /**
         * @brief Advances the iterator to the next element (post-increment).
         * @return A copy of this iterator before advancing.
         */
        HashTableIterator operator++(int);

        /**
         * @brief Compares two iterators for equality.
         * @param other The iterator to compare against.
         * @return True if both iterators point to the same element or are both at end.
         */
        bool operator==(const HashTableIterator &other) const;

        /**
         * @brief Compares two iterators for inequality.
         * @param other The iterator to compare against.
         * @return True if the iterators do not point to the same element.
         */
        bool operator!=(const HashTableIterator &other) const;

        /**
         * @brief Dereferences the iterator to access the current TableItem.
         * @return A reference to the TableItem at the current iterator position.
         */
        TableItem<K, T> &operator*();

    private:
        /** @brief Iterator to the current bucket. */
        PrimaryRegionIterator m_tablesCurrent;

        /** @brief Iterator past the last bucket. */
        PrimaryRegionIterator m_tablesLast;

        /** @brief Iterator within the current bucket. */
        SynonymTableIterator  m_synonymIterator;
    };

public:
    using Iterator = HashTableIterator;

public:
    /**
     * @brief Returns an iterator to the first element in the hash table.
     * @return A HashTableIterator pointing to the first non-empty element.
     */
    HashTableIterator begin() const;

    /**
     * @brief Returns an iterator past the last element in the hash table.
     * @return A HashTableIterator representing the end sentinel.
     */
    HashTableIterator end() const;

private:
    /** @brief Default number of primary buckets. */
    static const std::size_t CAPACITY = 100;

private:
    /** @brief Array of synonym tables. */
    amt::ImplicitSequence<SynonymTable> m_primaryRegion; 

    /** @brief Function used to hash keys. */
    HashFunctionType m_hashFunction;

    /** @brief Total element count. */
    std::size_t m_size;
};

//----------

/**
 * @brief A generic binary search tree base providing BST insert, find, remove, and rotations.
 * @tparam K        The key type; must support less-than and equality comparison.
 * @tparam T        The value type stored in the tree.
 * @tparam ItemType The node item type, typically TableItem<K,T> or a subclass thereof.
 */
template<typename K, typename T, typename ItemType>
class GeneralBinarySearchTree : public Table<K, T>, public ADS<ItemType> {
public:
    using IteratorType = typename amt::BinaryEH<ItemType>::IteratorType;

public:
    /**
     * @brief Constructs an empty binary search tree.
     */
    GeneralBinarySearchTree();

    /**
     * @brief Copy-constructs a binary search tree from another instance.
     * @param other The tree to copy.
     */
    GeneralBinarySearchTree(const GeneralBinarySearchTree &other);

    /**
     * @brief Destroys the binary search tree and releases internal state.
     */
    ~GeneralBinarySearchTree() override;

    /**
     * @brief Returns the number of elements currently stored in the tree.
     * @return The element count.
     */
    std::size_t size() const override;

    /**
     * @brief Removes all elements from the tree and resets the size to zero.
     */
    void clear() override;

    /**
     * @brief Inserts a new key-data pair into the BST and triggers rebalancing.
     * @param key   The unique key to insert.
     * @param data  The data value to associate with the key.
     * @throws std::logic_error If an element with the given key already exists.
     */
    void insert(const K &key, const T &data) override;

    /**
     * @brief Attempts to find data by key via BST traversal.
     * @param key  The key to search for.
     * @param data Output pointer set to the found data on success.
     * @return True if the key was found and @p data was set, false otherwise.
     */
    bool tryFind(const K &key, T *&data) const override;

    /**
     * @brief Removes and returns the element associated with the given key.
     * @param key The key of the element to remove.
     * @return The data value that was stored under @p key.
     * @throws std::out_of_range If no element with the given key exists.
     */
    T remove(const K &key) override;

    /**
     * @brief Returns an in-order iterator to the first element.
     * @return An iterator pointing to the first node in traversal order.
     */
    IteratorType begin() const;

    /**
     * @brief Returns an iterator past the last element.
     * @return An iterator representing the end sentinel.
     */
    IteratorType end() const;

protected:
    using BSTNodeType = typename amt::BinaryEH<ItemType>::BlockType;

    /**
     * @brief Provides access to the underlying binary explicit hierarchy.
     * @return Pointer to the internal BinaryEH memory structure.
     */
    amt::BinaryEH<ItemType> *getHierarchy() const;

    /**
     * @brief Removes a specific node from the tree, handling 0, 1, and 2-child cases.
     * @param node Pointer to the BST node to remove.
     */
    virtual void removeNode(BSTNodeType *node);

    /**
     * @brief Hook called after insert to allow subclasses to rebalance the tree.
     * @param node The newly inserted node that may trigger rebalancing.
     */
    virtual void balanceTree(BSTNodeType *node) {
        (void)node;
    }

    /**
     * @brief Searches for a node by key and sets @p node to the closest match found.
     * @param key  The key to search for.
     * @param node Output set to the matching node, or the last visited node on failure.
     * @return True if an exact match was found, false otherwise.
     */
    bool tryFindNodeWithKey(const K &key, BSTNodeType *&node) const;

    /**
     * @brief Performs a left rotation around the given node, promoting its right child.
     * @param node The node to rotate around; its right child becomes the new subtree root.
     */
    void rotateLeft(BSTNodeType *node);

    /**
     * @brief Performs a right rotation around the given node, promoting its left child.
     * @param node The node to rotate around; its left child becomes the new subtree root.
     */
    void rotateRight(BSTNodeType *node);

    /**
     * @brief Assigns the contents of another BST to this one.
     * @param other The BST to copy from.
     * @throws utils::type_missmatch if @p other is nullptr.
     */
    void assignSafe(const GeneralBinarySearchTree *other);

private:
    /** @brief The number of elements currently stored in the tree. */
    std::size_t m_size; 
};

//----------

/**
 * @brief A concrete binary search tree storing standard TableItem nodes.
 * @tparam K The key type; must support less-than and equality comparison.
 * @tparam T The value type stored in the tree.
 */
template<typename K, typename T>
class BinarySearchTree : public GeneralBinarySearchTree<K, T, TableItem<K, T>> {
public:
    /**
     * @brief Assigns the contents of another ADT to this tree.
     * @param other The ADT to copy from.
     * @return A reference to this tree after assignment.
     * @throws std::runtime_error Always; this operation is not yet implemented.
     */
    ADT &assign(const ADT &other) override;

    /**
     * @brief Checks structural equality with another ADT instance.
     * @param other The ADT to compare against.
     * @return True if both trees contain identical key-data pairs, false otherwise.
     */
    bool equals(const ADT &other) const override;
};

//----------

/**
 * @brief A TableItem extended with a random heap priority for Treap ordering.
 * @tparam K The key type used to identify elements.
 * @tparam T The value type stored in the node.
 */
template<typename K, typename T>
struct TreapItem : public TableItem<K, T> {
    int priority; /**< Random priority that maintains the min-heap property. */
};

/**
 * @brief A randomised BST (treap) that maintains heap order via random priorities.
 * @tparam K The key type; must support less-than and equality comparison.
 * @tparam T The value type stored in the tree.
 */
template<typename K, typename T>
class Treap : public GeneralBinarySearchTree<K, T, TreapItem<K, T>> {
public:
    /**
     * @brief Constructs an empty Treap seeded with the current random state.
     */
    Treap();

    /**
     * @brief Assigns the contents of another ADT to this treap.
     * @param other The ADT to copy from.
     * @return A reference to this treap after assignment.
     * @throws std::runtime_error Always; this operation is not yet implemented.
     */
    ADT &assign(const ADT &other) override;

    /**
     * @brief Checks structural equality with another ADT instance.
     * @param other The ADT to compare against.
     * @return True if both treaps contain identical key-data pairs, false otherwise.
     */
    bool equals(const ADT &other) const override;

protected:
    using BSTNodeType =
        typename GeneralBinarySearchTree<K, T, TreapItem<K, T>>::BSTNodeType;

    /**
     * @brief Removes a node by raising its priority to max and rotating it down to a leaf.
     * @param node The node to remove from the treap.
     */
    void removeNode(BSTNodeType *node) override;

    /**
     * @brief Assigns a random priority to the inserted node and rotates it into heap order.
     * @param node The newly inserted node to be balanced.
     */
    void balanceTree(BSTNodeType *node) override;

private:
    std::default_random_engine m_rng; /**< Random engine used to generate node priorities. */
};

//----------

template<typename K, typename T>
template<class SelfType>
bool Table<K, T>::equalsUnsafe(SelfType &self, const ADT &other) const {
    if (&self == &other) {
        return true;
    }

    const SelfType *o = dynamic_cast<const SelfType *>(&other);
    if (o == nullptr) {
        return false;
    }

    if (self.size() != o->size()) {
        return false;
    }

    for (const TableItem<K, T> &item : self) {
        T *otherData = nullptr;
        if (!o->tryFind(item.key, otherData) || *otherData != item.data) {
            return false;
        }
    }

    return true;
}

//----------

template<typename K, typename T>
T &Table<K, T>::find(const K &key) {
    T *data = nullptr;
    if (this->tryFind(key, data)) {
        return *data;
    }
    throw std::out_of_range("No such key.");
}

template<typename K, typename T>
bool Table<K, T>::contains(const K &key) const {
    T *data = nullptr;
    return this->tryFind(key, data);
}

//----------

template<typename K, typename T, typename SequenceType>
SequenceTable<K, T, SequenceType>::SequenceTable() :
    ADS<TableItem<K, T>>(new SequenceType()) {
}

template<typename K, typename T, typename SequenceType>
SequenceTable<K, T, SequenceType>::SequenceTable(const SequenceTable &other) :
    ADS<TableItem<K, T>>(new SequenceType(*other.getSequence())) {
}

template<typename K, typename T, typename SequenceType>
bool SequenceTable<K, T, SequenceType>::tryFind(const K &key, T *&data) const {
    BlockType *block = this->findBlockWithKey(key);
    if (block == nullptr) {
        return false;
    }
    TableItem<K, T> &item = block->data;
    T *dataPtr = &item.data;
    data = dataPtr;
    return true;
}

template<typename K, typename T, typename SequenceType>
typename SequenceTable<K, T, SequenceType>::Iterator
SequenceTable< K, T, SequenceType>::begin() const {
    return this->getSequence()->begin();
}

template<typename K, typename T, typename SequenceType>
typename SequenceTable<K, T, SequenceType>::Iterator
SequenceTable< K, T, SequenceType>::end() const {
    return this->getSequence()->end();
}

template<typename K, typename T, typename SequenceType>
SequenceType *SequenceTable<K, T, SequenceType>::getSequence() const {
    return dynamic_cast<SequenceType*>(this->m_memoryStructure);
}

template<typename K, typename T, typename SequenceType>
void SequenceTable< K, T, SequenceType>::assignSafe(const SequenceTable *other) {
    if (other == nullptr) {
        throw utils::type_missmatch();
    }
    this->getSequence()->assign(*other->getSequence());
}

template<typename K, typename T, typename SequenceType>
SequenceTable<K, T, SequenceType> &SequenceTable< K, T, SequenceType>::operator=(
    const SequenceTable &other
) {
    this->assignSafe(&other);
    return *this;
}

//----------

template<typename K, typename T, typename SequenceType>
typename UnsortedSequenceTable<K, T, SequenceType>::BlockType*
UnsortedSequenceTable<K, T, SequenceType>::findBlockWithKey(
    const K &key
) const {
    SequenceType *seq = this->getSequence();
    return seq->findBlockWithProperty([&key](BlockType *block){
        TableItem<K, T> &item = block->data;
        return item.key == key;
    });
}

//----------

template<typename K, typename T>
void UnsortedImplicitSequenceTable<K, T>::insert(const K &key, const T &data) {
    if (this->contains(key)) {
        throw std::logic_error("Duplicate key.");
    }
    BlockType &block = this->getSequence()->emplaceLast();
    TableItem<K, T> &item = block.data;
    item.key = key;
    item.data = data;
}

template<typename K, typename T>
T UnsortedImplicitSequenceTable<K, T>::remove(const K &key) {
    BlockType *block = this->findBlockWithKey(key);
    if (block == nullptr) {
        throw std::out_of_range("No such key.");
    }
    TableItem<K, T> &item = block->data;
    T tmp = item.data;
    BlockType *lastBlock = this->getSequence()->accessLast();
    std::swap(block->data, lastBlock->data);
    this->getSequence()->removeLast();
    return tmp;
}

template<typename K, typename T>
ADT &UnsortedImplicitSequenceTable<K, T>::assign(const ADT &other) {
    this->assignSafe(dynamic_cast<const UnsortedImplicitSequenceTable*>(&other));
    return *this;
}

template<typename K, typename T>
bool UnsortedImplicitSequenceTable<K, T>::equals(const ADT &other) const {
    return this->equalsUnsafe(*this, other);
}

//----------

template<typename K, typename T>
void UnsortedExplicitSequenceTable<K, T>::insert(const K &key, const T &data) {
    if (this->contains(key)) {
        throw std::logic_error("Duplicate key.");
    }
    BlockType &block = this->getSequence()->emplaceFirst();
    TableItem<K, T> &item = block.data;
    item.key = key;
    item.data = data;
}

template<typename K, typename T>
T UnsortedExplicitSequenceTable<K, T>::remove(const K &key) {
    BlockType *block = this->findBlockWithKey(key);
    if (block == nullptr) {
        throw std::out_of_range("No such key.");
    }
    TableItem<K, T> &item = block->data;
    T tmp = item.data;
    BlockType *firstBlock = this->getSequence()->accessFirst();
    std::swap(block->data, firstBlock->data);
    this->getSequence()->removeFirst();
    return tmp;
}

template<typename K, typename T>
ADT &UnsortedExplicitSequenceTable<K, T>::assign(const ADT &other) {
    this->assignSafe(dynamic_cast<const UnsortedExplicitSequenceTable*>(&other));
    return *this;
}

template<typename K, typename T>
bool UnsortedExplicitSequenceTable<K, T>::equals(const ADT &other) const {
    return this->equalsUnsafe(*this, other);
}

template<typename K, typename T>
bool UnsortedExplicitSequenceTable<K, T>::operator==(
    const UnsortedExplicitSequenceTable &other
) const {
    return this->equalsUnsafe(*this, other);
}

template<typename K, typename T>
bool UnsortedExplicitSequenceTable<K, T>::operator!=(
    const UnsortedExplicitSequenceTable &other
) const {
    return !this->equalsUnsafe(*this, other);
}

//----------

template<typename K, typename T>
void SortedSequenceTable<K, T>::insert(const K &key, const T &data) {
    if (this->isEmpty()) {
        BlockType &block = this->getSequence()->emplaceLast();
        TableItem<K, T> &item = block.data;
        item.data = data;
        item.key = key;
        return;
    }

    BlockType *block;
    if (this->tryFindBlockWithKey(key, block)) {
        throw std::logic_error("Duplicate key.");
    }

    TableItem<K, T> &item = key > block->data.key
        ? this->getSequence()->emplaceAfter(*block).data
        : this->getSequence()->emplaceBefore(*block).data;
    item.data = data;
    item.key = key;
}

template<typename K, typename T>
T SortedSequenceTable<K, T>::remove(const K &key) {
    BlockType *block;
    if (!this->tryFindBlockWithKey(key, block)) {
        throw std::out_of_range("No such key.");
    }

    T tmp = block->data.data;
    size_t index = this->getSequence()->calculateIndex(*block);
    this->getSequence()->remove(index);
    return tmp;
}

template<typename K, typename T>
typename SortedSequenceTable<K, T>::BlockType*
SortedSequenceTable<K, T>::findBlockWithKey(const K &key) const {
    BlockType *blockWithKey = nullptr;
    return this->tryFindBlockWithKey(key, blockWithKey) ? blockWithKey
                                                        : nullptr;
}

template<typename K, typename T>
ADT &SortedSequenceTable<K, T>::assign(const ADT &other) {
    this->assignSafe(dynamic_cast<const SortedSequenceTable*>(&other));
    return *this;
}

template<typename K, typename T>
bool SortedSequenceTable<K, T>::equals(const ADT &other) const {
    return this->equalsUnsafe(*this, other);
}

template<typename K, typename T>
bool SortedSequenceTable<K, T>::tryFindBlockWithKey(
    const K &key,
    BlockType *&lastBlock
) const {
    size_t first = 0;
    size_t last = this->size();
    while (first < last) {
        size_t mid = first + (last - first) / 2;
        lastBlock = this->getSequence()->access(mid);
        if (lastBlock->data.key < key) {
            first = mid + 1;
        } else if (lastBlock->data.key > key) {
            last = mid;
        } else {
            return true;
        }
    }
    return false;
}

//----------

template<typename K, typename T>
HashTable<K, T>::HashTable() :
    HashTable([](const K &key) { return std::hash<K>()(key); }, CAPACITY) {
}

template<typename K, typename T>
HashTable<K, T>::HashTable(const HashTable &other) :
    m_primaryRegion(other.m_primaryRegion),
    m_hashFunction(other.m_hashFunction),
    m_size(other.m_size)
{
}

template<typename K, typename T>
HashTable<K, T>::HashTable(
    HashFunctionType hashFunction,
    std::size_t capacity
) :
    m_primaryRegion(capacity, true),
    m_hashFunction(hashFunction),
    m_size(0) {
}

template<typename K, typename T>
ADT &HashTable<K, T>::assign(const ADT &other) {
    const HashTable *o = dynamic_cast<const HashTable*>(&other);
    if (o == nullptr) {
        throw utils::type_missmatch();
    }

    m_primaryRegion.assign(o->m_primaryRegion);
    m_size = o->m_size;

    return *this;
}

template<typename K, typename T>
bool HashTable<K, T>::equals(const ADT &other) const {
    return this->equalsUnsafe(*this, other);
}

template<typename K, typename T>
void HashTable<K, T>::clear() {
    m_size = 0;
    for (SynonymTable &synonyms : m_primaryRegion) {
        synonyms.clear();
    }
}

template<typename K, typename T>
std::size_t HashTable<K, T>::size() const {
    return m_size;
}

template<typename K, typename T>
bool HashTable<K, T>::isEmpty() const {
    return this->size() == 0;
}

template<typename K, typename T>
void HashTable<K, T>::insert(const K &key, const T &data) {
    std::size_t index = m_hashFunction(key) % m_primaryRegion.size();
    m_primaryRegion.access(index)->data.insert(key, data);
    m_size++;
}

template<typename K, typename T>
bool HashTable<K, T>::tryFind(const K &key, T *&data) const {
    std::size_t index = m_hashFunction(key) % m_primaryRegion.size();
    return m_primaryRegion.access(index)->data.tryFind(key, data);
}

template<typename K, typename T>
T HashTable<K, T>::remove(const K &key) {
    std::size_t index = m_hashFunction(key) % m_primaryRegion.size();
    T tmp = m_primaryRegion.access(index)->data.remove(key);
    m_size--;
    return tmp;
}

template<typename K, typename T>
HashTable<K, T>::HashTableIterator::HashTableIterator(
    PrimaryRegionIterator tablesFirst,
    PrimaryRegionIterator tablesLast
) :
    m_tablesCurrent(tablesFirst),
    m_tablesLast(tablesLast),
    m_synonymIterator(nullptr)
{
    while (m_tablesCurrent != m_tablesLast && (*m_tablesCurrent).isEmpty()) {
        ++m_tablesCurrent;
    }
    if (m_tablesCurrent != m_tablesLast) {
        m_synonymIterator = (*m_tablesCurrent).begin();
    }
}

template<typename K, typename T>
typename HashTable<K, T>::HashTableIterator&
HashTable<K, T>::HashTableIterator::operator++ () {
    ++m_synonymIterator;
    if (m_synonymIterator == (*m_tablesCurrent).end()) {
        ++m_tablesCurrent;
        while (m_tablesCurrent != m_tablesLast && (*m_tablesCurrent).isEmpty()) {
            ++m_tablesCurrent;
        }
        if (m_tablesCurrent != m_tablesLast) {
            m_synonymIterator = (*m_tablesCurrent).begin();
        }
    }
    return *this;
}

template<typename K, typename T>
typename HashTable<K, T>::HashTableIterator HashTable<K, T>::HashTableIterator::
    operator++(int) {
    HashTableIterator tmp(*this);
    ++(*this);
    return tmp;
}

template<typename K, typename T>
bool HashTable<K, T>::HashTableIterator::operator== (
    const HashTableIterator &other
) const {
    return m_tablesCurrent == other.m_tablesCurrent &&
           (m_tablesCurrent == m_tablesLast || m_synonymIterator == other.m_synonymIterator);
}

template<typename K, typename T>
bool HashTable<K, T>::HashTableIterator::operator!= (
    const HashTableIterator &other
) const {
    return !(*this == other);
}

template<typename K, typename T>
TableItem<K, T> &HashTable<K, T>::HashTableIterator::operator* () {
    return *m_synonymIterator;
}

template<typename K, typename T>
typename HashTable<K, T>::HashTableIterator HashTable<K, T>::begin() const {
    return HashTableIterator(m_primaryRegion.begin(), m_primaryRegion.end());
}

template<typename K, typename T>
typename HashTable<K, T>::HashTableIterator HashTable<K, T>::end() const {
    return HashTableIterator(m_primaryRegion.end(), m_primaryRegion.end());
}

//----------

template<typename K, typename T, typename ItemType>
GeneralBinarySearchTree<K, T, ItemType>::GeneralBinarySearchTree() :
    ADS<ItemType>(new amt::BinaryEH<ItemType>()),
    m_size(0) {
}

template<typename K, typename T, typename ItemType>
GeneralBinarySearchTree<K, T, ItemType>::GeneralBinarySearchTree(
    const GeneralBinarySearchTree &other
) :
    ADS<ItemType>(new amt::BinaryEH<ItemType>(), other),
    m_size(other.m_size) {
}

template<typename K, typename T, typename ItemType>
GeneralBinarySearchTree<K, T, ItemType>::~GeneralBinarySearchTree() {
    m_size = 0;
}

template<typename K, typename T, typename ItemType>
std::size_t GeneralBinarySearchTree<K, T, ItemType>::size() const {
    return m_size;
}

template<typename K, typename T, typename ItemType>
void GeneralBinarySearchTree<K, T, ItemType>::clear() {
    ADS<ItemType>::clear();
    m_size = 0;
}

template<typename K, typename T, typename ItemType>
void GeneralBinarySearchTree<K, T, ItemType>::insert(const K &key, const T &data) {
    BSTNodeType *node = nullptr;
    if (this->isEmpty()) {
        node = &this->getHierarchy()->emplaceRoot();
    } else {
        if (this->tryFindNodeWithKey(key, node)) {
            throw std::logic_error("Duplicate key.");
        }
        if (key < node->data.key) {
            node = &this->getHierarchy()->emplaceLeftSon(*node);
        } else {
            node = &this->getHierarchy()->emplaceRightSon(*node);
        }
    }
    node->data.key = key;
    node->data.data = data;
    this->m_size++;
    this->balanceTree(node);
}

template<typename K, typename T, typename ItemType>
bool GeneralBinarySearchTree<K, T, ItemType>::tryFind(
    const K &key,
    T *&data
) const {
    BSTNodeType *node = nullptr;
    if (this->tryFindNodeWithKey(key, node)) {
        data = &node->data.data;
        return true;
    }
    return false;
}

template<typename K, typename T, typename ItemType>
T GeneralBinarySearchTree<K, T, ItemType>::remove(const K &key) {
    BSTNodeType *node = nullptr;
    if (!this->tryFindNodeWithKey(key, node)) {
        throw std::out_of_range("No such key.");
    }
    T tmp = node->data.data;
    this->removeNode(node);
    this->m_size--;
    return tmp;
}

template<typename K, typename T, typename ItemType>
typename GeneralBinarySearchTree<K, T, ItemType>::IteratorType
    GeneralBinarySearchTree<K, T, ItemType>::begin() const {
    return this->getHierarchy()->begin();
}

template<typename K, typename T, typename ItemType>
typename GeneralBinarySearchTree<K, T, ItemType>::IteratorType
GeneralBinarySearchTree<K, T, ItemType>::end() const {
    return this->getHierarchy()->end();
}

template<typename K, typename T, typename ItemType>
amt::BinaryEH<ItemType>*
GeneralBinarySearchTree<K, T, ItemType>::getHierarchy() const {
    return dynamic_cast<amt::BinaryEH<ItemType> *>(this->m_memoryStructure);
}

template<typename K, typename T, typename ItemType>
void GeneralBinarySearchTree<K, T, ItemType>::removeNode(BSTNodeType *node) {
    BSTNodeType *parent = this->getHierarchy()->accessParent(*node);
    BSTNodeType *left = this->getHierarchy()->accessLeftSon(*node);
    BSTNodeType *right = this->getHierarchy()->accessRightSon(*node);

    if (left == nullptr && right == nullptr) {
        if (parent == nullptr) {
            this->getHierarchy()->clear();
        } else {
            if (this->getHierarchy()->isLeftSon(*node)) {
                this->getHierarchy()->removeLeftSon(*parent);
            } else {
                this->getHierarchy()->removeRightSon(*parent);
            }
        }
    } else if (left == nullptr || right == nullptr) {
        BSTNodeType *son = (left != nullptr) ? left : right;

        if (left != nullptr) {
            this->getHierarchy()->changeLeftSon(*node, nullptr);
        } else {
            this->getHierarchy()->changeRightSon(*node, nullptr);
        }

        if (parent == nullptr) {
            this->getHierarchy()->clear();
            this->getHierarchy()->changeRoot(son);
        } else {
            if (this->getHierarchy()->isLeftSon(*node)) {
                this->getHierarchy()->removeLeftSon(*parent);
                this->getHierarchy()->changeLeftSon(*parent, son);
            } else {
                this->getHierarchy()->removeRightSon(*parent);
                this->getHierarchy()->changeRightSon(*parent, son);
            }
        }
    } else {
        BSTNodeType *predecessor = left;
        while (this->getHierarchy()->accessRightSon(*predecessor) != nullptr) {
            predecessor = this->getHierarchy()->accessRightSon(*predecessor);
        }

        std::swap(node->data, predecessor->data);
        this->removeNode(predecessor);
    }
}

template<typename K, typename T, typename ItemType>
bool GeneralBinarySearchTree<K, T, ItemType>::tryFindNodeWithKey(
    const K &key,
    BSTNodeType *&node
) const {
    BSTNodeType *current = this->getHierarchy()->accessRoot();
    node = nullptr;
    while (current != nullptr) {
        node = current;
        if (key < current->data.key) {
            current = this->getHierarchy()->accessLeftSon(*current);
        } else if (key > current->data.key) {
            current = this->getHierarchy()->accessRightSon(*current);
        } else {
            return true;
        }
    }
    return false;
}

template<typename K, typename T, typename ItemType>
void GeneralBinarySearchTree<K, T, ItemType>::rotateLeft(BSTNodeType *node) {
    BSTNodeType *rightSon = this->getHierarchy()->accessRightSon(*node);
    BSTNodeType *parent = this->getHierarchy()->accessParent(*node);
    BSTNodeType *rightLeftSon = this->getHierarchy()->accessLeftSon(*rightSon);

    this->getHierarchy()->changeRightSon(*node, nullptr);
    this->getHierarchy()->changeLeftSon(*rightSon, nullptr);

    if (parent == nullptr) {
        this->getHierarchy()->changeRoot(rightSon);
    } else {
        if (this->getHierarchy()->isLeftSon(*node)) {
            this->getHierarchy()->changeLeftSon(*parent, rightSon);
        } else {
            this->getHierarchy()->changeRightSon(*parent, rightSon);
        }
    }

    this->getHierarchy()->changeRightSon(*node, rightLeftSon);
    this->getHierarchy()->changeLeftSon(*rightSon, node);
}

template<typename K, typename T, typename ItemType>
void GeneralBinarySearchTree<K, T, ItemType>::rotateRight(BSTNodeType *node) {
    BSTNodeType *leftSon = this->getHierarchy()->accessLeftSon(*node);
    BSTNodeType *parent = this->getHierarchy()->accessParent(*node);
    BSTNodeType *leftRightSon = this->getHierarchy()->accessRightSon(*leftSon);

    this->getHierarchy()->changeLeftSon(*node, nullptr);
    this->getHierarchy()->changeRightSon(*leftSon, nullptr);

    if (parent == nullptr) {
        this->getHierarchy()->changeRoot(leftSon);
    } else {
        if (this->getHierarchy()->isLeftSon(*node)) {
            this->getHierarchy()->changeLeftSon(*parent, leftSon);
        } else {
            this->getHierarchy()->changeRightSon(*parent, leftSon);
        }
    }

    this->getHierarchy()->changeLeftSon(*node, leftRightSon);
    this->getHierarchy()->changeRightSon(*leftSon, node);
}

template<typename K, typename T, typename ItemType>
void GeneralBinarySearchTree<K, T, ItemType>::assignSafe(
    const GeneralBinarySearchTree *other
) {
    if (other == nullptr) {
        throw utils::type_missmatch();
    }
    this->getHierarchy()->assign(*other->getHierarchy());
    m_size = other->m_size;
}

//----------

template<typename K, typename T>
 ADT &BinarySearchTree<K, T>::assign(const ADT &other) {
    this->assignSafe(dynamic_cast<const BinarySearchTree*>(&other));
    return *this;
}

template<typename K, typename T>
bool BinarySearchTree<K, T>::equals(const ADT &other) const {
    return this->equalsUnsafe(*this, other);
}

//----------

template<typename K, typename T>
Treap<K, T>::Treap() : m_rng(std::random_device()()) {
}

template<typename K, typename T>
void Treap<K, T>::removeNode(BSTNodeType *node) {
    node->data.priority = std::numeric_limits<int>::max();
    while (true) {
        BSTNodeType *left = this->getHierarchy()->accessLeftSon(*node);
        BSTNodeType *right = this->getHierarchy()->accessRightSon(*node);

        if (left == nullptr && right == nullptr) {
            GeneralBinarySearchTree<K, T, TreapItem<K, T>>::removeNode(node);
            break;
        } else if (left != nullptr && right != nullptr) {
            if (left->data.priority < right->data.priority) {
                this->rotateRight(node);
            } else {
                this->rotateLeft(node);
            }
        } else if (left != nullptr) {
            this->rotateRight(node);
        } else {
            this->rotateLeft(node);
        }
    }
}

template<typename K, typename T>
void Treap<K, T>::balanceTree(BSTNodeType *node) {
    node->data.priority = m_rng();
    BSTNodeType *parent = this->getHierarchy()->accessParent(*node);
    while (parent != nullptr && node->data.priority < parent->data.priority) {
        if (this->getHierarchy()->isLeftSon(*node)) {
            this->rotateRight(parent);
        } else {
            this->rotateLeft(parent);
        }
        parent = this->getHierarchy()->accessParent(*node);
    }
}

template<typename K, typename T>
ADT &Treap<K, T>::assign(const ADT &other) {
    this->assignSafe(dynamic_cast<const Treap*>(&other));
    return *this;
}

template<typename K, typename T>
bool Treap<K, T>::equals(const ADT &other) const {
    return this->equalsUnsafe(*this, other);
}


} // namespace ds::adt

#endif

#ifndef AUS_DATA_STRUCTURE_HPP
#define AUS_DATA_STRUCTURE_HPP

/**
 * @file data_structure.hpp
 * @brief Date-based multi-way hierarchy of accident records.
 *
 * Builds a four-level hierarchy on top of libds `MultiWayExplicitHierarchy`.
 * Depth 0 is the root, depth 1 is the year, depth 2 is the month, depth 3
 * is the day (the leaf that holds the records). Each leaf keeps an
 * `ImplicitList<const Accident*>` pointing into the Level-1 DataFrame, so
 * nothing is copied. `HierarchyIterator` walks the leaves in DFS preorder
 * so `DataFrame::execute` can run over any sub-tree.
 */

#include <libds/amt/explicit_hierarchy.hpp>
#include "../../libdf/data_handlers/data_frame.hpp"
#include "../../libds/adt/list.hpp"
#include "../../libds/adt/stack.hpp"
#include "../structs/accident.hpp"

namespace aus
{

    /**
     * @brief Payload stored in each hierarchy block.
     */
    struct Node
    {
        int                                    value{0};
        ds::adt::ImplicitList<const Accident*> accident_data;

        /** @brief Equality only on the level identifier. */
        bool operator==(const Node& other) const { return value == other.value; }
    };

    /// libds hierarchy alias on @ref Node.
    using DataStructureHierarchy = ds::amt::MultiWayExplicitHierarchy<Node>;

    /// libds block alias on @ref Node.
    using DataStructureBlock = ds::amt::MultiWayExplicitHierarchyBlock<Node>;

    class Data_Structure;

    /**
     * @brief Forward iterator that visits every record under a sub-tree.
     */
    class HierarchyIterator
    {
    public:
        using value_type        = Accident;                     // Type the iterator works with
        using reference         = const Accident&;              // What iterator returns
        using pointer           = const Accident*;              //
        using difference_type   = std::ptrdiff_t;               // How far are two things away from each other
        using iterator_category = std::forward_iterator_tag;    // Can read one place multiple times and goes forward
        using Block             = DataStructureBlock;

        /** @brief Past the end iterator. */
        HierarchyIterator() = default;

        /**
         * @brief Iterator at the first record under @p start.
         * @param tree Owning hierarchy.
         * @param start Root block of the sub-tree to walk.
         */
        HierarchyIterator(const Data_Structure* tree, Block* start);

        /** @brief Current record as a const reference. */
        const Accident& operator*() const;

        /**
         * @brief Moves to the next record (next index or next leaf).
         * @return *this after the step.
         */
        HierarchyIterator& operator++();

        /** @brief Equal when both iterators are at the same leaf and index. */
        bool operator==(const HierarchyIterator& o) const;

        /** @brief Negation of operator==. */
        bool operator!=(const HierarchyIterator& o) const;

    private:
        /**
         * @brief Walks the DFS stack until the next non-empty leaf.
         */
        void advance();

        const Data_Structure*          m_tree  = nullptr;            // On what hierarchy I am at
        Block*                         m_block = nullptr;            // Actual block I am reading from
        size_t                         m_idx   = 0;                  // Index of accident in the list
        ds::adt::ImplicitStack<Block*> m_stack;                      // Stack of unvisited Blocks
    };

    /**
     * @brief Owns the year/month/day tree built from a Level-1 DataFrame.
     */
    class Data_Structure
    {
    public:
        using Hierarchy = DataStructureHierarchy;
        using Block     = DataStructureBlock;
        using Iterator  = std::vector<Accident>::iterator;

        /**
         * @brief Builds the tree from every record in @p df.
         * @param df Source DataFrame.
         */
        Data_Structure(DataFrame<Accident>& df);

        /** @brief Root block. */
        Block* access_root() const;

        /**
         * @brief Parent of @p current_block, or nullptr at the root.
         * @param current_block Block whose parent is wanted.
         */
        Block* go_up(const Block* current_block) const;

        /**
         * @brief Child at index @p son_index under @p current_block.
         * @param current_block Block whose child is wanted.
         * @param son_index Zero-based index, must be in range.
         */
        Block* go_down(const Block* current_block, size_t son_index) const;

        /**
         * @brief Number of children of @p current_block.
         * @param current_block Block to query. Returns 0 for nullptr.
         */
        size_t son_count(const Block* current_block) const;

        /**
         * @brief Appends an extra range of records into the tree.
         * @param from Start iterator.
         * @param to Past-the-end iterator.
         */
        void additional_data(Iterator from, Iterator to);

        /**
         * @brief Forward iterator to the first record under @p start.
         * @param start Sub-tree root.
         */
        HierarchyIterator hier_begin(Block* start) const;

        /** @brief Past-the-end iterator. */
        HierarchyIterator hier_end() const;

        /**
         * @brief Inserts every record from [begin, end] into the tree.
         * @param begin Start iterator.
         * @param end Past-the-end iterator.
         */
        void build_range(Iterator begin, Iterator end);

    private:
        /**
         * @brief Builds the whole tree from @p df.
         * @param df Source DataFrame.
         */
        void build(DataFrame<Accident>& df);

        /**
         * @brief Linear search for a child block with the given value.
         * @param parent Parent block.
         * @param value Identifier to look for in Node::value.
         * @return Matching child, or nullptr.
         */
        Block* find_son(const Block& parent, int value) const;

        /**
         * @brief Adds a new child under @p parent with @p value.
         * @param parent Block to extend.
         * @param value Value stored in the new Node.
         * @return The new child.
         */
        Block& add_son(Block& parent, int value);

        Hierarchy m_hierarchy_data;
    };

    inline HierarchyIterator::HierarchyIterator(const Data_Structure* tree, Block* start) : m_tree(tree)
    {
        if (start)
        {
            m_stack.push(start);
            advance();
        }
    }

    inline const Accident& HierarchyIterator::operator*() const { return *m_block->data.accident_data.access(m_idx); }

    inline HierarchyIterator& HierarchyIterator::operator++()
    {
        if (++m_idx >= m_block->data.accident_data.size())
        {
            m_block = nullptr;
            m_idx   = 0;
            advance();
        }
        return *this;
    }

    inline bool HierarchyIterator::operator==(const HierarchyIterator& o) const
    {
        return m_block == o.m_block && m_idx == o.m_idx;
        // Compare if they are pointing on the same address and same index
    }

    inline bool HierarchyIterator::operator!=(const HierarchyIterator& o) const { return !(*this == o); }

    inline void HierarchyIterator::advance()
    {
        while (!m_stack.isEmpty())
        {
            Block* b = m_stack.peek();
            m_stack.pop();

            size_t n = m_tree->son_count(b);
            for (size_t i = n; i > 0; --i)
            {
                m_stack.push(m_tree->go_down(b, i - 1));
            }

            if (!b->data.accident_data.isEmpty())
            {
                m_block = b;
                m_idx   = 0;
                return;
            }
        }
    }

    inline HierarchyIterator Data_Structure::hier_begin(Block* start) const { return HierarchyIterator(this, start); }

    inline HierarchyIterator Data_Structure::hier_end() const { return HierarchyIterator(); }
} // namespace aus

#endif // AUS_DATA_STRUCTURE_HPP

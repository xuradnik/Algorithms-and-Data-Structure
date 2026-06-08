#ifndef COMPLEXITIES_HASH_MAP_ANALYZER_HPP
#define COMPLEXITIES_HASH_MAP_ANALYZER_HPP

#include "complexity_analyzer.hpp"

#include <libds/adt/table.hpp>

#include <random>

namespace cmx {

    /**
     * @brief Hash table type analyzed for U3 (matches Hash_Map_Data_Structure key type).
     */
    using HashMapType = ds::adt::HashTable<std::uint32_t, int>;

    /**
     * @brief Common base for hash table analyzers. Grows the table with unique random keys.
     */
    class HashMapAnalyzer : public ComplexityAnalyzer<HashMapType>
    {
    protected:
        explicit HashMapAnalyzer(const std::string& name);

        void growToSize(HashMapType& structure, std::size_t size) override;

        std::uint32_t getRandomKey() const;
        std::uint32_t getInsertedKey() const;
        int getRandomData() const;

        /**
         * @brief Marks the key returned by getRandomKey() as consumed so the
         *        next growToSize / measured insert uses a fresh unique key.
         */
        void advanceKey();

    private:
        mutable std::default_random_engine m_rngKey;
        mutable std::default_random_engine m_rngData;
        std::uint32_t                      m_nextKey;
        std::uint32_t                      m_lastInsertedKey;
        int                                m_data;
    };

    /**
     * @brief Measures the cost of inserting one new key-data pair into a table of size n.
     */
    class HashMapInsertAnalyzer : public HashMapAnalyzer
    {
    public:
        explicit HashMapInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(HashMapType& structure) override;
    };

    /**
     * @brief Measures the cost of finding an existing key in a table of size n.
     */
    class HashMapFindAnalyzer : public HashMapAnalyzer
    {
    public:
        explicit HashMapFindAnalyzer(const std::string& name);

    protected:
        void executeOperation(HashMapType& structure) override;
    };

    /**
     * @brief Composite that runs both hash table benchmarks.
     */
    class HashMapsAnalyzer : public CompositeAnalyzer
    {
    public:
        HashMapsAnalyzer();
    };

} // namespace cmx

#endif // COMPLEXITIES_HASH_MAP_ANALYZER_HPP

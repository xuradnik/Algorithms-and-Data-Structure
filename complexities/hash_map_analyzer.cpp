#include "hash_map_analyzer.hpp"

#include <memory>

namespace cmx {

    HashMapAnalyzer::HashMapAnalyzer(const std::string& name) :
        ComplexityAnalyzer(name),
        m_rngKey(144),
        m_rngData(144),
        m_nextKey(1),
        m_lastInsertedKey(0),
        m_data(0)
    {

        ComplexityAnalyzer::registerBeforeOperation([this](HashMapType&) {
            m_data = static_cast<int>(m_rngData());


            std::uniform_int_distribution<std::uint32_t> dist(1, m_nextKey - 1);
            m_lastInsertedKey = dist(m_rngKey);
        });
    }

    void HashMapAnalyzer::growToSize(HashMapType& structure, std::size_t size)
    {
        while (structure.size() < size) {
            structure.insert(m_nextKey, static_cast<int>(m_nextKey));
            ++m_nextKey;
        }
    }

    std::uint32_t HashMapAnalyzer::getRandomKey() const
    {

        return m_nextKey;
    }

    std::uint32_t HashMapAnalyzer::getInsertedKey() const
    {
        return m_lastInsertedKey;
    }

    int HashMapAnalyzer::getRandomData() const
    {
        return m_data;
    }

    void HashMapAnalyzer::advanceKey()
    {
        ++m_nextKey;
    }


    HashMapInsertAnalyzer::HashMapInsertAnalyzer(const std::string& name) :
        HashMapAnalyzer(name)
    {
    }

    void HashMapInsertAnalyzer::executeOperation(HashMapType& structure)
    {
        structure.insert(this->getRandomKey(), this->getRandomData());
        this->advanceKey();
    }

    //----------

    HashMapFindAnalyzer::HashMapFindAnalyzer(const std::string& name) :
        HashMapAnalyzer(name)
    {
    }

    void HashMapFindAnalyzer::executeOperation(HashMapType& structure)
    {
        int* found = nullptr;
        structure.tryFind(this->getInsertedKey(), found);
    }

    //----------

    HashMapsAnalyzer::HashMapsAnalyzer() : CompositeAnalyzer("HashTables")
    {
        this->addAnalyzer(std::make_unique<HashMapInsertAnalyzer>("hashtable-insert"));
        this->addAnalyzer(std::make_unique<HashMapFindAnalyzer>("hashtable-find"));
    }

} // namespace cmx

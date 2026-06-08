#ifndef COMPLEXITIES_LIST_ANALYZER_HPP
#define COMPLEXITIES_LIST_ANALYZER_HPP

#include "complexity_analyzer.hpp"

#include <random>
#include <vector>

namespace cmx {


/**
 * @brief Common base for list analyzers.
 */
class VectorAnalyzer : public ComplexityAnalyzer<std::vector<int>> {
protected:
    explicit VectorAnalyzer(const std::string &name);

protected:
    void growToSize(std::vector<int> &structure, std::size_t size) override;
    std::size_t getRandomIndex() const;
    int getRandomData() const;

private:
    std::default_random_engine m_rngData;
    std::default_random_engine m_rngIndex;
    std::size_t m_index;
    int m_data;
};

/**
 * @brief Analyzes complexity of an insertion at the beginning.
 */
class VectorInsertAnalyzer : public VectorAnalyzer {
public:
    explicit VectorInsertAnalyzer(const std::string &name);

protected:
    void executeOperation(std::vector<int> &structure) override;
};

/**
 * @brief Analyzes complexity of an erasure at the beginning.
 */
class VectorRemoveAnalyzer : public VectorAnalyzer {
public:
    explicit VectorRemoveAnalyzer(const std::string &name);

protected:
    void executeOperation(std::vector<int> &structure) override;
};

/**
 * @brief Container for all list analyzers.
 */
class ListsAnalyzer : public CompositeAnalyzer {
public:
    ListsAnalyzer();
};


} // namespace cmx

#endif

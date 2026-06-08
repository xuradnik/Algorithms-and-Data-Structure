#include "list_analyzer.hpp"

namespace cmx {


VectorAnalyzer::VectorAnalyzer(const std::string &name) :
    ComplexityAnalyzer(name),
    m_rngData(144),
    m_rngIndex(144),
    m_index(0),
    m_data(0) {
    ComplexityAnalyzer::registerBeforeOperation([this](std::vector<int> &list) {
        std::uniform_int_distribution<std::size_t> indexDist(
            std::size_t {0},
            list.size() - 1
        );
        m_index = indexDist(m_rngIndex);
        m_data = m_rngData();
    });
}

void VectorAnalyzer::growToSize(std::vector<int> &structure, std::size_t size) {
    const std::size_t toInsert = size - structure.size();
    for (std::size_t i = 0; i < toInsert; ++i) {
        structure.push_back(static_cast<int>(m_rngData()));
    }
}

std::size_t VectorAnalyzer::getRandomIndex() const {
    return m_index;
}

int VectorAnalyzer::getRandomData() const {
    return m_data;
}

//----------

VectorInsertAnalyzer::VectorInsertAnalyzer(const std::string &name) :
    VectorAnalyzer(name) {
}

void VectorInsertAnalyzer::executeOperation(std::vector<int> &structure) {
    auto data = this->getRandomData();
    structure.insert(structure.begin(), data);
}

//----------

VectorRemoveAnalyzer::VectorRemoveAnalyzer(const std::string &name) :
    VectorAnalyzer(name) {
}

void VectorRemoveAnalyzer::executeOperation(std::vector<int> &structure) {
    structure.erase(structure.begin());
}

//----------

ListsAnalyzer::ListsAnalyzer() : CompositeAnalyzer("Lists") {
    this->addAnalyzer(std::make_unique<VectorInsertAnalyzer>("vector-insert"));
    this->addAnalyzer(std::make_unique<VectorRemoveAnalyzer>("vector-remove"));
}


} // namespace cmx

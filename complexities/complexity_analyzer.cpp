#include "complexity_analyzer.hpp"

#include <filesystem>

namespace cmx {


Analyzer::Analyzer(std::string name) : m_name(std::move(name)) {
}

const std::string &Analyzer::getName() const {
    return m_name;
}

//----------

CompositeAnalyzer::CompositeAnalyzer(const std::string &name) : Analyzer(name) {
}

void CompositeAnalyzer::analyze() {
    for (const auto &analyzer : m_analyzers) {
        analyzer->analyze();
    }
}

void CompositeAnalyzer::setOutputDirectory(const std::filesystem::path &path) {
    for (const auto &analyzer : m_analyzers) {
        analyzer->setOutputDirectory(path);
    }
}

void CompositeAnalyzer::setReplicationCount(std::size_t count) {
    for (const auto &analyzer : m_analyzers) {
        analyzer->setReplicationCount(count);
    }
}

void CompositeAnalyzer::setStepSize(std::size_t size) {
    for (const auto &analyzer : m_analyzers) {
        analyzer->setStepSize(size);
    }
}

void CompositeAnalyzer::setStepCount(std::size_t count) {
    for (const auto &analyzer : m_analyzers) {
        analyzer->setStepCount(count);
    }
}

void CompositeAnalyzer::addAnalyzer(std::unique_ptr<Analyzer> analyzer) {
    m_analyzers.push_back(std::move(analyzer));
}

const std::vector<std::unique_ptr<Analyzer>> &CompositeAnalyzer::
    getAnalyzers() {
    return m_analyzers;
}

//----------

LeafAnalyzer::LeafAnalyzer(const std::string &name) :
    Analyzer(name),
    m_outputDir("."),
    m_replicationCount(DEFAULT_REPLICATION_COUNT),
    m_stepSize(DEFAULT_STEP_SIZE),
    m_stepCount(DEFAULT_STEP_COUNT),
    m_wasSuccessful(false) {
}

void LeafAnalyzer::setOutputDirectory(const std::filesystem::path &path) {
    m_outputDir = path;
}

void LeafAnalyzer::setReplicationCount(std::size_t count) {
    m_replicationCount = count;
}

void LeafAnalyzer::setStepSize(std::size_t size) {
    m_stepSize = size;
}

void LeafAnalyzer::setStepCount(std::size_t count) {
    m_stepCount = count;
}

std::filesystem::path LeafAnalyzer::getOutputPath() const {
    return absolute(m_outputDir / this->getName() += ".csv");
}

bool LeafAnalyzer::wasSuccessful() const {
    return m_wasSuccessful;
}

void LeafAnalyzer::resetSuccess() {
    m_wasSuccessful = false;
}

void LeafAnalyzer::setSuccess() {
    m_wasSuccessful = true;
}

std::size_t LeafAnalyzer::getReplicationCount() const {
    return m_replicationCount;
}

std::size_t LeafAnalyzer::getStepSize() const {
    return m_stepSize;
}

std::size_t LeafAnalyzer::getStepCount() const {
    return m_stepCount;
}


} // namespace cmx

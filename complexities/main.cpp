#include "list_analyzer.hpp"
#include "hash_map_analyzer.hpp"

int main() {
    cmx::ListsAnalyzer listsAnalyzer;
    listsAnalyzer.setOutputDirectory(".");
    listsAnalyzer.analyze();

    cmx::HashMapsAnalyzer hashMapsAnalyzer;
    hashMapsAnalyzer.setOutputDirectory(".");
    hashMapsAnalyzer.analyze();
}

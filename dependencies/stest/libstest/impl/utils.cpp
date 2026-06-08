#include <libstest/impl/utils.hpp>

#include <algorithm>


namespace stest {


std::vector<std::string> splitOn(std::string_view str, char delim) {
    const auto is_delim = [delim](const char c) {
        return c == delim;
    };
    std::vector<std::string> words;
    std::string_view::iterator strIt = str.begin();
    const std::string_view::iterator endIt = str.end();
    while (strIt != endIt) {
        const auto wordBegin = std::find_if_not(strIt, endIt, is_delim);
        const auto wordEnd   = std::find_if(wordBegin, endIt, is_delim);
        if (wordBegin != wordEnd) {
            words.emplace_back(wordBegin, wordEnd);
        }
        strIt = wordEnd;
    }
    return words;
}


} // namespace stest

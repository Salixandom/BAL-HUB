#include <fstream>
#include <filesystem>
#include <regex>
#include "file_utils.hpp"

namespace fs = std::filesystem;

vector<string> loadIgnorePatterns() {
    vector<string> patterns;

    ifstream in(".balignore");
    if(!in) return patterns;

    string line;
    while(getline(in, line)) {
        if(!line.empty()) {
            patterns.push_back(line);
        }
    }

    return patterns;
}


bool matchPattern(const string &pattern, const string &path) {
    string regexPattern = regex_replace(pattern, regex(R"(\.)"), R"(\.)");
    regexPattern = regex_replace(regexPattern, regex(R"(\*)"), R"(.*)");

    string normalized = fs::path(path).generic_string();
    return regex_match(normalized, regex(regexPattern));
}

bool isIgnored(const string &path, vector<string> &patterns) {
    for(const auto &pattern: patterns) {
        if(matchPattern(pattern, path)) return true;
    }
    return false;
}
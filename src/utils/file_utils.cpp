#include <fstream>
#include <filesystem>
#include <regex>
#include <chrono>
#include <sstream>
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


bool isHiddenFile(const string &filename) {
    return !filename.empty() && filename[0] == '.';
}

bool isInsideBal(const string &path) {
    return path.find(".bal/") == 0 || path.find("/.bal") != string::npos;
}


vector<string> getAllFiles(const string &rootDir, vector<string> &ignorePatterns) {
    vector<string> result;

    for(const auto &entry : fs::recursive_directory_iterator(rootDir)) {
        if(!entry.is_regular_file()) continue;  // skip non-regular files (e.g. directories, symlinks, etc.)

        string path = entry.path().string();

        if(isInsideBal(path)) continue;  // skip files inside .bal folder
        if(isHiddenFile(entry.path().filename().string())) continue;  // skip hidden files
        if(isIgnored(path, ignorePatterns)) continue;  // skip ignored files

        result.push_back(path);
    }
}

string globToRegex(const string &pattern) {
    string regex = "^";
    for(char c : pattern) {
        switch (c) {
        case '*':
            regex += ".*";
            break;
        case '?':
            regex += ".";
            break;
        case '.':
            regex += "\\.";
            break;
        default:
            regex += c;
        }
    }
    regex += "$";
    return regex;
}


vector<string> matchGlobPattern(const string &pattern, vector<string> &ignorePatterns) {
    vector<string> matched;
    regex matcher(globToRegex(pattern));

    for(const auto &entry: fs::recursive_directory_iterator(".")) {
        if(!entry.is_regular_file()) continue;

        string path = entry.path().string();
        if(isInsideBal(path) || isHiddenFile(entry.path().filename().string()) || isIgnored(path, ignorePatterns)) continue;

        string relative = fs::relative(path, ".").string();
        if(regex_match(relative, matcher)) {
            matched.push_back(relative);
        }
    }

    return matched;
}

string getTimeSuffix() {
    auto now = chrono::system_clock::now();
    auto in_time = chrono::system_clock::to_time_t(now);
    std::tm buf;

#ifdef _WIN32
    localtime_s(&buf, &in_time);
#else
    localtime_r(&in_time, &buf);
#endif

    stringstream ss;
    ss << "_" << std::put_time(&buf, "%Y%m%d%H%M%S");
    return ss.str();
}

bool moveToTrash(const string &filepath) {
    try {
        fs::create_directories(".bal/trash");

        string filename = fs::path(filepath).filename().string();
        string extension = fs::path(filename).extension().string();
        string base = fs::path(filename).stem().string();

        string timestamped = base + getTimeSuffix() + extension;
        string trashPath = ".bal/trash/" + timestamped;

        fs::rename(filepath, trashPath);
        return true;
    } catch (...) {
        return false;
    }
}
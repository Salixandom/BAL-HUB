#include <iostream>
#include <filesystem>
#include <regex>
#include <chrono>
#include <sstream>
#include <map>

using namespace std;
namespace fs = std::filesystem;

void restoreFromTrash(const string &filename) {
    string trashDir = ".bal/trash";
    if(!fs::exists(trashDir)) {
        cerr << "No trash directory found.\n";
        return;
    }

    string baseName = fs::path(filename).stem().string();
    string extension = fs::path(filename).extension().string();

    regex pattern(baseName + "_\\d{14}" + extension);
    fs::path latestMatch;
    std::time_t latestTime = 0;

    for(const auto &entry: fs::directory_iterator(trashDir)) {
        if(!entry.is_regular_file()) continue;

        string name = entry.path().filename().string();

        if(regex_match(name, pattern)) {
            string timeStr = name.substr(baseName.length() + 1, 14);
            std::tm t = {};
            std::istringstream ss(timeStr);
            ss >> std::get_time(&t, "%Y%m%d%H%M%S");

            if(!ss.fail()) {
                std::time_t fileTime = std::mktime(&t);
                if(fileTime > latestTime) {
                    latestTime = fileTime;
                    latestMatch = entry.path();
                }
            }
        }
    }

    if(latestMatch.empty()) {
        cerr << "No trashed version of '" << filename << "' found.\n";
        return;
    }

    try {
        fs::copy_file(latestMatch, filename, fs::copy_options::overwrite_existing);
        cout << "Restored: " << filename << " from trash.\n";
    } catch (...) {
        cerr << "Error: Failed to restore '" << filename << "'.\n";
    }
}

void listTrashFiles() {
    string trashDir = ".bal/trash";
    if (!fs::exists(trashDir)) {
        cout << "Trash is empty.\n";
        return;
    }

    map<string, int> fileVersions;

    for (const auto &entry : fs::directory_iterator(trashDir)) {
        if (!entry.is_regular_file())
            continue;

        string name = entry.path().filename().string();
        size_t underscorePos = name.find_last_of('_');
        size_t dotPos = name.find_last_of('.');

        if (underscorePos == string::npos || dotPos == string::npos || dotPos < underscorePos)
            continue;

        string base = name.substr(0, underscorePos);
        string ext = name.substr(dotPos); 
        string originalName = base + ext;

        fileVersions[originalName]++;
    }

    if(fileVersions.empty()) {
        cout << "Trash is empty" << endl;
        return;
    }

    cout << "=== Files in .bal/trash/ ===\n\n";
    for (const auto &[filename, count] : fileVersions) {
        cout << filename << "  (" << count << " version" << (count > 1 ? "s" : "") << ")\n";
    }
}


void listTrashFilesDetailed() {
    string trashDir = ".bal/trash";
    if (!fs::exists(trashDir)) {
        cout << "Trash is empty.\n";
        return;
    }

    map<string, vector<string>> fileVersions;

    for (const auto& entry : fs::directory_iterator(trashDir)) {
        if (!entry.is_regular_file()) continue;

        string name = entry.path().filename().string();

        size_t underscorePos = name.find_last_of('_');
        size_t dotPos = name.find_last_of('.');

        if (underscorePos == string::npos || dotPos == string::npos || dotPos < underscorePos)
            continue;

        string base = name.substr(0, underscorePos);
        string ext = name.substr(dotPos);
        string originalName = base + ext;

        fileVersions[originalName].push_back(name);
    }

    if (fileVersions.empty()) {
        cout << "Trash is empty.\n";
        return;
    }

    cout << "=== Files in .bal/trash/ ===\n\n";

    for (const auto& [filename, versions] : fileVersions) {
        cout << filename << "\n";
        for (const auto& version : versions) {
            cout << "  - " << version << "\n";
        }
        cout << "\n";
    }
}



void restoreAllFromTrash() {
    string trashDir = ".bal/trash";
    if (!fs::exists(trashDir)) {
        cout << "No trash directory found.\n";
        return;
    }

    map<string, pair<fs::path, std::time_t>> latestVersions;

    for (const auto &entry : fs::directory_iterator(trashDir)) {
        if (!entry.is_regular_file())
            continue;

        string name = entry.path().filename().string();

        size_t underscorePos = name.find_last_of('_');
        size_t dotPos = name.find_last_of('.');

        if (underscorePos == string::npos || dotPos == string::npos || dotPos < underscorePos)
            continue;

        string base = name.substr(0, underscorePos);
        string ext = name.substr(dotPos);
        string originalName = base + ext;

        string timeStr = name.substr(underscorePos + 1, 14);
        std::tm t = {};
        std::istringstream ss(timeStr);
        ss >> std::get_time(&t, "%Y%m%d%H%M%S");

        if (!ss.fail()) {
            std::time_t fileTime = std::mktime(&t);
            if (latestVersions.find(originalName) == latestVersions.end() || fileTime > latestVersions[originalName].second) {
                latestVersions[originalName] = {entry.path(), fileTime};
            }
        }
    }

    if (latestVersions.empty()) {
        cout << "Trash is empty.\n";
        return;
    }

    for (const auto &[originalName, info] : latestVersions) {
        const fs::path &source = info.first;
        try {
            fs::copy_file(source, originalName, fs::copy_options::overwrite_existing);
            cout << "Restored: " << originalName << endl;
        } catch (...) {
            cerr << "Error: Failed to restore '" << originalName << "'\n";
        }
    }
}

void restoreFromTrashByVersion(const string &filename, const string &timestamp) {
    string trashDir = ".bal/trash";
    if (!fs::exists(trashDir)) {
        cerr << "No trash directory found.\n";
        return;
    }

    string base = fs::path(filename).stem().string();
    string ext = fs::path(filename).extension().string();
    string expectedName = base + "_" + timestamp + ext;
    fs::path fullPath = fs::path(trashDir) / expectedName;

    if (!fs::exists(fullPath)) {
        cerr << "No version '" << expectedName << "' found in trash.\n";
        return;
    }

    try {
        fs::copy_file(fullPath, filename, fs::copy_options::overwrite_existing);
        cout << "Restored: " << filename << " from trash version: " << timestamp << endl;
    } catch (...) {
        cerr << "Error: Failed to restore '" << filename << "' from version.\n";
    }
}

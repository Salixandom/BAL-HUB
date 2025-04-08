#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include "nlohmann/json.hpp"
#include "hash_utils.hpp"
#include "file_utils.hpp"

using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

void status() {
    if(!fs::exists(".bal/index.json")) {
        cerr << "Error: Repository not initialized. Run `bal init` first." << endl;
        return;
    }

    vector<string> ignorePatterns = loadIgnorePatterns();

    json index;
    ifstream in(".bal/index.json");
    in >> index;
    in.close();

    unordered_set<string> stagedFiles;
    unordered_set<string> modifiedFiles;
    unordered_set<string> untrackedFiles;
    unordered_set<string> deletedFiles;

    for(auto &[filename, hash] : index.items()) {
        stagedFiles.insert(filename);
        if(fs::exists(filename)) {
            string currentHash = getFileHash(filename);
            if(currentHash != hash) {
                modifiedFiles.insert(filename);
            }
        } else {
            deletedFiles.insert(filename);
        }
    }

    for(const auto &entry: fs::recursive_directory_iterator(".")) {
        if(!entry.is_regular_file()) continue;

        string path = entry.path().string();

        if(path.find(".bal/") == 0 || path.find("/.bal") != string::npos) continue;

        if(entry.path().filename().string()[0] == '.') continue;

        if(isIgnored(path, ignorePatterns)) continue;

        if(stagedFiles.find(path) == stagedFiles.end()) {
            untrackedFiles.insert(path);
        }
    }

    cout << "\n=== Staged Files ===\n";
    for (const auto &file : stagedFiles) {
        if (modifiedFiles.find(file) == modifiedFiles.end())
            cout << "  - " << file << endl;
    }

    cout << "\n=== Modified Files (staged but changed) ===\n";
    for (const auto &file : modifiedFiles) {
        cout << "  - " << file << endl;
    }

    cout << "\n=== Untracked Files ===\n";
    for (const auto &file : untrackedFiles) {
        cout << "  - " << file << endl;
    }

    cout << "\n=== Deleted Files (staged but missing) ===\n";
    for (const auto &file : deletedFiles) {
        cout << "  - " << file << endl;
    }

    cout << endl;
}
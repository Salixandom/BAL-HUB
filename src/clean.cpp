#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_set>
#include "nlohmann/json.hpp"
#include "file_utils.hpp"

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

void cleanWorkingDir(bool force = false) {
    if (!fs::exists(".bal/index.json")) {
        cerr << "Error: Repository not initialized. Run `bal init` first.\n";
        return;
    }

    if (!force) {
        cout << "This will remove all untracked files. Continue? (y/n): ";
        char confirm;
        cin >> confirm;
        if (tolower(confirm) != 'y') {
            cout << "Clean aborted.\n";
            return;
        }
    }

    json index;
    ifstream in(".bal/index.json");
    in >> index;
    in.close();

    unordered_set<string> stagedFiles;
    for (auto &[file, hash] : index.items()) {
        stagedFiles.insert(file);
    }

    vector<string> ignorePatterns = loadIgnorePatterns();

    int cleaned = 0;
    for(const auto &entry: fs::recursive_directory_iterator(".")) {
        if(!entry.is_regular_file()) continue;

        string path = entry.path().string();

        if (isInsideBal(path) || isHiddenFile(entry.path().filename().string()) || isIgnored(path, ignorePatterns)) continue;

        if(stagedFiles.find(path) == stagedFiles.end()) {
            if(moveToTrash(path)) {
                cout << "Cleaned: " << path << endl;
            }
        }
    }

    if(cleaned == 0) {
        cout << "Nothing to clean. Working directory is clean" << endl;
    } else {
        cout << "\nTotal cleaned files: " << cleaned << endl;
    }
}
#include <iostream>
#include <fstream>
#include <filesystem>
#include "nlohmann/json.hpp"
#include "file_utils.hpp"

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

void removeFromBal(const string &filename, bool cachedOnly = false) {
    if (!fs::exists(".bal/index.json")) {
        cerr << "Error: Repository not initialized. Run `bal init` first." << endl;
        return;
    }

    json index;
    ifstream in(".bal/index.json");
    if (in)
        in >> index;
    in.close();

    if (!index.contains(filename)) {
        cerr << "Warning: File '" << filename << "' is not staged.\n";
        return;
    }

    index.erase(filename);

    ofstream out(".bal/index.json");
    out << index.dump(4);
    out.close();

    cout << "Unstaged: " << filename << endl;

    if(!cachedOnly && fs::exists(filename)) {
        if(moveToTrash(filename)) {
            cout << "Moved to trash: " << filename << endl;
        }
    }
}

void removeRecursive(const string &folder, bool cachedOnly = false) {
    if (!fs::exists(folder) || !fs::is_directory(folder)) {
        cerr << "Error: '" << folder << "' is not a valid directory." << endl;
        return;
    }

    if (!fs::exists(".bal/index.json")) {
        cerr << "Error: Repository not initialized. Run `bal init` first." << endl;
        return;
    }

    json index;
    ifstream in(".bal/index.json");
    if (in)
        in >> index;
    in.close();

    int removed = 0;

    for (const auto &entry : fs::recursive_directory_iterator(folder)) {
        if (!entry.is_regular_file())
            continue;

        string path = entry.path().string();

        if (index.contains(path)) {
            index.erase(path);
            cout << "Unstaged: " << path << endl;
            removed++;

            if (!cachedOnly && fs::exists(path)) {
                if(moveToTrash(path)) {
                    cout << "Moved to trash: " << path << endl;
                }
            }
        }
    }

    ofstream out(".bal/index.json");
    out << index.dump(4);
    out.close();

    if (removed == 0) {
        cout << "Note: No staged files found inside '" << folder << "'" << endl;
    }
}
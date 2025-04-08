#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include "nlohmann/json.hpp"
#include "hash_utils.hpp"
#include "file_utils.hpp"

using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

void add(const string &filename) {
    if (!fs::exists(".bal")){
        cout << "Error: Repository not initialized. Run `bal init` first." << endl;
        return;
    }

    ifstream file(".bal/index.json");
    json index;
    file >> index;    // reads the JSON data from the file
    file.close();

    vector<string> ignorePatterns = loadIgnorePatterns();
    vector<string> filesToAdd;
    
    if(filename == "."){
        for(const auto &entry: fs::recursive_directory_iterator(".")) {
            filesToAdd = getAllFiles(".", ignorePatterns); 
        }
    } else {
        if(!fs::exists(filename) || !fs::is_regular_file(filename)) {
            cerr << "Error: File '" << filename << "' does not exist or is not a regular file." << endl;
            return;
        }

        if(!isIgnored(filename, ignorePatterns) && !isHiddenFile(fs::path(filename).filename().string()) && !isInsideBal(filename)) {
            filesToAdd.push_back(filename);
        }
    }

    for(const string &file : filesToAdd) {
        string hash = getFileHash(file);
        if(!hash.empty()) {
            index[file] = hash;
            cout << "Staged: " << file << endl;
        }
    }

    ofstream out(".bal/index.json");
    out << index.dump(4);     // Convert the JSON object to a pretty-printed string, using 4 spaces for indentation.
    out.close();
}
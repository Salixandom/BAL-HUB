#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include "nlohmann/json.hpp"
#include "hash_utils.hpp"

using json = nlohmann::json;
using namespace std;

void add(const string &filename) {
    if (!filesystem::exists(".bal")){
        cout << "Error: Repository not initialized. Run `bal init` first." << endl;
        return;
    }

    ifstream file(".bal/index.json");
    json index;
    file >> index;    // reads the JSON data from the file
    file.close();

    vector<string> filesToAdd;
    
    if(filename == "."){
        for(const auto &entry: filesystem::recursive_directory_iterator(".")) {
            if(!entry.is_regular_file()) continue;

            string fullPath = entry.path().string();
            if(fullPath.find(".bal/") == 0 || fullPath.find("/.bal") != string::npos)   // check if .bal is the first/root directory and also checks if .bal is found  // npos means not found
                continue;

            if(entry.path().filename().string()[0] == '.') continue;  // skip hidden files

            filesToAdd.push_back(fullPath);
        }
    } else {
        if(!filesystem::exists(filename) || !filesystem::is_regular_file(filename)) {
            cerr << "Error: File '" << filename << "' does not exist or is not a regular file." << endl;
            return;
        }

        filesToAdd.push_back(filename);
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
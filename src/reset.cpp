#include <iostream>
#include <fstream>
#include <filesystem>
#include "nlohmann/json.hpp"

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

void resetIndex() {
    if (!fs::exists(".bal/index.json")) {
        cerr << "Error: Repository not initialized. Run `bal init` first." << endl;
        return;
    }

    ofstream out(".bal/index.json");
    out << "{}"; 
    out.close();

    cout << "Staging area cleared. All files have been unstaged." << endl;
}


void resetFile(const string &filename) {
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
        cerr << "Warning: File '" << filename << "' is not currently staged." << endl;
        return;
    }

    index.erase(filename);

    ofstream out(".bal/index.json");
    out << index.dump(4);
    out.close();

    cout << "Unstaged: " << filename << endl;
}
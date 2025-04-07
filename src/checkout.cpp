#include <filesystem>
#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

void checkout(const string &commitID) {
    string commitPath = ".bal/commits/" + commitID;

    if(!fs::exists(commitPath)) {
        cerr << "Error: Commit '" << commitID << "' does not exist." << endl;
        return;
    }

    // Confirm with user before overwriting staged files
    cout << "WARNING: This will overwrite current working directory files!" << endl;
    cout << "Proceed? (y/n): ";
    char confirm;
    cin >> confirm;
    
    if(confirm != 'y' && confirm != 'Y') {
        cout << "Checkout aborted." << endl;
        return;
    }

    int restored = 0;
    for(const auto &entry: fs::directory_iterator(commitPath)) {
        if(!entry.is_regular_file()) continue;  // skip non-regular files (e.g. directories, symlinks, etc.)

        string filename = entry.path().filename().string();
        fs::copy_file(entry.path(), filename, fs::copy_options::overwrite_existing);  // overwrite existing files
        restored++;
    }

    cout << "Restored " << restored << " file(s) from commit '" << commitID << "'." << endl;
}

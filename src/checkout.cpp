#include <filesystem>
#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

void checkout(const string &commitID) {
    string commitPath = ".bal/commits/" + commitID;

    if (!fs::exists(commitPath)) {
        cerr << "Error: Commit '" << commitID << "' does not exist." << endl;
        return;
    }

    cout << "WARNING: This will overwrite your working directory files!" << endl;
    cout << "Proceed? (y/n): ";
    char confirm;
    cin >> confirm;

    if (tolower(confirm) != 'y') {
        cout << "Checkout aborted." << endl;
        return;
    }

    int restored = 0;

    for (const auto &entry : fs::recursive_directory_iterator(commitPath)) {
        if (!entry.is_regular_file()) continue;

        fs::path relative = fs::relative(entry.path(), commitPath);
        fs::path targetPath = fs::current_path() / relative;

        fs::create_directories(targetPath.parent_path());

        if (fs::exists(targetPath)) {
            fs::create_directories(".bal/backup/" + commitID);
            fs::copy_file(targetPath, ".bal/backup/" + commitID + "/" + relative.filename().string(), fs::copy_options::overwrite_existing);
        }

        fs::copy_file(entry.path(), targetPath, fs::copy_options::overwrite_existing);
        cout << "✔ Restored: " << relative.string() << endl;
        restored++;
    }

    cout << "\nCheckout complete: " << restored << " file(s) restored from commit '" << commitID << "'." << endl;
    cout << "A backup of overwritten files (if any) is in .bal/backup/" << commitID << endl;
}

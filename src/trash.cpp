#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void cleanTrash() {
    string trashDir = ".bal/trash";

    if (!fs::exists(trashDir)) {
        cout << "Nothing to clean. Trash is already empty.\n";
        return;
    }

    int count = 0;
    for (const auto& entry : fs::directory_iterator(trashDir)) {
        try {
            fs::remove(entry.path());
            count++;
        } catch (...) {
            cerr << "Failed to delete: " << entry.path() << endl;
        }
    }

    if (count == 0) {
        cout << "Trash is already clean.\n";
    } else {
        cout << "Deleted " << count << " file(s) from trash.\n";
    }
}
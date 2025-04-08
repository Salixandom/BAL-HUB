#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

void logHistory() {
    ifstream logFile(".bal/log.json");

    if (!logFile) {
        cerr << "No commit history found\n";
        return;
    }

    json log;
    logFile >> log;
    logFile.close();

    if (log.empty()) {
        cerr << "No commit history found\n";
        return;
    }

    cout << "\n=== Commit History ===\n" << endl;

    int count = 0;
    for (auto it = log.rbegin(); it != log.rend(); ++it, ++count) {
        string id = (*it)["id"];
        string timestamp = (*it)["timestamp"];
        string message = (*it)["message"];
        int fileCount = (*it)["files"].size();

        if (count == 0) {
            cout << "HEAD → ";  // Mark latest commit
        }

        cout << "Commit ID  : " << id << "\n";
        cout << "Timestamp  : " << timestamp << endl;
        cout << "Message    : " << message << "\n";
        cout << "Files      : " << fileCount << " file(s)" << endl;
        cout << "-------------------------------\n";
    }

    cout << endl;
}

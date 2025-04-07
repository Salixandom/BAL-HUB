#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

void logHistory() {
    ifstream logFile(".bal/log.json");

    if(!logFile) {
        cerr << "No commit history found" << endl;
        return;
    }

    json log;
    logFile >> log;
    
    if(log.empty()) {
        cerr << "No commit history found" << endl;
        return;
    }

    cout << "\n=== Commit History ===\n" << endl;

    for(auto it = log.rbegin(); it != log.rend(); ++it) {
        cout << "Commit ID: " << (*it)["id"] << endl;
        cout << "Timestamp: " << (*it)["timestamp"] << endl;
        cout << "Message: " << (*it)["message"] << endl;
        cout << "--------------------------" << endl;
    }
}
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "nlohmann/json.hpp"

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

string getTimeStampID() {
    auto now = chrono::system_clock::now();  // get current time in cpp unique way
    auto in_time = chrono::system_clock::to_time_t(now); // convert to time_t(which is nanosecond format)
    tm buf;  // helps to breakdown the time into our readable format

#ifdef _WIN32
    localtime_s(&buf, &in_time);  // Windows
#else
    localtime_r(&in_time, &buf); // Linus/Unix
#endif

    stringstream ss;
    ss << put_time(&buf, "%Y%m%d%H%M%S");
    return ss.str();
}


void commit(const string &message) {
    if(!fs::exists(".bal/index.json")) {
        cerr << "Error: Repository not initialized. Run `bal init` first." << endl;
        return;
    }

    // Load staged files
    ifstream indexFile(".bal/index.json");
    json index;
    indexFile >> index;
    indexFile.close();

    if(index.empty()) {
        cerr << "Error: Nothing to commit." << endl;
        return;
    }

    // Create commit id and folder
    string commitID = getTimeStampID();
    fs::create_directory(".bal/commits/" + commitID);

    // Copy staged files to commit folder
    for(auto &[filename, hash] : index.items()) {
        fs::create_directories(fs::path(".bal/commits/" + commitID).parent_path());  //ensures folder exist

        try {
            fs::copy_file(filename, ".bal/commits/" + commitID + "/" + fs::path(filename).filename().string(), fs::copy_options::overwrite_existing);
        } catch(...) {
            cerr << "Error: Failed to copy: " << filename << endl;
        }
    }

    // Append commit info to log.json
    json logEntry = {
        {"id", commitID},
        {"message", message},
        {"timestamp", commitID},
        {"files", index}
    };

    json log;
    ifstream logFile(".bal/log.json");
    logFile >> log;
    logFile.close();

    log.push_back(logEntry);
    ofstream logOut(".bal/log.json");
    logOut << log.dump(4);
    logOut.close();

    // Clear staging area
    ofstream indexOut(".bal/index.json");
    indexOut << "{}";
    indexOut.close();

    cout << "Committed as " << commitID << " with message: " << message <<endl;
}


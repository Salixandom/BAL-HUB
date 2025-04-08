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
    if (!fs::exists(".bal/index.json")) {
        cerr << "Error: Repository not initialized. Run `bal init` first." << endl;
        return;
    }

    // Load staged index
    ifstream indexFile(".bal/index.json");
    json index;
    indexFile >> index;
    indexFile.close();

    if (index.empty()) {
        cerr << "Warning: Nothing is staged. Nothing to commit." << endl;
        return;
    }

    // Prepare commit ID and folder
    string commitID = getTimeStampID();
    fs::path commitDir = ".bal/commits/" + commitID;
    fs::create_directories(commitDir);

    // Copy staged files into commit folder
    int committedCount = 0;
    for (const auto& [filename, hash] : index.items()) {
        fs::path srcPath = filename;
        fs::path destPath = commitDir / srcPath;

        try {
            fs::create_directories(destPath.parent_path());  // maintain directory structure
            fs::copy_file(srcPath, destPath, fs::copy_options::overwrite_existing);
            cout << "Committed: " << filename << endl;
            committedCount++;
        } catch (...) {
            cerr << "Error: Failed to commit: " << filename << endl;
        }
    }

    // Load previous log (if exists)
    json log = json::array();
    ifstream logFile(".bal/log.json");
    if (logFile) logFile >> log;
    logFile.close();

    // Add new log entry
    json logEntry = {
        {"id", commitID},
        {"message", message},
        {"timestamp", commitID},
        {"files", index}
    };
    log.push_back(logEntry);

    // Write updated log
    ofstream logOut(".bal/log.json");
    logOut << log.dump(4);
    logOut.close();

    // Clear staging area
    ofstream indexOut(".bal/index.json");
    indexOut << "{}";
    indexOut.close();

    cout << "\nCommit complete: " << committedCount << " file(s) saved as " << commitID << endl;
    cout << "Message: " << message << endl;
}


void amendCommit(const string &newMessage) {
    if (!fs::exists(".bal/log.json")) {
        cerr << "Error: No commits found to amend." << endl;
        return;
    }

    json log;
    ifstream in(".bal/log.json");
    in >> log;
    in.close();

    if (log.empty()) {
        cerr << "Error: Commit log is empty." << endl;
        return;
    }

    // Amend the last commit message
    log.back()["message"] = newMessage;

    ofstream out(".bal/log.json");
    out << log.dump(4);
    out.close();

    cout << "Commit message updated for latest commit (" << log.back()["id"] << ")\n";
    cout << "New message: " << newMessage << endl;
}


void amendCommitFiles() {
    // Check repo and staging area
    if (!fs::exists(".bal/index.json") || !fs::exists(".bal/log.json")) {
        cerr << "Error: Repository not initialized or no commits exist." << endl;
        return;
    }

    // Load staged files
    json index;
    ifstream indexIn(".bal/index.json");
    indexIn >> index;
    indexIn.close();

    if (index.empty()) {
        cerr << "Warning: Nothing is staged. Nothing to update." << endl;
        return;
    }

    // Load commit log
    json log;
    ifstream logIn(".bal/log.json");
    logIn >> log;
    logIn.close();

    if (log.empty()) {
        cerr << "Error: Commit log is empty." << endl;
        return;
    }

    // Get latest commit
    json& lastCommit = log.back();
    string commitID = lastCommit["id"];
    fs::path commitDir = ".bal/commits/" + commitID;

    if (!fs::exists(commitDir)) {
        cerr << "Error: Commit directory does not exist for ID " << commitID << endl;
        return;
    }

    // Overwrite files in commit folder
    int updated = 0;
    for (const auto& [filename, hash] : index.items()) {
        fs::path srcPath = filename;
        fs::path destPath = commitDir / srcPath;

        try {
            fs::create_directories(destPath.parent_path());
            fs::copy_file(srcPath, destPath, fs::copy_options::overwrite_existing);
            cout << "Updated: " << filename << endl;
            updated++;
        } catch (...) {
            cerr << "Error: Could not update " << filename << endl;
        }
    }

    // Update files in commit log
    lastCommit["files"] = index;

    ofstream logOut(".bal/log.json");
    logOut << log.dump(4);
    logOut.close();

    // Clear staging
    ofstream indexOut(".bal/index.json");
    indexOut << "{}";
    indexOut.close();

    cout << "\nCommit updated (" << commitID << "): " << updated << " file(s) replaced.\n";
}
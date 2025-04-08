#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <unordered_set>
#include "nlohmann/json.hpp"
#include "hash_utils.hpp"

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

vector<string> readLines(const string &path) {
    vector<string> lines;
    ifstream in(path);
    string line;
    while(getline(in, line)) {
        lines.push_back(line);
    }
    return lines;
}

void printDiffHeader(const string &filename) {
    cout << "\n\033[1m=== Diff for " << filename << " ===\033[0m\n";
    cout << "--- staged/" << filename << endl;
    cout << "+++ working/" << filename << endl;
}

void printDiff(const string &filename, const vector<string> &oldLines, const vector<string> &newLines) {
    printDiffHeader(filename);

    size_t i = 0, j = 0;

    while (i < oldLines.size() || j < newLines.size()) {
        if (i < oldLines.size() && (j >= newLines.size() || oldLines[i] != newLines[j])) {
            cout << "\033[31m- [" << i + 1 << "] " << oldLines[i] << "\033[0m\n";
            i++;
        } else if (j < newLines.size() && (i >= oldLines.size() || oldLines[i] != newLines[j])) {
            cout << "\033[32m+ [" << j + 1 << "] " << newLines[j] << "\033[0m\n";
            j++;
        } else {
            cout << "\033[90m  [" << i + 1 << "] " << oldLines[i] << "\033[0m\n";
            i++;
            j++;
        }
    }

    cout << endl;
}

void diffSingleFile(const string &filename) {
    if (!fs::exists(".bal/index.json")) {
        cerr << "Error: Repository not initialized. Run `bal init` first." << endl;
        return;
    }

    json index;
    ifstream in(".bal/index.json");
    in >> index;
    in.close();

    if(index.find(filename) == index.end()) {
        cerr << "Error: File '" << filename << "' is not staged. Nothing to compare" << endl;
        return;
    }

    vector<string> workingLines;
    bool fileDeleted = false;

    if (fs::exists(filename)) {
        workingLines = readLines(filename);
    } else {
        fileDeleted = true;
    }

    string latestCommitPath;
    json log;
    ifstream logFile(".bal/log.json");
    logFile >> log;
    logFile.close();

    for(auto it = log.rbegin(); it != log.rend(); ++it) {
        if((*it)["files"].contains(filename)) {
            latestCommitPath = ".bal/commits/" + string((*it)["id"]) + "/" + fs::path(filename).filename().string();
            break;
        }
    }


    vector<string> stagedLines;

    if(!latestCommitPath.empty() && fs::exists(latestCommitPath)) {
        stagedLines = readLines(latestCommitPath);
    } else {
        cout << "File '" << filename << "' is newly staged. Showing entire content as new:\n";
        stagedLines = {};
    }

    if (fileDeleted)
    {
        printDiffHeader(filename);
        for (size_t i = 0; i < stagedLines.size(); ++i) {
            cout << "\033[31m- [" << i + 1 << "] " << stagedLines[i] << "\033[0m\n";
        }
        cout << "\033[33m(Note: File is deleted from working directory)\033[0m\n";
        return;
    }

    string currentHash = getFileHash(filename);
    string stagedHash = index[filename];

    if (currentHash == stagedHash) {
        return;
    }

    printDiff(filename, stagedLines, workingLines);
}


void diffAllFiles() {
    ifstream in(".bal/index.json");
    json index;
    in >> index;
    in.close();

    bool hasChanges = false;
    for(auto &[filename, hash] : index.items()) {
        string currentHash = fs::exists(filename) ? getFileHash(filename) : "";
        if(currentHash != hash) {
            diffSingleFile(filename);
            hasChanges = true;
        }
    }

    if(!hasChanges) {
        cout << "No changes detected. All files are up-to-date." << endl;
    }
}


void diff(const string &filename = "") {
    if(filename.empty()) {
        diffAllFiles();
    } else {
        diffSingleFile(filename);
    }
}


void diffWithCommit(const string &commitID, const string &filename) {
    string commitFilePath = ".bal/commits/" + commitID + "/" + fs::path(filename).filename().string();

    if(!fs::exists(commitFilePath)) {
        cerr << "Error: Commit '" << commitID << "' does not exist." << endl;
        return;
    }

    if(!fs::exists(filename)) {
        cerr << "Error: File '" << filename << "' does not exist in current working directory." << endl;
        return;
    }

    vector<string> committedLines = readLines(commitFilePath);
    vector<string> workingLines = readLines(filename);

    string workingHash = getFileHash(filename);
    string commitHash = getFileHash(commitFilePath);

    if(workingHash == commitHash) {
        cout << "File '" << filename << "' is up-to-date with commit '" << commitID << "'." << endl;
        return;
    }

    printDiff(filename + " (Commit: " + commitID + ")", committedLines, workingLines);
}

void diffWholeCommit(const string &commitID) {
    string commitDir = ".bal/commits/" + commitID;

    if(!fs::exists(commitDir)) {
        cerr << "Error: Commit '" << commitID << "' does not exist." << endl;
        return;
    }

    bool hasChanges = false;

    for(const auto &entry: fs::directory_iterator(commitDir)) {
        if(!entry.is_regular_file()) continue;  // skip non-regular files (e.g. directories, symlinks, etc.)

        string filename = entry.path().filename().string();
        string commitFilePath = entry.path().string();
        string workingFilePath = filename;

        vector<string> committedLines = readLines(commitFilePath);
        vector<string> workingLines;

        bool fileDeleted = false;

        if(fs::exists(workingFilePath)) {
            workingLines = readLines(workingFilePath);
        } else {
            fileDeleted = true;
        }

        string workingHash = fs::exists(workingFilePath) ? getFileHash(workingFilePath) : "";
        string commitHash = getFileHash(commitFilePath);

        if(workingHash == commitHash) continue;

        hasChanges = true;

        if(fileDeleted) {
            printDiffHeader(filename);
            for(size_t i = 0; i < committedLines.size(); i++) {
                cout << "\033[31m- [" << i + 1 << "] " << committedLines[i] << "\033[0m\n";
            }
            cout << "\033[33m(Note: File is deleted from working directory)\033[0m\n";
        } else {
            printDiff(filename + " (commit " + commitID + ")", committedLines, workingLines);
        }
    }

    if(!hasChanges) {
        cout << "No changes compared to commit " << commitID << endl;
    }
}


void diffCommitToCommit(const string &commitA, const string &commitB) {
    string dirA = ".bal/commits/" + commitA;
    string dirB = ".bal/commits/" + commitB;

    if (!fs::exists(dirA)) {
        cerr << "Error: Commit A (" << commitA << ") does not exist.\n";
        return;
    }
    if (!fs::exists(dirB)) {
        cerr << "Error: Commit B (" << commitB << ") does not exist.\n";
        return;
    }

    unordered_set<string> allFiles;

    for(const auto &entry: fs::directory_iterator(dirA)) {
        if(entry.is_regular_file()) allFiles.insert(entry.path().filename().string());
    }
    for (const auto &entry : fs::directory_iterator(dirB)){
        if (entry.is_regular_file()) allFiles.insert(entry.path().filename().string());
    }

    bool hasChanges = false;

    for(const string &file: allFiles) {
        string pathA = dirA + "/" + file;
        string pathB = dirB + "/" + file;

        vector<string> linesA, linesB;

        if(fs::exists(pathA)) linesA = readLines(pathA);
        if(fs::exists(pathB)) linesB = readLines(pathB);

        string hashA = fs::exists(pathA) ? getFileHash(pathA) : "";
        string hashB = fs::exists(pathB) ? getFileHash(pathB) : "";

        if(hashA == hashB) continue;

        hasChanges = true;

        printDiff(file + " (commit " + commitA + " → " + commitB + ")", linesA, linesB);
    }

    if (!hasChanges) {
        cout << "No differences between commits " << commitA << " and " << commitB << ".\n";
    }
}

void diffCommitToCommitFile(const string &commitA, const string &commitB, const string &filename) {
    string pathA = ".bal/commits/" + commitA + "/" + fs::path(filename).filename().string();
    string pathB = ".bal/commits/" + commitB + "/" + fs::path(filename).filename().string();

    if (!fs::exists(pathA)) {
        cerr << "Error: '" << filename << "' not found in commit " << commitA << ".\n";
        return;
    }
    if (!fs::exists(pathB)) {
        cerr << "Error: '" << filename << "' not found in commit " << commitB << ".\n";
        return;
    }

    vector<string> linesA = readLines(pathA);
    vector<string> linesB = readLines(pathB);

    string hashA = getFileHash(pathA);
    string hashB = getFileHash(pathB);

    if (hashA == hashB) {
        cout << "No changes in '" << filename << "' between commits " << commitA << " and " << commitB << ".\n";
        return;
    }

    printDiff(filename + " (commit " + commitA + " → " + commitB + ")", linesA, linesB);
}

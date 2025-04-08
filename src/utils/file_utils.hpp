#pragma once
#include <string>
#include <vector>

using namespace std;

// load .balignore patterns
vector<string> loadIgnorePatterns();

// checks if a file should be ignored
bool isIgnored(const string &path, vector<string> &patterns);

// list all valid files from a directory
vector<string> getAllFiles(const string &rootDir, vector<string> &ignorePatterns);

// path checks
bool isHiddenFile(const string &filename);
bool isInsideBal(const string &path);
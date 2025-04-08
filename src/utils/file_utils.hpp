#pragma once
#include <string>
#include <vector>

using namespace std;

vector<string> loadIgnorePatterns();
bool isIgnored(const string &path, vector<string> &patterns);
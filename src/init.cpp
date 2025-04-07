#include <filesystem>
#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;


void init() {
    filesystem::create_directories(".bal/commits");

    ofstream index(".bal/index.json");   // stores the information(hash and filename) about staged files
    index << "{}";
    index.close();

    ofstream log(".bal/log.json");  // stores the commit history (all the previous index json with extra timestamp and data)
    log << "[]";
    log.close();

    cout << "Initialized empty BAL repo" << endl;
}
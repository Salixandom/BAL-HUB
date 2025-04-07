#include "hash_utils.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

// using the FNV-1a algorithm

string getFileHash(const string& filename) {
    const uint64_t FNV_PRIME = 1099511628211u;  // FNV prime
    uint64_t hash = 14695981039346656037u;  // FNV offset basis  // These constants are defined by the Algorithm itself

    ifstream file(filename, ios::binary);
    if(!file) {
        cerr << "Error: Failed to open file: " << filename << endl;
        return "";
    }

    char c;
    while(file.get(c)) {
        hash ^= static_cast<uint64_t>(c);
        hash *= FNV_PRIME;
    }

    stringstream ss;
    ss << hex << setw(16) << setfill('0') << hash;
    return ss.str();
}
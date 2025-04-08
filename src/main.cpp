#include <iostream>
#include <string>

using namespace std;

void init();
void add(const string &filename);
void commit(const string &message);
void logHistory();
void checkout(const string &commitID);
void status();


int main(int argc, char* argv[]) {
    if(argc < 2) {
        cout << "Usage: bal <command>" << endl;
        return 1;
    }

    string command = argv[1];

    if(command == "init") {
        init();
    } else if(command == "add" && argc == 3) {
        add(argv[2]);
    } else if(command == "commit" && argc == 4 && string(argv[2]) == "-m") {
        commit(argv[3]);
    } else if(command == "log") {
        logHistory();
    } else if(command == "checkout" && argc == 3) {
        checkout(argv[2]);
    } else if(command == "status") {
        status();
    } else {
        cout << "Unrecognized command: " << command << endl;
    }

    return 0;
}
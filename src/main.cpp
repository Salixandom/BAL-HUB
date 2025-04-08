#include <iostream>
#include <string>

using namespace std;

void init();
void add(const string &filename);
void commit(const string &message);
void logHistory();
void checkout(const string &commitID);
void status();
void diff(const string &filename);
void diffWithCommit(const string &commitID, const string &filename);
void diffWholeCommit(const string &commitID);
void diffCommitToCommit(const string &commitA, const string &commitB);
void diffCommitToCommitFile(const string &commitA, const string &commitB, const string &filename);
void removeFromBal(const string &filename, bool cachedOnly = false);
void removeRecursive(const string &folder, bool cachedOnly = false);
void resetIndex();
void resetFile(const string &filename);
void cleanWorkingDir(bool force = false);

int main(int argc, char *argv[]){
    
    if(argc < 2) {
        cout << "Usage: bal <command>" << endl;
        return 1;
    }

    string command = argv[1];

    if(command == "init") {
        init();
    } else if (command == "add" && argc == 3) {
        string arg = argv[2];

        if (arg == "--all" || arg == "-A") add(".");
        else add(arg);
    } else if(command == "commit" && argc == 4 && string(argv[2]) == "-m") {
        commit(argv[3]);
    } else if(command == "log") {
        logHistory();
    } else if(command == "checkout" && argc == 3) {
        checkout(argv[2]);
    } else if(command == "status") {
        status();
    } else if(command == "diff") {
        if (argc == 3) {
            string arg = argv[2];
            if (arg.rfind("--commit=", 0) == 0) {
                string commitID = arg.substr(9);
                diffWholeCommit(commitID);
            } else {
                diff(argv[2]);
            }
        } else if(argc == 4 && string(argv[2]) == "--commit") {
            string commitID = argv[3];
            diffWholeCommit(commitID);
        } else if (argc == 5 && string(argv[2]) == "--commit") {
            diffWithCommit(argv[3], argv[4]);
        } else if (argc == 4) {
            diffWithCommit(argv[3], argv[2]);
        } else if (argc == 5 && string(argv[2]) == "--commit" && string(argv[4]) == "--to") {
            cerr << "Usage: bal diff --commit <idA> --to <idB>" << endl;
        } else if (argc == 6 && string(argv[2]) == "--commit" && string(argv[4]) == "--to") {
            diffCommitToCommit(argv[3], argv[5]);
        } else if (argc == 7 && string(argv[2]) == "--commit" && string(argv[4]) == "--to") {
            string commitA = argv[3];
            string commitB = argv[5];
            string filename = argv[6];
            diffCommitToCommitFile(commitA, commitB, filename);
        } else {
            diff("");
        }
    } else if(command == "rm") {
        if(argc == 3) {
            removeFromBal(argv[2], false);
        } else if(argc == 4 && string(argv[2]) == "--cached") {
            removeFromBal(argv[3], true);
        } else if(argc == 4 && string(argv[2]) == "-r") {
            removeRecursive(argv[3], false);
        } else if(argc == 5 && string(argv[2]) == "-r" && string(argv[3]) == "--cached") {
            removeRecursive(argv[4], true);
        }
    } else if (command == "reset") {
        if(argc == 3) {
            resetFile(argv[2]);
        } else {
            resetIndex();
        }
    } else if(command == "clean") {
        if(argc == 3 && (string(argv[2]) == "--force" || string(argv[2]) == "--f"))
            cleanWorkingDir(true);
        else
            cleanWorkingDir(false);
    } else {
        cout << "Unrecognized command: " << command << endl;
    }

    return 0;
}
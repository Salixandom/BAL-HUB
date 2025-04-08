#include <iostream>
using namespace std;

void printHelp()
{
    cout << R"(
BAL — Build And Learn (Custom Git in C++)

Usage:
  bal <command> [options]

Core Commands:
  init                      Initialize a new BAL repository
  add <file>               Stage a specific file
  add . / --all            Stage all non-ignored files
  commit -m "msg"          Commit staged changes
  commit --amend "msg"     Change message of the last commit
  commit --amend-files     Replace files in the last commit
  log                      Show commit history
  checkout <commitID>      Restore files from a commit

File Management:
  reset                    Unstage all files
  reset <file>             Unstage specific file
  rm <file>                Unstage + move file to trash
  rm -r <folder>           Recursively unstage + trash
  rm --cached <file>       Only unstage, keep file
  clean                    Remove untracked files (asks)
  clean --force            Remove untracked files (no prompt)

Trash & Restore:
  restore <file>                   Restore latest version from trash
  restore <file> --version <ts>   Restore specific version
  restore --list                  Show trashed files
  restore --list --detailed       Show all trash versions
  restore --all                   Restore everything from trash
  trash --list                    Alias to list trash
  trash --clean                   Permanently empty trash

Diff Viewer:
  diff                            Show all changed files
  diff <file>                     Diff working vs staged
  diff <file> <commit>            Diff file vs commit version
  diff --commit <id>              Diff working vs full commit
  diff --commit A --to B          Diff two commits

Help:
  --help                          Show this help message
)" << endl;
}

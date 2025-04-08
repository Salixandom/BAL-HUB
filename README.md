# BAL — Build And Learn (Custom Git Implementation in C++)

BAL is a lightweight version control system built from scratch in C++ to mimic the core functionality of Git.  
It's designed for educational purposes and helps you understand how version control systems like Git work under the hood.

> 🎓 Created as a personal learning project by [@Salixandom](https://github.com/Salixandom)

---

## 🧠 Features Implemented

### 🔨 Core Commands
- ✅ `init` – Initialize a new BAL repository
- ✅ `add <file>` – Stage individual files
- ✅ `add .` / `add --all` – Stage all files recursively
- ✅ `commit -m "msg"` – Save a snapshot with a message
- ✅ `commit --amend` – Edit the last commit message
- ✅ `commit --amend-files` – Update files in the last commit
- ✅ `log` – View commit history with file count
- ✅ `checkout <commit_id>` – Restore files from a previous commit

### 🧼 File Management
- ✅ `reset` – Unstage all files
- ✅ `reset <file>` – Unstage specific file
- ✅ `rm <file>` – Unstage and delete file (move to trash)
- ✅ `rm -r <dir>` – Recursively unstage and trash folder
- ✅ `rm --cached` – Only unstage, keep file
- ✅ `.balignore` – Ignore patterns for staging and cleaning

### 🔁 Diff & Compare
- ✅ `diff <file>` – Compare working vs staged
- ✅ `diff --commit <id>` – Compare working vs a commit
- ✅ `diff <file> <commit>` – Compare file vs commit
- ✅ `diff --commit A --to B` – Compare full commits
- ✅ Color-coded diff with line numbers

### 🗑️ Trash System
- ✅ Files are moved to `.bal/trash/` instead of deleted
- ✅ `restore <file>` – Restore most recent version
- ✅ `restore <file> --version <timestamp>` – Restore specific version
- ✅ `restore --list` – Show files in trash
- ✅ `restore --list --detailed` – Show all timestamped versions
- ✅ `restore --all` – Restore all trashed files
- ✅ `trash --list` – Alias for listing trash
- ✅ `trash --clean` – Empty the trash folder

### 🧹 Cleaning
- ✅ `clean` – Remove all untracked files (with confirmation)
- ✅ `clean --force` – Remove untracked files without prompt


---

## 📁 Project Structure

BAL/
├── include/            # External libraries (e.g., nlohmann/json)
│   └── nlohmann/
│       └── json.hpp
├── src/                # Source files
│   ├── main.cpp        # CLI argument handler
│   ├── init.cpp        # Handles 'bal init'
│   ├── add.cpp         # Handles 'bal add'
│   ├── commit.cpp      # Handles 'bal commit'
│   ├── log.cpp         # Handles 'bal log'
│   ├── checkout.cpp    # Handles 'bal checkout'
│   ├── clean.cpp       # Handles 'bal clean'
│   ├── diff.cpp        # Handles 'bal diff'
│   ├── reset.cpp       # Handles 'bal reset'
│   ├── restore.cpp     # Handles 'bal restore'
│   ├── rm.cpp          # Handles 'bal rm'
│   ├── status.cpp      # Handles 'bal status'
│   ├── trash.cpp       # Handles 'bal trash'
│   └── utils/          # Utility modules
│       ├── hash_utils.cpp
│       ├── file_utils.cpp
│       ├── file_utils.hpp
│       └── hash_utils.hpp
├── build/              # (Ignored) Compiled binaries and objects
├── CMakeLists.txt      # CMake build configuration
└── README.md           # Project documentation

---

## 🚀 Getting Started

### 🔧 Build Instructions (with CMake)

1. Clone the repository:
   ```bash
   git clone https://github.com/Salixandom/BAL-HUB.git
   cd BAL-HUB
   ```

2. Build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

3. Use the executable:
   ```bash
   ./bal init
   ./bal add file.txt
   ./bal commit -m "First commit"
   ```
> 🪟 On Windows: Use .\bal.exe instead of ./bal

## 🧪 Example Usage
  ```bash
  bal init
  bal add <file name>
  bal commit -m "Initial commit"
  bal log
  bal diff <file name>
  bal checkout <commitID from log>
  bal rm -r src/
  bal restore <file name> --version <commit id>
  ```

## 📦 Planned Features (Coming Soon)
-  Branching system (checkout -b, merge)
-  Interactive rebase or commit squash
-  Basic remote support (push/pull via HTTP)
-  Compression with zlib for internal files
-  More user-friendly status and color themes

## 💡 Why I Built This
> I built BAL to deeply understand how Git works internally — from hashing files to managing commits, logs, and restores.
> It was a full-stack C++ challenge and a hands-on way to master file I/O, JSON handling, and CLI tool design.

## 🛡️ Disclaimer
BAL is meant for educational purposes only and is not intended for production use. It does not handle merge conflicts, branches, or remote repositories (yet!).

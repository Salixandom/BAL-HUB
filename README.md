# BAL — Build And Learn (Custom Git Implementation in C++)

BAL is a lightweight version control system built from scratch in C++ to mimic the core functionality of Git.  
It's designed for educational purposes and helps you understand how version control systems like Git work under the hood.

> 🎓 Created as a personal learning project by [@Salixandom](https://github.com/Salixandom)

---

## 🧠 Features Implemented

- ✅ `init` – Initialize a new BAL repository
- ✅ `add <file>` – Stage individual files
- ✅ `add .` – Stage all files recursively
- ✅ `commit -m "msg"` – Save a snapshot with a message
- ✅ `log` – View commit history
- ✅ `checkout <commit_id>` – Restore files from a previous commit

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
│   └── utils/          # Utility modules
│       ├── hash_utils.cpp
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
  bal checkout <commitID from log>
  ```

## 📦 Planned Features (Coming Soon)
-  status command
-  .balignore support
-  File/directory diff viewer
-  Branching system
-  Optional compression with zlib

## 💡 Why I Built This
> I built BAL to deeply understand how Git works internally — from hashing files to managing commits, logs, and restores.
> It was a full-stack C++ challenge and a hands-on way to master file I/O, JSON handling, and CLI tool design.

## 🛡️ Disclaimer
BAL is meant for educational purposes only and is not intended for production use. It does not handle merge conflicts, branches, or remote repositories (yet!).

# Smart Blood Donation Emergency Matching System

A console-based C++ application (single admin user, no login, no GUI, no
database) that manages blood donors, tracks blood requests, and performs
emergency donor matching using core Data Structures and Algorithms.

## Project Structure
```
Smart-Blood-Donation-System/
├── include/
│   ├── Donor.h            (model — encapsulated, getters/setters)
│   ├── BloodRequest.h     (model + priority_queue comparator)
│   ├── DonationRecord.h   (model)
│   ├── DonorManager.h
│   ├── RequestManager.h
│   └── FileManager.h
├── src/
│   ├── DonorManager.cpp
│   ├── RequestManager.cpp
│   ├── FileManager.cpp
│   └── main.cpp
├── data/
│   ├── donors.txt
│   ├── requests.txt
│   └── history.txt
├── README.md
├── PROJECT_REPORT.md
├── PPT_CONTENT.md
└── VIVA_PREP.md
```

## Setting Up in VS Code

1. Install the **C/C++** extension (Microsoft) in VS Code.
2. Install **MinGW-w64** (Windows) if `g++` isn't already available —
   check with `g++ --version` in a terminal. On Linux/Mac, `g++` is
   usually already installed.
3. Open the `Smart-Blood-Donation-System` folder in VS Code
   (`File → Open Folder`).
4. Open a terminal inside VS Code (`` Ctrl+` ``).

### Creating the folders/files yourself (if starting from scratch)
```bash
mkdir Smart-Blood-Donation-System
cd Smart-Blood-Donation-System
mkdir include src data
# then create each .h file inside include/, each .cpp file inside src/
```

### Compile
```bash
g++ -Iinclude src/main.cpp src/DonorManager.cpp src/RequestManager.cpp src/FileManager.cpp -o BloodDonationSystem
```
`-Iinclude` tells g++ where to find the header files so `#include "../include/DonorManager.h"` style includes resolve correctly.

### Run
Windows (PowerShell/cmd):
```
.\BloodDonationSystem.exe
```
Linux/Mac:
```
./BloodDonationSystem
```

Data is saved to and loaded from the `data/` folder automatically.

## Sample Data

The `data/` folder ships with 6 sample donors, 2 pending blood requests,
and 1 completed donation, so you can explore the menus immediately
without registering everything from scratch first.

## Quick Feature Map

| Menu | Feature | Data Structure Used |
|---|---|---|
| Donor Management | Register / View / Search / Update / Delete | `vector`, `unordered_map` |
| Blood Request | Add / View | `priority_queue` |
| Emergency Matching | Find & assign a donor immediately | `unordered_map` (search) + `list` (logging) |
| Donation History | View all completed donations | `list` |
| Reports | Totals, availability, blood-group counts | (derived from `vector`/`unordered_map`) |

See `PROJECT_REPORT.md` for full design rationale, `VIVA_PREP.md` for
expected questions and answers, and `PPT_CONTENT.md` for a ready-to-use
slide outline.

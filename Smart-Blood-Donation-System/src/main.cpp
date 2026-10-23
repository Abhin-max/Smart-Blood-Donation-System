#include <iostream>
#include <limits>
#include "../include/DonorManager.h"
#include "../include/RequestManager.h"
#include "../include/FileManager.h"
using namespace std;

const string DONOR_FILE   = "data/donors.txt";
const string REQUEST_FILE = "data/requests.txt";
const string HISTORY_FILE = "data/history.txt";

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


bool readChoice(int &choice) {
    cin >> choice;
    if (cin.eof()) return false;
    if (cin.fail()) { clearInput(); choice = -1; }
    return true;
}

void saveAll(DonorManager &dm, RequestManager &rm) {
    FileManager::saveDonors(DONOR_FILE, dm.allDonorsRaw());
    FileManager::saveRequests(REQUEST_FILE, rm.snapshotRequests());
    FileManager::saveHistory(HISTORY_FILE, rm.getHistory());
    cout << "Data saved to /data.\n";
}

// ---------------- Donor Management submenu ----------------

void registerDonorFlow(DonorManager &dm) {
    string name, bloodGroup, phone, city;
    int age;
    cout << "Name: "; cin.ignore(); getline(cin, name);
    cout << "Age: "; cin >> age;
    cout << "Blood Group (e.g. O+, AB-): "; cin >> bloodGroup;
    cout << "Phone: "; cin >> phone;
    cout << "City: "; cin >> city;

    int id = dm.registerDonor(name, age, bloodGroup, phone, city);
    cout << "Registered successfully. Donor ID = " << id << "\n";
}

void searchByBloodGroupFlow(DonorManager &dm) {
    string bg; cout << "Enter blood group: "; cin >> bg;
    auto results = dm.searchByBloodGroup(bg);
    if (results.empty()) { cout << "No donors found for " << bg << ".\n"; return; }
    for (auto &d : results) {
        cout << d.getId() << " | " << d.getName() << " | " << d.getAge() << " | "
             << d.getCity() << " | " << (d.isAvailable() ? "Available" : "Unavailable") << "\n";
    }
}

void searchByIdFlow(DonorManager &dm) {
    int id; cout << "Enter donor ID: "; cin >> id;
    Donor d;
    if (!dm.getDonorById(id, d)) { cout << "No donor with that ID.\n"; return; }
    cout << d.getId() << " | " << d.getName() << " | " << d.getAge() << " | " << d.getBloodGroup()
         << " | " << d.getPhone() << " | " << d.getCity() << " | "
         << (d.isAvailable() ? "Available" : "Unavailable") << "\n";
}

void updateDonorFlow(DonorManager &dm) {
    int id; cout << "Enter donor ID to update: "; cin >> id;
    Donor d;
    if (!dm.getDonorById(id, d)) { cout << "No donor with that ID.\n"; return; }

    string name, bloodGroup, phone, city;
    int age;
    cout << "New Name [" << d.getName() << "]: "; cin.ignore(); getline(cin, name);
    cout << "New Age [" << d.getAge() << "]: "; cin >> age;
    cout << "New Blood Group [" << d.getBloodGroup() << "]: "; cin >> bloodGroup;
    cout << "New Phone [" << d.getPhone() << "]: "; cin >> phone;
    cout << "New City [" << d.getCity() << "]: "; cin >> city;

    dm.updateDonor(id, name, age, bloodGroup, phone, city);
    cout << "Donor updated.\n";
}

void deleteDonorFlow(DonorManager &dm) {
    int id; cout << "Enter donor ID to delete: "; cin >> id;
    if (dm.deleteDonor(id)) cout << "Donor deleted.\n";
    else cout << "No donor with that ID.\n";
}

void donorManagementMenu(DonorManager &dm) {
    int choice;
    do {
        cout << "\n---------- DONOR MANAGEMENT ----------\n";
        cout << "1. Register Donor\n";
        cout << "2. View All Donors\n";
        cout << "3. Search by Blood Group\n";
        cout << "4. Search by ID\n";
        cout << "5. Update Donor\n";
        cout << "6. Delete Donor\n";
        cout << "7. Back\n";
        cout << "Choice: ";
        if (!readChoice(choice)) return;

        switch (choice) {
            case 1: registerDonorFlow(dm); break;
            case 2: dm.displayAll(); break;
            case 3: searchByBloodGroupFlow(dm); break;
            case 4: searchByIdFlow(dm); break;
            case 5: updateDonorFlow(dm); break;
            case 6: deleteDonorFlow(dm); break;
            case 7: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 7);
}

// ---------------- Blood Request submenu ----------------

void addRequestFlow(RequestManager &rm) {
    string patientName, bloodGroup;
    int units, level;
    cout << "Patient Name: "; cin.ignore(); getline(cin, patientName);
    cout << "Blood Group needed: "; cin >> bloodGroup;
    cout << "Units required: "; cin >> units;
    cout << "Urgency (1=CRITICAL, 2=MODERATE, 3=LOW): "; cin >> level;

    int id = rm.addRequest(patientName, bloodGroup, units, level);
    cout << "Request added. Request ID = " << id << "\n";
}

void bloodRequestMenu(RequestManager &rm) {
    int choice;
    do {
        cout << "\n---------- BLOOD REQUEST ----------\n";
        cout << "1. Add Request\n";
        cout << "2. View Requests\n";
        cout << "3. Back\n";
        cout << "Choice: ";
        if (!readChoice(choice)) return;

        switch (choice) {
            case 1: addRequestFlow(rm); break;
            case 2: rm.viewRequests(); break;
            case 3: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 3);
}

// ---------------- Emergency Matching ----------------

void emergencyMatchingFlow(RequestManager &rm) {
    string bloodGroup, patientName;
    int units;
    cout << "Required blood group: "; cin >> bloodGroup;
    cout << "Patient name: "; cin.ignore(); getline(cin, patientName);
    cout << "Units required: "; cin >> units;

    int donorId;
    if (rm.matchDonor(bloodGroup, patientName, units, donorId)) {
        cout << "\nDonor found and matched!\n";
        cout << "Donor ID: " << donorId << "\n";
    } else {
        cout << "\nNo available donor found for blood group " << bloodGroup << ".\n";
    }
}

// ---------------- Reports ----------------

void showReports(DonorManager &dm) {
    cout << "\n=== REPORTS ===\n";
    cout << "Total Donors: " << dm.totalDonors() << "\n";
    cout << "Available Donors: " << dm.availableCount() << "\n";
    cout << "\nBlood Group Wise Count:\n";
    auto counts = dm.bloodGroupWiseCount();
    for (auto &pair : counts) {
        cout << "  " << pair.first << ": " << pair.second << "\n";
    }
}

// ---------------- Main menu ----------------

void printMainMenu() {
    cout << "\n======================================\n";
    cout << "     SMART BLOOD DONATION SYSTEM\n";
    cout << "======================================\n";
    cout << "1. Donor Management\n";
    cout << "2. Blood Request\n";
    cout << "3. Emergency Matching\n";
    cout << "4. Donation History\n";
    cout << "5. Reports\n";
    cout << "6. Save Data\n";
    cout << "7. Exit\n";
    cout << "--------------------------------------\n";
    cout << "Choice: ";
}

int main() {
    DonorManager donorManager;
    RequestManager requestManager(donorManager);

    // Load persisted data automatically at startup.
    for (auto &d : FileManager::loadDonors(DONOR_FILE)) donorManager.loadDonor(d);
    for (auto &r : FileManager::loadRequests(REQUEST_FILE)) requestManager.loadRequest(r);
    for (auto &h : FileManager::loadHistory(HISTORY_FILE)) requestManager.loadHistoryRecord(h);

    cout << "Smart Blood Donation Emergency Matching System\n";
    cout << "Loaded " << donorManager.totalDonors() << " donor(s) from previous session.\n";

    int choice;
    do {
        printMainMenu();
        if (!readChoice(choice)) break;

        switch (choice) {
            case 1: donorManagementMenu(donorManager); break;
            case 2: bloodRequestMenu(requestManager); break;
            case 3: emergencyMatchingFlow(requestManager); break;
            case 4: requestManager.showHistory(); break;
            case 5: showReports(donorManager); break;
            case 6: saveAll(donorManager, requestManager); break;
            case 7: {
                char confirm;
                cout << "Save before exiting? (y/n): ";
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') saveAll(donorManager, requestManager);
                cout << "Goodbye!\n";
                break;
            }
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 7);

    return 0;
}

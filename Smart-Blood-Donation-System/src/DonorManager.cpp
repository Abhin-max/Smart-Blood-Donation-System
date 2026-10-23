#include "../include/DonorManager.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

DonorManager::DonorManager() : nextId(1) {}

void DonorManager::addToIndex(const string &bloodGroup, int position) {
    bloodGroupIndex[bloodGroup].push_back(position);
}

void DonorManager::removeFromIndex(const string &bloodGroup, int position) {
    auto &bucket = bloodGroupIndex[bloodGroup];
    bucket.erase(remove(bucket.begin(), bucket.end(), position), bucket.end());
}

int DonorManager::registerDonor(string name, int age, string bloodGroup,
                                 string phone, string city) {
    int id = nextId++;
    Donor d(id, name, age, bloodGroup, phone, city, true, false);
    donors.push_back(d);
    addToIndex(bloodGroup, (int)donors.size() - 1);
    return id;
}

void DonorManager::loadDonor(const Donor &d) {
    donors.push_back(d);
    addToIndex(d.getBloodGroup(), (int)donors.size() - 1);
    if (d.getId() >= nextId) nextId = d.getId() + 1;
}

void DonorManager::displayAll() const {
    bool any = false;
    cout << left << setw(5) << "ID" << setw(15) << "Name" << setw(5) << "Age"
         << setw(6) << "Group" << setw(12) << "Phone" << setw(12) << "City"
         << "Status\n";
    cout << string(65, '-') << "\n";
    for (const auto &d : donors) {
        if (d.isDeleted()) continue;
        any = true;
        cout << left << setw(5) << d.getId() << setw(15) << d.getName() << setw(5) << d.getAge()
             << setw(6) << d.getBloodGroup() << setw(12) << d.getPhone() << setw(12) << d.getCity()
             << (d.isAvailable() ? "Available" : "Unavailable") << "\n";
    }
    if (!any) cout << "No donors registered yet.\n";
}


bool DonorManager::getDonorById(int id, Donor &out) const {
    int pos = id - 1;
    if (pos < 0 || pos >= (int)donors.size()) return false;
    if (donors[pos].isDeleted()) return false;
    out = donors[pos];
    return true;
}


vector<Donor> DonorManager::searchByBloodGroup(const string &bg) const {
    vector<Donor> result;
    auto it = bloodGroupIndex.find(bg);
    if (it == bloodGroupIndex.end()) return result;
    for (int pos : it->second) {
        if (!donors[pos].isDeleted()) result.push_back(donors[pos]);
    }
    return result;
}

bool DonorManager::updateDonor(int id, string name, int age, string bloodGroup,
                                string phone, string city) {
    int pos = id - 1;
    if (pos < 0 || pos >= (int)donors.size() || donors[pos].isDeleted()) return false;

    Donor &d = donors[pos];
    
    if (d.getBloodGroup() != bloodGroup) {
        removeFromIndex(d.getBloodGroup(), pos);
        addToIndex(bloodGroup, pos);
    }
    d.setName(name);
    d.setAge(age);
    d.setBloodGroup(bloodGroup);
    d.setPhone(phone);
    d.setCity(city);
    return true;
}


bool DonorManager::deleteDonor(int id) {
    int pos = id - 1;
    if (pos < 0 || pos >= (int)donors.size() || donors[pos].isDeleted()) return false;
    donors[pos].setDeleted(true);
    donors[pos].setAvailable(false);
    return true;
}

int DonorManager::findAvailableDonor(const string &bloodGroup) const {
    auto it = bloodGroupIndex.find(bloodGroup);
    if (it == bloodGroupIndex.end()) return -1;
    for (int pos : it->second) {
        const Donor &d = donors[pos];
        if (!d.isDeleted() && d.isAvailable()) return d.getId();
    }
    return -1;
}

bool DonorManager::markDonated(int id) {
    int pos = id - 1;
    if (pos < 0 || pos >= (int)donors.size() || donors[pos].isDeleted()) return false;
    donors[pos].setAvailable(false);
    return true;
}

vector<Donor> DonorManager::allDonors() const {
    vector<Donor> result;
    for (const auto &d : donors) if (!d.isDeleted()) result.push_back(d);
    return result;
}

vector<Donor> DonorManager::allDonorsRaw() const {
    return donors;
}

int DonorManager::totalDonors() const {
    int count = 0;
    for (const auto &d : donors) if (!d.isDeleted()) count++;
    return count;
}

int DonorManager::availableCount() const {
    int count = 0;
    for (const auto &d : donors) if (!d.isDeleted() && d.isAvailable()) count++;
    return count;
}

unordered_map<string,int> DonorManager::bloodGroupWiseCount() const {
    unordered_map<string,int> counts;
    for (const auto &d : donors) {
        if (!d.isDeleted()) counts[d.getBloodGroup()]++;
    }
    return counts;
}

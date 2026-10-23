#ifndef DONOR_MANAGER_H
#define DONOR_MANAGER_H

#include <vector>
#include <unordered_map>
#include <string>
#include "Donor.h"
using namespace std;


class DonorManager {
private:
    vector<Donor> donors;
    unordered_map<string, vector<int>> bloodGroupIndex;
    int nextId;

    void addToIndex(const string &bloodGroup, int position);
    void removeFromIndex(const string &bloodGroup, int position);

public:
    DonorManager();

    int registerDonor(string name, int age, string bloodGroup, string phone, string city);
    void loadDonor(const Donor &d);

    void displayAll() const;
    bool getDonorById(int id, Donor &out) const;          
    vector<Donor> searchByBloodGroup(const string &bg) const;

    bool updateDonor(int id, string name, int age, string bloodGroup, string phone, string city);
    bool deleteDonor(int id); 
    
    int findAvailableDonor(const string &bloodGroup) const;
    bool markDonated(int id); 

    vector<Donor> allDonors() const; 
    vector<Donor> allDonorsRaw() const; 
    int totalDonors() const;
    int availableCount() const;
    unordered_map<string,int> bloodGroupWiseCount() const;
};

#endif

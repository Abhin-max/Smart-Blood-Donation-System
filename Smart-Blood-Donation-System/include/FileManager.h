#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include <list>
#include "Donor.h"
#include "BloodRequest.h"
#include "DonationRecord.h"
using namespace std;

class FileManager {
public:
    static void saveDonors(const string &path, const vector<Donor> &donors);
    static vector<Donor> loadDonors(const string &path);

    static void saveRequests(const string &path, const vector<BloodRequest> &requests);
    static vector<BloodRequest> loadRequests(const string &path);

    static void saveHistory(const string &path, const list<DonationRecord> &history);
    static vector<DonationRecord> loadHistory(const string &path);
};

#endif

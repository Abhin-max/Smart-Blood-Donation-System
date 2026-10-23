#include "../include/FileManager.h"
#include <fstream>
using namespace std;

void FileManager::saveDonors(const string &path, const vector<Donor> &donors) {
    ofstream out(path);
    for (const auto &d : donors) out << d.toFileString() << "\n";
}

vector<Donor> FileManager::loadDonors(const string &path) {
    vector<Donor> result;
    ifstream in(path);
    string line;
    while (getline(in, line)) {
        if (!line.empty()) result.push_back(Donor::fromFileString(line));
    }
    return result;
}

void FileManager::saveRequests(const string &path, const vector<BloodRequest> &requests) {
    ofstream out(path);
    for (const auto &r : requests) out << r.toFileString() << "\n";
}

vector<BloodRequest> FileManager::loadRequests(const string &path) {
    vector<BloodRequest> result;
    ifstream in(path);
    string line;
    while (getline(in, line)) {
        if (!line.empty()) result.push_back(BloodRequest::fromFileString(line));
    }
    return result;
}

void FileManager::saveHistory(const string &path, const list<DonationRecord> &history) {
    ofstream out(path);
    for (const auto &rec : history) out << rec.toFileString() << "\n";
}

vector<DonationRecord> FileManager::loadHistory(const string &path) {
    vector<DonationRecord> result;
    ifstream in(path);
    string line;
    while (getline(in, line)) {
        if (!line.empty()) result.push_back(DonationRecord::fromFileString(line));
    }
    return result;
}

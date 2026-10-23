#include "../include/RequestManager.h"
#include <iostream>
using namespace std;

RequestManager::RequestManager(DonorManager &dm)
    : donorManager(dm), nextRequestId(1), arrivalCounter(0) {}


int RequestManager::addRequest(string patientName, string bloodGroup, int units, int urgencyLevel) {
    int id = nextRequestId++;
    BloodRequest r(id, patientName, bloodGroup, units, urgencyLevel, arrivalCounter++);
    pendingRequests.push(r);
    return id;
}

void RequestManager::loadRequest(const BloodRequest &r) {
    pendingRequests.push(r);
    if (r.getRequestId() >= nextRequestId) nextRequestId = r.getRequestId() + 1;
    if (r.getArrivalOrder() >= arrivalCounter) arrivalCounter = r.getArrivalOrder() + 1;
}

void RequestManager::loadHistoryRecord(const DonationRecord &rec) {
    history.push_back(rec);
}


vector<BloodRequest> RequestManager::snapshotRequests() {
    vector<BloodRequest> ordered;
    priority_queue<BloodRequest, vector<BloodRequest>, CompareRequest> temp = pendingRequests;
    while (!temp.empty()) {
        ordered.push_back(temp.top());
        temp.pop();
    }
    return ordered;
}

void RequestManager::viewRequests() {
    vector<BloodRequest> ordered = snapshotRequests();
    if (ordered.empty()) {
        cout << "No blood requests recorded yet.\n";
        return;
    }
    cout << "Pending requests (most urgent first):\n";
    for (auto &r : ordered) {
        cout << "  #" << r.getRequestId() << " " << r.getPatientName()
             << " | " << r.getBloodGroup() << " | " << r.getUnits() << " unit(s) | "
             << r.urgencyLabel() << "\n";
    }
}


bool RequestManager::matchDonor(const string &bloodGroup, const string &patientName,
                                 int units, int &donorId) {
    donorId = donorManager.findAvailableDonor(bloodGroup);
    if (donorId == -1) return false;

    Donor d;
    donorManager.getDonorById(donorId, d);
    donorManager.markDonated(donorId);

    DonationRecord record(donorId, d.getName(), patientName, bloodGroup, units);
    history.push_back(record); 
    return true;
}

void RequestManager::showHistory() const {
    if (history.empty()) {
        cout << "No completed donations yet.\n";
        return;
    }
    cout << "Donation History (" << history.size() << " total):\n";
    for (const auto &rec : history) {
        cout << "  Donor #" << rec.getDonorId() << " (" << rec.getDonorName() << ") -> "
             << rec.getPatientName() << " [" << rec.getBloodGroup() << ", "
             << rec.getUnits() << " unit(s)]\n";
    }
}

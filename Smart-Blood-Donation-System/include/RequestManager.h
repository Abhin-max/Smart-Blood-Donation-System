#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include <queue>
#include <list>
#include <vector>
#include "BloodRequest.h"
#include "DonationRecord.h"
#include "DonorManager.h"
using namespace std;


class RequestManager {
private:
    priority_queue<BloodRequest, vector<BloodRequest>, CompareRequest> pendingRequests;
    list<DonationRecord> history;
    DonorManager &donorManager;
    int nextRequestId;
    int arrivalCounter;

public:
    RequestManager(DonorManager &dm);

    int addRequest(string patientName, string bloodGroup, int units, int urgencyLevel);
    void loadRequest(const BloodRequest &r); 
    void loadHistoryRecord(const DonationRecord &rec);

   
    void viewRequests();
    vector<BloodRequest> snapshotRequests(); 

    
    bool matchDonor(const string &bloodGroup, const string &patientName, int units, int &donorId);

    void showHistory() const;
    list<DonationRecord> getHistory() const { return history; }
};

#endif

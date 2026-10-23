#ifndef BLOOD_REQUEST_H
#define BLOOD_REQUEST_H

#include <string>
#include <sstream>
using namespace std;


class BloodRequest {
private:
    int requestId;
    string patientName;
    string bloodGroup;
    int units;
    int urgencyLevel;
    int arrivalOrder;  

public:
    BloodRequest(int requestId = 0, string patientName = "", string bloodGroup = "",
                 int units = 1, int urgencyLevel = 3, int arrivalOrder = 0) {
        this->requestId = requestId;
        this->patientName = patientName;
        this->bloodGroup = bloodGroup;
        this->units = units;
        this->urgencyLevel = urgencyLevel;
        this->arrivalOrder = arrivalOrder;
    }

    
    int getRequestId() const { return requestId; }
    string getPatientName() const { return patientName; }
    string getBloodGroup() const { return bloodGroup; }
    int getUnits() const { return units; }
    int getUrgencyLevel() const { return urgencyLevel; }
    int getArrivalOrder() const { return arrivalOrder; }

    string urgencyLabel() const {
        if (urgencyLevel == 1) return "CRITICAL";
        if (urgencyLevel == 2) return "MODERATE";
        return "LOW";
    }

    string toFileString() const {
        ostringstream oss;
        oss << requestId << "|" << patientName << "|" << bloodGroup << "|"
            << units << "|" << urgencyLevel << "|" << arrivalOrder;
        return oss.str();
    }

    static BloodRequest fromFileString(const string &line) {
        stringstream ss(line);
        string token;
        int fid, funits, flevel, farrival; string fname, fbg;

        getline(ss, token, '|'); fid = stoi(token);
        getline(ss, token, '|'); fname = token;
        getline(ss, token, '|'); fbg = token;
        getline(ss, token, '|'); funits = stoi(token);
        getline(ss, token, '|'); flevel = stoi(token);
        getline(ss, token, '|'); farrival = stoi(token);

        return BloodRequest(fid, fname, fbg, funits, flevel, farrival);
    }
};

struct CompareRequest {
    bool operator()(const BloodRequest &a, const BloodRequest &b) const {
        if (a.getUrgencyLevel() != b.getUrgencyLevel())
            return a.getUrgencyLevel() > b.getUrgencyLevel();
        return a.getArrivalOrder() > b.getArrivalOrder();
    }
};

#endif

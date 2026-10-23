#ifndef DONATION_RECORD_H
#define DONATION_RECORD_H

#include <string>
#include <sstream>
using namespace std;


class DonationRecord {
private:
    int donorId;
    string donorName;
    string patientName;
    string bloodGroup;
    int units;

public:
    DonationRecord(int donorId = 0, string donorName = "", string patientName = "",
                    string bloodGroup = "", int units = 1) {
        this->donorId = donorId;
        this->donorName = donorName;
        this->patientName = patientName;
        this->bloodGroup = bloodGroup;
        this->units = units;
    }

    
    int getDonorId() const { return donorId; }
    string getDonorName() const { return donorName; }
    string getPatientName() const { return patientName; }
    string getBloodGroup() const { return bloodGroup; }
    int getUnits() const { return units; }

    string toFileString() const {
        ostringstream oss;
        oss << donorId << "|" << donorName << "|" << patientName << "|" << bloodGroup << "|" << units;
        return oss.str();
    }

    static DonationRecord fromFileString(const string &line) {
        stringstream ss(line);
        string token;
        int fdonorId, funits; string fdonorName, fpatient, fbg;

        getline(ss, token, '|'); fdonorId = stoi(token);
        getline(ss, token, '|'); fdonorName = token;
        getline(ss, token, '|'); fpatient = token;
        getline(ss, token, '|'); fbg = token;
        getline(ss, token, '|'); funits = stoi(token);

        return DonationRecord(fdonorId, fdonorName, fpatient, fbg, funits);
    }
};

#endif

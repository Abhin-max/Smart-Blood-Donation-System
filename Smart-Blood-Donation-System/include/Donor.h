#ifndef DONOR_H
#define DONOR_H

#include <string>
#include <sstream>
using namespace std;


class Donor {
private:
    int id;
    string name;
    int age;
    string bloodGroup;
    string phone;
    string city;
    bool available;   
    bool deleted;     

public:
        Donor(int id = 0, string name = "", int age = 0, string bloodGroup = "",
          string phone = "", string city = "", bool available = true, bool deleted = false) {
        this->id = id;
        this->name = name;
        this->age = age;
        this->bloodGroup = bloodGroup;
        this->phone = phone;
        this->city = city;
        this->available = available;
        this->deleted = deleted;
    }

    
    int getId() const { return id; }
    string getName() const { return name; }
    int getAge() const { return age; }
    string getBloodGroup() const { return bloodGroup; }
    string getPhone() const { return phone; }
    string getCity() const { return city; }
    bool isAvailable() const { return available; }
    bool isDeleted() const { return deleted; }

    
    void setName(string n) { name = n; }
    void setAge(int a) { age = a; }
    void setBloodGroup(string bg) { bloodGroup = bg; }
    void setPhone(string p) { phone = p; }
    void setCity(string c) { city = c; }
    void setAvailable(bool a) { available = a; }
    void setDeleted(bool d) { deleted = d; }

    
    string toFileString() const {
        ostringstream oss;
        oss << id << "|" << name << "|" << age << "|" << bloodGroup << "|"
            << phone << "|" << city << "|" << (available ? 1 : 0) << "|" << (deleted ? 1 : 0);
        return oss.str();
    }

    static Donor fromFileString(const string &line) {
        stringstream ss(line);
        string token;
        int fid, fage; string fname, fbg, fphone, fcity; bool favail, fdel;

        getline(ss, token, '|'); fid = stoi(token);
        getline(ss, token, '|'); fname = token;
        getline(ss, token, '|'); fage = stoi(token);
        getline(ss, token, '|'); fbg = token;
        getline(ss, token, '|'); fphone = token;
        getline(ss, token, '|'); fcity = token;
        getline(ss, token, '|'); favail = (token == "1");
        getline(ss, token, '|'); fdel = (token == "1");

        return Donor(fid, fname, fage, fbg, fphone, fcity, favail, fdel);
    }
};

#endif

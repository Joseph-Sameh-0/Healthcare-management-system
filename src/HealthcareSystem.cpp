#include "HealthcareSystem.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

void HealthcareSystem::addDoctor(const string &doctorID, const string &name, const string &address) {
    fstream doctorsFile("data/Doctors.txt", ios::app);
    if (!doctorsFile) {
        cerr << "Error: Could not open Doctors file.\n";
        return;
    }
    string record = doctorID + "|" + name + "|" + address + "\n";
    doctorsFile << record;
    doctorsFile.close();

    // Update indexes
    updatePrimaryIndex("data/PrimaryIndexDoctors.txt", doctorID, record);
    updateSecondaryIndex("data/SecondaryIndexDoctors.txt", name, doctorID);
}

void HealthcareSystem::updatePrimaryIndex(const string &file, const string &key, const string &record) {
    // Implementation of updatePrimaryIndex
}

void HealthcareSystem::updateSecondaryIndex(const string &file, const string &secondaryKey, const string &primaryKey) {
    // Implementation of updateSecondaryIndex
}

void HealthcareSystem::addAppointment(const string &appointmentID, const string &date, const string &doctorID) {
    // Implementation of addAppointment
}

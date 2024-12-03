#ifndef HEALTHCARESYSTEM_H
#define HEALTHCARESYSTEM_H

#include <string>
#include "data/primary_index.cpp"
#include "data/secondary_index.cpp"
using namespace std;

class HealthcareSystem
{
private:
    void updatePrimaryIndex(const string &file, const string &key, const long long byteOffset);
    void updateSecondaryIndex(const string &file, const string &secondaryKey, const string &primaryKey);

public:
    PrimaryIndex dIndex;
    PrimaryIndex aIndex;
    SecondaryIndex<char[30]> dSIndex;
    SecondaryIndex<char[15]> aSIndex;
    void addDoctor(const string &doctorID, const string &name, const string &address);
    void updateDoctor(const string &id, const string &newName, const string &newAddress);
    void deleteDoctor(const string &doctorID);

    void updateAppointment(const string &appointmentID, const string &newDate, const string &newDoctorID);
    void deleteAppointment(const string &appointmentID);
    void addAppointment(const string &appointmentID, const string &date, const string &doctorID);
    HealthcareSystem()
    {
        dIndex = PrimaryIndex("D:/VS Code/CPP/3rd year/1st term/file managment/Project/Healthcare-management-system/data/PrimaryIndexDoctors.txt");
        aIndex = PrimaryIndex("D:/VS Code/CPP/3rd year/1st term/file managment/Project/Healthcare-management-system/data/PrimaryIndexAppointments.txt");
        dSIndex = SecondaryIndex<char[30]>("D:/VS Code/CPP/3rd year/1st term/file managment/Project/Healthcare-management-system/data/SecondaryIndexDoctors.txt");
        aSIndex = SecondaryIndex<char[15]>("D:/VS Code/CPP/3rd year/1st term/file managment/Project/Healthcare-management-system/data/SecondaryIndexAppointments.txt");
    }
};

#endif

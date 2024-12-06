#ifndef HEALTHCARESYSTEM_H
#define HEALTHCARESYSTEM_H

#include <string>
#include <string>
#include "primary_index.cpp"
#include "secondary_index.cpp"
using namespace std;

class HealthcareSystem {
public:
    void addDoctor( const string &doctorID, const string &name, const string &address);
    void printDoctor(int byteOffset);
    void addAppointment(const string &appointmentID, const string &date, const string &doctorID);
    void printAppointment(int byteOffset);
    void deleteDoctor (const string &doctorID);
    void deleteAppointment (const string &AppointmentID);
    PrimaryIndex dIndex;
    PrimaryIndex aIndex;
    SecondaryIndex<char[30]> dSIndex;
    SecondaryIndex<char[15]> aSIndex;
    HealthcareSystem()
    {
        dIndex = PrimaryIndex("F:/college/File Managment/Healthcare/Healthcare-management-system/data/PrimaryIndexDoctors.txt");
        aIndex = PrimaryIndex("F:/college/File Managment/Healthcare/Healthcare-management-system/data/PrimaryIndexAppointments.txt");
        dSIndex = SecondaryIndex<char[30]>("F:/college/File Managment/Healthcare/Healthcare-management-system/data/SecondaryIndexDoctors.txt");
        aSIndex = SecondaryIndex<char[15]>("F:/college/File Managment/Healthcare/Healthcare-management-system/data/SecondaryIndexAppointments.txt");
    }
    // Other methods: deleteDoctor, deleteAppointment, etc.

private:
    void updatePrimaryIndex(const string &file, const string &key, const long long byteOffset);
    void updateSecondaryIndex(const string &file, const string &secondaryKey, const string &primaryKey);
};

#endif

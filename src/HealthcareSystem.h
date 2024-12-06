#ifndef HEALTHCARESYSTEM_H
#define HEALTHCARESYSTEM_H

#include <string>
#include "primary_index.cpp"
#include "secondary_index.cpp"
using namespace std;

class HealthcareSystem
{
private:
    void updatePrimaryIndex(const string &file, const string &key, const long long byteOffset);
    void updateSecondaryIndex(const string &file, const string &secondaryKey, const string &primaryKey);

public:
    void addDoctor(const string &doctorID, const string &name, const string &address);
    void addAppointment(const string &appointmentID, const string &date, const string &doctorID);
    void deleteDoctor (const string &doctorID);
    void deleteAppointment (const string &AppointmentID);
    // Other methods: deleteDoctor, deleteAppointment, etc.
    PrimaryIndex dIndex;
    PrimaryIndex aIndex;
    SecondaryIndex<char[30]> dSIndex;
    SecondaryIndex<char[15]> aSIndex;
    HealthcareSystem()
    {
        dIndex = PrimaryIndex("../data/PrimaryIndexDoctors.txt");
        aIndex = PrimaryIndex("../data/PrimaryIndexAppointments.txt");
        dSIndex = SecondaryIndex<char[30]>("../data/SecondaryIndexDoctors.txt", "../data/LinkedListDoctors.txt");
        aSIndex = SecondaryIndex<char[15]>("../data/SecondaryIndexAppointments.txt", "../data/LinkedListAppointments.txt");
    }
};

#endif

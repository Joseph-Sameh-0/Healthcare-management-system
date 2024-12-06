#ifndef HEALTHCARESYSTEM_H
#define HEALTHCARESYSTEM_H

#include <string>
#include "primary_index.cpp"
#include "secondary_index.cpp"
using namespace std;

struct Query
{
    string target;
    string source;
    string condition[4];
};

class HealthcareSystem
{
private:
public:
    void addDoctor(const string &doctorID, const string &name, const string &address);
    void addAppointment(const string &appointmentID, const string &date, const string &doctorID);
    void deleteDoctor(const string &doctorID);
    void deleteAppointment(const string &AppointmentID);
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
    Query parseQuery(const string &query);
    void executeQuery(Query query);
    void getDoctorData(Query query, long long byteOffSet);
    void getAppointmentData(Query query, long long byteOffSet);
};

#endif

#ifndef HEALTHCARESYSTEM_H
#define HEALTHCARESYSTEM_H

#include <string>
#include "primary_index.cpp"
#include "secondary_index.cpp"
#include <filesystem>
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
    void addAppointment(const string &appointmentID, const string &doctorID, const string &date);
    void deleteDoctor(const string &doctorID);
    void deleteAppointment(const string &AppointmentID);
    void updateDoctor(const string &id, const string &newName);
    void updateAppointment(const string &appointmentID, const string &newDate);

    // Other methods: deleteDoctor, deleteAppointment, etc.
    PrimaryIndex dIndex;
    PrimaryIndex aIndex;
    SecondaryIndex<char[30]> dSIndex;
    SecondaryIndex<char[15]> aSIndex;
    HealthcareSystem()
    {
        namespace fs = std::filesystem;
        if (!fs::exists("../data"))
        {
            fs::create_directory("../data");
        }

        dIndex = PrimaryIndex("../data/PrimaryIndexDoctors.txt");
        aIndex = PrimaryIndex("../data/PrimaryIndexAppointments.txt");
        dSIndex = SecondaryIndex<char[30]>("../data/SecondaryIndexDoctors.txt", "../data/LinkedListDoctors.txt");
        aSIndex = SecondaryIndex<char[15]>("../data/SecondaryIndexAppointments.txt", "../data/LinkedListAppointments.txt");
        fstream file;
        file.open("../data/Doctors.txt", ios::in | ios::out | ios::app);
        file.close();
        file.open("../data/Appointments.txt", ios::in | ios::out | ios::app);
        file.close();
    }
    void parseQuery(const string &query);
    void executeQuery(Query query);
    void getDoctorData(ifstream &file, Query query, long long byteOffSet);
    void getAppointmentData(ifstream &file, Query query, long long byteOffSet);
    void printInfoAsQuery(const string &id, const string &type);
};

#endif

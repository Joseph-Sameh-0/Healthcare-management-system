#include "HealthcareSystem.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#define DELETE_FLAG '*'
#define EMPTY_FLAG ' '

using namespace std;

void HealthcareSystem::addDoctor(const string &doctorID, const string &name, const string &address)
{
    fstream doctorsFile("../data/Doctors.txt", ios::app);

    if (!doctorsFile)
    {
        cerr << "Error: Could not open Doctors file.\n";
        return;
    }

    // Create the record with length indicator
    string record = doctorID + "|" + name + "|" + address;
    int recordLength = record.length();
    string recordWithLength = to_string(recordLength + 2) + "|" + record + "\n";

    // Write the record to the file
    doctorsFile << recordWithLength;
    doctorsFile.close();

    // Update indexes
    updatePrimaryIndex("../data/PrimaryIndexDoctors.txt", doctorID, record.length()); // TODO: update with byte offset after calculate it

    updateSecondaryIndex("../data/SecondaryIndexDoctors.txt", name, doctorID);
}

void HealthcareSystem::updatePrimaryIndex(const string &file, const string &key, const long long byteOffset)
{
    (void)file;
    (void)key;
    (void)byteOffset;
    // Implementation goes here
}

void HealthcareSystem::updateSecondaryIndex(const string &file, const string &secondaryKey, const string &primaryKey)
{
    (void)file;
    (void)secondaryKey;
    (void)primaryKey;
    // Implementation goes here
}

void HealthcareSystem::addAppointment(const string &appointmentID, const string &date, const string &doctorID)
{
    (void)appointmentID;
    (void)date;
    (void)doctorID;
    // Implementation goes here
}

void HealthcareSystem::deleteAppointment(const string &AppointmentID)
{
    // Check if the doctor exists in the index
    int byteOffset = aIndex.getByteOffset(AppointmentID.c_str());
    if (byteOffset == -1)
    {
        cout << "Error: Appointment with ID " << AppointmentID << " does not exist." << endl;
        return;
    }

    // Open the file in binary read/write mode
    fstream AppointmentFile("../data/Appointments.txt", ios::in | ios::out);
    if (!AppointmentFile)
    {
        cerr << "Error: Could not open file." << endl;
        return;
    }

    int firstDeletedRecord, nextDeletedRecord, recordSize;
    char flag;

    // Read the current head of the availability list
    AppointmentFile.seekg(0, ios::beg);
    AppointmentFile.read((char *)&firstDeletedRecord, sizeof(int));

    // Seek to the byte offset of the record to be deleted
    AppointmentFile.seekg(byteOffset, ios::beg);
    AppointmentFile.get(flag);                                     // Read the flag
    AppointmentFile.read((char *)&nextDeletedRecord, sizeof(int)); // Read the pointer to the next deleted record
    AppointmentFile.read((char *)&recordSize, sizeof(int));        // Read the size of the record

    if (flag == DELETE_FLAG) // Record is already deleted
    {
        cout << "Error: Appointment with ID " << AppointmentID << " is already deleted." << endl;
        AppointmentFile.close();
        return;
    }

    char record[recordSize];
    AppointmentFile.read((char *)&record, recordSize);
    stringstream s(record);
    string docId;
    getline(s, docId, '|');
    getline(s, docId, '|');
    getline(s, docId, '\n');
    // cout << docId << endl;                             // Print the name of the doctor being deleted
    aIndex.deleteID(AppointmentID.c_str());                 // Remove the doctor from the primary index
    aSIndex.deleteId(docId.c_str(), AppointmentID.c_str()); // Delete the doctor from the doctor index

    // Mark the record as deleted by writing the DELETE_FLAG ('*')
    AppointmentFile.seekp(byteOffset, ios::beg);
    AppointmentFile.put(DELETE_FLAG);

    // Update the deleted record to point to the current head of the availability list
    AppointmentFile.write((char *)&firstDeletedRecord, sizeof(int));

    // Update the availability list head to this record's byte offset
    AppointmentFile.seekp(0, ios::beg);
    AppointmentFile.write((char *)&byteOffset, sizeof(int));

    AppointmentFile.close();

    // Remove the doctor from the primary index
    aIndex.deleteID(AppointmentID.c_str());
    // aSIndex.deleteId(AppointmentID.c_str());
    cout << "Appointment with ID " << AppointmentID << " deleted successfully." << endl;
}

void HealthcareSystem::deleteDoctor(const string &doctorID)
{
    // Check if the doctor exists in the index
    int byteOffset = dIndex.getByteOffset(doctorID.c_str());
    if (byteOffset == -1)
    {
        cout << "Error: Doctor with ID " << doctorID << " does not exist." << endl;
        return;
    }

    // Open the file in binary read/write mode
    fstream doctorFile("../data/Doctors.txt", ios::in | ios::out | ios::binary);
    if (!doctorFile)
    {
        cerr << "Error: Could not open file." << endl;
        return;
    }

    int firstDeletedRecord, nextDeletedRecord, recordSize;
    char flag;

    // Read the current head of the availability list
    doctorFile.seekg(0, ios::beg);
    doctorFile.read((char *)&firstDeletedRecord, sizeof(int));

    // Seek to the byte offset of the record to be deleted
    doctorFile.seekg(byteOffset, ios::beg);
    doctorFile.get(flag);                                     // Read the flag
    doctorFile.read((char *)&nextDeletedRecord, sizeof(int)); // Read the pointer to the next deleted record
    doctorFile.read((char *)&recordSize, sizeof(int));        // Read the size of the record

    if (flag == DELETE_FLAG) // Record is already deleted
    {
        cout << "Error: Doctor with ID " << doctorID << " is already deleted." << endl;
        doctorFile.close();
        return;
    }

    char record[recordSize];
    doctorFile.read((char *)&record, recordSize);
    stringstream s(record);
    string dName;
    getline(s, dName, '|');
    getline(s, dName, '|');
    // cout << dName << endl;                             // Print the name of the doctor being deleted
    dIndex.deleteID(doctorID.c_str());                 // Remove the doctor from the primary index
    dSIndex.deleteId(dName.c_str(), doctorID.c_str()); // Delete the doctor from the doctor index

    // Mark the record as deleted by writing the DELETE_FLAG ('*')
    doctorFile.seekp(byteOffset, ios::beg);
    doctorFile.put(DELETE_FLAG);

    // Update the deleted record to point to the current head of the availability list
    doctorFile.write((char *)&firstDeletedRecord, sizeof(int));

    // Update the availability list head to this record's byte offset
    doctorFile.seekp(0, ios::beg);
    doctorFile.write((char *)&byteOffset, sizeof(int));

    doctorFile.close();

    cout << "Doctor with ID " << doctorID << " deleted successfully." << endl;
}

#include "HealthcareSystem.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdio>

using namespace std;

#define DELETE_FLAG '*'
#define EMPTY_FLAG ' '

void HealthcareSystem::addDoctor(const string &doctorID, const string &name, const string &address)
{
    // File for storing doctor records
     fstream doctorFile("F:/college/File Managment/Healthcare/Healthcare-management-system/data/Doctors.txt", ios::in | ios::out | ios::binary);

    if (!doctorFile) // Create file if it doesn't exist
    {
        // Initialize the file with the avail list header
        doctorFile.open("F:/college/File Managment/Healthcare/Healthcare-management-system/data/Doctors.txt", ios::out | ios::binary);
        int availListOffset = -1; // Initial header (no deleted records)
        doctorFile.write((char *)&availListOffset, sizeof(int)); // Write initial avail list
        doctorFile.write((char *)&"|", sizeof(char));            // Write initial avail list
        doctorFile.close();
    }
    doctorFile.open("F:/college/File Managment/Healthcare/Healthcare-management-system/data/Doctors.txt", ios::in | ios::out | ios::binary);
    

    string record = doctorID + "|" + name + "|" + address;
    int recordSize = record.length();
    int firstDeletedRecord, nextDeletedRecord = -1, byteOffset = -1;
    char flag;

    doctorFile.seekg(0, ios::beg);
    doctorFile.read((char *)&firstDeletedRecord, sizeof(int));

    if (firstDeletedRecord != -1) // Check if there are deleted records to reuse
    {

        byteOffset = firstDeletedRecord;
        int currRecordSize;

        while (byteOffset != -1)
        {
            doctorFile.seekg(byteOffset, ios::beg);
            doctorFile.get(flag);                                     // Read the flag
            doctorFile.read((char *)&nextDeletedRecord, sizeof(int)); // Next deleted record pointer
            doctorFile.read((char *)&currRecordSize, sizeof(int));    // Current record size

            if (flag == DELETE_FLAG && recordSize <= currRecordSize) // Found a deleted record
            {
                // Update avail list header
                doctorFile.seekp(0, ios::beg);
                doctorFile.write((char *)&nextDeletedRecord, sizeof(int));

                doctorFile.seekp(byteOffset, ios::beg);
                doctorFile.put(EMPTY_FLAG);
                int jj = -1;
                doctorFile.write((char *)&jj, sizeof(int));
                doctorFile.write((char *)&currRecordSize, sizeof(int));
                doctorFile.write(record.c_str(), recordSize);
                doctorFile.close();

                return;
            }
            byteOffset = nextDeletedRecord;
        }
        
    }

// If no deleted records were suitable, append the new record to the end of the file
    doctorFile.seekp(0, ios::end);
    doctorFile.put(EMPTY_FLAG);
    int jj = -1;
    doctorFile.write((char *)&jj, sizeof(int));
    doctorFile.write((char *)&recordSize, sizeof(int));
    doctorFile.write(record.c_str(), recordSize);
    doctorFile.close();

    // Update primary and secondary indexes
    //     dIndex.add(doctorID.c_str(), byteOffset);
    //     dSIndex.add(name.c_str(), doctorID.c_str());
}

void HealthcareSystem::printDoctor(int byteOffset)
{
    ifstream doctorFile("F:/college/File Managment/Healthcare/Healthcare-management-system/data/Doctors.txt", ios::in);
    doctorFile.seekg(byteOffset + sizeof(char) + sizeof(int));
    int recordSize;
    doctorFile.read((char *)&recordSize, sizeof(int));
    char record[recordSize];
    doctorFile.read((char *)&record, recordSize);
    cout << record;
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

void HealthcareSystem::deleteDoctor(char id[15])
{
    // File for storing doctor records
    fstream doctorFile("F:/college/File Managment/Healthcare/Healthcare-management-system/data/Doctors.txt", ios::in | ios::out | ios::binary);

    if (!doctorFile)
    {
        cerr << "Error: Could not open the Doctors file.\n";
        return;
    }

    int byteOffset = dIndex.getByteOffset(id);
    if (byteOffset == -1)
    {
        cerr << "Error: Doctor with ID " << id << " not found.\n";
        return;
    }

    doctorFile.seekg(byteOffset, ios::beg);
    char flag;
    doctorFile.get(flag);

    if (flag == DELETE_FLAG)
    {
        cerr << "Error: Doctor with ID " << id << " is already deleted.\n";
        return;
    }

    // Mark the record as deleted
    int firstDeletedRecord = -1;
    doctorFile.seekg(0, ios::beg);
    doctorFile.read((char *)&firstDeletedRecord, sizeof(int));

    doctorFile.seekp(byteOffset, ios::beg);
    doctorFile.put(DELETE_FLAG);                                // Set delete flag
    doctorFile.write((char *)&firstDeletedRecord, sizeof(int)); // Update the pointer to the next deleted record

    // Update the header with the new deleted record
    doctorFile.seekp(0, ios::beg);
    doctorFile.write((char *)&byteOffset, sizeof(int));

    doctorFile.close();

    // Remove the record from indexes
    // dIndex.remove(id);
    // dSIndex.removeByPrimaryKey(id);
}

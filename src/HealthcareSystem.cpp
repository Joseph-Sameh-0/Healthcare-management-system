#include "HealthcareSystem.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdio>

using namespace std;

#define DELETE_FLAG '*'
#define EMPTY_FLAG ' '

void HealthcareSystem ::addDoctor(const string &doctorID, const string &name, const string &address)
{

    if (dIndex.getByteOffset(doctorID.c_str()) != -1)
    {
        cout << "This Doctor is already exist" << endl;
        return;
    }

    fstream doctorFile;
    doctorFile.open("../data/Doctors.txt", ios::in | ios::out); // Open file for reading and writing
    if (!doctorFile.is_open())
    {
        cout << "Failed to open file." << endl;
        return;
    }

    doctorFile.seekg(0, ios::end);
    int filesize = doctorFile.tellg();
    if (filesize <= 0)
    {
        int availListOffset = -1; // No deleted records initially
        doctorFile.seekp(0, ios::beg);
        doctorFile.write((char *)&availListOffset, sizeof(int)); // Write initial avail list
        doctorFile.put('|');                                     // Write initial separator (optional)
    }

    string record = doctorID + "|" + name + "|" + address + "\n"; // Removed \n
    int recordSize = record.length();
    int firstDeletedRecord, nextDeletedRecord = -1, byteOffset = -1;
    char flag;

    doctorFile.seekg(0, ios::beg);
    doctorFile.read((char *)&firstDeletedRecord, sizeof(int)); // Read first deleted record pointer

    if (firstDeletedRecord != -1) // Check if there are deleted records to reuse
    {
        cout << "Record size: " << recordSize << endl;

        byteOffset = firstDeletedRecord;
        int currRecordSize;

        while (byteOffset != -1)
        {
            // Move to the byteOffset and read the flag and nextDeletedRecord
            doctorFile.seekg(byteOffset, ios::beg);
            doctorFile.get(flag); // Read the flag

            doctorFile.read((char *)&nextDeletedRecord, sizeof(int)); // Read next deleted record pointer
            doctorFile.read((char *)&currRecordSize, sizeof(int));    // Read current record size

            if (flag == DELETE_FLAG && recordSize <= currRecordSize) // Found a deleted record and suitable size
            {
                cout << "Reusing record at offset: " << byteOffset << endl;

                // Update the available list header
                doctorFile.seekp(0, ios::beg);
                doctorFile.write((char *)&nextDeletedRecord, sizeof(int)); // Update header

                // Seek to the position of the deleted record and mark it as reused
                doctorFile.seekp(byteOffset, ios::beg);
                doctorFile.put(EMPTY_FLAG); // Mark the record as reused (empty)
                int jj = -1;
                doctorFile.write((char *)&jj, sizeof(int)); // Reset nextDeletedRecord for this slot

                const char *DocId = doctorID.c_str();
                dIndex.update_index(DocId, byteOffset);

                // If the new record is smaller than the deleted one, split into two parts
                if (recordSize < currRecordSize)
                {
                    int remainingSize = currRecordSize - recordSize - sizeof(char) - sizeof(int) - sizeof(int);
                    if (remainingSize >= 10)
                    { // Minimum size for a new record (arbitrary threshold)
                        // Write size for first part (new record)
                        doctorFile.write((char *)&recordSize, sizeof(int));
                        doctorFile.write(record.c_str(), recordSize);

                        // Create new deleted record with remaining space
                        int newOffset = doctorFile.tellp();
                        doctorFile.put(DELETE_FLAG);

                        // Get the current first deleted record
                        doctorFile.seekg(0, ios::beg);
                        int currentFirst;
                        doctorFile.read((char *)&currentFirst, sizeof(int));

                        // Write the link to the next deleted record
                        doctorFile.seekp(newOffset + 1, ios::beg);
                        doctorFile.write((char *)&currentFirst, sizeof(int));
                        doctorFile.write((char *)&remainingSize, sizeof(int));

                        // Update the header to point to this new deleted record
                        doctorFile.seekp(0, ios::beg);
                        doctorFile.write((char *)&newOffset, sizeof(int));
                    }
                    else
                    {
                        // If remaining space is too small, use the entire slot
                        doctorFile.write((char *)&currRecordSize, sizeof(int));
                        doctorFile.write(record.c_str(), recordSize);
                        // Pad with spaces if needed
                        for (int i = 0; i < currRecordSize - recordSize; i++)
                        {
                            doctorFile.put(' ');
                        }
                    }
                }
                else
                {
                    // Same size or exact fit
                    doctorFile.write((char *)&currRecordSize, sizeof(int));
                    doctorFile.write(record.c_str(), recordSize);
                }

                doctorFile.close();
                return;
            }

            // Move to the next deleted record
            byteOffset = nextDeletedRecord;
        }
    }

    // If no deleted records were suitable, append the new record to the end of the file
    doctorFile.seekp(0, ios::end);
    byteOffset = doctorFile.tellp();
    doctorFile.put(EMPTY_FLAG); // Mark new slot as empty
    int jj = -1;
    doctorFile.write((char *)&jj, sizeof(int));         // No next deleted record
    doctorFile.write((char *)&recordSize, sizeof(int)); // Write the size of the new record
    doctorFile.write(record.c_str(), recordSize);       // Write the actual record
    const char *DocId = doctorID.c_str();
    dIndex.update_index(DocId, byteOffset);
    doctorFile.close();
}

// void HealthcareSystem::addDoctor(const string &doctorID, const string &name, const string &address)
// {

//     if (dIndex.getByteOffset(doctorID.c_str()) != -1)
//     {
//         cout << "This Doctor is already exist" << endl;
//         return;
//     }
//     // File for storing doctor records
//     fstream doctorFile;
//     doctorFile.open("F:/college/File Managment/Healthcare/Healthcare-management-system/data/Doctors.txt", ios::in | ios::out | ios::app);
//     doctorFile.seekg(0, ios::end);
//     int filesize = doctorFile.tellg();
//     if (filesize <= 0) // Create file if it doesn't exist
//     {
//         // Initialize the file with the avail list header
//         // doctorFile.open("F:/college/File Managment/Healthcare/Healthcare-management-system/data/Doctors.txt",  ios::out | ios::binary);
//         int availListOffset = -1;                                // Initial header (no deleted records)
//         doctorFile.write((char *)&availListOffset, sizeof(int)); // Write initial avail list
//         doctorFile.write((char *)&"|", sizeof(char));            // Write initial avail list
//         // doctorFile.close();
//     }
//     // doctorFile.open("F:/college/File Managment/Healthcare/Healthcare-management-system/data/Doctors.txt", ios::in | ios::out | ios::binary);

//     string record = doctorID + "|" + name + "|" + address + "\n";
//     int recordSize = record.length();
//     int firstDeletedRecord, nextDeletedRecord = -1, byteOffset = -1;
//     char flag;

//     doctorFile.seekg(0, ios::beg);
//     doctorFile.read((char *)&firstDeletedRecord, sizeof(int));

//     if (firstDeletedRecord != -1) // Check if there are deleted records to reuse
//     {

//         byteOffset = firstDeletedRecord;
//         int currRecordSize;

//         while (byteOffset != -1)
//         {
//             doctorFile.seekg(byteOffset, ios::beg);
//             doctorFile.get(flag);                                     // Read the flag
//             doctorFile.read((char *)&nextDeletedRecord, sizeof(int)); // Next deleted record pointer
//             doctorFile.read((char *)&currRecordSize, sizeof(int));    // Current record size

//             if (flag == DELETE_FLAG && recordSize <= currRecordSize) // Found a deleted record
//             {
//                 // Update avail list header
//                 doctorFile.seekp(0, ios::beg);
//                 doctorFile.write((char *)&nextDeletedRecord, sizeof(int));//////////

//                 doctorFile.seekp(byteOffset, ios::beg);
//                 doctorFile.put(EMPTY_FLAG);
//                 int jj = -1;
//                 doctorFile.write((char *)&jj, sizeof(int));
//                 doctorFile.write((char *)&currRecordSize, sizeof(int));
//                 doctorFile.write(record.c_str(), recordSize);
//                 doctorFile.close();
//                 dIndex.update_index(doctorID.c_str(), byteOffset);

//                 return;
//             }
//             byteOffset = nextDeletedRecord;
//         }

//     }

//     // If no deleted records were suitable, append the new record to the end of the file
//     doctorFile.seekp(0, ios::end);
//     byteOffset = doctorFile.tellp(); // Get the byte offset for the new record
//     doctorFile.put(EMPTY_FLAG);
//     int jj = -1;
//     doctorFile.write((char *)&jj, sizeof(int));
//     doctorFile.write((char *)&recordSize, sizeof(int));
//     doctorFile.write(record.c_str(), recordSize);
//     doctorFile.close();

//     // Update primary and secondary indexes
//     const char *DocId = doctorID.c_str();
//     dIndex.update_index(DocId, byteOffset);
//     //     dSIndex.add(name.c_str(), doctorID.c_str());
//     cout << "Doctor added successfully" << endl;
// }

void HealthcareSystem::printDoctor(int byteOffset)
{
    ifstream doctorFile("F:/college/File Managment/Healthcare/Healthcare-management-system/data/Doctors.txt", ios::in);
    doctorFile.seekg(byteOffset + sizeof(char) + sizeof(int));
    int recordSize;
    doctorFile.read((char *)&recordSize, sizeof(int));
    char record[recordSize];
    doctorFile.read((char *)&record, recordSize);
    stringstream s(record);
    string g;
    getline(s, g, '\n');
    cout << g<<endl;
}

void HealthcareSystem::printAppointment(int byteOffset)
{
    ifstream AppointmentFile("../data/Appointments.txt", ios::in);
    AppointmentFile.seekg(byteOffset + sizeof(char) + sizeof(int));
    int recordSize;
    AppointmentFile.read((char *)&recordSize, sizeof(int));
    char record[recordSize];
    AppointmentFile.read((char *)&record, recordSize);
    stringstream s(record);
    string g;
    getline(s, g, '\n');
    cout << g<<endl;
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
    if (aIndex.getByteOffset(appointmentID.c_str()) != -1)
    {
        cout << "This Appointment already exists" << endl;
        return;
    }

    fstream appointmentFile;
    appointmentFile.open("../data/Appointments.txt", ios::in | ios::out ); // Open file for reading and writing
    if (!appointmentFile.is_open())
    {
        cout << "Failed to open file." << endl;
        return;
    }

    appointmentFile.seekg(0, ios::end);
    int filesize = appointmentFile.tellg();
    if (filesize <= 0)
    {
        int availListOffset = -1; // No deleted records initially
        appointmentFile.seekp(0, ios::beg);
        appointmentFile.write((char *)&availListOffset, sizeof(int)); // Write initial avail list
        appointmentFile.put('|');                                     // Write initial separator (optional)
    }

    string record = appointmentID + "|" + date + "|" + doctorID + "\n";
    int recordSize = record.length();
    int firstDeletedRecord, nextDeletedRecord = -1, byteOffset = -1;
    char flag;

    appointmentFile.seekg(0, ios::beg);
    appointmentFile.read((char *)&firstDeletedRecord, sizeof(int)); // Read first deleted record pointer

    if (firstDeletedRecord != -1) // Check if there are deleted records to reuse
    {
        byteOffset = firstDeletedRecord;
        int currRecordSize;

        while (byteOffset != -1)
        {
            // Move to the byteOffset and read the flag and nextDeletedRecord
            appointmentFile.seekg(byteOffset, ios::beg);
            appointmentFile.get(flag); // Read the flag
            appointmentFile.read((char *)&nextDeletedRecord, sizeof(int)); // Read next deleted record pointer
            appointmentFile.read((char *)&currRecordSize, sizeof(int));    // Read current record size

            if (flag == DELETE_FLAG && recordSize <= currRecordSize) // Found a deleted record and suitable size
            {
                cout << "Reusing record at offset: " << byteOffset << endl;

                // Update the available list header
                appointmentFile.seekp(0, ios::beg);
                appointmentFile.write((char *)&nextDeletedRecord, sizeof(int)); // Update header

                // Seek to the position of the deleted record and mark it as reused
                appointmentFile.seekp(byteOffset, ios::beg);
                appointmentFile.put(EMPTY_FLAG); // Mark the record as reused (empty)
                int jj = -1;
                appointmentFile.write((char *)&jj, sizeof(int)); // Reset nextDeletedRecord for this slot

                const char *AppId = appointmentID.c_str();
                aIndex.update_index(AppId, byteOffset);

                if (recordSize < currRecordSize)
                {
                    int remainingSize = currRecordSize - recordSize - sizeof(char) - sizeof(int) - sizeof(int);
                    if (remainingSize >= 10) // Minimum size for a new record (arbitrary threshold)
                    {
                        appointmentFile.write((char *)&recordSize, sizeof(int));
                        appointmentFile.write(record.c_str(), recordSize);

                        // Create new deleted record with remaining space
                        int newOffset = appointmentFile.tellp();
                        appointmentFile.put(DELETE_FLAG);
                        appointmentFile.seekg(0, ios::beg);
                        int currentFirst;
                        appointmentFile.read((char *)&currentFirst, sizeof(int));

                        appointmentFile.seekp(newOffset + 1, ios::beg);
                        appointmentFile.write((char *)&currentFirst, sizeof(int));
                        appointmentFile.write((char *)&remainingSize, sizeof(int));

                        appointmentFile.seekp(0, ios::beg);
                        appointmentFile.write((char *)&newOffset, sizeof(int));
                    }
                    else
                    {
                        appointmentFile.write((char *)&currRecordSize, sizeof(int));
                        appointmentFile.write(record.c_str(), recordSize);
                        // for (int i = 0; i < currRecordSize - recordSize; i++)
                        // {
                        //     appointmentFile.put(' ');
                        // }
                    }
                }
                else
                {
                    appointmentFile.write((char *)&currRecordSize, sizeof(int));
                    appointmentFile.write(record.c_str(), recordSize);
                }

                appointmentFile.close();
                return;
            }

            byteOffset = nextDeletedRecord;
        }
    }

    appointmentFile.seekp(0, ios::end);
    byteOffset = appointmentFile.tellp();
    appointmentFile.put(EMPTY_FLAG); // Mark new slot as empty
    int jj = -1;
    appointmentFile.write((char *)&jj, sizeof(int));        // No next deleted record
    appointmentFile.write((char *)&recordSize, sizeof(int)); // Write the size of the new record
    appointmentFile.write(record.c_str(), recordSize);       // Write the actual record
    const char *AppId = appointmentID.c_str();
    aIndex.update_index(AppId, byteOffset);
    cout << "Appointment added Successfully \n" ;
    appointmentFile.close();
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
    fstream doctorFile("F:/college/File Managment/Healthcare/Healthcare-management-system/data/Doctors.txt", ios::in | ios::out | ios::binary);
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

    // Mark the record as deleted by writing the DELETE_FLAG ('*')
    doctorFile.seekp(byteOffset, ios::beg);
    doctorFile.put(DELETE_FLAG);

    // Update the deleted record to point to the current head of the availability list
    doctorFile.write((char *)&firstDeletedRecord, sizeof(int));

    // Update the availability list head to this record's byte offset
    doctorFile.seekp(0, ios::beg);
    doctorFile.write((char *)&byteOffset, sizeof(int));

    doctorFile.close();

    // Remove the doctor from the primary index
    dIndex.deleteID(doctorID.c_str());

    cout << "Doctor with ID " << doctorID << " deleted successfully." << endl;
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
    fstream AppointmentFile("F:/college/File Managment/Healthcare/Healthcare-management-system/data/Appointments.txt", ios::in | ios::out );
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

    cout << "Appointment with ID " << AppointmentID << " deleted successfully." << endl;
}
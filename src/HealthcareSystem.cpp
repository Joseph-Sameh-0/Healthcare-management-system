#include "HealthcareSystem.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

// void HealthcareSystem::addDoctor(const string &doctorID, const string &name, const string &address) {
//     fstream doctorsFile("../data/Doctors.txt", ios::app);


//     if (!doctorsFile) {
//         cerr << "Error: Could not open Doctors file.\n";
//         return;
//     }

//     // Create the record with length indicator
//     string record = doctorID + "|" + name + "|" + address;
//     int recordLength = record.length();
//     string recordWithLength = to_string(recordLength+2) + "|" + record + "\n";

//     // Write the record to the file
//     doctorsFile << recordWithLength;
//     doctorsFile.close();

//  // Modify these function signatures to accept const char* instead of char*
// void updatePrimaryIndex(const string &indexFile, const char* key, long long byteOffset);
// void updateSecondaryIndex(const string &indexFile, const char* key, const char* doctorID);
// }

void HealthcareSystem::updatePrimaryIndex(const string &file, const string &key, const long long byteOffset) {
    (void)file;
    (void)key;
    (void)byteOffset;
    // Implementation goes here
}

void HealthcareSystem::updateSecondaryIndex(const string &file, const string &secondaryKey, const string &primaryKey) {
    (void)file;
    (void)secondaryKey;
    (void)primaryKey;
    // Implementation goes here
}

// void HealthcareSystem::addAppointment(const string &appointmentID, const string &date, const string &doctorID) {
//     (void)appointmentID;
//     (void)date;
//     (void)doctorID;
//     // Implementation goes here

// }

void HealthcareSystem::deleteDoctor(const string& id) {
    fstream file("../data/Doctors.txt", ios::in | ios::out | ios::binary);

    short M = -1; // Marker for end of the Avail List
    short Offset = dIndex.getByteOffset((char*)id.c_str()); // Locate the record by ID
    short RecordSize, Header;

    if (Offset == -1) {
        cout << "Record not found!" << endl;
        return;
    }

    // Read the record size
    file.seekg(Offset, ios::beg);
    file.read((char*)&RecordSize, sizeof(RecordSize));

    // Insert '*' at the beginning of the record
    file.seekp(Offset, ios::beg);
    file.put('*'); // Add marker

    // Write metadata: size and previous offset
    file.write((char*)&RecordSize, sizeof(RecordSize)); // Write size
    file.put('|'); // Delimiter after size

    // Update the Avail List
    file.seekg(0, ios::beg);
    file.read((char*)&Header, sizeof(Header)); // Read current Avail List head

    file.write((char*)&Header, sizeof(Header)); // Link to previous Avail List head
    file.seekp(Offset + sizeof(RecordSize) + 1, ios::beg); // Position for the previous offset
    file.write((char*)&Header, sizeof(Header)); // Link to previous Avail List head

    // Update the header to point to the new Avail List head
    file.seekp(0, ios::beg);
    file.write((char*)&Offset, sizeof(Offset));

    cout << "Record successfully deleted." << endl;
    file.close();
}

void HealthcareSystem::updateDoctor(const string& id, const string& newName, const string& newAddress) {
    fstream doctorsFile("../data/Doctors.txt", ios::in | ios::out | ios::binary);
    
    long long byteOffset = dIndex.getByteOffset((char*)id.c_str());
    if (byteOffset == -1) {
        cout << "Doctor not found!" << endl;
        return;
    }

    // Read the current record size
    short recordSize;
    doctorsFile.seekg(byteOffset, ios::beg);
    doctorsFile.read((char*)&recordSize, sizeof(recordSize));

    // Create the new record
    string record = id + "|" + newName + "|" + newAddress + "|";
    short newRecordSize = record.length();

    if (newRecordSize <= recordSize) {
        // Update in place if new record fits
        doctorsFile.seekp(byteOffset, ios::beg);
        string recordWithLength = record;
        recordWithLength.insert(0, string((char*)&newRecordSize, sizeof(newRecordSize)));
        doctorsFile.write(recordWithLength.c_str(), recordWithLength.length());
    } else {
        // Delete old record and add new one at the end
        deleteDoctor(id);
        doctorsFile.seekp(0, ios::end);
        string recordWithLength = record;
        recordWithLength.insert(0, string((char*)&newRecordSize, sizeof(newRecordSize)));
        doctorsFile.write(recordWithLength.c_str(), recordWithLength.length());
        doctorsFile.close();

        // Update indexes
        dIndex.deleteID((char*)id.c_str());
    dIndex.update_index((char*)id.c_str(), byteOffset);
    
    dSIndex.deleteKey((char*)newName.c_str());
    dSIndex.update_index((char*)newName.c_str(), (char*)id.c_str());

    cout << "Doctor information updated successfully." << endl;
}

void HealthcareSystem::updateAppointment(const string &appointmentID, const string &newDate, const string &newDoctorID) {
    fstream appointmentsFile("../data/Appointments.txt", ios::in | ios::out | ios::binary);

    long long byteOffset = aIndex.getByteOffset((char*)appointmentID.c_str());
    if (byteOffset == -1) {
        cout << "Appointment not found!" << endl;
        return;
    }

    short recordSize;
    appointmentsFile.seekg(byteOffset, ios::beg);
    appointmentsFile.read((char*)&recordSize, sizeof(recordSize));

    string record = appointmentID + "|" + newDate + "|" + newDoctorID + "|";
    short newRecordSize = record.length();

    if (newRecordSize <= recordSize) {
        appointmentsFile.seekp(byteOffset, ios::beg);
        string recordWithLength = record;
        recordWithLength.insert(0, string((char*)&newRecordSize, sizeof(newRecordSize)));
        appointmentsFile.write(recordWithLength.c_str(), recordWithLength.length());
    } else {
        deleteAppointment(appointmentID);

        appointmentsFile.seekp(0, ios::end);
        string recordWithLength = record;
        recordWithLength.insert(0, string((char*)&newRecordSize, sizeof(newRecordSize)));
        appointmentsFile.write(recordWithLength.c_str(), recordWithLength.length());
        
        aIndex.update_index((char*)appointmentID.c_str(), stoll(newDate));

        cout << "Appointment information updated successfully." << endl;
        appointmentsFile.close();
}


void addAppointment(const string &appointmentID, const string &doctorID, const string &patientID, const string &date, const string &time)
{
    fstream appointmentFile;
    appointmentFile.open("Appointments.txt", ios::in | ios::out);  // Open file for reading and writing
    if (!appointmentFile.is_open()) {
        cout << "Failed to open file." << endl;
        return;
    }

    // Initialize file if empty
    appointmentFile.seekg(0, ios::end);
    int filesize = appointmentFile.tellg();
    if (filesize <= 0) {
        int availListOffset = -1;  // No deleted records initially
        appointmentFile.seekp(0, ios::beg);
        appointmentFile.write((char *)&availListOffset, sizeof(int));  // Write initial avail list
        appointmentFile.put('|');  // Optional separator
    }

    string record = appointmentID + "|" + doctorID + "|" + patientID + "|" + date + "|" + time;
    int recordSize = record.length();
    int firstDeletedRecord, nextDeletedRecord = -1, byteOffset = -1;
    char flag;

    // Read the head of the available list
    appointmentFile.seekg(0, ios::beg);
    appointmentFile.read((char *)&firstDeletedRecord, sizeof(int));  // Read the availList head

    if (firstDeletedRecord != -1) {  // Check if there are deleted records to reuse
        byteOffset = firstDeletedRecord;
        int prevOffset = -1;  // Track the previous deleted record offset
        int currRecordSize;
        bool foundSlot = false;

        while (byteOffset != -1 && !foundSlot) {
            appointmentFile.seekg(byteOffset, ios::beg);
            appointmentFile.get(flag);  // Read the flag
            appointmentFile.read((char *)&nextDeletedRecord, sizeof(int));  // Read next deleted record pointer
            appointmentFile.read((char *)&currRecordSize, sizeof(int));     // Read current record size

            if (flag == DELETE_FLAG && recordSize <= currRecordSize) {  // Found a suitable deleted record
                if (prevOffset == -1) {
                    // Update the availList head if reusing the first record
                    appointmentFile.seekp(0, ios::beg);
                    appointmentFile.write((char *)&nextDeletedRecord, sizeof(int));
                } else {
                    // Update the previous record's nextDeletedRecord pointer
                    appointmentFile.seekp(prevOffset + 1, ios::beg);  // +1 to skip flag
                    appointmentFile.write((char *)&nextDeletedRecord, sizeof(int));
                }

                // Mark the reused record as active
                appointmentFile.seekp(byteOffset, ios::beg);
                appointmentFile.put(EMPTY_FLAG);  // Mark record as active
                int jj = -1;
                appointmentFile.write((char *)&jj, sizeof(int));  // Clear nextDeletedRecord

                // Write the new record data
                appointmentFile.write((char *)&currRecordSize, sizeof(int));
                appointmentFile.write(record.c_str(), recordSize);

                // Pad remaining space with spaces
                for (int i = 0; i < currRecordSize - recordSize; i++) {
                    appointmentFile.put(' ');
                }

                foundSlot = true;
            } else {
                // Move to the next deleted record
                prevOffset = byteOffset;
                byteOffset = nextDeletedRecord;
            }
        }

        if (foundSlot) {
            appointmentFile.close();
            return;
        }
    }

    // If no deleted records were suitable, append the record to the end of the file
    appointmentFile.seekp(0, ios::end);
    appointmentFile.put(EMPTY_FLAG);  // Mark new record as active
    int jj = -1;
    appointmentFile.write((char *)&jj, sizeof(int));  // No nextDeletedRecord
    appointmentFile.write((char *)&recordSize, sizeof(int));  // Write record size
    appointmentFile.write(record.c_str(), recordSize);  // Write record data

    appointmentFile.close();
}
void addDoctor(const string &doctorID, const string &name, const string &address)
{
    fstream doctorFile;
    doctorFile.open("Doctors.txt", ios::in | ios::out);  // Open file for reading and writing
    if (!doctorFile.is_open()) {
        cout << "Failed to open file." << endl;
        return;
    }

    // Initialize file if empty
    doctorFile.seekg(0, ios::end);
    int filesize = doctorFile.tellg();
    if (filesize <= 0) {
        int availListOffset = -1;  // No deleted records initially
        doctorFile.seekp(0, ios::beg);
        doctorFile.write((char *)&availListOffset, sizeof(int));  // Write initial avail list
        doctorFile.put('|');  // Optional separator
    }

    string record = doctorID + "|" + name + "|" + address;
    int recordSize = record.length();
    int firstDeletedRecord, nextDeletedRecord = -1, byteOffset = -1;
    char flag;

    // Read the head of the available list
    doctorFile.seekg(0, ios::beg);
    doctorFile.read((char *)&firstDeletedRecord, sizeof(int));  // Read the availList head

    if (firstDeletedRecord != -1) {  // Check if there are deleted records to reuse
        byteOffset = firstDeletedRecord;
        int prevOffset = -1;  // Track the previous deleted record offset
        int currRecordSize;
        bool foundSlot = false;

        while (byteOffset != -1 && !foundSlot) {
            doctorFile.seekg(byteOffset, ios::beg);
            doctorFile.get(flag);  // Read the flag
            doctorFile.read((char *)&nextDeletedRecord, sizeof(int));  // Read next deleted record pointer
            doctorFile.read((char *)&currRecordSize, sizeof(int));     // Read current record size

            if (flag == DELETE_FLAG && recordSize <= currRecordSize) {  // Found a suitable deleted record
                if (prevOffset == -1) {
                    // Update the availList head if reusing the first record
                    doctorFile.seekp(0, ios::beg);
                    doctorFile.write((char *)&nextDeletedRecord, sizeof(int));
                } else {
                    // Update the previous record's nextDeletedRecord pointer
                    doctorFile.seekp(prevOffset + 1, ios::beg);  // +1 to skip flag
                    doctorFile.write((char *)&nextDeletedRecord, sizeof(int));
                }

                // Mark the reused record as active
                doctorFile.seekp(byteOffset, ios::beg);
                doctorFile.put(EMPTY_FLAG);  // Mark record as active
                int jj = -1;
                doctorFile.write((char *)&jj, sizeof(int));  // Clear nextDeletedRecord

                // Write the new record data
                doctorFile.write((char *)&currRecordSize, sizeof(int));
                doctorFile.write(record.c_str(), recordSize);

                // Pad remaining space with spaces
                for (int i = 0; i < currRecordSize - recordSize; i++) {
                    doctorFile.put(' ');
                }

                foundSlot = true;
            } else {
                // Move to the next deleted record
                prevOffset = byteOffset;
                byteOffset = nextDeletedRecord;
            }
        }

        if (foundSlot) {
            doctorFile.close();
            return;
        }
    }

    // If no deleted records were suitable, append the record to the end of the file
    doctorFile.seekp(0, ios::end);
    doctorFile.put(EMPTY_FLAG);  // Mark new record as active
    int jj = -1;
    doctorFile.write((char *)&jj, sizeof(int));  // No nextDeletedRecord
    doctorFile.write((char *)&recordSize, sizeof(int));  // Write record size
    doctorFile.write(record.c_str(), recordSize);  // Write record data

    doctorFile.close();
}
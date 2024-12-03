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

void HealthcareSystem::addDoctor(const string &doctorID, const string &name, const string &address) {
    if (dIndex.getByteOffset((char*)doctorID.c_str()) != -1) { 
        cout << "Doctor with ID " << doctorID << " already exists!" << endl;
        return;
    }

    string record = doctorID + "|" + name + "|" + address;
    int recordLength = record.length();
    string recordWithLength = to_string(recordLength + 2) + "|" + record + "\n";

    fstream doctorsFile("../data/Doctors.txt", ios::app);
    if (!doctorsFile) {
        cerr << "Error: Could not open Doctors file.\n";
        return;
    }

    doctorsFile << recordWithLength;
    doctorsFile.close();

    long long byteOffset = doctorsFile.tellp();
    updatePrimaryIndex("../data/PrimaryIndexDoctors.txt", doctorID.c_str(), byteOffset); 
    updateSecondaryIndex("../data/SecondaryIndexDoctors.txt", name.c_str(), doctorID.c_str()); 
}

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

void HealthcareSystem::addAppointment(const std::string& appointmentID, const std::string& date, const std::string& doctorID) {    if (aIndex.getByteOffset((char*)appointmentID.c_str()) != -1) {
        cout << "Appointment with ID " << appointmentID << " already exists!" << endl;
        return;
    }
    string record = appointmentID + "|" + date + "|" + doctorID;
    int recordLength = record.length();
    string recordWithLength = to_string(recordLength + 2) + "|" + record + "\n";

    fstream appointmentsFile("../data/Appointments.txt", ios::app);
    if (!appointmentsFile) {
        cerr << "Error: Could not open Appointments file.\n";
        return;
    }

    appointmentsFile << recordWithLength;
    long long byteOffset = appointmentsFile.tellp() - recordWithLength.length();
    appointmentsFile.close();

    updatePrimaryIndex("../data/PrimaryIndexAppointments.txt", appointmentID.c_str(), byteOffset);
    updateSecondaryIndex("../data/SecondaryIndexAppointments.txt", date.c_str(), appointmentID.c_str());

    cout << "Appointment added successfully." << endl;
}

void HealthcareSystem::deleteAppointment(const string &appointmentID) {
    fstream file("../data/Appointments.txt", ios::in | ios::out | ios::binary);

    short M = -1; // Marker for end of the Avail List
    long long offset = aIndex.getByteOffset((char*)appointmentID.c_str());

    if (offset == -1) {
        cout << "Appointment not found!" << endl;
        return;
    }

    short recordSize;
    file.seekg(offset, ios::beg);
    file.read((char*)&recordSize, sizeof(recordSize));

    file.seekp(offset, ios::beg);
    file.put('*'); // Mark as deleted

    short availListHead;
    file.seekg(0, ios::beg);
    file.read((char*)&availListHead, sizeof(availListHead));

    file.seekp(offset + 1, ios::beg);
    file.write((char*)&availListHead, sizeof(availListHead));

    file.seekp(0, ios::beg);
    file.write((char*)&offset, sizeof(offset));

    aIndex.deleteID((char*)appointmentID.c_str());

    cout << "Appointment successfully deleted." << endl;
    file.close();
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
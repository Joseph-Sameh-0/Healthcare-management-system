#include "HealthcareSystem.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <regex>
#define DELETE_FLAG '*'
#define EMPTY_FLAG ' '

void HealthcareSystem::addAppointment(const string &appointmentID, const string &doctorID, const string &date)
{
    if (aIndex.getByteOffset(appointmentID.c_str()) != -1)
    {
        cout << "Appointment ID already exists." << endl;
        return;
    }
    // make sure that the doctor exists
    if (dIndex.getByteOffset(doctorID.c_str()) == -1)
    {
        cout << "Error: Doctor with ID " << doctorID << " does not exist." << endl;
        return;
    }

    fstream appointmentFile;
    appointmentFile.open("../data/Appointments.txt", ios::in | ios::out); // Open file for reading and writing
    if (!appointmentFile.is_open())
    {
        cout << "Failed to open file." << endl;
        return;
    }

    // Initialize file if empty
    appointmentFile.seekg(0, ios::end);
    int filesize = appointmentFile.tellg();
    if (filesize <= 0)
    {
        int availListOffset = -1; // No deleted records initially
        appointmentFile.seekp(0, ios::beg);
        appointmentFile.write((char *)&availListOffset, sizeof(int)); // Write initial avail list
        appointmentFile.put('|');                                     // Optional separator
    }

    string record = appointmentID + "|" + date + "|" + doctorID + "\n\n";
    int recordSize = record.length();
    int firstDeletedRecord, nextDeletedRecord = -1, byteOffset = -1;
    char flag;

    // Read the head of the available list
    appointmentFile.seekg(0, ios::beg);
    appointmentFile.read((char *)&firstDeletedRecord, sizeof(int)); // Read the availList head

    if (firstDeletedRecord != -1)
    { // Check if there are deleted records to reuse
        byteOffset = firstDeletedRecord;
        int prevOffset = -1; // Track the previous deleted record offset
        int currRecordSize;
        bool foundSlot = false;

        while (byteOffset != -1 && !foundSlot)
        {
            appointmentFile.seekg(byteOffset, ios::beg);
            appointmentFile.get(flag);                                     // Read the flag
            appointmentFile.read((char *)&nextDeletedRecord, sizeof(int)); // Read next deleted record pointer
            appointmentFile.read((char *)&currRecordSize, sizeof(int));    // Read current record size

            if (flag == DELETE_FLAG && recordSize <= currRecordSize)
            { // Found a suitable deleted record
                if (prevOffset == -1)
                {
                    // Update the availList head if reusing the first record
                    appointmentFile.seekp(0, ios::beg);
                    appointmentFile.write((char *)&nextDeletedRecord, sizeof(int));
                }
                else
                {
                    // Update the previous record's nextDeletedRecord pointer
                    appointmentFile.seekp(prevOffset + 1, ios::beg); // +1 to skip flag
                    appointmentFile.write((char *)&nextDeletedRecord, sizeof(int));
                }

                // Mark the reused record as active
                appointmentFile.seekp(byteOffset, ios::beg);

                aIndex.add(appointmentID.c_str(), byteOffset);
                aSIndex.add(doctorID.c_str(), appointmentID.c_str());

                appointmentFile.put(EMPTY_FLAG); // Mark record as active
                int jj = -1;
                appointmentFile.write((char *)&jj, sizeof(int)); // Clear nextDeletedRecord

                // Write the new record data
                appointmentFile.write((char *)&currRecordSize, sizeof(int));
                appointmentFile.write(record.c_str(), recordSize);

                // Pad remaining space with spaces
                for (int i = 0; i < currRecordSize - recordSize; i++)
                {
                    appointmentFile.put(' ');
                }

                foundSlot = true;
            }
            else
            {
                // Move to the next deleted record
                prevOffset = byteOffset;
                byteOffset = nextDeletedRecord;
            }
        }

        if (foundSlot)
        {
            appointmentFile.close();
            return;
        }
    }

    // If no deleted records were suitable, append the record to the end of the file
    appointmentFile.seekp(0, ios::end);
    aIndex.add(appointmentID.c_str(), appointmentFile.tellp());
    aSIndex.add(doctorID.c_str(), appointmentID.c_str());

    appointmentFile.put(EMPTY_FLAG); // Mark new record as active
    int jj = -1;
    appointmentFile.write((char *)&jj, sizeof(int));         // No nextDeletedRecord
    appointmentFile.write((char *)&recordSize, sizeof(int)); // Write record size
    appointmentFile.write(record.c_str(), recordSize);       // Write record data

    appointmentFile.close();
}

void HealthcareSystem::addDoctor(const string &doctorID, const string &name, const string &address)
{
    if (dIndex.getByteOffset(doctorID.c_str()) != -1)
    {
        cout << "Error: Doctor with ID " << doctorID << " already exists." << endl;
        return;
    }

    fstream doctorFile;
    doctorFile.open("../data/Doctors.txt", ios::in | ios::out); // Open file for reading and writing
    if (!doctorFile.is_open())
    {
        cout << "Failed to open file." << endl;
        return;
    }

    // Initialize file if empty
    doctorFile.seekg(0, ios::end);
    int filesize = doctorFile.tellg();
    if (filesize <= 0)
    {
        int availListOffset = -1; // No deleted records initially
        doctorFile.seekp(0, ios::beg);
        doctorFile.write((char *)&availListOffset, sizeof(int)); // Write initial avail list
        doctorFile.put('|');                                     // Optional separator
    }

    string record = doctorID + "|" + name + "|" + address + "\n\n";
    int recordSize = record.length();
    int firstDeletedRecord, nextDeletedRecord = -1, byteOffset = -1;
    char flag;

    // Read the head of the available list
    doctorFile.seekg(0, ios::beg);
    doctorFile.read((char *)&firstDeletedRecord, sizeof(int)); // Read the availList head

    if (firstDeletedRecord != -1)
    { // Check if there are deleted records to reuse
        byteOffset = firstDeletedRecord;
        int prevOffset = -1; // Track the previous deleted record offset
        int currRecordSize;
        bool foundSlot = false;

        while (byteOffset != -1 && !foundSlot)
        {
            doctorFile.seekg(byteOffset, ios::beg);
            doctorFile.get(flag);                                     // Read the flag
            doctorFile.read((char *)&nextDeletedRecord, sizeof(int)); // Read next deleted record pointer
            doctorFile.read((char *)&currRecordSize, sizeof(int));    // Read current record size

            if (flag == DELETE_FLAG && recordSize <= currRecordSize)
            { // Found a suitable deleted record
                if (prevOffset == -1)
                {
                    // Update the availList head if reusing the first record
                    doctorFile.seekp(0, ios::beg);
                    doctorFile.write((char *)&nextDeletedRecord, sizeof(int));
                }
                else
                {
                    // Update the previous record's nextDeletedRecord pointer
                    doctorFile.seekp(prevOffset + 1, ios::beg); // +1 to skip flag
                    doctorFile.write((char *)&nextDeletedRecord, sizeof(int));
                }

                // Mark the reused record as active
                doctorFile.seekp(byteOffset, ios::beg);

                dIndex.add(doctorID.c_str(), byteOffset);
                dSIndex.add(name.c_str(), doctorID.c_str());

                doctorFile.put(EMPTY_FLAG); // Mark record as active
                int jj = -1;
                doctorFile.write((char *)&jj, sizeof(int)); // Clear nextDeletedRecord

                // Write the new record data
                doctorFile.write((char *)&currRecordSize, sizeof(int));
                doctorFile.write(record.c_str(), recordSize);

                // Pad remaining space with spaces
                for (int i = 0; i < currRecordSize - recordSize; i++)
                {
                    doctorFile.put(' ');
                }

                foundSlot = true;
            }
            else
            {
                // Move to the next deleted record
                prevOffset = byteOffset;
                byteOffset = nextDeletedRecord;
            }
        }

        if (foundSlot)
        {
            doctorFile.close();
            return;
        }
    }

    // If no deleted records were suitable, append the record to the end of the file
    doctorFile.seekp(0, ios::end);
    dIndex.add(doctorID.c_str(), doctorFile.tellp());
    dSIndex.add(name.c_str(), doctorID.c_str());
    doctorFile.put(EMPTY_FLAG); // Mark new record as active
    int jj = -1;
    doctorFile.write((char *)&jj, sizeof(int));         // No nextDeletedRecord
    doctorFile.write((char *)&recordSize, sizeof(int)); // Write record size
    doctorFile.write(record.c_str(), recordSize);       // Write record data

    doctorFile.close();
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

    const int recSize = recordSize;
    char record[recSize];
    AppointmentFile.read((char *)&record, recordSize);
    stringstream s(record);
    string docId;
    getline(s, docId, '|');
    getline(s, docId, '|');
    getline(s, docId, '\n');
    // cout << docId << endl;                             // Print the name of the doctor being deleted
    aIndex.deleteID(AppointmentID.c_str());                 // Remove the appointment from the primary index
    aSIndex.deleteId(docId.c_str(), AppointmentID.c_str()); // Delete the appointment from the doctor index

    // Mark the record as deleted by writing the DELETE_FLAG ('*')
    AppointmentFile.seekp(byteOffset, ios::beg);
    AppointmentFile.put(DELETE_FLAG);

    // Update the deleted record to point to the current head of the availability list
    AppointmentFile.write((char *)&firstDeletedRecord, sizeof(int));

    // Update the availability list head to this record's byte offset
    AppointmentFile.seekp(0, ios::beg);
    AppointmentFile.write((char *)&byteOffset, sizeof(int));

    AppointmentFile.close();
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

    const int recSize = recordSize;
    char record[recSize];
    doctorFile.read(record, recSize);
    stringstream s(record);
    string dName;
    getline(s, dName, '|');
    getline(s, dName, '|');
    // cout << dName << endl; // Print the name of the doctor being deleted
    /////////////////////////////////////////////////////////////////////////
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

void HealthcareSystem::updateDoctor(const string &id, const string &newName)
{
    fstream doctorsFile("../data/Doctors.txt", ios::in | ios::out | ios::binary);

    long long byteOffset = dIndex.getByteOffset((char *)id.c_str());
    if (byteOffset == -1)
    {
        cout << "Doctor not found!" << endl;
        return;
    }

    // Read the current record size
    int recordSize;
    doctorsFile.seekg(byteOffset + sizeof(char) + sizeof(int), ios::beg);
    doctorsFile.read((char *)&recordSize, sizeof(int));

    const int recSize = recordSize;
    char oldRecord[recSize];
    doctorsFile.read(oldRecord, recSize);
    stringstream s(oldRecord);
    
    string oldName;
    string oldAddress;
    getline(s, oldName, '|');
    
    getline(s, oldName, '|');
    
    getline(s, oldAddress, '\n');
    

    // Create the new record
    string record = id + "|" + newName + "|" + oldAddress + "\n";
    short newRecordSize = record.length();

    if (newRecordSize <= recordSize)
    {
        // Update in place if new record fits
        doctorsFile.seekp(byteOffset, ios::beg);

        doctorsFile.put(EMPTY_FLAG);
        int jj = -1;
        doctorsFile.write((char *)&jj, sizeof(int)); // Clear nextDeletedRecord
        // Write the new record data
        doctorsFile.write((char *)&recordSize, sizeof(int));
        doctorsFile.write(record.c_str(), recordSize);
        doctorsFile.close();

    }
    else
    {
        // Delete old record and add new one at the end
        deleteDoctor(id);
        addDoctor(id, newName, oldAddress);
        cout << "Doctor information updated successfully." << endl;
        return;
    }

    // Update indexes
    dIndex.deleteID((char *)id.c_str());
    dIndex.add((char *)id.c_str(), byteOffset);
    dSIndex.editKey(oldName.c_str(), newName.c_str(), id.c_str());
}

void HealthcareSystem::updateAppointment(const string &id, const string &newDoctorID, const string &newDate)
{
    fstream appointmentsFile("../data/Appointments.txt", ios::in | ios::out | ios::binary);

    // Get the byte offset of the appointment record using the index
    long long byteOffset = aIndex.getByteOffset((char *)id.c_str());
    if (byteOffset == -1)
    {
        cout << "Appointment not found!" << endl;
        return;
    }

    // Read the current record size
    int recordSize;
    appointmentsFile.seekg(byteOffset + sizeof(char) + sizeof(int), ios::beg);
    appointmentsFile.read((char *)&recordSize, sizeof(int));

    // Read the current record
    const int recSize = recordSize;
    char oldRecord[recSize];
    appointmentsFile.read(oldRecord, recSize);

    // Extract the old details
    stringstream s(oldRecord);
    string oldDoctorID;
    getline(s, oldDoctorID, '|'); // Skip appointment ID
    getline(s, oldDoctorID, '|');
    getline(s, oldDoctorID, '\n');

    // Create the new record
    string record = id + "|" + newDate + "|" + newDoctorID + "\n";
    short newRecordSize = record.length();

    if (newRecordSize <= recordSize)
    {
        // Update in place if new record fits
        appointmentsFile.seekp(byteOffset, ios::beg);

        appointmentsFile.put(EMPTY_FLAG);
        int jj = -1;
        appointmentsFile.write((char *)&jj, sizeof(int)); // Clear nextDeletedRecord
        // Write the new record data
        appointmentsFile.write((char *)&recordSize, sizeof(int));
        appointmentsFile.write(record.c_str(), recordSize);
        appointmentsFile.close();
    }
    else
    {
        // Delete old record and add a new one at the end
        deleteAppointment(id);
        addAppointment(id, newDoctorID, newDate);
        cout << "Appointment information updated successfully." << endl;
        return;
    }

    // Update indexes
    aIndex.deleteID((char *)id.c_str());
    aIndex.add((char *)id.c_str(), byteOffset);
    aSIndex.editKey(oldDoctorID.c_str(), newDoctorID.c_str(), id.c_str());
}

// query handling
void HealthcareSystem::parseQuery(const string &query)
{
    Query parsedQuery;

    // Extract target using regex
    regex targetRegex("Select\\s+(.*)\\s+from");
    smatch targetMatch;
    if (regex_search(query, targetMatch, targetRegex))
    {
        parsedQuery.target = targetMatch[1].str();
    }

    // Extract source using regex
    regex sourceRegex("from\\s+(\\w+)");
    smatch sourceMatch;
    if (regex_search(query, sourceMatch, sourceRegex))
    {
        parsedQuery.source = sourceMatch[1].str();
    }

    // Extract condition using regex
    regex conditionRegex("where\\s+(\\w+)\\s*(=|!=|>|<|>=|<=)\\s*'([^']*)'");
    smatch conditionMatch;

    if (regex_search(query, conditionMatch, conditionRegex))
    {
        parsedQuery.condition[0] = conditionMatch[0];
        parsedQuery.condition[1] = conditionMatch[1];
        parsedQuery.condition[2] = conditionMatch[2];
        parsedQuery.condition[3] = conditionMatch[3];
    }

    executeQuery(parsedQuery);
}

void HealthcareSystem::executeQuery(Query query)
{
    if (query.source == "Doctors")
    {
        ifstream file;
        file.open("../data/Doctors.txt");
        if (query.condition[1] == "DoctorID")
        {
            long long byteOffSet = dIndex.getByteOffset(query.condition[3].c_str());
            if (byteOffSet == -1)
            {
                cout << "Doctor is not found" << endl;
                return;
            }
            getDoctorData(file, query, byteOffSet);
        }
        else if (query.condition[1] == "DoctorName")
        {
            vector<string> doctorIDs = dSIndex.getIds(query.condition[3].c_str());
            if (doctorIDs.empty())
            {
                cout << "doctor is not found" << endl;
                return;
            }
            for (int i = 0; i < doctorIDs.size(); i++)
            {
                long long byteOffSet = dIndex.getByteOffset(doctorIDs[i].c_str());
                getDoctorData(file, query, byteOffSet);

            }
        }
        else{
            vector<long long> byteOffSets = dIndex.getAllOffset();
            if (byteOffSets.empty())
            {
                cout << "No Doctors" << endl;
                return;
            }
            for (int i = 0; i < byteOffSets.size(); i++)
            {
                getDoctorData(file, query, byteOffSets[i]);

            }
        }
        file.close();
    }
    else if (query.source == "Appointments")
    {
        ifstream file;
        file.open("../data/Appointments.txt");
        if (query.condition[1] == "AppointmentID")
        {
            long long byteOffSet = aIndex.getByteOffset(query.condition[3].c_str());
            if (byteOffSet == -1)
            {
                cout << "Appointment is not found" << endl;
                return;
            }
            getAppointmentData(file, query, byteOffSet);
        }
        else if (query.condition[1] == "DoctorID")
        {
            vector<string> appointmentIDs = aSIndex.getIds(query.condition[3].c_str());
            if (appointmentIDs.empty())
            {
                cout << "doctor is not found" << endl;
                return;
            }
            for (int i = 0; i < appointmentIDs.size(); i++)
            {
                long long byteOffSet = aIndex.getByteOffset(appointmentIDs[i].c_str());
                getAppointmentData(file, query, byteOffSet);
            }
        }
        else{
            vector<long long> byteOffSets = aIndex.getAllOffset();
            if (byteOffSets.empty())
            {
                cout << "No Appointments" << endl;
                return;
            }
            for (int i = 0; i < byteOffSets.size(); i++)
            {
                getAppointmentData(file, query, byteOffSets[i]);

            }
        }
        file.close();
    }
}

void HealthcareSystem::getDoctorData(ifstream& file, Query query, long long byteOffSet)
{
    file.seekg(byteOffSet + sizeof(char), ios::beg);
    int recordSize;
    file.read((char *)&recordSize, sizeof(int));
    file.read((char *)&recordSize, sizeof(int));
    const int size = recordSize;
    char l[size];
    file.read(l, recordSize);
    stringstream newline(l);
    string line;
    getline(newline, line, '\n');
    if (query.target == "all")
    {
        cout << line << endl;
    }
    else if (query.target == "DoctorID")
    {
        istringstream iss(line);
        string field;
        getline(iss, field, '|');
        cout << field << endl;
    }
    else if (query.target == "DoctorName")
    {
        istringstream iss(line);
        string field;
        getline(iss, field, '|');
        getline(iss, field, '|');
        cout << field << endl;
    }
    else if (query.target == "DoctorAddress")
    {
        istringstream iss(line);
        string field;
        getline(iss, field, '|');
        getline(iss, field, '|');
        getline(iss, field, '\n');
        cout << field << endl;
    }
}

void HealthcareSystem::getAppointmentData(ifstream& file, Query query, long long byteOffSet)
{
    file.seekg(byteOffSet + sizeof(char), ios::beg);
    int recordSize;
    file.read((char *)&recordSize, sizeof(int));
    file.read((char *)&recordSize, sizeof(int));
    const int size = recordSize;
    char l[size];
    file.read(l, recordSize);
    stringstream newline(l);
    string line;
    getline(newline, line, '\n');
    if (query.target == "all")
    {
        cout << line << endl;
    }
    else if (query.target == "AppointmentID")
    {
        istringstream iss(line);
        string field;
        getline(iss, field, '|');
        cout << field << endl;
    }
    else if (query.target == "AppointmentDate")
    {
        istringstream iss(line);
        string field;
        getline(iss, field, '|');
        getline(iss, field, '|');
        cout << field << endl;
    }
    else if (query.target == "DoctorID")
    {
        istringstream iss(line);
        string field;
        getline(iss, field, '|');
        getline(iss, field, '|');
        getline(iss, field, '\n');
        cout << field << endl;
    }
}
void HealthcareSystem::printInfoAsQuery(const string& id, const string& type) {
    string query;

    if (type == "doctor") {
        query = "Select all from Doctors where Doctor ID='" + id + "';";
    } else if (type == "appointment") {
        query = "Select all from Appointments where Appointment ID='" + id + "';";
    } else {
        cerr << "Error: Unknown type '" << type << "'!" << endl;
        return;
    }

    return parseQuery(query);
}


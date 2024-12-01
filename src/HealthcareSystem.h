#ifndef HEALTHCARESYSTEM_H
#define HEALTHCARESYSTEM_H

#include <string>
using namespace std;

class HealthcareSystem {
public:
    void addDoctor(const string &doctorID, const string &name, const string &address);
    void addAppointment(const string &appointmentID, const string &date, const string &doctorID);
    string searchDoctor(const string doctorID);

    // Other methods: deleteDoctor, deleteAppointment, etc.

private:
    void updatePrimaryIndex(const string &file, const string &key, const long long byteOffset);
    void updateSecondaryIndex(const string &file, const string &secondaryKey, const string &primaryKey);
};

#endif

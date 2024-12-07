#include <iostream>
#include <bits/stdc++.h>
#include "HealthcareSystem.cpp"

using namespace std;
struct Doctor {
    string id;
    string name;
    string address;
};
Doctor getDoctorInfoFromUser() {
    Doctor doc;

    cout << "Enter Doctor ID (max 14 characters): ";
    cin >> doc.id;
    if (doc.id.length() >= 15) {
        cerr << "Error: Doctor ID must be 14 characters or less!" << endl;
        return doc;
    }

    cin.ignore(); // Clear buffer.
    cout << "Enter Doctor Name (max 29 characters): ";
    getline(cin, doc.name);
    if (doc.name.length() >= 30) {
        cerr << "Error: Doctor Name must be 29 characters or less!" << endl;
        return doc;
    }

    cout << "Enter Doctor Address (max 29 characters): ";
    getline(cin, doc.address);
    if (doc.address.length() >= 30) {
        cerr << "Error: Address must be 29 characters or less!" << endl;
        return doc;
    }

    return doc;
}
struct Appointment {
    string id;         // Appointment ID 
    string date;       // Appointment Date
    string doctorId;   // Doctor ID (secondary key)
};

Appointment getAppointmentInfoFromUser() {
    Appointment appt;

    // Validate Appointment ID
    cout << "Enter Appointment ID (max 14 characters): ";
    cin >> appt.id;
    if (appt.id.length() >= 15) {
        cerr << "Error: Appointment ID must be 14 characters or less!" << endl;
        return appt;
    }

    cin.ignore(); // Clear input buffer

    // Validate Appointment Date
    cout << "Enter Appointment Date (max 29 characters): ";
    getline(cin, appt.date);
    if (appt.date.length() >= 30) {
        cerr << "Error: Appointment Date must be 29 characters or less!" << endl;
        return appt;
    }

    // Validate Doctor ID
    cout << "Enter Doctor ID (max 14 characters): ";
    cin >> appt.doctorId;
    if (appt.doctorId.length() >= 15) {
        cerr << "Error: Doctor ID must be 14 characters or less!" << endl;
        return appt;
    }

    return appt;
}

int main()
{
    HealthcareSystem sys;
    int choice;
    char userQuery[1000];
    Doctor doc;
    Appointment appt;
    do
    {
        cout << "Welcome to Hospital Management System" << endl;
        cout << "1. Add New Doctor" << endl;
        cout << "2. Add New Appointment" << endl;
        cout << "3. Update Doctor Name (Doctor ID)" << endl;
        cout << "4. Update Appointment Date (Appointment ID)" << endl;
        cout << "5. Delete Appointment (Appointment ID)" << endl;
        cout << "6. Delete Doctor (Doctor ID)" << endl;
        cout << "7. Print Doctor Info (Doctor ID)" << endl;
        cout << "8. Print Appointment Info (Appointment ID)" << endl;
        cout << "9. Write Query" << endl;
        cout << "10. Exit" << endl;

        cout << "Enter the number of your choice your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            // Add new doctor
            doc = getDoctorInfoFromUser();
            sys.addDoctor(doc.id, doc.name, doc.address);
            break;
        case 2:
            // Add new appointment
            appt = getAppointmentInfoFromUser();
            sys.addAppointment(appt.id, appt.doctorId, appt.date);
            break;
        case 3:
            // Update doctor name
            cout << "Enter Doctor ID: ";
            cin.ignore();
            getline(cin, doc.id);
            cout << "Enter New Doctor Name: ";
            getline(cin, doc.name);
            sys.updateDoctor(doc.id, doc.name);
            break;
        case 4:
            // Update appointment date
            cout << "Enter Appointment ID: ";
            cin.ignore();
            getline(cin, appt.id);
            cout << "Enter New Appointment Date: ";
            getline(cin, appt.date);
            sys.updateAppointment(appt.id, appt.date);
            break;
        case 5:
            // Delete appointment
            cout << "Enter Appointment ID: ";
            cin.ignore();
            getline(cin, appt.id);
            sys.deleteAppointment(appt.id);
            break;
        case 6:
            // Delete Doctor
            cout << "Enter Doctor ID: ";
            cin.ignore();
            getline(cin, doc.id);
            sys.deleteDoctor(doc.id);
            break;
        case 7:
            // Print doctor info
            cout << "Enter Doctor ID: ";
            cin.ignore();
            getline(cin, doc.id);
            sys.printInfoAsQuery(doc.id, "doctor");
            break;
        case 8:
            // Print appointment info
            cout << "Enter Appointment ID: ";
            cin.ignore();
            getline(cin, appt.id);
            sys.printInfoAsQuery(appt.id, "appointment");
            break;
        case 9:
            cout << "Enter your query: ";
            cin.ignore();
            cin.getline(userQuery, 1000);
            sys.parseQuery(userQuery);
            break;
        case 10:
            cout << "Exiting the program..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 10);

    return 0;
}

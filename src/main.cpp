#include <iostream>
#include "HealthcareSystem.h"
using namespace std;

int main() {
    HealthcareSystem system;

    int choice;
    do {
        cout << "\n--- Healthcare Management System ---\n";
        cout << "1. Add Doctor\n2. Add Appointment\n3. Delete Doctor\n4. Delete Appointment\n";
        cout << "5. Update Doctor Name\n6. Update Appointment Date\n7. Print Doctor Info\n";
        cout << "8. Print Appointment Info\n9. Execute Query\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string id, name, address;
                cout << "Enter Doctor ID: "; cin >> id;
                cout << "Enter Doctor Name: "; cin.ignore(); getline(cin, name);
                cout << "Enter Address: "; getline(cin, address);
                system.addDoctor(id, name, address);
                break;
            }
            case 2: {
                string id, date, doctorId;
                cout << "Enter Appointment ID: "; cin >> id;
                cout << "Enter Appointment Date: "; cin.ignore(); getline(cin, date);
                cout << "Enter Doctor ID: "; cin >> doctorId;
                system.addAppointment(id, date, doctorId);
                break;
            }
            case 0:
                cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}

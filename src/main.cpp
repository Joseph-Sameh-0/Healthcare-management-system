#include <iostream>
#include <bits/stdc++.h>
#include "HealthcareSystem.cpp"

using namespace std;

int main()
{
    HealthcareSystem sys;
    int choice;
    char userQuery[1000];
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
            sys.addDoctor("1111", "aaaaaa", "aaaaa");
            sys.addDoctor("2", "b", "bb");
            sys.addDoctor("cc", "cccc", "c");
            break;
        case 2:
            // Add new appointment
            sys.addAppointment("1","2","2-2-2025");
            sys.addAppointment("2","1111","1-3-2025");
            sys.addAppointment("3","2","3-8-2024");

            break;
        case 3:
            // Update doctor name
            sys.updateDoctor("1111", "dddddd", "ddddd");
            break;
        case 4:
            // Update appointment date
            break;
        case 5:
            // Delete appointment
            break;
        case 6:
            // Delete Doctor
            sys.deleteDoctor("cc");
            sys.deleteDoctor("2");
            sys.deleteDoctor("1111");
            break;
        case 7:
            // Print doctor info
            break;
        case 8:
            // Print appointment info
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

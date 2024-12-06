#include <iostream>
#include <bits/stdc++.h>
#include "HealthcareSystem.cpp"
using namespace std;

int main()
{
    int choice;
    HealthcareSystem sys;
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
            // sys.addDoctor("1", "jox", "alf maskn");
            // sys.addDoctor("2", "joseph", "alf maskn");
            // sys.addDoctor("3", "gg", "rr");
            sys.printAppointment(5);
            // sys.addDoctor("3", "hegazy", "alf maskn");
            // sys.addDoctor("2", "h", "alf ");
            // sys.printDoctor(31);
            // sys.printDoctor(60);
            
            // for (PrimaryIndexRow r:sys.dIndex.getVector()){
            //     cout << r.byteOffset<<endl;
            // }
            // sys.addDoctor("3", "hegazy", "alf maskn");
            // sys.printDoctor(sizeof(int) + sizeof(char));
            // sys.printDoctor(sizeof(int) + sizeof(char)+16);

            break;
        case 2:
            // sys.addAppointment("111111" , "11-12-2024" , "111111");
            // sys.addAppointment("33333333" , "1-12-2024" , "33333");
            sys.addAppointment("2" , "1-2-2024" , "2");
            break;
        case 3:
            // Update doctor name
            break;
        case 4:
            // Update appointment date
            break;
        case 5:
            sys.deleteAppointment("111111");
            sys.deleteAppointment("2");
            break;
        case 6:
            sys.deleteDoctor("2");
            sys.deleteDoctor("3");
            break;
        case 7:
            // Print doctor info
            break;
        case 8:
            // Print appointment info
            break;
        case 9:
            // Write query
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
cmake_minimum_required(VERSION 3.23)
project(Healthcare_management_system)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(Healthcare_management_system
        .idea/.gitignore
        .idea/Healthcare-management-system.iml
        .idea/modules.xml
        .idea/vcs.xml
        .idea/workspace.xml
        .vscode/c_cpp_properties.json
        .vscode/settings.json
        .vscode/tasks.json
        bin/HealthcareManagementSystem.exe
        data/Appointments.txt
        data/Doctors.txt
        data/PrimaryIndexAppointments.txt
        data/PrimaryIndexDoctors.txt
        data/SecondaryIndexAppointments.txt
        data/SecondaryIndexDoctors.txt
        docs/AssignmentRequirements.pdf
        docs/ReadMe.txt
        src/HealthcareSystem.cpp
        src/HealthcareSystem.h
        src/main.cpp
        src/main.exe
        src/utils.cpp
        src/utils.h
        tests/results.txt
        tests/test_cases.txt
        .gitattributes
        .gitignore)

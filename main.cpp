// =====================================================================
//  main.cpp  —  Hospital Emergency Queue System
//  Author    : DSA Project
//  Language  : C++17
//
//  DATA STRUCTURES USED:
//    1. Priority Queue (Binary Max-Heap) — Emergency triage queue
//    2. Stack (LIFO)                     — Doctor assignment pool
//    3. Queue (FIFO)                     — General waiting room
//    4. Doubly Linked List               — Patient treatment history
//    5. Hash Map (unordered_map)         — O(1) patient lookup by ID
//    6. Array / Vector                   — Sorting demonstrations
//
//  ALGORITHMS USED:
//    1. Heapify-Up / Heapify-Down        — Priority queue maintenance
//    2. Quick Sort                        — Sort by priority
//    3. Merge Sort                        — Sort by arrival time
//    4. Insertion Sort                    — Sort by age / name
//    5. Binary Search                     — Search in sorted array
// =====================================================================

#include <iostream>
#include <string>
#include <limits>
#include "HospitalSystem.h"
using namespace std;

// =====================================================================
//  Input Helpers
// =====================================================================
int getIntInput(const string& prompt, int min, int max) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= min && val <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "  [!] Invalid input. Enter a number between "
             << min << " and " << max << ".\n" << RESET;
    }
}

string getStringInput(const string& prompt) {
    string val;
    cout << prompt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, val);
    return val;
}

string getStringInputDirect(const string& prompt) {
    string val;
    cout << prompt;
    getline(cin, val);
    return val;
}

// =====================================================================
//  Sub-menu: Register Patient
// =====================================================================
void menuRegisterPatient(HospitalSystem& sys) {
    sys.printSectionHeader("REGISTER NEW PATIENT");
    cout << "\n";

    string name = getStringInput("  Patient Name       : ");
    int    age  = getIntInput(   "  Age (1-120)        : ", 1, 120);

    cout << "\n  Priority Levels:\n";
    cout << RED    << "    1. CRITICAL    — Life-threatening (chest pain, stroke)\n" << RESET;
    cout << YELLOW << "    2. HIGH        — Severe pain, major fracture\n"           << RESET;
    cout << CYAN   << "    3. MEDIUM      — Moderate symptoms\n"                     << RESET;
    cout << GREEN  << "    4. LOW         — Minor illness / small injury\n"          << RESET;
    cout << WHITE  << "    5. OBSERVATION — Routine / walk-in\n"                     << RESET;

    int priority = getIntInput("\n  Select Priority (1-5): ", 1, 5);
    string condition = getStringInputDirect("  Medical Condition   : ");

    int id = sys.registerPatient(name, age, priority, condition);

    cout << GREEN << "\n  ✔ Patient registered successfully!\n"
         << "    Patient ID: " << BOLD << id << RESET << "\n";
    cout << YELLOW << "    Please note this ID for future reference.\n" << RESET;
}

// =====================================================================
//  Sub-menu: Search
// =====================================================================
void menuSearch(HospitalSystem& sys) {
    sys.printSectionHeader("SEARCH PATIENT");
    cout << "\n  1. Search by Patient ID\n";
    cout << "  2. Search by Name\n";
    int choice = getIntInput("\n  Select: ", 1, 2);
    if (choice == 1) {
        int id = getIntInput("  Enter Patient ID: ", 1, 99999);
        sys.searchPatient(id);
    } else {
        string name = getStringInput("  Enter Name (partial): ");
        sys.searchByName(name);
    }
}

// =====================================================================
//  Sub-menu: Sort & View
// =====================================================================
void menuSortView(HospitalSystem& sys) {
    sys.printSectionHeader("VIEW / SORT PATIENTS");
    cout << "\n  1. Sort by Priority      (Quick Sort)\n";
    cout << "  2. Sort by Arrival Time  (Merge Sort)\n";
    cout << "  3. Sort by Age           (Insertion Sort)\n";
    cout << "  4. Sort by Name          (Insertion Sort)\n";
    int choice = getIntInput("\n  Select sort: ", 1, 4);
    sys.displaySortedPatients(choice);
}

// =====================================================================
//  MAIN MENU
// =====================================================================
void printMainMenu() {
    cout << "\n" << BOLD << "  ══════════════════════════════════════\n";
    cout << "   MAIN MENU\n";
    cout << "  ══════════════════════════════════════\n" << RESET;
    cout << "   " << CYAN  << "1." << RESET << "  Register New Patient\n";
    cout << "   " << CYAN  << "2." << RESET << "  Treat Next Emergency Patient\n";
    cout << "   " << CYAN  << "3." << RESET << "  Treat Next Waiting Room Patient\n";
    cout << "   " << CYAN  << "4." << RESET << "  Discharge Patient\n";
    cout << "   " << CYAN  << "5." << RESET << "  Upgrade Patient Priority\n";
    cout << "   " << BLUE  << "6." << RESET << "  View Emergency Queue\n";
    cout << "   " << BLUE  << "7." << RESET << "  View Waiting Room\n";
    cout << "   " << BLUE  << "8." << RESET << "  View Doctor Pool\n";
    cout << "   " << BLUE  << "9." << RESET << "  View All Patients (Sorted)\n";
    cout << "   " << BLUE  << "10." << RESET << " View Patient History Log\n";
    cout << "   " << BLUE  << "11." << RESET << " Search Patient\n";
    cout << "   " << BLUE  << "12." << RESET << " View Patient's Audit Trail\n";
    cout << "   " << GREEN << "13." << RESET << " System Statistics\n";
    cout << "   " << RED   << "0." << RESET  << "  Exit\n";
    cout << BOLD << "  ══════════════════════════════════════\n" << RESET;
}

// =====================================================================
//  DEMO — Load sample data for testing
// =====================================================================
void loadDemoData(HospitalSystem& sys) {
    sys.printSectionHeader("LOADING DEMO DATA");
    cout << "\n  Adding sample patients...\n";

    sys.registerPatient("Rahul Verma",    45, 1, "Cardiac Arrest");
    sys.registerPatient("Sunita Patel",   62, 2, "Severe Head Injury");
    sys.registerPatient("Amit Sharma",    30, 3, "Broken Arm");
    sys.registerPatient("Pooja Reddy",    25, 4, "High Fever");
    sys.registerPatient("Deepak Kumar",   55, 2, "Stroke Symptoms");
    sys.registerPatient("Meera Joshi",    38, 5, "Routine Checkup");
    sys.registerPatient("Vikas Singh",    72, 1, "Severe Breathing Issue");
    sys.registerPatient("Nandini Iyer",   29, 3, "Abdominal Pain");
    sys.registerPatient("Suresh Nair",    40, 4, "Sprained Ankle");
    sys.registerPatient("Lata Gupta",     68, 2, "High Blood Pressure");

    cout << GREEN << "  ✔ 10 sample patients loaded.\n" << RESET;
}

// =====================================================================
//  ENTRY POINT
// =====================================================================
int main() {
    // Enable ANSI colors on Windows
    #ifdef _WIN32
    system("chcp 65001 > nul");
    system("cls");
    #endif

    HospitalSystem sys;
    sys.printBanner();

    cout << "  Welcome to the Hospital Emergency Management System.\n";
    cout << "  Load demo data to get started?\n";
    cout << "  1. Yes — Load 10 sample patients\n";
    cout << "  2. No  — Start fresh\n";
    int choice = getIntInput("\n  Select: ", 1, 2);
    if (choice == 1) loadDemoData(sys);

    while (true) {
        printMainMenu();
        int option = getIntInput("\n  Enter option: ", 0, 13);

        switch (option) {
            case 0:
                cout << CYAN << "\n  Thank you for using the Hospital Emergency System.\n"
                     << "  Stay safe!\n\n" << RESET;
                return 0;

            case 1:
                menuRegisterPatient(sys);
                break;

            case 2:
                sys.treatNextEmergency();
                break;

            case 3:
                sys.treatNextWaiting();
                break;

            case 4: {
                int id = getIntInput("  Enter Patient ID to discharge: ", 1, 99999);
                sys.dischargePatient(id);
                break;
            }

            case 5: {
                int id = getIntInput("  Enter Patient ID: ", 1, 99999);
                cout << "  New Priority: 1=CRITICAL  2=HIGH  3=MEDIUM  4=LOW\n";
                int np = getIntInput("  Select: ", 1, 4);
                sys.upgradePriority(id, np);
                break;
            }

            case 6:
                sys.displayEmergencyQueue();
                break;

            case 7:
                sys.displayWaitingRoom();
                break;

            case 8:
                sys.displayDoctors();
                break;

            case 9:
                menuSortView(sys);
                break;

            case 10:
                sys.displayHistory();
                break;

            case 11:
                menuSearch(sys);
                break;

            case 12: {
                int id = getIntInput("  Enter Patient ID: ", 1, 99999);
                sys.displayPatientHistory(id);
                break;
            }

            case 13:
                sys.displayStatistics();
                break;

            default:
                cout << RED << "  Invalid option.\n" << RESET;
        }

        cout << "\n  Press ENTER to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (cin.peek() != '\n') cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

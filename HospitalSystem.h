#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include "Patient.h"
#include "PriorityQueue.h"
#include "PatientHistory.h"
#include "DoctorStack.h"
#include "WaitingRoomQueue.h"
#include "PatientHashMap.h"
#include "SortingAlgorithms.h"
using namespace std;

// =====================================================================
//  HospitalSystem.h  —  Central manager that orchestrates all DSA modules
//  Implements: Display utilities, business logic, stat reporting
// =====================================================================

// ANSI color codes for terminal
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define BOLD    "\033[1m"
#define RESET   "\033[0m"
#define WHITE   "\033[37m"

class HospitalSystem {
private:
    PriorityQueue    emergencyQueue;    // Max-Heap (by urgency)
    WaitingRoomQueue waitingRoom;       // FIFO queue
    PatientHistory   history;          // Doubly linked list
    DoctorStack      doctorPool;        // Stack of doctors
    PatientHashMap   patientDB;         // Hash map for fast lookup

    int nextPatientId;
    int totalAdmitted;

    // ---- Doctor map (id -> Doctor) for release ----
    unordered_map<int, Doctor> activeDoctors; // patients currently being treated

public:
    HospitalSystem() : nextPatientId(1001), totalAdmitted(0) {
        initializeDoctors();
    }

    // ===================================================================
    //  INITIALIZATION
    // ===================================================================
    void initializeDoctors() {
        doctorPool.addDoctor(Doctor(1, "Dr. Riya Sharma",    "Emergency Medicine"));
        doctorPool.addDoctor(Doctor(2, "Dr. Arjun Mehta",   "Cardiology"));
        doctorPool.addDoctor(Doctor(3, "Dr. Priya Nair",    "Neurology"));
        doctorPool.addDoctor(Doctor(4, "Dr. Karan Singh",   "Orthopedics"));
        doctorPool.addDoctor(Doctor(5, "Dr. Ananya Reddy",  "General Surgery"));
    }

    // ===================================================================
    //  PATIENT REGISTRATION
    // ===================================================================
    int registerPatient(const string& name, int age, int priorityLevel,
                        const string& condition)
    {
        Priority p;
        switch (priorityLevel) {
            case 1: p = Priority::CRITICAL;    break;
            case 2: p = Priority::HIGH;        break;
            case 3: p = Priority::MEDIUM;      break;
            case 4: p = Priority::LOW;         break;
            case 5: p = Priority::OBSERVATION; break;
            default: p = Priority::LOW;
        }

        Patient patient(nextPatientId++, name, age, p, condition);

        // Store in hash map (O(1) lookup)
        patientDB.registerPatient(patient);

        // Route to appropriate queue
        if (p == Priority::OBSERVATION || p == Priority::LOW) {
            waitingRoom.enqueue(patient);
            history.addRecord(patient, "REGISTERED_WAITING",
                              "Added to waiting room queue");
        } else {
            emergencyQueue.insert(patient);
            history.addRecord(patient, "REGISTERED_EMERGENCY",
                              "Added to emergency priority queue");
        }

        return patient.id;
    }

    // ===================================================================
    //  TREAT NEXT PATIENT (from emergency queue)
    // ===================================================================
    void treatNextEmergency() {
        if (emergencyQueue.isEmpty()) {
            cout << RED << "\n  [!] Emergency queue is empty.\n" << RESET;
            return;
        }
        if (!doctorPool.hasAvailable()) {
            cout << RED << "\n  [!] No doctors currently available. Please wait.\n" << RESET;
            return;
        }

        Patient p = emergencyQueue.extractMax();
        Doctor  d = doctorPool.assignDoctor();

        p.isAdmitted     = true;
        p.doctorAssigned = d.name;
        patientDB.update(p);

        activeDoctors[d.id] = d;

        cout << GREEN << "\n  ✔ Patient admitted for treatment:\n" << RESET;
        printPatientCard(p);
        cout << GREEN << "    Assigned Doctor: " << BOLD << d.name
             << RESET << " (" << d.specialization << ")\n";

        history.addRecord(p, "TREATED",
                          "Assigned to " + d.name + " [" + d.specialization + "]");
        totalAdmitted++;
    }

    // ===================================================================
    //  DISCHARGE PATIENT & RELEASE DOCTOR
    // ===================================================================
    void dischargePatient(int patientId) {
        Patient* p = patientDB.findById(patientId);
        if (!p) {
            cout << RED << "  [!] Patient ID " << patientId << " not found.\n" << RESET;
            return;
        }
        if (!p->isAdmitted) {
            cout << YELLOW << "  [!] Patient is not currently admitted.\n" << RESET;
            return;
        }

        // Find and release doctor
        for (auto& [did, doc] : activeDoctors) {
            if (doc.name == p->doctorAssigned) {
                doctorPool.releaseDoctor(doc);
                activeDoctors.erase(did);
                break;
            }
        }

        history.addRecord(*p, "DISCHARGED", "Patient discharged successfully");
        patientDB.remove(patientId);

        cout << GREEN << "\n  ✔ Patient " << BOLD << p->name
             << RESET << GREEN << " (ID: " << patientId
             << ") discharged successfully.\n" << RESET;
    }

    // ===================================================================
    //  UPGRADE PRIORITY (emergency escalation)
    // ===================================================================
    void upgradePriority(int patientId, int newPriorityLevel) {
        Patient* p = patientDB.findById(patientId);
        if (!p) {
            cout << RED << "  [!] Patient not found.\n" << RESET;
            return;
        }
        Priority newP;
        switch (newPriorityLevel) {
            case 1: newP = Priority::CRITICAL; break;
            case 2: newP = Priority::HIGH;     break;
            case 3: newP = Priority::MEDIUM;   break;
            default: newP = Priority::LOW;
        }

        bool updated = emergencyQueue.updatePriority(patientId, newP);
        if (updated) {
            p->priority = newP;
            patientDB.update(*p);
            cout << GREEN << "\n  ✔ Priority updated to: "
                 << BOLD << p->priorityToString() << RESET << "\n";
            history.addRecord(*p, "PRIORITY_UPDATED",
                              "New priority: " + p->priorityToString());
        } else {
            cout << YELLOW << "  [!] Patient not in emergency queue.\n" << RESET;
        }
    }

    // ===================================================================
    //  TREAT FROM WAITING ROOM (FIFO)
    // ===================================================================
    void treatNextWaiting() {
        if (waitingRoom.isEmpty()) {
            cout << RED << "\n  [!] Waiting room is empty.\n" << RESET;
            return;
        }
        if (!doctorPool.hasAvailable()) {
            cout << RED << "\n  [!] No doctors available.\n" << RESET;
            return;
        }
        Patient p = waitingRoom.dequeue();
        Doctor  d = doctorPool.assignDoctor();
        p.isAdmitted = true;
        p.doctorAssigned = d.name;
        patientDB.update(p);
        activeDoctors[d.id] = d;

        cout << GREEN << "\n  ✔ Waiting room patient called:\n" << RESET;
        printPatientCard(p);
        cout << GREEN << "    Assigned Doctor: " << BOLD << d.name << RESET
             << " (" << d.specialization << ")\n";
        history.addRecord(p, "TREATED_FROM_WAITING",
                          "Called from waiting room");
        totalAdmitted++;
    }

    // ===================================================================
    //  SEARCH PATIENT
    // ===================================================================
    void searchPatient(int id) {
        Patient* p = patientDB.findById(id);
        if (p) {
            cout << CYAN << "\n  Patient Found:\n" << RESET;
            printPatientCard(*p);
        } else {
            cout << RED << "  [!] Patient ID " << id << " not found in database.\n" << RESET;
        }
    }

    void searchByName(const string& name) {
        auto results = patientDB.findByName(name);
        if (results.empty()) {
            cout << RED << "  [!] No patients found with name: " << name << "\n" << RESET;
        } else {
            cout << CYAN << "\n  Search Results for \"" << name << "\":\n" << RESET;
            for (auto* p : results) printPatientCard(*p);
        }
    }

    // ===================================================================
    //  DISPLAY FUNCTIONS
    // ===================================================================
    void displayEmergencyQueue() {
        printSectionHeader("EMERGENCY PRIORITY QUEUE  (Max-Heap)");
        if (emergencyQueue.isEmpty()) {
            cout << "  " << YELLOW << "Queue is empty.\n" << RESET;
            return;
        }
        auto sorted = emergencyQueue.getSortedSnapshot();
        printTableHeader();
        int rank = 1;
        for (const auto& p : sorted) {
            printTableRow(rank++, p);
        }
        printTableFooter();
    }

    void displayWaitingRoom() {
        printSectionHeader("WAITING ROOM QUEUE  (FIFO)");
        if (waitingRoom.isEmpty()) {
            cout << "  " << YELLOW << "Waiting room is empty.\n" << RESET;
            return;
        }
        auto q = waitingRoom.getSnapshot();
        printTableHeader();
        int rank = 1;
        while (!q.empty()) {
            printTableRow(rank++, q.front());
            q.pop();
        }
        printTableFooter();
    }

    void displayHistory() {
        printSectionHeader("PATIENT HISTORY  (Doubly Linked List)");
        if (history.totalRecords() == 0) {
            cout << "  " << YELLOW << "No records yet.\n" << RESET;
            return;
        }
        cout << "  " << setw(6)  << left << "ID"
             << setw(20) << "Name"
             << setw(22) << "Action"
             << setw(22) << "Timestamp"
             << "Notes\n";
        cout << "  " << string(90, '-') << "\n";
        for (const auto& r : history.getAll()) {
            cout << "  " << setw(6)  << r.patient.id
                 << setw(20) << r.patient.name.substr(0, 18)
                 << setw(22) << r.action.substr(0, 20)
                 << setw(22) << r.getTimeStr()
                 << r.notes.substr(0, 30) << "\n";
        }
    }

    void displayDoctors() {
        printSectionHeader("DOCTOR POOL  (Stack)");
        const auto& docs = doctorPool.getAllDoctors();
        cout << "  " << setw(5)  << "ID"
             << setw(25) << "Name"
             << setw(22) << "Specialization"
             << setw(12) << "Status"
             << "Served\n";
        cout << "  " << string(80, '-') << "\n";
        for (const auto& d : docs) {
            string status = d.isAvailable ? GREEN + string("Available") + RESET
                                          : RED   + string("Busy")      + RESET;
            cout << "  " << setw(5)  << d.id
                 << setw(25) << d.name
                 << setw(22) << d.specialization
                 << "  " << (d.isAvailable ? GREEN : RED)
                 << setw(10) << (d.isAvailable ? "Available" : "Busy")
                 << RESET << "  " << d.patientsServed << "\n";
        }
        cout << "\n  Available: " << GREEN << doctorPool.availableCount() << RESET
             << " / " << docs.size() << "\n";
    }

    void displaySortedPatients(int sortType) {
        // Collect all patients from both queues + hash map
        const auto& allMap = patientDB.getAll();
        vector<Patient> patients;
        for (const auto& [id, p] : allMap) patients.push_back(p);

        if (patients.empty()) {
            cout << YELLOW << "  No patients in system.\n" << RESET;
            return;
        }

        string sortName;
        if (sortType == 1) {
            SortingAlgorithms::quickSort(patients, 0, patients.size()-1,
                                        SortingAlgorithms::byPriorityAsc);
            sortName = "Priority (Quick Sort)";
        } else if (sortType == 2) {
            SortingAlgorithms::mergeSort(patients, 0, patients.size()-1,
                                         SortingAlgorithms::byArrivalAsc);
            sortName = "Arrival Time (Merge Sort)";
        } else if (sortType == 3) {
            SortingAlgorithms::insertionSort(patients,
                                             SortingAlgorithms::byAgeAsc);
            sortName = "Age (Insertion Sort)";
        } else {
            SortingAlgorithms::insertionSort(patients,
                                             SortingAlgorithms::byNameAsc);
            sortName = "Name (Insertion Sort)";
        }

        printSectionHeader("ALL PATIENTS — Sorted by " + sortName);
        printTableHeader();
        int rank = 1;
        for (const auto& p : patients) printTableRow(rank++, p);
        printTableFooter();
    }

    void displayStatistics() {
        printSectionHeader("SYSTEM STATISTICS");
        int totalInSystem = emergencyQueue.size() + waitingRoom.size();
        cout << "\n";
        cout << "  " << BOLD << "Emergency Queue Size  : " << RESET
             << CYAN << emergencyQueue.size() << RESET << "\n";
        cout << "  " << BOLD << "Waiting Room Size     : " << RESET
             << CYAN << waitingRoom.size() << RESET << "\n";
        cout << "  " << BOLD << "Total in System       : " << RESET
             << CYAN << totalInSystem << RESET << "\n";
        cout << "  " << BOLD << "Total Admitted Today  : " << RESET
             << GREEN << totalAdmitted << RESET << "\n";
        cout << "  " << BOLD << "Total History Records : " << RESET
             << YELLOW << history.totalRecords() << RESET << "\n";
        cout << "  " << BOLD << "Available Doctors     : " << RESET
             << GREEN << doctorPool.availableCount() << RESET
             << " / " << doctorPool.getAllDoctors().size() << "\n";
        cout << "  " << BOLD << "Total Registered (DB) : " << RESET
             << MAGENTA << patientDB.totalRegistered() << RESET << "\n\n";

        // Priority breakdown
        cout << "  " << BOLD << "Emergency Queue Breakdown:\n" << RESET;
        int crit=0,high=0,med=0,low=0;
        for (const auto& p : emergencyQueue.getRaw()) {
            switch (p.priority) {
                case Priority::CRITICAL: crit++; break;
                case Priority::HIGH:     high++; break;
                case Priority::MEDIUM:   med++;  break;
                default:                 low++;  break;
            }
        }
        cout << "    " << RED    << "● CRITICAL   : " << crit << RESET << "\n";
        cout << "    " << YELLOW << "● HIGH       : " << high << RESET << "\n";
        cout << "    " << CYAN   << "● MEDIUM     : " << med  << RESET << "\n";
        cout << "    " << GREEN  << "● LOW        : " << low  << RESET << "\n\n";
    }

    void displayPatientHistory(int id) {
        auto records = history.searchById(id);
        if (records.empty()) {
            cout << RED << "  No history found for ID " << id << "\n" << RESET;
            return;
        }
        cout << CYAN << "\n  History for Patient ID: " << id << "\n" << RESET;
        cout << "  " << string(70, '-') << "\n";
        for (const auto& r : records) {
            cout << "  [" << r.getTimeStr() << "]  "
                 << BOLD << r.action << RESET << "  —  " << r.notes << "\n";
        }
    }

    // ===================================================================
    //  PRINT HELPERS
    // ===================================================================
    void printBanner() {
        cout << "\n";
        cout << BOLD << CYAN;
        cout << "  ╔══════════════════════════════════════════════════════════════╗\n";
        cout << "  ║          HOSPITAL EMERGENCY MANAGEMENT SYSTEM               ║\n";
        cout << "  ║          DSA Project — C++  |  Priority Queue & More        ║\n";
        cout << "  ╚══════════════════════════════════════════════════════════════╝\n";
        cout << RESET << "\n";
    }

    void printSectionHeader(const string& title) {
        cout << "\n" << BOLD << BLUE
             << "  ┌─────────────────────────────────────────────────────────────┐\n"
             << "  │  " << left << setw(61) << title << "│\n"
             << "  └─────────────────────────────────────────────────────────────┘\n"
             << RESET;
    }

    void printTableHeader() {
        cout << "\n  " << BOLD
             << setw(5)  << "Rank"
             << setw(7)  << "ID"
             << setw(20) << "Name"
             << setw(5)  << "Age"
             << setw(13) << "Priority"
             << setw(10) << "Arrived"
             << setw(8)  << "Status"
             << "Condition\n" << RESET;
        cout << "  " << string(90, '-') << "\n";
    }

    void printTableRow(int rank, const Patient& p) {
        string priorityColor;
        switch (p.priority) {
            case Priority::CRITICAL:    priorityColor = RED;     break;
            case Priority::HIGH:        priorityColor = YELLOW;  break;
            case Priority::MEDIUM:      priorityColor = CYAN;    break;
            case Priority::LOW:         priorityColor = GREEN;   break;
            case Priority::OBSERVATION: priorityColor = WHITE;   break;
        }
        cout << "  "
             << setw(5)  << rank
             << setw(7)  << p.id
             << setw(20) << p.name.substr(0, 18)
             << setw(5)  << p.age
             << priorityColor << setw(13) << p.priorityToString() << RESET
             << setw(10) << p.getArrivalTimeStr()
             << setw(8)  << (p.isAdmitted ? GREEN + string("In Tx") + RESET
                                           : YELLOW + string("Waiting") + RESET)
             << p.condition.substr(0, 25) << "\n";
    }

    void printTableFooter() {
        cout << "  " << string(90, '-') << "\n";
    }

    void printPatientCard(const Patient& p) {
        string priorityColor;
        switch (p.priority) {
            case Priority::CRITICAL:    priorityColor = RED;    break;
            case Priority::HIGH:        priorityColor = YELLOW; break;
            case Priority::MEDIUM:      priorityColor = CYAN;   break;
            case Priority::LOW:         priorityColor = GREEN;  break;
            case Priority::OBSERVATION: priorityColor = WHITE;  break;
        }
        cout << "  " << string(50, '-') << "\n";
        cout << "  ID         : " << BOLD << p.id << RESET << "\n";
        cout << "  Name       : " << BOLD << p.name << RESET << "\n";
        cout << "  Age        : " << p.age << "\n";
        cout << "  Priority   : " << priorityColor << BOLD
             << p.priorityToString() << RESET << "\n";
        cout << "  Condition  : " << p.condition << "\n";
        cout << "  Arrived    : " << p.getArrivalTimeStr() << "\n";
        cout << "  Doctor     : " << p.doctorAssigned << "\n";
        cout << "  Status     : " << (p.isAdmitted
                                     ? GREEN + string("In Treatment") + RESET
                                     : YELLOW + string("Waiting") + RESET) << "\n";
        cout << "  " << string(50, '-') << "\n";
    }
};

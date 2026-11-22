#pragma once
#include <list>
#include <string>
#include "Patient.h"
using namespace std;

// =====================================================================
//  PatientHistory.h  —  Doubly Linked List for treatment history
//  DSA Concepts: Doubly Linked List (using std::list)
//  Also stores audit log entries per action
// =====================================================================

struct HistoryRecord {
    Patient     patient;
    string      action;        // "REGISTERED", "TREATED", "DISCHARGED", etc.
    time_t      timestamp;
    string      notes;

    HistoryRecord(const Patient& p, const string& act, const string& n = "")
        : patient(p), action(act), notes(n)
    {
        timestamp = time(nullptr);
    }

    string getTimeStr() const {
        char buf[64];
        tm* t = localtime(&timestamp);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
        return string(buf);
    }
};

class PatientHistory {
private:
    list<HistoryRecord> records;   // Doubly linked list

public:
    void addRecord(const Patient& p, const string& action, const string& notes = "") {
        records.emplace_back(p, action, notes);
    }

    // ---- Print full history (forward traversal) ----
    void printHistory() const;

    // ---- Search history by patient ID ----
    list<HistoryRecord> searchById(int id) const {
        list<HistoryRecord> result;
        for (const auto& r : records)
            if (r.patient.id == id) result.push_back(r);
        return result;
    }

    // ---- Search history by patient name ----
    list<HistoryRecord> searchByName(const string& name) const {
        list<HistoryRecord> result;
        for (const auto& r : records) {
            string pName = r.patient.name;
            string sName = name;
            // Case-insensitive search
            for (char& c : pName) c = tolower(c);
            for (char& c : sName) c = tolower(c);
            if (pName.find(sName) != string::npos)
                result.push_back(r);
        }
        return result;
    }

    // ---- Get total records ----
    int totalRecords() const { return records.size(); }

    // ---- Get total treated ----
    int totalTreated() const {
        int count = 0;
        for (const auto& r : records)
            if (r.action == "TREATED" || r.action == "DISCHARGED") count++;
        return count;
    }

    const list<HistoryRecord>& getAll() const { return records; }
};

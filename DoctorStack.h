#pragma once
#include <stack>
#include <string>
#include <vector>
using namespace std;

// =====================================================================
//  DoctorStack.h  —  Stack for available doctors (LIFO assignment)
//  DSA Concepts: Stack (LIFO) — recently freed doctors are reused first
// =====================================================================

struct Doctor {
    int    id;
    string name;
    string specialization;
    bool   isAvailable;
    int    patientsServed;

    Doctor() : id(0), isAvailable(true), patientsServed(0) {}
    Doctor(int id, const string& name, const string& spec)
        : id(id), name(name), specialization(spec),
          isAvailable(true), patientsServed(0) {}
};

class DoctorStack {
private:
    stack<Doctor>  availableStack;
    vector<Doctor> allDoctors;     // For reporting purposes

public:
    // ---- Add doctor to system ----
    void addDoctor(const Doctor& d) {
        allDoctors.push_back(d);
        availableStack.push(d);
    }

    // ---- Assign next available doctor ----
    Doctor assignDoctor() {
        if (availableStack.empty())
            throw runtime_error("No doctors available!");
        Doctor d = availableStack.top();
        availableStack.pop();
        // Mark busy in allDoctors
        for (auto& doc : allDoctors)
            if (doc.id == d.id) { doc.isAvailable = false; break; }
        return d;
    }

    // ---- Release doctor back to pool ----
    void releaseDoctor(const Doctor& d) {
        Doctor updated = d;
        updated.isAvailable = true;
        updated.patientsServed++;
        availableStack.push(updated);
        for (auto& doc : allDoctors)
            if (doc.id == d.id) {
                doc.isAvailable = true;
                doc.patientsServed++;
                break;
            }
    }

    bool   hasAvailable()  const { return !availableStack.empty(); }
    int    availableCount() const { return availableStack.size();  }
    const vector<Doctor>& getAllDoctors() const { return allDoctors; }
};

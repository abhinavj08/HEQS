#pragma once
#include <string>
#include <ctime>
using namespace std;

// =====================================================================
//  Patient.h  —  Patient data model
//  DSA Concepts: Struct, Enums, Comparison operators (for heap ordering)
// =====================================================================

enum class Priority {
    CRITICAL    = 1,   // Life-threatening (highest)
    HIGH        = 2,   // Severe pain / major injury
    MEDIUM      = 3,   // Moderate symptoms
    LOW         = 4,   // Minor illness
    OBSERVATION = 5    // Routine / walk-in (lowest)
};

struct Patient {
    int         id;
    string      name;
    int         age;
    Priority    priority;
    string      condition;
    time_t      arrivalTime;
    bool        isAdmitted;
    string      doctorAssigned;
    int         waitTimeMinutes;    // estimated wait

    // ---- Constructor ----
    Patient(int id, const string& name, int age, Priority priority,
            const string& condition)
        : id(id), name(name), age(age), priority(priority),
          condition(condition), isAdmitted(false),
          doctorAssigned("Unassigned"), waitTimeMinutes(0)
    {
        arrivalTime = time(nullptr);
    }

    // ---- Default Constructor ----
    Patient() : id(0), age(0), priority(Priority::LOW),
                isAdmitted(false), waitTimeMinutes(0)
    {
        arrivalTime = time(nullptr);
    }

    // ---- Comparator: lower priority value = higher urgency ----
    bool operator<(const Patient& other) const {
        if (priority != other.priority)
            return static_cast<int>(priority) > static_cast<int>(other.priority);
        return arrivalTime > other.arrivalTime; // earlier arrival wins on tie
    }

    // ---- Helper: priority to string ----
    string priorityToString() const {
        switch (priority) {
            case Priority::CRITICAL:    return "CRITICAL";
            case Priority::HIGH:        return "HIGH";
            case Priority::MEDIUM:      return "MEDIUM";
            case Priority::LOW:         return "LOW";
            case Priority::OBSERVATION: return "OBSERVATION";
            default:                    return "UNKNOWN";
        }
    }

    // ---- Helper: arrival time formatted ----
    string getArrivalTimeStr() const {
        char buf[64];
        tm* t = localtime(&arrivalTime);
        strftime(buf, sizeof(buf), "%H:%M:%S", t);
        return string(buf);
    }
};

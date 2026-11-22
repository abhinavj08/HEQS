#pragma once
#include <queue>
#include <string>
#include "Patient.h"
using namespace std;

// =====================================================================
//  WaitingRoomQueue.h  —  Simple FIFO Queue for observation patients
//  DSA Concepts: Queue (FIFO) — regular waiting room (non-emergency)
// =====================================================================

class WaitingRoomQueue {
private:
    queue<Patient> waitQueue;
    int            totalServed;

public:
    WaitingRoomQueue() : totalServed(0) {}

    void enqueue(const Patient& p) {
        waitQueue.push(p);
    }

    Patient dequeue() {
        if (waitQueue.empty())
            throw runtime_error("Waiting room is empty!");
        Patient p = waitQueue.front();
        waitQueue.pop();
        totalServed++;
        return p;
    }

    const Patient& front() const {
        if (waitQueue.empty())
            throw runtime_error("Waiting room is empty!");
        return waitQueue.front();
    }

    bool   isEmpty()      const { return waitQueue.empty();  }
    int    size()         const { return waitQueue.size();   }
    int    getServedCount() const { return totalServed;      }

    // ---- Peek all patients in queue ----
    queue<Patient> getSnapshot() const { return waitQueue; }
};

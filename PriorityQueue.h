#pragma once
#include <vector>
#include <stdexcept>
#include "Patient.h"
using namespace std;

// =====================================================================
//  PriorityQueue.h  —  Max-Heap based Priority Queue (Min by urgency)
//  DSA Concepts: Binary Heap, Heapify-Up, Heapify-Down
//  Time Complexity:
//    Insert  : O(log n)
//    Extract : O(log n)
//    Peek    : O(1)
// =====================================================================

class PriorityQueue {
private:
    vector<Patient> heap;

    // ---- Index helpers ----
    int parent(int i) { return (i - 1) / 2; }
    int left(int i)   { return 2 * i + 1;   }
    int right(int i)  { return 2 * i + 2;   }

    // ---- Bubble up after insertion ----
    void heapifyUp(int idx) {
        while (idx > 0 && heap[parent(idx)] < heap[idx]) {
            swap(heap[parent(idx)], heap[idx]);
            idx = parent(idx);
        }
    }

    // ---- Push down after extraction ----
    void heapifyDown(int idx) {
        int n = heap.size();
        int largest = idx;
        int l = left(idx), r = right(idx);

        if (l < n && heap[largest] < heap[l]) largest = l;
        if (r < n && heap[largest] < heap[r]) largest = r;

        if (largest != idx) {
            swap(heap[idx], heap[largest]);
            heapifyDown(largest);
        }
    }

public:
    // ---- Insert patient ----
    void insert(const Patient& p) {
        heap.push_back(p);
        heapifyUp(heap.size() - 1);
    }

    // ---- Get highest-priority patient ----
    Patient extractMax() {
        if (heap.empty())
            throw runtime_error("Queue is empty!");
        Patient top = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) heapifyDown(0);
        return top;
    }

    // ---- Peek without removing ----
    const Patient& peek() const {
        if (heap.empty())
            throw runtime_error("Queue is empty!");
        return heap[0];
    }

    // ---- Remove a patient by ID (for discharge / cancel) ----
    bool removeById(int id) {
        for (int i = 0; i < (int)heap.size(); i++) {
            if (heap[i].id == id) {
                heap[i] = heap.back();
                heap.pop_back();
                if (i < (int)heap.size()) {
                    heapifyUp(i);
                    heapifyDown(i);
                }
                return true;
            }
        }
        return false;
    }

    // ---- Update priority of existing patient ----
    bool updatePriority(int id, Priority newPriority) {
        for (int i = 0; i < (int)heap.size(); i++) {
            if (heap[i].id == id) {
                heap[i].priority = newPriority;
                heapifyUp(i);
                heapifyDown(i);
                return true;
            }
        }
        return false;
    }

    // ---- Get snapshot (sorted copy) of heap ----
    vector<Patient> getSortedSnapshot() const {
        vector<Patient> copy = heap;
        // Simple insertion sort for display (n is small in emergency queue)
        for (int i = 1; i < (int)copy.size(); i++) {
            Patient key = copy[i];
            int j = i - 1;
            while (j >= 0 && copy[j] < key) {
                copy[j + 1] = copy[j];
                j--;
            }
            copy[j + 1] = key;
        }
        return copy;
    }

    bool    isEmpty() const { return heap.empty(); }
    int     size()    const { return heap.size();  }
    const vector<Patient>& getRaw() const { return heap; }
};

#pragma once
#include <vector>
#include <string>
#include "Patient.h"
using namespace std;

// =====================================================================
//  SortingAlgorithms.h  —  Multiple sorting algorithms on patients
//  DSA Concepts: Merge Sort, Quick Sort, Insertion Sort
// =====================================================================

class SortingAlgorithms {
public:

    // ==================================================================
    //  MERGE SORT — O(n log n), stable
    //  Used for: sorting by arrival time
    // ==================================================================
    static void mergeSort(vector<Patient>& arr, int left, int right,
                          bool(*cmp)(const Patient&, const Patient&))
    {
        if (left >= right) return;
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, cmp);
        mergeSort(arr, mid + 1, right, cmp);
        merge(arr, left, mid, right, cmp);
    }

    static void merge(vector<Patient>& arr, int left, int mid, int right,
                      bool(*cmp)(const Patient&, const Patient&))
    {
        vector<Patient> L(arr.begin() + left,  arr.begin() + mid + 1);
        vector<Patient> R(arr.begin() + mid + 1, arr.begin() + right + 1);
        int i = 0, j = 0, k = left;
        while (i < (int)L.size() && j < (int)R.size())
            arr[k++] = cmp(L[i], R[j]) ? L[i++] : R[j++];
        while (i < (int)L.size())  arr[k++] = L[i++];
        while (j < (int)R.size())  arr[k++] = R[j++];
    }

    // ==================================================================
    //  QUICK SORT — O(n log n) avg, in-place
    //  Used for: sorting by priority
    // ==================================================================
    static void quickSort(vector<Patient>& arr, int low, int high,
                          bool(*cmp)(const Patient&, const Patient&))
    {
        if (low < high) {
            int pi = partition(arr, low, high, cmp);
            quickSort(arr, low, pi - 1, cmp);
            quickSort(arr, pi + 1, high, cmp);
        }
    }

    static int partition(vector<Patient>& arr, int low, int high,
                         bool(*cmp)(const Patient&, const Patient&))
    {
        Patient pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (cmp(arr[j], pivot)) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    // ==================================================================
    //  INSERTION SORT — O(n^2), good for small/nearly-sorted arrays
    //  Used for: sorting by age or name
    // ==================================================================
    static void insertionSort(vector<Patient>& arr,
                              bool(*cmp)(const Patient&, const Patient&))
    {
        for (int i = 1; i < (int)arr.size(); i++) {
            Patient key = arr[i];
            int j = i - 1;
            while (j >= 0 && !cmp(arr[j], key)) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }

    // ==================================================================
    //  Comparator functions
    // ==================================================================
    static bool byPriorityAsc(const Patient& a, const Patient& b) {
        return static_cast<int>(a.priority) < static_cast<int>(b.priority);
    }
    static bool byArrivalAsc(const Patient& a, const Patient& b) {
        return a.arrivalTime < b.arrivalTime;
    }
    static bool byAgeAsc(const Patient& a, const Patient& b) {
        return a.age < b.age;
    }
    static bool byNameAsc(const Patient& a, const Patient& b) {
        return a.name < b.name;
    }

    // ==================================================================
    //  BINARY SEARCH — O(log n) on sorted array by ID
    // ==================================================================
    static int binarySearchById(const vector<Patient>& arr, int targetId) {
        int low = 0, high = arr.size() - 1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (arr[mid].id == targetId)       return mid;
            else if (arr[mid].id < targetId)   low = mid + 1;
            else                               high = mid - 1;
        }
        return -1; // not found
    }
};

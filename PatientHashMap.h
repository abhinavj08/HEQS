#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include "Patient.h"
using namespace std;

// =====================================================================
//  PatientHashMap.h  —  Hash Map for O(1) patient lookup
//  DSA Concepts: Hash Table / Unordered Map
//  Supports lookup by patient ID or name index
// =====================================================================

class PatientHashMap {
private:
    unordered_map<int, Patient>     idMap;      // id  -> Patient
    unordered_map<string, vector<int>> nameMap; // name -> [ids]

public:
    // ---- Register patient ----
    void registerPatient(const Patient& p) {
        idMap[p.id] = p;
        string nameLower = p.name;
        for (char& c : nameLower) c = tolower(c);
        nameMap[nameLower].push_back(p.id);
    }

    // ---- Lookup by ID — O(1) ----
    Patient* findById(int id) {
        auto it = idMap.find(id);
        return (it != idMap.end()) ? &it->second : nullptr;
    }

    // ---- Lookup by name — O(1) avg ----
    vector<Patient*> findByName(const string& name) {
        string nameLower = name;
        for (char& c : nameLower) c = tolower(c);
        vector<Patient*> result;
        auto it = nameMap.find(nameLower);
        if (it != nameMap.end()) {
            for (int id : it->second) {
                auto pit = idMap.find(id);
                if (pit != idMap.end())
                    result.push_back(&pit->second);
            }
        }
        return result;
    }

    // ---- Update patient record ----
    void update(const Patient& p) {
        idMap[p.id] = p;
    }

    // ---- Remove patient ----
    bool remove(int id) {
        auto it = idMap.find(id);
        if (it == idMap.end()) return false;
        string nameLower = it->second.name;
        for (char& c : nameLower) c = tolower(c);
        auto& ids = nameMap[nameLower];
        ids.erase(std::remove(ids.begin(), ids.end(), id), ids.end());
        idMap.erase(it);
        return true;
    }

    // ---- Check existence ----
    bool exists(int id) const { return idMap.count(id) > 0; }

    int  totalRegistered() const { return idMap.size(); }
    const unordered_map<int, Patient>& getAll() const { return idMap; }
};

# 🏥 Hospital Emergency Queue Management System
### DSA Project | C++17

> A fully functional Hospital Emergency Management System demonstrating **7 core Data Structures** and **5 Algorithms** in a real-world scenario.

---

## 📁 Project Structure

```
DSA Project/
├── main.cpp              ← Entry point & interactive menu
├── Patient.h             ← Patient data model (struct, enum)
├── PriorityQueue.h       ← Binary Max-Heap (emergency triage)
├── DoctorStack.h         ← Stack (doctor assignment pool)
├── WaitingRoomQueue.h    ← FIFO Queue (general waiting room)
├── PatientHistory.h      ← Doubly Linked List (audit log)
├── PatientHashMap.h      ← Hash Map (O(1) patient lookup)
├── SortingAlgorithms.h   ← Quick Sort, Merge Sort, Insertion Sort, Binary Search
├── HospitalSystem.h      ← Central orchestrator (all modules wired together)
└── HospitalEMS.exe       ← Compiled executable
```

---

## 🧩 Data Structures Used

| # | Data Structure | File | Real-World Use |
|---|---------------|------|----------------|
| 1 | **Binary Max-Heap (Priority Queue)** | `PriorityQueue.h` | Emergency triage — sickest patient treated first |
| 2 | **Stack (LIFO)** | `DoctorStack.h` | Doctor pool — recently freed doctor assigned first |
| 3 | **Queue (FIFO)** | `WaitingRoomQueue.h` | General waiting room — fair first-come-first-served |
| 4 | **Doubly Linked List** | `PatientHistory.h` | Full audit trail of all patient actions |
| 5 | **Hash Map (unordered_map)** | `PatientHashMap.h` | O(1) patient lookup by ID or name |
| 6 | **Vector / Dynamic Array** | `PriorityQueue.h` | Underlying heap storage |
| 7 | **Struct / Enum** | `Patient.h`, `DoctorStack.h` | Data modeling |

---

## ⚙️ Algorithms Used

| # | Algorithm | Complexity | Used For |
|---|-----------|-----------|----------|
| 1 | **Heapify-Up / Heapify-Down** | O(log n) | Maintaining heap invariant on insert/extract |
| 2 | **Quick Sort** | O(n log n) avg | Sort patients by priority |
| 3 | **Merge Sort** | O(n log n) stable | Sort patients by arrival time |
| 4 | **Insertion Sort** | O(n²) | Sort by age or name (small n) |
| 5 | **Binary Search** | O(log n) | Search in sorted patient arrays |

---

## 🚀 How to Compile & Run

### Prerequisites
- g++ with C++17 support (MinGW / MSYS2 / GCC)

### Compile
```bash
g++ -std=c++17 -o HospitalEMS.exe main.cpp
```

### Run
```bash
./HospitalEMS.exe
```

---

## 🖥️ Features & Menu

```
MAIN MENU
══════════════════════════════════════
 1.  Register New Patient
 2.  Treat Next Emergency Patient        ← Pops from Priority Queue (Max-Heap)
 3.  Treat Next Waiting Room Patient     ← Dequeues from FIFO Queue
 4.  Discharge Patient                   ← Releases doctor back to Stack
 5.  Upgrade Patient Priority            ← Re-heapifies the Priority Queue
 6.  View Emergency Queue               ← Sorted snapshot of heap
 7.  View Waiting Room                  ← FIFO queue view
 8.  View Doctor Pool                   ← Stack status
 9.  View All Patients (Sorted)         ← Choose sorting algorithm
 10. View Patient History Log           ← Doubly linked list traversal
 11. Search Patient                     ← Hash Map O(1) lookup
 12. View Patient's Audit Trail         ← History search by ID
 13. System Statistics                  ← Priority breakdown + counts
  0. Exit
```

---

## 🎯 Priority Levels

| Level | Name | Description | Color |
|-------|------|-------------|-------|
| 1 | **CRITICAL** | Cardiac arrest, stroke, life-threatening | 🔴 Red |
| 2 | **HIGH** | Severe injury, major fracture | 🟡 Yellow |
| 3 | **MEDIUM** | Moderate symptoms, abdominal pain | 🔵 Cyan |
| 4 | **LOW** | Minor illness, sprain | 🟢 Green |
| 5 | **OBSERVATION** | Routine / walk-in checkup | ⚪ White |

Patients 1–3 go into the **Priority Queue (Heap)**. Patients 4–5 go into the **Waiting Room (FIFO Queue)**.

---

## 📐 Complexity Summary

```
OPERATION                     DATA STRUCTURE      TIME COMPLEXITY
─────────────────────────────────────────────────────────────────
Register patient              Hash Map + Heap      O(log n)
Treat next emergency          Max-Heap extract     O(log n)
Treat next waiting patient    FIFO dequeue         O(1)
Search patient by ID          Hash Map             O(1) avg
Upgrade priority              Heap update          O(log n)
Sort by priority              Quick Sort           O(n log n) avg
Sort by arrival time          Merge Sort           O(n log n)
Sort by age / name            Insertion Sort       O(n²)
History lookup by ID          Linked List scan     O(n)
Assign/release doctor         Stack push/pop       O(1)
```

---

## 💡 Sample Demo Output

```
╔══════════════════════════════════════════════════════════════╗
║          HOSPITAL EMERGENCY MANAGEMENT SYSTEM               ║
║          DSA Project — C++  |  Priority Queue & More        ║
╚══════════════════════════════════════════════════════════════╝

  EMERGENCY PRIORITY QUEUE  (Max-Heap)
  Rank   ID   Name              Age  Priority     Arrived   Status  Condition
  ──────────────────────────────────────────────────────────────────────────────
     1  1001  Rahul Verma        45  CRITICAL     14:30:01  Waiting Cardiac Arrest
     2  1007  Vikas Singh        72  CRITICAL     14:30:03  Waiting Severe Breathing
     3  1002  Sunita Patel       62  HIGH         14:30:01  Waiting Severe Head Injury
     4  1005  Deepak Kumar       55  HIGH         14:30:02  Waiting Stroke Symptoms
     5  1010  Lata Gupta         68  HIGH         14:30:03  Waiting High Blood Pressure
     6  1003  Amit Sharma        30  MEDIUM       14:30:01  Waiting Broken Arm
     7  1008  Nandini Iyer       29  MEDIUM       14:30:03  Waiting Abdominal Pain
```

---

## 👨‍💻 Author
**DSA Project — Hospital Emergency Queue System**
*C++17 | Data Structures & Algorithms*

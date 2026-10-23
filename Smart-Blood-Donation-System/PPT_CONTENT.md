# PPT Content — Smart Blood Donation Emergency Matching System

Copy each slide's content directly into PowerPoint/Google Slides. Keep
one idea per slide; the speaker notes under each are for you to say,
not to put on the slide itself.

---

### Slide 1 — Title
**Smart Blood Donation Emergency Matching System**
A C++ Console Application using Core Data Structures
[Your Name] · [Roll Number] · [College/Department] · [Date]

---

### Slide 2 — Problem Statement
- Hospitals often struggle to find the right blood donor quickly during emergencies.
- Manually searching donor lists by blood group and availability is slow and error-prone.
- There is no simple, structured way to prioritize urgent requests over routine ones.

*Speaker note: keep this to 15 seconds — the problem should feel obvious, not need selling.*

---

### Slide 3 — Objective
- Maintain a searchable donor database.
- Log blood requests with an urgency level.
- Instantly match an available donor for a requested blood group.
- Track donation history and generate basic reports.

---

### Slide 4 — Scope
**In scope:** console app, single admin user, file-based persistence.
**Out of scope (by design):** GUI, database, login system, networking,
blood-group compatibility rules.

*Speaker note: framing these as deliberate choices (not missing features) heads off "why didn't you..." questions before they're asked.*

---

### Slide 5 — System Architecture
```
Donor.h  BloodRequest.h  DonationRecord.h   (models)
        ↓
DonorManager   RequestManager                (logic)
        ↓
FileManager                                  (persistence)
        ↓
main.cpp                                     (menu / UI)
```

---

### Slide 6 — Data Structures Used
| Structure | Used For |
|---|---|
| `vector<Donor>` | Master donor storage |
| `unordered_map<string, vector<int>>` | Fast search by blood group |
| `priority_queue<BloodRequest>` | Urgency-ordered request queue |
| `list<DonationRecord>` | Donation history log |

---

### Slide 7 — Why unordered_map?
- Average O(1) lookup by blood group vs O(n) linear scan.
- Groups donors into buckets — matching only scans the relevant bucket.

---

### Slide 8 — Why priority_queue?
- Emergency requests must be served by urgency, not by arrival order.
- A binary heap gives O(log n) insert and O(log n) removal-of-highest-priority.
- Custom comparator inverts default max-heap behavior so CRITICAL (1) outranks LOW (3).

---

### Slide 9 — Why list for history?
- History is append-only and always read front-to-back.
- No random access ever needed → linked list avoids vector's reallocation cost.

---

### Slide 10 — A Clever Simplification: O(1) ID Lookup
- Donor IDs are sequential; donors are soft-deleted, never physically removed.
- Donor ID N is always at vector index N-1 → direct array access, no second hash map needed.

*Speaker note: this slide is your strongest "I understand trade-offs" moment — spend real time on it.*

---

### Slide 11 — Module Overview
1. Donor Management (Register / View / Search / Update / Delete)
2. Blood Request (Add / View, priority-ordered)
3. Emergency Matching (immediate donor lookup + auto-logging)
4. Donation History
5. Reports (totals, availability, blood-group distribution)
6. File Handling (auto-load on start, explicit save)

---

### Slide 12 — Time Complexity Summary
| Operation | Complexity |
|---|---|
| Register donor | O(1) amortized |
| Search by ID | O(1) |
| Search by blood group | O(k) |
| Add request | O(log n) |
| Emergency Matching | O(1) + O(k) |
| Add history record | O(1) |

---

### Slide 13 — Live Demo
(Switch to the running application here — see the demo script in `VIVA_PREP.md`.)

---

### Slide 14 — Conclusion
- All four required data structures used in specific, justified roles.
- Clean OOP design with full encapsulation.
- Deliberately scoped to stay explainable, not to look "impressive."

---

### Slide 15 — Future Enhancements
- Blood-group compatibility rules
- Distance/location-based donor ranking
- Authentication for multiple admins
- GUI or web interface

---

### Slide 16 — Thank You / Questions

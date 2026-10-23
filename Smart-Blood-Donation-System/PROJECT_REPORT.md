# Project Report: Smart Blood Donation Emergency Matching System

## 1. Introduction

Hospitals and blood banks frequently face delays during emergencies
because there is no fast, organized way to identify which registered
donors are the right blood group and currently available. The **Smart
Blood Donation Emergency Matching System** is a console-based C++
application that models this problem directly: it stores donor records,
logs incoming blood requests with an urgency level, and performs an
immediate matching operation that returns a suitable donor for a given
blood group.

The project is built as a single-admin console tool — there is no login
system, GUI, or database — so that the focus stays entirely on correct,
well-justified use of core data structures rather than on peripheral
engineering.

## 2. Objectives

- Maintain a searchable record of blood donors (register, view, update, delete).
- Log blood requests with an urgency level, and be able to view them in priority order.
- Given a blood group, immediately find an available donor and record the donation.
- Keep a persistent history of every completed donation.
- Generate simple summary reports (totals, availability, blood-group distribution).
- Persist all data to disk between runs.

## 3. Scope and Deliberate Simplifications

To keep the project at a clear, explainable, above-average college
level (rather than an industry-scale system), the following are
explicitly **out of scope**, by design:

- **No blood-group compatibility rules** (e.g. O- as a universal donor).
  Matching is an exact blood-group match. This keeps the matching logic
  simple and exactly explainable.
- **No login/authentication.** There is exactly one user (the admin
  operating the console), so authentication would add complexity without
  adding to the DSA content being demonstrated.
- **No GUI, no database, no networking.** A console application with
  flat-file persistence keeps the project's complexity centered entirely
  on the data structures and algorithms, which is the point of the
  exercise.

## 4. System Design

### 4.1 Classes (OOP Design)

Three plain model classes, all using full encapsulation (private fields,
public getters/setters, parameterized constructors):

- **Donor** — id, name, age, bloodGroup, phone, city, availability, and
  a `deleted` flag used for soft deletion (explained in §4.3).
- **BloodRequest** — requestId, patientName, bloodGroup, units,
  urgencyLevel, and an arrivalOrder counter used to break ties between
  requests of equal urgency.
- **DonationRecord** — donorId, donorName, patientName, bloodGroup, units.
  Created automatically whenever Emergency Matching succeeds.

Two manager classes contain all the logic and own the data structures:

- **DonorManager** — owns the `vector<Donor>` and the
  `unordered_map<string, vector<int>>` blood-group index.
- **RequestManager** — owns the `priority_queue<BloodRequest>` and the
  `list<DonationRecord>` history log. Holds a reference to DonorManager
  so Emergency Matching can call into it.

**FileManager** is a stateless utility class (static methods only) that
converts between in-memory objects and the `|`-delimited text files in
`data/`.

### 4.2 Data Structures Used — and Why

| Structure | Where | Why this one |
|---|---|---|
| `vector<Donor>` | `DonorManager::donors` | The master, ordered store of every donor record. |
| `unordered_map<string, vector<int>>` | `DonorManager::bloodGroupIndex` | Groups donor positions by blood group so a search only touches donors of that group, not the whole vector. Average O(1) to find the bucket. |
| `priority_queue<BloodRequest,...,CompareRequest>` | `RequestManager::pendingRequests` | A binary heap. Guarantees "View Requests" reports the most urgent request first, regardless of the order requests were added in. |
| `list<DonationRecord>` | `RequestManager::history` | An append-only log, always read front-to-back. A linked list avoids the reallocation cost a vector pays as it grows, and needs no random access. |

### 4.3 Why ID Search Doesn't Need a Second Hash Map

Donor IDs are assigned sequentially starting at 1, and donors are
**never physically removed** from the vector — "Delete Donor" just sets
a `deleted` flag to true (a "soft delete"). Because positions in the
vector never shift, donor ID `N` is **guaranteed** to sit at vector
index `N - 1` for the lifetime of the program. That turns "search by
ID" into a direct array access — O(1) — without needing a hash map for
it at all. This is a deliberate design choice, not an oversight, and is
one of the more interesting points to bring up in the viva.

The trade-off: soft-deleted donors still occupy a vector slot forever.
For a college-scale dataset this is a complete non-issue, and it's
explicitly the kind of trade-off worth naming if asked.

### 4.4 Why `unordered_map` Is Used Only for Blood Group

Blood group is a small, repeated, non-numeric category — exactly the
case `unordered_map` (hash table) is built for: O(1) average lookup of
"all donors in this bucket." ID lookup, as explained above, gets O(1)
for free from the array-position invariant, so a second hash map there
would be redundant.

### 4.5 Time Complexity Summary

Let **n** = total donors, **k** = donors of one blood group, **r** = pending requests.

| Operation | Complexity | Reason |
|---|---|---|
| Register donor | O(1) amortized | vector push_back + one hash map bucket insert |
| Search by ID | O(1) | direct array index (id - 1) |
| Search by blood group | O(k) | O(1) bucket lookup + O(k) scan of that bucket only |
| Update donor | O(1) (or O(k) if blood group changes, to move index buckets) | array index + bucket adjustment |
| Delete donor | O(1) | just flips the `deleted` flag |
| Add blood request | O(log r) | priority_queue push (heap insert) |
| View requests | O(r log r) | drain-and-restore trick (see §4.6) |
| Emergency Matching | O(1) + O(k) | hash bucket lookup + scan for first available donor |
| Add history record | O(1) | list push_back |
| Display all donors / reports | O(n) | one pass over the donor vector |

### 4.6 The "View Requests" Drain-and-Restore Trick

`std::priority_queue` does not support iteration — there is no way to
"peek" at every element without removing them. `viewRequests()` works
around this the standard way: it pops every element off (which comes
out already in priority order, since that's what a heap guarantees),
collects them into a `vector`, displays them, and then re-inserts every
element back into the queue. This costs O(r log r) instead of O(r), but
at the scale of a manually-operated console tool this is unnoticeable —
and understanding *why* this trick is necessary is a strong, specific
thing to be able to explain in the viva.

## 5. Project Flow

1. On startup, `main()` loads `donors.txt`, `requests.txt`, and
   `history.txt` into the respective managers.
2. The main menu offers: Donor Management, Blood Request, Emergency
   Matching, Donation History, Reports, Save Data, Exit.
3. **Donor Management** is a submenu for the 6 CRUD-style operations.
4. **Blood Request** is a submenu for adding a request (pushed into the
   priority queue) and viewing all pending requests in urgency order.
5. **Emergency Matching** is a single, immediate action: type in a
   blood group, and the system finds the first available donor with an
   exact match, marks them unavailable, and logs a `DonationRecord`.
6. **Donation History** and **Reports** are read-only views over the
   accumulated data.
7. **Save Data** writes the current in-memory state back to the three
   text files. **Exit** offers to save before quitting.

## 6. Conclusion

The system demonstrates all four required data structures in roles
that are each individually justified rather than decorative, uses
proper OOP encapsulation throughout, and keeps its scope intentionally
narrow so that every design decision — including the ones that look
like simplifications — can be explained and defended confidently.

## 7. Future Enhancements (if asked)

- Blood-group compatibility rules (e.g. O- as a universal donor)
- Location/distance-based donor ranking
- Multiple admin accounts with authentication
- A GUI or web front-end in place of the console menu

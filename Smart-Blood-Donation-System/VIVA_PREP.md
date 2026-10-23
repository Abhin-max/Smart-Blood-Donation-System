# Viva Preparation Guide

## Part 1 — Explain Every Class

### Donor (`Donor.h`)
A plain data-holding class with full encapsulation: every field
(`id`, `name`, `age`, `bloodGroup`, `phone`, `city`, `available`,
`deleted`) is `private`. Access happens only through getters/setters.
The `deleted` flag implements **soft delete** — see Part 3, Q6.

### BloodRequest (`BloodRequest.h`)
Same encapsulation pattern. Holds `urgencyLevel` (1=CRITICAL,
2=MODERATE, 3=LOW) and `arrivalOrder` (an incrementing counter used to
break ties between requests of equal urgency — first-come-first-served).
Also defines `CompareRequest`, the comparator struct that the
`priority_queue` uses to decide ordering.

### DonationRecord (`DonationRecord.h`)
A simple record created automatically the moment Emergency Matching
succeeds. Stored in `RequestManager`'s `list<DonationRecord>`.

### DonorManager (`DonorManager.h` / `.cpp`)
Owns and manages the `vector<Donor>` and the blood-group
`unordered_map`. Every donor-related operation (register, search,
update, delete, availability, matching) lives here.

### RequestManager (`RequestManager.h` / `.cpp`)
Owns the `priority_queue<BloodRequest>` and the `list<DonationRecord>`
history. Holds a reference to `DonorManager` so Emergency Matching can
call into donor logic without those two classes being merged into one.

### FileManager (`FileManager.h` / `.cpp`)
A stateless utility class — every method is `static`. Converts objects
to/from `|`-delimited lines and reads/writes the three text files using
`fstream`.

---

## Part 2 — Explain Key Functions

**`DonorManager::registerDonor(...)`** — assigns the next sequential
ID, appends a new `Donor` to the vector, and records its position in
the blood-group index. O(1) amortized.

**`DonorManager::getDonorById(int id, Donor &out)`** — computes
`position = id - 1` and reads directly from the vector. No searching
at all: O(1). Returns false if the position is out of range or the
donor there has been soft-deleted.

**`DonorManager::searchByBloodGroup(...)`** — looks up the bucket of
positions for that blood group in the hash map (O(1) average), then
reads each donor at those positions, skipping deleted ones (O(k)).

**`DonorManager::updateDonor(...)`** — finds the donor by direct index,
and — only if the blood group actually changed — removes its position
from the old bucket and adds it to the new one, so the index stays
accurate.

**`DonorManager::deleteDonor(...)`** — sets `deleted = true` and
`available = false`. Does **not** erase from the vector (see Part 3, Q6).

**`DonorManager::findAvailableDonor(...)`** — the core of Emergency
Matching. Looks up the blood-group bucket, then returns the first donor
in it that is both available and not deleted.

**`RequestManager::addRequest(...)`** — pushes a new `BloodRequest`
into the priority queue. O(log n) heap insert.

**`RequestManager::viewRequests()`** — the "drain and restore" trick:
copies the queue, pops everything off it (which comes out in priority
order automatically) into a vector for display, and the original queue
is left untouched (only the copy was drained). See Part 3, Q4.

**`RequestManager::matchDonor(...)`** — calls
`DonorManager::findAvailableDonor`, marks that donor unavailable via
`markDonated`, and appends a new `DonationRecord` to the history list.

---

## Part 3 — Expected Viva Questions & Answers

**Q1: Why `vector` for donor storage instead of an array?**
A `vector` grows dynamically — we don't know the number of donors in
advance, and a plain array would need a fixed, guessed size. `vector`
also gives us `.size()`, bounds-checked access with `.at()`, and clean
iteration, all of which a raw array doesn't.

**Q2: Why `unordered_map` instead of `map`?**
`map` is a red-black tree — O(log n) per operation and keeps keys
sorted. We never need sorted blood groups, so `unordered_map`'s average
O(1) hashing is strictly faster for our use case.

**Q3: What's the worst-case complexity of `unordered_map`?**
O(n), if every key collides into the same bucket. In practice this
doesn't happen — the standard library's hash function spreads keys
well and resizes automatically as load factor increases.

**Q4: Why can't you just iterate the `priority_queue` directly to show all pending requests?**
`std::priority_queue` intentionally exposes no iterator — only
`top()`, `push()`, and `pop()`. To display every element, you must pop
them all out. So `viewRequests()` copies the queue first, drains the
*copy*, and displays it — the real queue is never touched.

**Q5: How does the `priority_queue` know CRITICAL should come before LOW?**
`std::priority_queue` is a max-heap by default — the largest value
stays on top. Our urgency levels are numbered the opposite way
(1 = CRITICAL is the smallest number but the highest priority), so the
`CompareRequest` comparator inverts the comparison. Ties are then
broken by `arrivalOrder` so equal-urgency requests stay FIFO.

**Q6: Why "soft delete" instead of actually removing a donor from the vector?**
Donor IDs are sequential, and the whole reason `getDonorById` can be
O(1) is that donor ID N is *always* at vector index N-1. If we erased
an element, every donor after it would shift down by one position and
that guarantee would break — we'd need to either re-index everyone
(expensive) or add a second `unordered_map<int,int>` just for ID
lookup (which is exactly the redundant complexity we were trying to
avoid). Flipping a `deleted` flag is O(1) and keeps the invariant intact.

**Q7: Doesn't soft-delete waste memory forever?**
At the scale of a college project (tens or hundreds of donors) this is
completely negligible. It's a real trade-off worth naming honestly if
asked — for a production system handling millions of records you'd
likely periodically compact deleted entries, which is exactly the kind
of "future enhancement" conversation this project is meant to invite.

**Q8: Why does Emergency Matching not pull from the `pendingRequests` priority_queue?**
They're deliberately two separate features. "Add Blood Request" /
"View Requests" is a prioritized *record* of requests for tracking —
it demonstrates the priority_queue. "Emergency Matching" is an
immediate, on-the-spot action: you type a blood group and get a donor
right now. Keeping them decoupled means each data structure has
exactly one clear job, which is easier to explain than one feature
doing double duty.

**Q9: Why `list` for history and not `vector`?**
History is append-only and always read front-to-back — we never need
random access or insertion in the middle. `list` avoids the
reallocation/copy cost a `vector` pays when it outgrows its capacity,
and is the textbook fit for a sequential, append-only log.

**Q10: What is the time complexity of every core operation?**
See the table in `PROJECT_REPORT.md` §4.5 — know it without looking.

**Q11: Why no blood-group compatibility rules (e.g. O- as universal donor)?**
A deliberate scope decision to keep matching logic — and its
explanation — exact and simple. Named explicitly as a future
enhancement, not hidden as an oversight.

**Q12: Why no login system?**
The system has exactly one user (the admin operating the console), so
authentication would add complexity without adding to the DSA content
being demonstrated.

**Q13: How is data persisted?**
Plain `|`-delimited text files, one record per line, via `ifstream`/
`ofstream`. No database — kept simple deliberately, since the DSA
logic is the point of the project, not storage engineering.

**Q14: What happens if two requests have the same urgency level?**
The `arrivalOrder` field (an incrementing counter) breaks the tie —
whichever request was created first is served first.

---

## Part 4 — Suggested Demo Script (3–5 minutes)

1. Launch the program — point out it auto-loaded the sample donors from `data/`.
2. **Donor Management → View All Donors** — show the 6 seeded donors.
3. **Donor Management → Register Donor** — add one live.
4. **Donor Management → Search by Blood Group** — search `O+`, show 2 results.
5. **Donor Management → Update Donor** — change a donor's city.
6. **Blood Request → Add Request** — add a CRITICAL request for a blood group with no donor (e.g. `AB-`).
7. **Blood Request → View Requests** — show it's sorted with CRITICAL requests first, even if added last.
8. **Emergency Matching** — enter a blood group that *does* have a donor (e.g. `O+`) — show the match.
9. **Emergency Matching** again with the *same* blood group — if only one donor existed, show "No available donor found" (this demonstrates the donor was correctly marked unavailable).
10. **Donation History** — show the completed match logged.
11. **Reports** — show totals, availability, and blood-group counts update correctly.
12. **Save Data**, then restart the program — show everything reloads correctly.

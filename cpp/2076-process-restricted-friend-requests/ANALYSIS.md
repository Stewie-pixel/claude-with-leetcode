# Union‑Find (Disjoint Set Union)

## Video Solution

For more details about **Process Restricted Friend Requests**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=EVnIVVF-Jfs).

## Concept

Union‑Find (also called Disjoint Set Union, DSU) maintains a collection of disjoint sets and supports two operations efficiently:

* **find(x)** – returns the representative (root) of the set containing *x*.
* **union(x, y)** – merges the sets containing *x* and *y*.

With path compression and union by rank (or size) both operations run in almost‑constant amortized time ≈ O(α(n)), where α is the inverse Ackermann function.

In this problem we cannot freely union two people if doing so would make any restricted pair end up in the same component. The trick is to keep, for each component, the set of other components it is *restricted* to. Before merging two components we check whether their restriction sets intersect; if they do, the request must be rejected.

## When to Use It

Use Union‑Find when you need to handle **dynamic connectivity** queries (are two nodes in the same component?) together with **updates that merge components**. Typical clues:

* “friend request”, “network”, “connected components”, “merge groups”.
* There are **constraints that forbid certain merges** (e.g., restricted pairs, enemies, incompatible items).
* Queries are processed **online** (in given order) and earlier merges affect later ones.

If the problem only asks about static connectivity after all edges are added, a simple DFS/BFS suffices. When merges happen interleaved with queries, Union‑Find is the go‑to technique.

## Template

Below is a minimal DSU skeleton with path compression and union by rank. In the lecture we extend it with a `restriction_set` per root.

```python
class DSU:
    def __init__(self, n):
        self.parent = list(range(n))
        self.rank   = [0] * n
        # each component starts with an empty restriction set
        self.restricted = [set() for _ in range(n)]

    def find(self, x):
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]

    def union(self, x, y):
        xr, yr = self.find(x), self.find(y)
        if xr == yr:
            return False               # already together
        # union by rank
        if self.rank[xr] < self.rank[yr]:
            xr, yr = yr, xr
        self.parent[yr] = xr
        if self.rank[xr] == self.rank[yr]:
            self.rank[xr] += 1
        # merge restriction sets (small‑to‑large)
        if len(self.restricted[xr]) < len(self.restricted[yr]):
            xr, yr = yr, xr
        self.restricted[xr].update(self.restricted[yr])
        self.restricted[yr].clear()
        return True
```

## LeetCode Problem Walkthrough

### Problem: 2076. Process Restricted Friend Requests
https://leetcode.com/problems/process-restricted-friend-requests/

---

### Approach 1: Brute Force – Copy DSU per Request

**Algorithm**
For each friend request `[u, v]`:
1. If `u` and `v` are already in the same component → request succeeds.
2. Otherwise, **temporarily** unite their components in a copy of the DSU.
3. Scan every restriction `[a, b]`; if `find(a) == find(b)` in the copied DSU, the union would create a forbidden connection → reject.
4. If no restriction is violated, apply the union to the real DSU and mark the request as successful.

**Implementation**
```python
from typing import List

class Solution:
    def friendRequests(self, n: int,
                       restrictions: List[List[int]],
                       requests: List[List[int]]) -> List[bool]:
        # basic DSU without restriction tracking
        parent = list(range(n))
        rank   = [0] * n

        def find(x):
            while parent[x] != x:
                parent[x] = parent[parent[x]]
                x = parent[x]
            return x

        def union(x, y):
            xr, yr = find(x), find(y)
            if xr == yr:
                return
            if rank[xr] < rank[yr]:
                xr, yr = yr, xr
            parent[yr] = xr
            if rank[xr] == rank[yr]:
                rank[xr] += 1

        answer = []
        for u, v in requests:
            ru, rv = find(u), find(v)
            if ru == rv:                     # already friends
                answer.append(True)
                continue

            # try union in a temporary copy
            temp_parent = parent[:]
            temp_rank   = rank[:]

            def temp_find(x):
                while temp_parent[x] != x:
                    temp_parent[x] = temp_parent[temp_parent[x]]
                    x = temp_parent[x]
                return x

            def temp_union(x, y):
                xr, yr = temp_find(x), temp_find(y)
                if xr == yr:
                    return
                if temp_rank[xr] < temp_rank[yr]:
                    xr, yr = yr, xr
                temp_parent[yr] = xr
                if temp_rank[xr] == temp_rank[yr]:
                    temp_rank[xr] += 1

            temp_union(u, v)

            blocked = False
            for a, b in restrictions:
                if temp_find(a) == temp_find(b):
                    blocked = True
                    break

            if not blocked:
                union(u, v)               # apply to real DSU
                answer.append(True)
            else:
                answer.append(False)

        return answer
```

**Complexity Analysis**
*Time*: For each of `q` requests we copy the DSU (`O(n)`) and then scan all `r` restrictions, each `find` costing nearly O(1). → **O(q·(n + r))**.  
*Space*: The DSU arrays plus a temporary copy → **O(n)**.

---

### Approach 2: Intermediate – Track Restriction Sets per Component (no small‑to‑large)

**Intuition**
Instead of copying the DSU for every request, we can answer the “would this union break any restriction?” question by looking at the *current* components. If we know, for each component, which other components it is restricted to, then merging `C_u` and `C_v` is safe **iff** there is **no** restriction linking a member of `C_u` to a member of `C_v`. This reduces the per‑request work to a set‑intersection test.

**Algorithm**
1. Initialise a DSU where each node also stores a `set` of restricted component roots.
2. Populate those sets from the `restrictions` list: for each `[x, y]` add `find(y)` to the restriction set of `find(x)` and vice‑versa.
3. For each request `[u, v]`:
   * Find roots `ru`, `rv`. If equal → success.
   * Check whether `rv` appears in `ru`’s restriction set **or** `ru` appears in `rv`’s set. If yes → request fails.
   * Otherwise, union the two components and **merge** their restriction sets (union‑by‑size to keep it cheap).

**Implementation**
```python
from typing import List, Set

class Solution:
    def friendRequests(self, n: int,
                       restrictions: List[List[int]],
                       requests: List[List[int]]) -> List[bool]:
        parent = list(range(n))
        rank   = [0] * n
        # restriction_sets[root] = set of roots that this component cannot be united with
        restriction_sets: List[Set[int]] = [set() for _ in range(n)]

        def find(x):
            while parent[x] != x:
                parent[x] = parent[parent[x]]
                x = parent[x]
            return x

        def union(x, y):
            xr, yr = find(x), find(y)
            if xr == yr:
                return
            # union by rank
            if rank[xr] < rank[yr]:
                xr, yr = yr, xr
            parent[yr] = xr
            if rank[xr] == rank[yr]:
                rank[xr] += 1
            # merge restriction sets (small‑to‑large)
            if len(restriction_sets[xr]) < len(restriction_sets[yr]):
                xr, yr = yr, xr
            restriction_sets[xr].update(restriction_sets[yr])
            restriction_sets[yr].clear()

        # 1️⃣ Fill initial restriction information
        for a, b in restrictions:
            ra, rb = find(a), find(b)
            if ra != rb:
                restriction_sets[ra].add(rb)
                restriction_sets[rb].add(ra)

        answer = []
        for u, v in requests:
            ru, rv = find(u), find(v)
            if ru == rv:
                answer.append(True)
                continue

            # 2️⃣ Check if merging would violate any restriction
            if rv in restriction_sets[ru] or ru in restriction_sets[rv]:
                answer.append(False)
                continue

            # 3️⃣ Safe to unite
            union(u, v)
            answer.append(True)

        return answer
```

**Complexity Analysis**
*Time*:
  * Building initial restriction sets: O(r·α(n)).
  * Each request: two `find` calls (≈ O(1)), a set‑lookup O(1), and a union that merges two restriction sets.  
    Using union‑by‑size on the sets guarantees each restriction entry moves at most O(log n) times, giving an amortized O(α(n) + log n) per request.  
    Overall **O((n + r + q)·α(n) + q·log n)**, which in practice is near‑linear.
*Space*: O(n + r) for DSU arrays and restriction sets.

---

### Approach 3: Most Optimal – Union‑Find with Restriction Sets + Small‑to‑Large + Path Compression

**Intuition**
Approach 2 already gives near‑optimal performance. The final polish is to ensure:
* **Path compression** in `find` (already present).
* **Union by rank/size** for the DSU tree.
* **Small‑to‑large merging** of the restriction sets so that each restriction entry is moved only O(log n) times over the whole sequence.
These three tricks together give the classic **inverse‑Ackermann** amortized bound for DSU operations, making the algorithm essentially linear for the given constraints (n, r, q ≤ 1000).

**Implementation**
The code below is identical to Approach 2 but highlights the three optimizations in comments.

```python
from typing import List, Set

class Solution:
    def friendRequests(self, n: int,
                       restrictions: List[List[int]],
                       requests: List[List[int]]) -> List[bool]:
        parent = list(range(n))
        rank   = [0] * n
        # restriction_sets[root] holds the roots this component is forbidden to merge with
        restriction_sets: List[Set[int]] = [set() for _ in range(n)]

        def find(x: int) -> int:
            # ----> PATH COMPRESSION <----
            if parent[x] != x:
                parent[x] = find(parent[x])
            return parent[x]

        def union(x: int, y: int) -> None:
            xr, yr = find(x), find(y)
            if xr == yr:
                return
            # ----> UNION BY RANK <----
            if rank[xr] < rank[yr]:
                xr, yr = yr, xr
            parent[yr] = xr
            if rank[xr] == rank[yr]:
                rank[xr] += 1
            # ----> SMALL‑TO‑LARGE MERGE OF RESTRICTION SETS <----
            if len(restriction_sets[xr]) < len(restriction_sets[yr]):
                xr, yr = yr, xr
            restriction_sets[xr].update(restriction_sets[yr])
            restriction_sets[yr].clear()

        # Populate initial restriction information
        for a, b in restrictions:
            ra, rb = find(a), find(b)
            if ra != rb:
                restriction_sets[ra].add(rb)
                restriction_sets[rb].add(ra)

        ans: List[bool] = []
        for u, v in requests:
            ru, rv = find(u), find(v)
            if ru == rv:
                ans.append(True)
                continue

            # If either side sees the other as restricted, request fails
            if rv in restriction_sets[ru] or ru in restriction_sets[rv]:
                ans.append(False)
                continue

            union(u, v)
            ans.append(True)

        return ans
```

**Complexity Analysis**
*Time*:  
- `find` with path compression: **O(α(n))** amortized.  
- `union` includes rank‑based tree merging (**O(1)**) and small‑to‑large set merging, which guarantees each restriction element moves at most **O(log n)** times.  
- Overall **O((n + r + q)·α(n) + q·log n)** → practically linear for the limits (≤ 1000).  

*Space*: **O(n + r)** for parent, rank, and restriction sets.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We walk through the first example to see how the algorithm decides.

**Example 1**  
`n = 3`  
`restrictions = [[0, 1]]`  
`requests = [[0, 2], [2, 1]]`

#### Initialization
- Each node is its own component: `{0}`, `{1}`, `{2}`.
- Restriction sets:
  - comp 0 → {1}
  - comp 1 → {0}
  - comp 2 → {}

#### Request 0: `[0, 2]`
- `find(0) = 0`, `find(2) = 2` (different).
- Check restrictions:
  - Is `2` in set of `0`? No (`{1}`).
  - Is `0` in set of `2`? No (`{}`).
- Safe → union `0` and `2`.
  - New component root = `0` (by rank).
  - Merge restriction sets: `{0}` gets `{1}` ∪ `{}` = `{1}`.
  - Component `{0,2}` now has restriction set `{1}`.
- Result: **true**.

#### Request 1: `[2, 1]`
- `find(2)` → root `0` (since 2 united with 0).  
  `find(1)` → root `1`.
- Different components: `0` vs `1`.
- Check restrictions:
  - Is `1` in set of component `0`? **Yes** (`{1}`) → violation.
  - (Symmetrically, `0` is in set of component `1` as well.)
- Request fails → **false**.
- No union performed.

**Final answer**: `[true, false]`, matching the expected output.

---
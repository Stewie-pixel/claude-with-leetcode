# Greedy

## Video Solution

For more details about **Maximize Sum of Device Ratings**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=pnHsoNK3Nio)

## Concept

Greedy algorithms build a solution step‑by‑step, making the locally optimal choice at each step with the hope that these choices lead to a globally optimal solution.  
The key idea is to identify a decision that can be made without jeopardizing future decisions – often by sorting, scanning, or maintaining a simple invariant.

## When to Use It

- The problem asks for an **optimal value** (maximum/minimum, longest/shortest, etc.) and the decision at one step does not affect the feasibility of later steps.
- You can sort or order the elements and then process them in that order while keeping track of a running value (e.g., a sum, a count, a threshold).
- The solution can be expressed as “take the best available option now and never reconsider it.”
- Typical clues:  
  - “You may perform the operation any number of times …”  
  - “Choose … that has not been used before” (each item can be used at most once)  
  - “Return the maximum possible …”  

## Template

```python
def greedy_solve(data):
    # 1. Put the data in an order that lets us make a safe local choice
    data.sort(key=...)                     # or use a heap / two pointers
    
    # 2. Initialise the answer / state
    ans = 0
    state = ...                            # e.g., current sum, last taken value
    
    # 3. Scan the ordered data and apply the greedy rule
    for x in data:
        if can_take(x, state):             # local optimality test
            ans += value(x)                # update answer
            state = update(state, x)       # maintain invariant
    
    # 4. Return the result
    return ans
```

---

## LeetCode Problem Walkthrough

### Problem: 3961. Maximize Sum of Device Ratings

https://leetcode.com/problems/maximize-sum-of-device-ratings/

### Approach 1: Brute Force

**Algorithm**  
Try every possible choice of source devices and, for each source, every unit that could be removed and every destination for that unit.  
For each configuration compute the resulting ratings and keep the maximum sum.

**Implementation**

```python
from itertools import product, combinations
from math import inf

def maxRatings_brute(units):
    m, n = len(units), len(units[0])
    best = 0
    # which devices are used as sources
    for src_mask in range(1 << m):
        src_devices = [i for i in range(m) if src_mask & (1 << i)]
        # for each source, pick a unit index to remove
        for rem in product(range(n), repeat=len(src_devices)):
            # build temporary copy after removals
            tmp = [row[:] for row in units]
            for dev, idx in zip(src_devices, rem):
                tmp[dev].pop(idx)          # remove exactly one unit
            # now distribute the removed units arbitrarily;
            # to maximise the sum of mins we would give each removed unit
            # to the device whose current min is smallest (greedy inside)
            # but for a true brute force we would also try all destinations –
            # which is exponential and omitted here for brevity.
            # This illustrates that even a simplified brute force is already
            # infeasible for the given constraints.
            cur = sum(min(row) if row else 0 for row in tmp)
            best = max(best, cur)
    return best
```

**Complexity Analysis**

- Time complexity: O(2^m * n^k * …) – exponential in the number of devices and units; infeasible for m·n ≤ 2·10^5.  
- Space complexity: O(m·n) to hold a copy of the matrix.

---

### Approach 2: Intermediate Optimization (Greedy on Min & Second Min)

**Intuition**  
Only the *minimum* value of a device matters for its rating.  
When we remove a unit from a device we have two options:

1. Remove a unit **strictly larger** than the current minimum – the rating does **not** change.  
2. Remove the current minimum – the rating may drop to the *second smallest* value (or become 0 if the device size becomes 0).

Thus each device can contribute **at most one** unit to the pool without hurting its rating, namely any unit > min.  
If we are forced to take the minimum (because all units are equal), the rating falls to the second min.

Consequently, the problem reduces to:
- Every device `i` has a `min1[i]` (smallest) and `min2[i]` (second smallest, ∞ if size = 1).  
- We may optionally take one unit from each device.  
  - If we take a “safe” unit (value > min1), the device’s rating stays `min1[i]`.  
  - If we take the minimum, the rating becomes `min2[i]`.  
- All taken units form a pool that we can distribute arbitrarily; giving a unit to a device can only **increase** its rating if the unit’s value ≥ its current rating, otherwise it has no effect.  
  Hence, to maximise the sum of ratings we will give each taken unit to the device whose rating is currently smallest, raising it as much as possible.

A known optimal strategy for this kind of “take‑one‑from‑each‑and‑re‑distribute” problem is:
- Keep the sum of all second minima (`sumMin2`).  
- From one device we will *sacrifice* its second minimum (use it as the unit we must take as a minimum).  
- All other devices contribute their second minimum safely, leaving their rating at their original minimum.  
- The device we sacrifice ends up with rating equal to its **global minimum** among all first minima (`G`).  

The best device to sacrifice is the one whose second minimum is smallest, because we lose the least from `sumMin2`.

**Algorithm**
1. For each device compute `min1` (smallest) and `min2` (second smallest).  
2. Track:
   - `G = min(min1 over all devices)`  
   - `sumMin2 = Σ min2`  
   - `minMin2 = min(min2 over all devices)`  
3. Answer = `G + sumMin2 - minMin2`.

**Implementation**

```python
class Solution:
    def maxRatings(self, units: List[List[int]]) -> int:
        m = len(units)
        n = len(units[0])

        # Special case: each device has only one unit → we cannot move anything
        if n == 1:
            return sum(device[0] for device in units)

        G = float('inf')          # global minimum of all first minima
        sumMin2 = 0               # sum of all second minima
        minMin2 = float('inf')    # smallest second minimum

        for dev in units:
            min1 = float('inf')
            min2 = float('inf')
            for v in dev:
                if v <= min1:
                    min2 = min1
                    min1 = v
                elif v < min2:
                    min2 = v
            G = min(G, min1)
            sumMin2 += min2
            minMin2 = min(minMin2, min2)

        # If a device has size 1, its second minimum is ∞;
        # such a device can never be the source of a “forced” min removal.
        # The formula still works because minMin2 will be ∞ only if all
        # devices have size 1, which we handled earlier.
        return int(G + sumMin2 - minMin2)
```

**Complexity Analysis**

- Time complexity: O(m·n) – one pass to find the two smallest values in each device.  
- Space complexity: O(1) – only a few scalar variables are used.

---

### Approach 3: Most Optimal (Same as Approach 2, presented as the final answer)

**Intuition**  
The intermediate optimisation already yields the optimal formula; no further improvement is possible because:
- The global minimum `G` is unavoidable: at least one device must lose its smallest unit (otherwise we would have taken zero units from every device, which cannot increase any rating).  
- All other devices can keep their original minimum by giving away a unit larger than their current minimum (if such a unit exists).  
- The only loss incurred is the second minimum of the device we choose to sacrifice; picking the smallest second minimum minimises this loss.

Thus the formula derived above is provably optimal.

**Implementation**  
*(Identical to Approach 2 – shown here for completeness.)*

```python
class Solution:
    def maxRatings(self, units: List[List[int]]) -> int:
        m, n = len(units), len(units[0])
        if n == 1:
            return sum(d[0] for d in units)

        G = float('inf')
        sumMin2 = 0
        minMin2 = float('inf')

        for dev in units:
            min1 = min2 = float('inf')
            for v in dev:
                if v <= min1:
                    min2 = min1
                    min1 = v
                elif v < min2:
                    min2 = v
            G = min(G, min1)
            sumMin2 += min2
            minMin2 = min(minMin2, min2)

        return int(G + sumMin2 - minMin2)
```

**Complexity Analysis**

- Time complexity: O(m·n) – linear in the total number of entries (≤ 2·10⁵).  
- Space complexity: O(1).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the first example:

**Input:** `units = [[1,3], [2,2]]`

| Device | elements | min1 | min2 |
|--------|----------|------|------|
| 0      | [1, 3]   | 1    | 3    |
| 1      | [2, 2]   | 2    | 2    |

**Step‑by‑step computation**

```
Initialize: G = +∞, sumMin2 = 0, minMin2 = +∞

Process device 0:
    min1 = 1, min2 = 3
    G = min(+∞, 1) = 1
    sumMin2 = 0 + 3 = 3
    minMin2 = min(+∞, 3) = 3

Process device 1:
    min1 = 2, min2 = 2
    G = min(1, 2) = 1
    sumMin2 = 3 + 2 = 5
    minMin2 = min(3, 2) = 2

Final values:
    G = 1
    sumMin2 = 5
    minMin2 = 2

Answer = G + sumMin2 - minMin2 = 1 + 5 - 2 = 4
```

The result matches the expected output `4`.  
The intuition: we sacrifice device 0’s second minimum (3) as the unit we must take as a minimum; device 0’s rating becomes its global minimum `1`. All other devices keep their original minima, and we add the sacrificed unit to the pool, which does not hurt any rating. The total rating sum is `1 (device 0) + 2 (device 1) = 3` from the unchanged minima plus the rescued `1` from the sacrificed unit given to device 0, yielding `4`.

---
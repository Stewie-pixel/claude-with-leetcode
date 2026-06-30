# Greedy

## Video Solution

For more details about **Maximum Building Height**, watch the walkthrough at [https://www.youtube.com/watch?v=SQer5cwy4lQ](https://www.youtube.com/watch?v=SQer5cwy4lQ)

## Concept

Greedy algorithms make locally optimal choices at each step with the hope of finding a global optimum.  
In this problem the local choice is: **given a height limit at a building, the tallest possible height at any neighboring building is limited by +1 per step**.  
By propagating these limits from left to right and then right to left we obtain the tightest possible upper bound for every restricted building. The highest achievable height will either be at a restricted building or somewhere in the gap between two restrictions, where the shape of the feasible region is an upside‑down V.

## When to Use It

Use a greedy “propagation‑and‑peak” pattern when you see:

* A sequence of positions with a constraint that the value can change by at most `k` between adjacent positions (here `k = 1`).  
* Individual positions have explicit upper bounds (`maxHeight`).  
* You need the maximum possible value anywhere in the sequence.  
* The constraints are monotone – tightening a bound never makes a previously feasible solution infeasible.

## Template

```python
# Greedy two‑pass + peak template
def solve(n, restrictions):
    # 1️⃣  Add the implicit restriction for building 1 (height 0)
    restrictions.append([1, 0])
    restrictions.sort()                     # O(m log m)

    # 2️⃣  Forward pass: enforce left‑to‑right limit
    for i in range(1, len(restrictions)):
        dist = restrictions[i][0] - restrictions[i-1][0]
        restrictions[i][1] = min(restrictions[i][1],
                                 restrictions[i-1][1] + dist)

    # 3️⃣  Backward pass: enforce right‑to‑left limit
    for i in range(len(restrictions)-2, -1, -1):
        dist = restrictions[i+1][0] - restrictions[i][0]
        restrictions[i][1] = min(restrictions[i][1],
                                 restrictions[i+1][1] + dist)

    # 4️⃣  Compute answer: max at a restriction or in between two restrictions
    ans = restrictions[-1][1] + (n - restrictions[-1][0])   # tail after last restriction
    for i in range(len(restrictions)-1):
        x1, h1 = restrictions[i]
        x2, h2 = restrictions[i+1]
        # highest point of the upside‑down V between them
        peak = (h1 + h2 + (x2 - x1)) // 2
        ans = max(ans, peak)
    return ans
```

---

## LeetCode Problem Walkthrough

### Problem: 1840. Maximum Building Height
https://leetcode.com/problems/maximum-building-height/

### Approach 1: Brute Force

**Algorithm**  
1. Iterate over every possible height `H` from `0` to `n‑1` (the tallest we could ever need).  
2. For each `H`, check whether a valid assignment exists where the tallest building is exactly `H`.  
   * Start with `height[1] = 0`.  
   * Move left‑to‑right, setting each building to the minimum of its restriction (if any) and `prev_height + 1`.  
   * Move right‑to‑left, tightening each building to `min(current, next_height + 1)`.  
   * After both passes, verify that every restriction is satisfied and that the maximum height equals `H`.  
3. Return the largest `H` that passes the check.

**Implementation**

```python
def maxBuilding_bruteforce(n, restrictions):
    # helper: can we achieve max height == target ?
    def feasible(target):
        # start with all heights = +inf, then apply constraints
        height = [float('inf')] * (n + 1)   # 1‑indexed
        height[1] = 0                       # first building fixed

        # apply explicit restrictions
        for idx, limit in restrictions:
            height[idx] = min(height[idx], limit)

        # left‑to‑right pass
        for i in range(2, n + 1):
            height[i] = min(height[i], height[i-1] + 1)

        # right‑to‑left pass
        for i in range(n-1, 0, -1):
            height[i] = min(height[i], height[i+1] + 1)

        # check restrictions and compute actual max
        actual_max = 0
        for i in range(1, n + 1):
            actual_max = max(actual_max, height[i])
            for idx, limit in restrictions:
                if i == idx and height[i] > limit:
                    return False
        return actual_max == target

    for H in range(n, -1, -1):          # try from high to low
        if feasible(H):
            return H
    return 0
```

**Complexity Analysis**

- Time complexity: `O(n * (n + r))` – for each height we scan the whole array (`n`) and apply restrictions (`r`). In the worst case this is `O(n²)` and impossible for `n ≤ 10⁹`.  
- Space complexity: `O(n)` – the height array.

---

### Approach 2: Forward / Backward Propagation (Greedy)

**Intuition**  
Instead of guessing the answer, we directly compute the *tightest* upper bound each building can have by propagating the constraints from both ends.  
If we know the maximum allowed height at building `i`, then building `i+1` cannot exceed `height[i] + 1`. Doing this left‑to‑right gives the best we can achieve from the start; a right‑to‑left pass does the same from the end. After both passes, every building’s value is the highest it can possibly be while respecting *all* restrictions.

**Algorithm**  
1. Append the implicit restriction `[1, 0]` and sort by position.  
2. Forward pass: for each consecutive pair, shrink the right building’s limit to `min(current, previous + distance)`.  
3. Backward pass: do the same from right to left.  
4. The answer is the maximum of:  
   * the height at the last restriction plus the remaining distance to `n` (the tail can keep increasing by 1 each step), and  
   * for every gap between two restrictions, the peak of the upside‑down V that can be formed: `(h1 + h2 + distance) // 2`.

**Implementation**

```python
def maxBuilding_greedy(n, restrictions):
    restrictions.append([1, 0])          # building 1 must be 0
    restrictions.sort()                 # sort by position

    m = len(restrictions)

    # left‑to‑right propagation
    for i in range(1, m):
        dist = restrictions[i][0] - restrictions[i-1][0]
        restrictions[i][1] = min(restrictions[i][1],
                                 restrictions[i-1][1] + dist)

    # right‑to‑left propagation
    for i in range(m-2, -1, -1):
        dist = restrictions[i+1][0] - restrictions[i][0]
        restrictions[i][1] = min(restrictions[i][1],
                                 restrictions[i+1][1] + dist)

    # compute answer
    ans = restrictions[-1][1] + (n - restrictions[-1][0])   # tail segment
    for i in range(m-1):
        x1, h1 = restrictions[i]
        x2, h2 = restrictions[i+1]
        peak = (h1 + h2 + (x2 - x1)) // 2
        ans = max(ans, peak)
    return ans
```

**Complexity Analysis**

- Time complexity: `O(m log m)` – sorting dominates; the two passes are linear. (`m = len(restrictions) ≤ 10⁵`)  
- Space complexity: `O(m)` – the updated restrictions list (in‑place).

---

### Approach 3: Brute Force with Early Exit (Illustrative Only)

**Intuition**  
The brute‑force method is useful for understanding why a greedy solution works: we can stop checking once we find a feasible height because we scan from high to low. However, it remains impractical for large `n`, which motivates the greedy optimization.

**Implementation** – same as Approach 1 but with a small optimization: break as soon as a feasible height is found (already shown).

**Complexity Analysis** – identical to Approach 1; included only for completeness.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the greedy algorithm on **Example 3**:

```
n = 10
restrictions = [[5,3],[2,5],[7,4],[10,3]]
```

**Step 0 – add building 1 restriction & sort**

| id | maxHeight |
|----|-----------|
| 1  | 0         |
| 2  | 5         |
| 5  | 3         |
| 7  | 4         |
|10  | 3         |

**Step 1 – forward pass**

| i | dist | new maxHeight[i] (min(old, prev+dist)) |
|---|------|----------------------------------------|
|1→2| 1    | min(5, 0+1) = 1 |
|2→5| 3    | min(3, 1+3) = 3 |
|5→7| 2    | min(4, 3+2) = 4 |
|7→10|3    | min(3, 4+3) = 3 |

Result after forward: `[0,1,3,4,3]`

**Step 2 – backward pass**

| i | dist | new maxHeight[i] (min(curr, next+dist)) |
|---|------|------------------------------------------|
|10←7|3    | min(3, 3+3) = 3 |
|7←5 |2    | min(4, 3+2) = 4 |
|5←2 |3    | min(3, 4+3) = 3 |
|2←1 |1    | min(1, 3+1) = 1 |

Result after backward (unchanged): `[0,1,3,4,3]`

**Step 3 – compute answer**

*Tail after last restriction:* `3 + (10‑10) = 3`  
*Peak between restrictions:*
- (1,0) & (2,1): `(0+1+1)//2 = 1`
- (2,1) & (5,3): `(1+3+3)//2 = 3`
- (5,3) & (7,4): `(3+4+2)//2 = 4`
- (7,4) & (10,3): `(4+3+3)//2 = 5`

Maximum = **5**, which matches the expected output.

--- 

### Summary

* The greedy two‑pass propagation gives the tightest possible height limits for every restricted building.  
* The tallest building will either sit at a restriction or at the apex of an upside‑down V formed between two restrictions, computable with `(h1 + h2 + distance)//2`.  
* This yields an `O(m log m)` solution that easily handles `n` up to `10⁹` and up to `10⁵` restrictions.  

Commit this lecture as `/lectures/2025-09-26-greedy.md`.
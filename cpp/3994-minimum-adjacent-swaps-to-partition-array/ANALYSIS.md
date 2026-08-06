# Greedy

## Video Solution

For more details about **Minimum Adjacent Swaps to Partition Array**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=UlvnPb0fRlE).

## Concept

Greedy algorithms make a locally optimal choice at each step with the hope of finding a global optimum.  
In this problem we treat each element’s “desired region” as a priority:

- **Low** (`< a`) must end up left of everything else.  
- **Mid** (`[a, b]`) must be after all lows but before all highs.  
- **High** (`> b`) must be rightmost.

When we scan the array left‑to‑right we can decide **greedily** how many swaps the current element needs to reach its correct region given everything we have already seen.  
If the current element is low, every previously seen mid or high element must be crossed (each crossing costs one adjacent swap).  
If it is mid, only previously seen highs need to be crossed.  
Highs need none.  
Summing these contributions yields the minimal number of adjacent swaps.

## When to Use It

Use a greedy counting strategy when you can:

- Define a total order on items (here: low < mid < high).  
- Express the cost of placing an item as a function of items already processed (e.g., how many higher‑priority items are before it).  
- Show that moving an item past those higher‑priority items is unavoidable and locally optimal leads to a global optimum.

Typical clues:  
- “minimum adjacent swaps to …”  
- “make the array satisfy … ordering”  
- cost equals number of inversions with respect to some ranking.

## Template

```python
# Greedy counting template for ordering problems
def greedy_count(nums, rank):
    """
    nums: list of items
    rank: function that maps an item to its priority (lower = earlier)
    Returns minimal adjacent swaps to sort by rank.
    """
    # counts of each rank seen so far
    seen = [0, 0, 0]          # we know there are exactly three ranks here
    swaps = 0
    for x in nums:
        r = rank(x)           # 0 = low, 1 = mid, 2 = high
        # all previously seen items with higher rank must be crossed
        for higher in range(r + 1, 3):
            swaps += seen[higher]
        seen[r] += 1
    return swaps % (10**9 + 7)
```

---

## LeetCode Problem Walkthrough

### Problem: ???. Minimum Adjacent Swaps to Partition Array  
https://leetcode.com/problems/minimum-adjacent-swaps-to-partition-array/

### Approach 1: Brute Force (Simulate bubble sort)

**Algorithm**

1. Define a comparison function that orders elements by the three regions: low < mid < high.  
2. Run a bubble‑sort‑like pass: whenever a pair is out of order, swap them and increment a counter.  
3. The total number of swaps performed equals the minimum adjacent swaps because bubble sort only swaps adjacent inverted pairs and each swap removes exactly one inversion.  

**Implementation**

```python
class Solution:
    def minAdjacentSwaps(self, nums: List[int], a: int, b: int) -> int:
        def rank(x):
            if x < a:
                return 0          # low
            if x <= b:
                return 1          # mid
            return 2              # high

        arr = nums[:]            # work on a copy
        swaps = 0
        n = len(arr)
        for i in range(n):
            for j in range(0, n - i - 1):
                if rank(arr[j]) > rank(arr[j + 1]):   # out of order
                    arr[j], arr[j + 1] = arr[j + 1], arr[j]
                    swaps += 1
        return swaps % (1_000_000_007)
```

**Complexity Analysis**

- Time complexity: **O(n²)** — two nested loops over the array.  
- Space complexity: **O(n)** — the copy of the array (could be O(1) if we swap in‑place, but we keep a copy to avoid modifying the input).

---

### Approach 2: Intermediate Optimization (Fenwick Tree / BIT)

**Intuition**

The answer equals the number of *inversions* with respect to the rank ordering: for each element, count how many previously seen elements have a higher rank.  
A Binary Indexed Tree (Fenwick) lets us query prefix sums and update counts in O(log M) time, where M is the number of distinct ranks (here M = 3).  
Even though M is tiny, the BIT illustrates the standard technique for larger rank sets and runs in O(n log M).

**Algorithm**

1. Map each value to its rank (0, 1, 2).  
2. Initialise a BIT of size 3.  
3. Iterate through the array:  
   - Query the BIT for the sum of counts of ranks **greater** than the current rank (i.e., total seen – prefix sum up to current rank).  
   - Add that to the answer.  
   - Update the BIT at the current rank by +1.  
4. Return answer modulo 1e9+7.

**Implementation**

```python
class Solution:
    def minAdjacentSwaps(self, nums: List[int], a: int, b: int) -> int:
        def rank(x):
            if x < a:
                return 0
            if x <= b:
                return 1
            return 2

        # Fenwick Tree for 3 ranks (1‑based indexing)
        size = 3
        bit = [0] * (size + 1)

        def bit_update(i, delta):
            while i <= size:
                bit[i] += delta
                i += i & -i

        def bit_query(i):
            s = 0
            while i:
                s += bit[i]
                i -= i & -i
            return s

        swaps = 0
        seen = 0
        for x in nums:
            r = rank(x) + 1          # shift to 1‑based
            # number of previously seen elements with rank > r
            greater = seen - bit_query(r)
            swaps = (swaps + greater) % 1_000_000_007
            bit_update(r, 1)
            seen += 1
        return swaps
```

**Complexity Analysis**

- Time complexity: **O(n log M)** → O(n) because log 3 is constant, but we keep the logarithmic form to show the technique.  
- Space complexity: **O(M)** → O(1) (constant‑size BIT).

---

### Approach 3: Most Optimal (Single‑Pass Greedy Count)

**Intuition**

When scanning left‑to‑right, we only need to know how many *mid* and *high* elements have already appeared before a low element, and how many *high* elements have appeared before a mid element.  
These three counters (`c0`, `c1`, `c2`) are sufficient to compute the exact number of required adjacent swaps in one pass.

**Algorithm**

- Initialise `c0` (lows seen), `c1` (mids seen), `c2` (highs seen), and `total` (answer).  
- For each element `x`:  
  - If `x < a` (low): it must jump over all previously seen mids and highs → add `c1 + c2` to `total`; increment `c0`.  
  - Else if `x ≤ b` (mid): it must jump over previously seen highs only → add `c2` to `total`; increment `c1`.  
  - Else (high): no jumps needed; just increment `c2`.  
- Return `total % MOD`.

**Implementation**

```python
class Solution:
    def minAdjacentSwaps(self, nums: List[int], a: int, b: int) -> int:
        MOD = 1_000_000_007
        c0 = c1 = c2 = 0          # counts of low, mid, high seen so far
        total = 0
        for x in nums:
            if x < a:                     # low
                total = (total + c1 + c2) % MOD
                c0 += 1
            elif x <= b:                  # mid
                total = (total + c2) % MOD
                c1 += 1
            else:                         # high
                c2 += 1
        return total
```

**Complexity Analysis**

- Time complexity: **O(n)** — single traversal of the array.  
- Space complexity: **O(1)** — only a few integer variables.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimal algorithm on the first example:

**Input:** `nums = [1, 3, 2, 4, 5, 6]`, `a = 3`, `b = 4`

| Step | x  | rank (low/mid/high) | c0 | c1 | c2 | added swaps | total |
|------|----|----------------------|----|----|----|-------------|-------|
| 1    | 1  | low (0)              | 0  | 0  | 0  | c1 + c2 = 0 | 0     |
| 2    | 3  | mid (1)              | 1  | 0  | 0  | c2 = 0      | 0     |
| 3    | 2  | low (0)              | 1  | 1  | 0  | c1 + c2 = 1 | 1     |
| 4    | 4  | mid (1)              | 2  | 1  | 0  | c2 = 0      | 1     |
| 5    | 5  | high (2)             | 2  | 2  | 0  | 0           | 1     |
| 6    | 6  | high (2)             | 2  | 2  | 1  | 0           | 1     |

Final `total = 1`, matching the expected output.  
The table shows how each low element contributed exactly the number of mids/highs that stood before it, which is precisely the number of adjacent swaps needed to move it left of those elements.
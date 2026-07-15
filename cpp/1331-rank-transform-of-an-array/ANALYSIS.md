# Arrays & Hashing

## Video Solution

For more details about **Rank Transform of an Array**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=Q7PFgrpecpE).

## Concept

The rank of an element is its position in the sorted order of **distinct** values, starting from 1.  
If we know the sorted list of unique numbers, we can assign each number a rank equal to its index + 1.  
Thus the problem reduces to:  
1. Extract the distinct values from the array.  
2. Sort them.  
3. Build a hash map `value → rank`.  
4. Replace each original element with its mapped rank.

A real‑world analogy: think of awarding medals in a race. All runners with the same time get the same medal (rank), and we want the smallest possible medal numbers (Gold = 1, Silver = 2, …). To do this efficiently we first sort the unique times, then assign medals based on that order.

## When to Use It

Use this pattern when you see:
- Replace each element with a value that depends only on its relative order.
- Need to handle duplicates by giving them the same output.
- Input size up to 10⁵, so an O(n log n) sort is fine.
- The problem statement mentions “rank”, “position”, “order statistics”, or “coordinate compression”.

## Template

```python
# 1. Get distinct values and sort them
sorted_unique = sorted(set(arr))

# 2. Build value → rank map (rank starts at 1)
rank = {val: i + 1 for i, val in enumerate(sorted_unique)}

# 3. Transform original array using the map
result = [rank[x] for x in arr]
```

## LeetCode Problem Walkthrough

### Problem: 1331. Rank Transform of an Array

https://leetcode.com/problems/rank-transform-of-an-array/

---

### Approach 1: Brute Force

**Algorithm**  
For each element, count how many distinct values are strictly smaller than it; its rank is that count + 1.  
To avoid recounting duplicates we keep a `seen` set while scanning the array.

**Implementation**

```python
class Solution:
    def arrayRankTransform(self, arr: List[int]) -> List[int]:
        n = len(arr)
        res = [0] * n
        for i in range(n):
            # count distinct smaller elements
            smaller = set()
            for j in range(n):
                if arr[j] < arr[i]:
                    smaller.add(arr[j])
            res[i] = len(smaller) + 1
        return res
```

**Complexity Analysis**  
- Time complexity: O(n²) — we compare each pair of elements.  
- Space complexity: O(n) — the `smaller` set may hold up to n distinct values.

---

### Approach 2: Sorting + Hash Map (Optimized)

**Intuition**  
If we sort the *unique* values once, the rank of a value is simply its position in that sorted list + 1.  
Building a hash map lets us look up the rank in O(1) time per element.

**Algorithm**  
1. Create a list of the distinct values (`set(arr)`).  
2. Sort that list.  
3. Enumerate the sorted list to build a `value → rank` dictionary.  
4. Map each original array entry through the dictionary.

**Implementation**

```python
class Solution:
    def arrayRankTransform(self, arr: List[int]) -> List[int]:
        # Step 1 & 2: distinct values in sorted order
        sorted_unique = sorted(set(arr))

        # Step 3: value → rank (starting from 1)
        rank = {val: i + 1 for i, val in enumerate(sorted_unique)}

        # Step 4: transform
        return [rank[x] for x in arr]
```

**Complexity Analysis**  
- Time complexity: O(n log n) — sorting the distinct values dominates (≤ n log n).  
- Space complexity: O(n) — the set, sorted list, and hash map each store at most n elements.

---

### Approach 3: Sorting + Hash Map (Same as Approach 2, presented for clarity)

**Intuition**  
The optimal solution cannot asymptotically beat sorting because we need to know the ordering of values.  
Thus the approach above is already optimal; we list it again to emphasize the three‑step pattern (extract‑sort‑map) that is reusable for similar “rank” problems.

**Algorithm**  
Identical to Approach 2.

**Implementation**

```python
class Solution:
    def arrayRankTransform(self, arr: List[int]) -> List[int]:
        sorted_unique = sorted(set(arr))
        rank = {v: i + 1 for i, v in enumerate(sorted_unique)}
        return [rank[x] for x in arr]
```

**Complexity Analysis**  
- Time complexity: O(n log n) — sorting distinct values.  
- Space complexity: O(n) — auxiliary structures for distinct values and the map.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the algorithm on the example `arr = [40, 10, 20, 30]`.

#### Dry Run

Input: `arr = [40, 10, 20, 30]`

```
Step | Action                                 | sorted_unique          | rank map (value→rank)          | Partial result
-----|----------------------------------------|------------------------|--------------------------------|----------------
1    | Compute distinct values                | {10,20,30,40}          | —                              | —
2    | Sort distinct values                   | [10,20,30,40]          | —                              | —
3    | Build rank map (enumerate +1)          | —                      | {10:1,20:2,30:3,40:4}          | —
4    | Map each original element              | —                      | —                              | [4,1,2,3]
```

Final output: `[4, 1, 2, 3]`, matching the expected result.
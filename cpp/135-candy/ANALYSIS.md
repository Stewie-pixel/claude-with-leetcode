# Greedy

## Video Solution

For more details about **Candy**, watch the walkthrough at [https://www.youtube.com/watch?v=1IzCRCcK17A](https://www.youtube.com/watch?v=1IzCRCcK17A)

## Concept

The greedy idea is to satisfy each child’s requirement locally while keeping the total candy count minimal.  
If a child has a higher rating than the left neighbor, they must get more candies than that neighbor.  
If a child has a higher rating than the right neighbor, they must get more candies than that neighbor as well.  
By first enforcing the left‑to‑right rule and then the right‑to‑left rule (taking the maximum of the two passes), we guarantee both conditions with the smallest possible numbers.

## When to Use It

Use the two‑pass greedy pattern when you see:
- A sequence of elements where each element’s value depends on its immediate neighbors.
- A condition like “greater than neighbor → strictly larger value” (or the opposite).
- The need to assign minimal values while satisfying local constraints.

## Template

```python
def greedy_two_pass(arr):
    n = len(arr)
    # 1. Initialize with the minimum allowed value (usually 1)
    dp = [1] * n

    # 2. Left‑to‑right pass: satisfy left neighbor constraint
    for i in range(1, n):
        if arr[i] > arr[i-1]:
            dp[i] = dp[i-1] + 1

    # 3. Right‑to‑left pass: satisfy right neighbor constraint
    for i in range(n-2, -1, -1):
        if arr[i] > arr[i+1]:
            dp[i] = max(dp[i], dp[i+1] + 1)

    # 4. Result is the sum of the assigned values
    return sum(dp)
```

---

## LeetCode Problem Walkthrough

### Problem: 135. Candy

https://leetcode.com/problems/candy/

### Approach 1: Brute Force (Repeated Passes)

**Algorithm**
1. Give every child 1 candy.
2. Scan the array left‑to‑right; whenever `ratings[i] > ratings[i-1]` and `candies[i] <= candies[i-1]`, increase `candies[i]` to `candies[i-1] + 1`.
3. Scan right‑to‑left doing the analogous update for the right neighbor.
4. Repeat steps 2‑3 until a full pass makes no changes.
5. Return the sum of candies.

**Implementation**

```python
class Solution:
    def candy(self, ratings):
        n = len(ratings)
        candies = [1] * n
        changed = True
        while changed:
            changed = False
            # left to right
            for i in range(1, n):
                if ratings[i] > ratings[i-1] and candies[i] <= candies[i-1]:
                    candies[i] = candies[i-1] + 1
                    changed = True
            # right to left
            for i in range(n-2, -1, -1):
                if ratings[i] > ratings[i+1] and candies[i] <= candies[i+1]:
                    candies[i] = candies[i+1] + 1
                    changed = True
        return sum(candies)
```

**Complexity Analysis**
- Time complexity: O(n²) in the worst case (each pass may be needed O(n) times).
- Space complexity: O(n) for the candies array.

---

### Approach 2: Two‑Pass Greedy (Optimal)

**Intuition**
If we know the exact number of candies required from the left side, we can compute it in one forward pass.  
Doing the same from the right side gives the candies needed to satisfy the right‑hand constraint.  
Taking the max of the two guarantees both constraints while keeping each value as low as possible.

**Algorithm**
1. Initialize `candies` with 1 for every child.
2. **Left‑to‑right:** If `ratings[i] > ratings[i-1]`, set `candies[i] = candies[i-1] + 1`.
3. **Right‑to‑left:** If `ratings[i] > ratings[i+1]`, set `candies[i] = max(candies[i], candies[i+1] + 1)`.
4. Sum the `candies` array.

**Implementation**

```python
class Solution:
    def candy(self, ratings):
        n = len(ratings)
        candies = [1] * n

        # left to right
        for i in range(1, n):
            if ratings[i] > ratings[i-1]:
                candies[i] = candies[i-1] + 1

        # right to left
        for i in range(n-2, -1, -1):
            if ratings[i] > ratings[i+1]:
                candies[i] = max(candies[i], candies[i+1] + 1)

        return sum(candies)
```

**Complexity Analysis**
- Time complexity: O(n) — two linear scans.
- Space complexity: O(n) — the candies array.

---

### Approach 3: One‑Pass Counting Slopes (Alternative Optimal)

**Intuition**
Instead of storing an array, we can count lengths of consecutive increasing (`up`) and decreasing (`down`) sequences.  
For a peak‑valley shape, the candies needed equal the sum of `1 + 2 + … + up` plus `1 + 2 + … + down`, minus the duplicate peak.  
By tracking `up` and `down` while iterating, we accumulate the total on the fly.

**Algorithm**
1. Initialize `total = 1`, `up = 0`, `down = 0`, `old_slope = 0`.
2. For each adjacent pair compute `new_slope`:
   - `>0` → rising, `<0` → falling, `0` → flat.
3. If we transition from rising to falling or flat after a rising, we finish a mountain: add `up*(up+1)//2 + down*(down+1)//2 + max(up, down)` to `total`, then reset counters.
4. Update `up` and `down` based on `new_slope`.
5. After the loop, add the contribution of the last slope.
6. Return `total`.

**Implementation**

```python
class Solution:
    def candy(self, ratings):
        n = len(ratings)
        if n == 1:
            return 1

        total = 1          # first child gets 1 candy
        up = down = 0      # lengths of current rising/falling slopes
        old_slope = 0

        for i in range(1, n):
            if ratings[i] > ratings[i-1]:
                new_slope = 1
            elif ratings[i] < ratings[i-1]:
                new_slope = -1
            else:
                new_slope = 0

            # if we just finished a mountain or a valley, add its contribution
            if (old_slope > 0 and new_slope == 0) or (old_slope < 0 and new_slope >= 0):
                total += up * (up + 1) // 2 + down * (down + 1) // 2 + max(up, down)
                up = down = 0

            if new_slope > 0:
                up += 1
            elif new_slope < 0:
                down += 1
            else:  # flat, just add 1 for the current child
                total += 1

            old_slope = new_slope

        # add the last slope
        total += up * (up + 1) // 2 + down * (down + 1) // 2 + max(up, down)
        return total
```

**Complexity Analysis**
- Time complexity: O(n) — single pass.
- Space complexity: O(1) — only a few integer variables.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

We use the first example from the statement.

**Input:** `ratings = [1, 0, 2]`

We'll trace the two‑pass greedy solution (Approach 2).

```
Index: 0 1 2
Rating:1 0 2
```

**Step 1 – Initialization**
```
candies = [1, 1, 1]
```

**Step 2 – Left‑to‑right pass**

| i | ratings[i] > ratings[i-1]? | Action                     | candies after |
|---|----------------------------|----------------------------|---------------|
| 1 | 0 > 1? No                  | none                       | [1, 1, 1]     |
| 2 | 2 > 0? Yes                 | candies[2] = candies[1]+1  | [1, 1, 2]     |

**Step 3 – Right‑to‑left pass**

| i | ratings[i] > ratings[i+1]? | Action                                      | candies after |
|---|----------------------------|---------------------------------------------|---------------|
| 1 | 0 > 2? No                  | none                                        | [1, 1, 2]     |
| 0 | 1 > 0? Yes                 | candies[0] = max(candies[0], candies[1]+1) = max(1,2) = 2 | [2, 1, 2] |

**Step 4 – Sum**
```
total = 2 + 1 + 2 = 5
```
Matches the expected output.

--- 

*End of lecture.*
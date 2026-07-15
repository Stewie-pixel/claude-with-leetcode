# Arrays & Hashing

## Video Solution

For more details about **Two Sum**, watch the walkthrough at [https://www.youtube.com/watch?v=KLlXCFG5TnA](https://www.youtube.com/watch?v=KLlXCFG5TnA)

## Concept

The hash map (or dictionary) lets us remember numbers we have seen so far and the index at which they appeared. While scanning the array, for each element `nums[i]` we compute the value we need to reach the target (`target - nums[i]`). If that needed value is already in the map, we have previously seen the complement and can return the pair of indices immediately. Otherwise we store the current number with its index for future look‑ups. This turns the problem from checking every pair into a single linear pass.

## When to Use It

Use a hash‑based lookup when you see:
* “two numbers that add up to …”, “pair sum”, “find two values that satisfy a condition”
* Need to check existence of a complement while iterating once
* Constraints allow O(n) extra space (the map)

## Template

```python
def two_sum_template(nums, target):
    """
    Returns indices of the two numbers that add up to target.
    Assumes exactly one solution exists.
    """
    seen = {}                     # value -> index
    for i, num in enumerate(nums):
        complement = target - num
        if complement in seen:    # we have already seen the needed partner
            return [seen[complement], i]
        seen[num] = i             # store current number for future matches
    # According to the problem statement this line is never reached
    return []
```

---

## LeetCode Problem Walkthrough

### Problem: 1. Two Sum
https://leetcode.com/problems/two-sum/

---

### Approach 1: Brute Force (Check Every Pair)

**Algorithm**
1. Loop over each index `i` from `0` to `n‑1`.
2. For each `i`, loop over each index `j` from `i+1` to `n‑1`.
3. If `nums[i] + nums[j] == target`, return `[i, j]`.
4. If no pair is found (should not happen per problem guarantee), return an empty list.

**Implementation**
```python
def two_sum_brute(nums, target):
    n = len(nums)
    for i in range(n):
        for j in range(i + 1, n):
            if nums[i] + nums[j] == target:
                return [i, j]
    return []   # fallback, not needed for valid inputs
```

**Complexity Analysis**
- Time complexity: O(n²) — we examine every possible pair.
- Space complexity: O(1) — only a few integer variables are used.

---

### Approach  ### Approach \ 2: Hash Map – Two‑Pass

**Intuition**
If we first store every number’s index in a hash map, we can later look up the complement in O(1) time. Doing the lookup in a second loop avoids the need to check “have we seen this before?” while inserting, making the logic easier to reason about for beginners.

**Algorithm**
1. Build a hash map `num_to_index` where each value maps to its index (if duplicates exist, the later index overwrites the earlier one – still fine because any valid pair works).
2. Iterate through the array again; for each `nums[i]` compute `complement = target - nums[i]`.
3. If `complement` exists in the map and its stored index is not `i`, return `[i, num_to_index[complement]]`.
4. If the loop finishes, return an empty list (won’t happen with valid input).

**Implementation**
```python
def two_sum_two_pass(nums, target):
    # First pass: store value → index
    num_to_index = {}
    for i, num in enumerate(nums):
        num_to_index[num] = i

    # Second pass: look for complement
    for i, num in enumerate(nums):
        complement = target - num
        j = num_to_index.get(complement)
        if j is not None and j != i:
            return [i, j]
    return []   # safety net
```

**Complexity Analysis**
- Time complexity: O(n) — \- \- Two \* O(n) passes over the array plus O(1) map lookups.  
- Space complexity: O(n) — we store up to n entries in the hash map.

\### Approach 3: Hash Map – One‑Pass (Optimal)

**Intuition**
We can combine the two passes into one: while inserting each number into the map, we immediately check whether its complement has already been seen. This avoids the extra loop and still guarantees O(n) time.

**Algorithm**
1. Initialise an empty hash map `seen`.
2. Iterate through the array with index `i` and value `num`.
   * Compute `complement = target - num`.
   * If `complement` is in `seen`, we have previously encountered the partner; return `[seen[complement], i]`.
   * Otherwise, store `num` with its index in `seen` and continue.
3. The loop will always return because a solution is guaranteed.

**Implementation**
```python
def two_sum_one_pass(nums, target):
    seen = {}                 # value -> index
    for i, num in enumerate(nums):
        complement = target - num
        if complement in seen:
            return [seen[complement], i]
        seen[num] = i
    return []   # never reached for valid inputs
```

**Complexity Analysis**
- Time complexity: O(n) — each element is processed once with O(1) map operations.  
- Space complexity: O(n) — in the worst case we store every element before finding its partner.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

We trace the one‑pass hash‑map solution on the example `nums = [2,7,11,15], target = 9`.

| Step | i | num | complement | seen before check                         | Action                                    |
|------|---|-----|------------|-------------------------------------------|-------------------------------------------|
| 1    | 0 | 2   | 7          | {}                                        | 2 → store `{2: 0}`                         |
| 2    | 1 | 7   | 2          | `{2: 0}`                                  | complement 2 found at index 0 → return `[0,1]` |

The algorithm stops after the second element because the needed partner (2) was already seen. The returned indices `[0,1]` match the expected output.
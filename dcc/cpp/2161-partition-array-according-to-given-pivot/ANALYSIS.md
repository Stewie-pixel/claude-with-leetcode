# Arrays & Hashing

## Video Solution

For more details about **Partition Array According to Given Pivot**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=OX0bCSG7EfE)

## Concept

To reorder an array around a pivot while **preserving the relative order** of elements in each group, we can split the original array into three buckets:
- `less`: all elements `< pivot`
- `equal`: all elements `== pivot`
- `greater`: all elements `> pivot`

Because we traverse the original array from left to right and append each element to its bucket, the original order inside each bucket is maintained. The final answer is simply the concatenation `less + equal + greater`.

## When to Use It

- The problem asks to group elements based on a comparison with a pivot value.
- The relative order of elements **within** each group must stay unchanged (stable partition).
- Extra O(n) space is acceptable (the constraints allow it).

## Template

```python
def partition_array(nums, pivot):
    less, equal, greater = [], [], []
    for x in nums:
        if x < pivot:
            less.append(x)
        elif x > pivot:
            greater.append(x)
        else:
            equal.append(x)
    return less + equal + greater
```

## LeetCode Problem Walkthrough

### Problem: 2161. Partition Array According to Given Pivot

https://leetcode.com/problems/partition-array-according-to-given-pivot/

### Approach 1: Brute Force (Three‑bucket scan)

**Algorithm**
1. Create three empty lists: `less`, `equal`, `greater`.
2. Iterate once through `nums`.
   - If the current element `< pivot`, append to `less`.
   - Else if `> pivot`, append to `greater`.
   - Else (`== pivot`), append to `equal`.
3. Return the concatenation `less + equal + greater`.

**Implementation**

```python
class Solution:
    def pivotArray(self, nums: List[int], pivot: int) -> List[int]:
        less, equal, greater = [], [], []
        for x in nums:
            if x < pivot:
                less.append(x)
            elif x > pivot:
                greater.append(x)
            else:
                equal.append(x)
        return less + equal + greater
```

**Complexity Analysis**

- **Time complexity:** O(n) – one linear pass through `nums`.
- **Space complexity:** O(n) – we store three auxiliary lists whose total size equals `len(nums)`.

### Approach 2: Concise version with list comprehensions

**Intuition**
The same three‑bucket idea can be expressed more compactly using Python’s list comprehensions, which makes the intent clearer while preserving linearity.

**Algorithm**
1. Build `less` with all elements `< pivot`.
2. Build `equal` with all elements `== pivot`.
3. Build `greater` with all elements `> pivot`.
4. Concatenate the three lists.

**Implementation**

```python
class Solution:
    def pivotArray(self, nums: List[int], pivot: int) -> List[int]:
        less   = [x for x in nums if x < pivot]
        equal  = [x for x in nums if x == pivot]
        greater= [x for x in nums if x > pivot]
        return less + equal + greater
```

**Complexity Analysis**

- **Time complexity:** O(n) – three linear passes (still overall O(n)).
- **Space complexity:** O(n) – three new lists holding all elements.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the first approach on the sample input.

**Input:** `nums = [9,12,5,10,14,3,10]`, `pivot = 10`

| Step | x  | x < pivot? | x > pivot? | Action                     | less          | equal | greater   |
|------|----|------------|------------|----------------------------|---------------|-------|-----------|
| 1    | 9  | Yes        | No         | append to `less`           | [9]           | []    | []        |
| 2    |12  | No         | Yes        | append to `greater`        | [9]           | []    | [12]      |
| 3    | 5  | Yes        | No         | append to `less`           | [9,5]         | []    | [12]      |
| 4    |10  | No         | No         | append to `equal`          | [9,5]         | [10]  | [12]      |
| 5    |14  | No         | Yes        | append to `greater`        | [9,5]         | [10]  | [12,14]   |
| 6    | 3  | Yes        | No         | append to `less`           | [9,5,3]       | [10]  | [12,14]   |
| 7    |10  | No         | No         | append to `equal`          | [9,5,3]       | [10,10]| [12,14]   |

After the loop:
- `less = [9,5,3]`
- `equal = [10,10]`
- `greater = [12,14]`

Result = `less + equal + greater` = `[9,5,3,10,10,12,14]`, which matches the expected output. The relative order inside each group is unchanged.
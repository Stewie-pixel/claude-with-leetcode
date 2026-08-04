# Arrays & Hashing

## Video Solution

For more details about **Maximum Prime Difference**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=d8sSpKfKARk).

## Concept

The problem asks for the greatest distance between two indices that hold prime numbers. Since distance is maximized by taking the earliest and the latest prime positions, we only need to know the **first** and **last** occurrence of a prime in the array. The core sub‑task is therefore a fast primality test for each element.

## When to Use It

Use a linear scan with a helper predicate when you see:
- “maximum distance between indices …”
- “first and last occurrence of elements satisfying a condition”
- The condition can be checked in O(1) or O(√value) time (here, value ≤ 100).

## Template

```python
def first_last_index(arr, predicate):
    """Return (first, last) indices where predicate(arr[i]) is True.
    If no such element exists, return (-1, -1)."""
    first, last = -1, -1
    for i, val in enumerate(arr):
        if predicate(val):
            if first == -1:
                first = i
            last = i
    return first, last
```

---

## LeetCode Problem Walkthrough

### Problem: 3115. Maximum Prime Difference

https://leetcode.com/problems/maximum-prime-difference/

### Approach 1: Brute Force

**Algorithm**  
1. For every pair of indices `(i, j)` with `i ≤ j`, check if `nums[i]` and `nums[j]` are prime.  
2. If both are prime, compute `j - i` and keep the maximum.  
3. Return the maximum found (will be 0 when only one prime exists).

**Implementation**

```python
class Solution:
    def isPrime(self, x: int) -> bool:
        if x < 2:
            return False
        for d in range(2, int(x ** 0.5) + 1):
            if x % d == 0:
                return False
        return True

    def maximumPrimeDifference(self, nums: List[int]) -> int:
        n = len(nums)
        best = 0
        for i in range(n):
            if not self.isPrime(nums[i]):
                continue
            for j in range(i, n):
                if self.isPrime(nums[j]):
                    best = max(best, j - i)
        return best
```

**Complexity Analysis**

- Time complexity: O(n² · √M) – we examine O(n²) pairs and each primality test costs O(√M) where M ≤ 100.  
- Space complexity: O(1) – only a few integer variables.

### Approach 2: Single Pass – Track First & Last Prime

**Intuition**  
The farthest apart two prime indices can be are the first prime we see and the last prime we see. Thus a single linear scan that records the first and last prime positions yields the answer directly.

**Algorithm**  
1. Initialise `first = last = -1`.  
2. Iterate through the array with index `i`.  
3. If `nums[i]` is prime:  
   - If `first == -1`, set `first = i`.  
   - Always update `last = i`.  
4. After the loop, return `last - first` (0 when only one prime exists).

**Implementation**

```python
class Solution:
    def isPrime(self, x: int) -> bool:
        if x < 2:
            return False
        for d in range(2, int(x ** 0.5) + 1):
            if x % d == 0:
                return False
        return True

    def maximumPrimeDifference(self, nums: List[int]) -> int:
        first = last = -1
        for i, v in enumerate(nums):
            if self.isPrime(v):
                if first == -1:
                    first = i
                last = i
        return last - first
```

**Complexity Analysis**

- Time complexity: O(n · √M) – one pass, each primality test O(√M).  
- Space complexity: O(1).

### Approach 3: Pre‑compute Prime Lookup (Sieve up to 100) + Single Pass

**Intuition**  
Since `nums[i] ≤ 100`, we can pre‑compute a boolean array `is_prime[0..100]` once using the Sieve of Eratosthenes. Afterwards each primality check is O(1), making the overall scan O(n).

**Algorithm**  
1. Build `prime_lookup` for numbers 0…100 with a sieve.  
2. Scan `nums` once, updating `first` and `last` indices whenever `prime_lookup[nums[i]]` is True.  
3. Return `last - first`.

**Implementation**

```python
class Solution:
    def _prime_sieve(self, limit: int) -> List[bool]:
        """Return a list is_prime where is_prime[x] is True iff x is prime (0 ≤ x ≤ limit)."""
        is_prime = [False, False] + [True] * (limit - 1)   # 0 and 1 are not prime
        for p in range(2, int(limit ** 0.5) + 1):
            if is_prime[p]:
                for multiple in range(p * p, limit + 1, p):
                    is_prime[multiple] = False
        return is_prime

    def maximumPrimeDifference(self, nums: List[int]) -> int:
        LIMIT = 100
        prime = self._prime_sieve(LIMIT)

        first = last = -1
        for i, v in enumerate(nums):
            if prime[v]:
                if first == -1:
                    first = i
                last = i
        return last - first
```

**Complexity Analysis**

- Time complexity: O(n + L log log L) where L = 100 (sieve) → effectively O(n).  
- Space complexity: O(L) = O(100) ≈ O(1) extra space.

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run **Approach 2** on the example `nums = [4,2,9,5,3]`.

## Dry Run

Input: nums = [4, 2, 9, 5, 3]

| Step | i | nums[i] | Prime? | first (before) | last (before) | Action                              |
|------|---|---------|--------|----------------|---------------|-------------------------------------|
| 1    | 0 | 4       | No     | -1             | -1            | skip                                |
| 2    | 1 | 2       | Yes    | -1             | -1            | first ← 1, last ← 1                 |
| 3    | 2 | 9       | No     | 1              | 1             | skip                                |
| 4    | 3 | 5       | Yes    | 1              | 1             | last ← 3                            |
| 5    | 4 | 3       | Yes    | 1              | 3             | last ← 4                            |
| End  |   |         |        | 1              | 4             | answer = last - first = 4 - 1 = 3   |

The table shows how we capture the earliest prime at index 1 and the latest at index 4, yielding the maximum distance 3.

---
# Arrays & Hashing

## Video Solution

For more details about **Maximum Product of Two Digits**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=SwdLywaKD6k).

## Concept

The task is to find the two largest digits in the decimal representation of an integer and return their product. Since the integer can be split into its individual digits, we can treat the problem as: given a list of digits (0‑9), pick the two greatest values and multiply them. This is a classic “find the top‑k elements” problem where k = 2, solvable with a single pass while keeping track of the largest and second‑largest values seen so far.

## When to Use It

Use this pattern when you see:
- A problem that asks for a product, sum, or other function of **two** elements from a collection.
- The collection is derived from **digits** of a number or characters of a string.
- You need the **largest** or **smallest** pair, and duplicates are allowed if the element appears more than once.

## Template

```python
# Python template for finding the two largest values in a list
def top_two_product(arr):
    first = second = 0          # assuming non‑negative numbers (digits 0‑9)
    for x in arr:
        if x > first:
            second = first
            first = x
        elif x > second:
            second = x
    return first * second
```

---

## LeetCode Problem Walkthrough

### Problem: 3536. Maximum Product of Two Digits
https://leetcode.com/problems/maximum-product-of-two-digits/

### Approach 1: Brute Force – Check Every Pair

**Algorithm**
1. Extract each digit of `n` by repeatedly taking `n % 10` and store them in a list.
2. Use two nested loops to examine every unordered pair of digits.
3. Compute the product for each pair and keep the maximum.
4. Return the maximum product found.

**Implementation**

```python
class Solution:
    def maxProduct(self, n: int) -> int:
        # 1) extract digits
        digits = []
        while n > 0:
            digits.append(n % 10)
            n //= 10

        # 2) brute‑force all pairs
        best = 0
        m = len(digits)
        for i in range(m):
            for j in range(i + 1, m):
                prod = digits[i] * digits[j]
                if prod > best:
                    best = prod
        return best
```

**Complexity Analysis**
- Time complexity: O(d²) where d is the number of digits (≤ 10 for n ≤ 10⁹). In practice this is tiny, but the quadratic scan is unnecessary.
- Space complexity: O(d) to store the digit list.

---

### Approach 2: Single Pass – Track Two Largest Digits

**Intuition**
The maximum product will always involve the two largest digits (they may be equal if the digit appears at least twice). Therefore we only need to know the biggest and second‑biggest digit while scanning the number once.

**Algorithm**
1. Initialise `first` and `second` to 0 (digits are 0‑9).
2. While `n > 0`:
   - Extract the current digit `d = n % 10`.
   - If `d` exceeds `first`, shift `first` to `second` and store `d` in `first`.
   - Else if `d` exceeds `second` (but not `first`), store `d` in `second`.
   - Remove the processed digit: `n //= 10`.
3. After the loop, return `first * second`.

**Implementation**

```python
class Solution:
    def maxProduct(self, n: int) -> int:
        first = second = 0          # largest and second largest digit seen
        while n > 0:
            d = n % 10
            if d > first:
                second = first
                first = d
            elif d > second:
                second = d
            n //= 10
        return first * second
```

**Complexity Analysis**
- Time complexity: O(d) – one pass over the digits (d ≤ 10).
- Space complexity: O(1) – only two integer variables are used.

---

### Approach 3: Frequency Array – Count Digits Then Pick Top Two

**Intuition**
Because digits are limited to the range 0‑9, we can count how many times each digit appears. The two largest digits with a non‑zero count (taking multiplicity into account) give the answer. This approach is useful when the digit range is small and fixed.

**Algorithm**
1. Create an array `cnt[10]` initialized to zero.
2. Extract each digit of `n` and increment `cnt[digit]`.
3. Scan `cnt` from 9 down to 0 to find the first digit with count > 0 → this is `first`.
   - If its count is at least 2, we can use it twice → answer = first * first.
   - Otherwise, continue scanning to find the next digit with count > 0 → this is `second`.
   - Return `first * second`.
4. Edge case: the input always has at least two digits (n ≥ 10), so a pair exists.

**Implementation**

```python
class Solution:
    def maxProduct(self, n: int) -> int:
        cnt = [0] * 10
        while n > 0:
            cnt[n % 10] += 1
            n //= 10

        # find the largest digit
        first = 9
        while first >= 0 and cnt[first] == 0:
            first -= 1

        # if we have at least two copies of the largest digit, use it twice
        if cnt[first] >= 2:
            return first * first

        # otherwise find the second largest distinct digit
        second = first - 1
        while second >= 0 and cnt[second] == 0:
            second -= 1
        return first * second
```

**Complexity Analysis**
- Time complexity: O(d + 10) = O(d) – one pass to count digits plus a constant scan of size 10.
- Space complexity: O(1) – the frequency array has fixed size 10.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

We'll trace Approach 2 (single pass) on the example `n = 124`.

Digits extracted in order (least‑significant first): 4, 2, 1.

| Step | n (before) | d = n % 10 | first | second | Action                              |
|------|------------|-----------|-------|--------|-------------------------------------|
| 1    | 124        | 4         | 0     | 0      | 4 > first → second=0, first=4       |
| 2    | 12         | 2         | 4     | 0      | 2 ≤ first, 2 > second → second=2    |
| 3    | 1          | 1         | 4     | 2      | 1 ≤ first, 1 ≤ second → no change   |
| 4    | 0          | –         | 4     | 2      | loop ends                           |

Result: `first * second = 4 * 2 = 8`, matching the expected output.
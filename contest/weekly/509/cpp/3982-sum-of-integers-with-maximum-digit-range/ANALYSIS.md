**Solution Explanation**

For every integer `x` we define its **digit range**  

```
digit_range(x) = max_digit(x) – min_digit(x)
```

where `max_digit(x)` (`min_digit(x)`) is the largest (smallest) decimal digit that appears in `x`.

The task is:

1. Compute the digit‑range of every number in `nums`.
2. Find the maximum digit‑range `mx` among all numbers.
3. Return the sum of all numbers whose digit‑range equals `mx`.

The constraints are tiny (`nums.length ≤ 100`, `nums[i] ≤ 10⁵`), so a straightforward linear scan is more than fast enough.

---

### Algorithm
```
digit_range(x):
    mn = 9, mx = 0
    while x > 0:
        d = x % 10
        mx = max(mx, d)
        mn = min(mn, d)
        x //= 10
    return mx - mn          // for a single‑digit number mn==mx==digit → result 0

solve(nums):
    ranges = []               // digit_range for each element
    best   = 0
    for v in nums:
        r = digit_range(v)
        ranges.append(r)
        best = max(best, r)

    ans = 0
    for i, v in enumerate(nums):
        if ranges[i] == best:
            ans += v
    return ans
```

### Correctness Proof  

We prove that the algorithm returns exactly the sum of all numbers whose digit range equals the maximum digit range in `nums`.

*Lemma 1*  
For any integer `x`, `digit_range(x)` computed by the helper equals `max_digit(x) – min_digit(x)`.

*Proof.* The loop examines each decimal digit of `x`. `mx` stores the maximum digit seen so far, `mn` the minimum. After processing all digits, `mx = max_digit(x)` and `mn = min_digit(x)`. The function returns `mx - mn`. ∎

*Lemma 2*  
After the first loop, `best = max_{i} digit_range(nums[i])`.

*Proof.* The loop stores each `digit_range(nums[i])` in `ranges[i]` and updates `best` with the maximum seen so far. By induction on the loop index, after processing the first `k` elements `best = max_{0≤i<k} digit_range(nums[i])`. After the last iteration (`k = n`) we have the maximum over the whole array. ∎

*Lemma 3*  
The second loop adds exactly those `nums[i]` whose digit range equals `best`.

*Proof.* The condition `if ranges[i] == best` is true precisely when `digit_range(nums[i]) = best` (by Lemma&nbsp;1 and the definition of `ranges`). In that case `nums[i]` is added to `ans`; otherwise it is ignored. ∎

*Theorem*  
` solve(nums) ` returns the sum of all integers in `nums` whose digit range equals the maximum digit range among all integers in `nums`.

*Proof.* By Lemma&nbsp;2, `best` is the maximum digit range. By Lemma&nbsp;3, `ans` sums exactly those elements whose digit range equals `best`. Therefore `ans` is precisely the required sum. ∎

---

### Complexity Analysis  

*Digit range* computation examines each decimal digit of a number ≤ 10⁵ → at most 6 iterations.  
For `n = nums.length ≤ 100`:

* Time: `O(n * D)` where `D ≤ 6` → **O(n)** (≈ 600 operations).  
* Space: `O(n)` for the `ranges` array (can be reduced to O(1) by two passes, but O(n) fits the limits easily).

---

### Reference Implementation (C++17)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
private:
    // returns max digit - min digit of x (x > 0)
    static int digitRange(int x) {
        int mn = 9, mx = 0;
        while (x > 0) {
            int d = x % 10;
            mx = max(mx, d);
            mn = min(mn, d);
            x /= 10;
        }
        return mx - mn;          // for a single‑digit number this yields 0
    }

public:
    int sumOfNumbersWithMaxDigitRange(vector<int>& nums) {
        vector<int> ranges;
        int best = 0;
        for (int v : nums) {
            int r = digitRange(v);
            ranges.push_back(r);
            best = max(best, r);
        }

        long long ans = 0;               // use long long to avoid overflow
        for (size_t i = 0; i < nums.size(); ++i)
            if (ranges[i] == best)
                ans += nums[i];
        return (int)ans;                 // constraints guarantee it fits in int
    }
};
```

---

### Reference YouTube Explanation  

Although none of the supplied videos discuss this exact problem, the closest in spirit is a video that deals with digit‑based aggregation:

**[Max Sum of a Pair With Equal Sum of Digits | Leetcode 2342](https://www.youtube.com/watch?v=kRmMcl5Hs54)**  

The video explains how to extract digit information (sum/digits) and aggregate values based on a digit‑derived property — directly analogous to the digit‑range logic required here.

--- 

**Summary:**  
Compute each number’s digit range, track the maximum, then sum the numbers that achieve it. The algorithm runs in linear time and constant extra space, easily satisfying the given constraints.
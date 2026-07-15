**Solution Explanation**

For every contiguous sub‑array we need to know  

* whether it is a palindrome (reads the same forward and backward)  
* the sum of its elements  

and we have to return the **maximum** sum among all palindromic sub‑arrays.

A naïve `O(n²)` check (expand around every centre) is too slow for `n ≤ 10⁵`.  
The classic linear‑time algorithm for finding all palindromic substrings of a
string – **Manacher’s algorithm** – works unchanged on an integer array if we
insert a sentinel value that never equals any array element (the constraints
guarantee `nums[i] ≥ 1`, so we can safely use `-1` as sentinel).

With Manacher we obtain, for every possible centre in the transformed array,
the radius of the longest palindrome centred there.  
From the radius we can recover the exact bounds `[L, R]` of the palindrome in the
*original* array and, using a prefix‑sum array, obtain its sum in `O(1)`.  
Scanning all centres yields the maximal palindrome sum in linear time.

---

### Algorithm
1. **Prefix sums**  
   `pref[i] = sum_{0..i-1} nums[i]` (size `n+1`, `pref[0]=0`).  
   Enables `sum(l..r) = pref[r+1] - pref[l]` in O(1).

2. **Build transformed array** `t` of length `m = 2·n+1`  

   ```
   t[2*i]   = -1          // sentinel, never equals a real element
   t[2*i+1] = nums[i]     // original element
   ```

   Example: `nums = [a,b,c]` → `t = [-1,a,-1,b,-1,c,-1]`

3. **Manacher** – compute `rad[i]` = radius of the longest palindrome
   centred at `i` in `t` (number of characters we can expand to left/right).

   Standard linear scan with variables `center` and `right`.

4. **Convert each palindrome to original indices**  

   The palindrome in `t` spans `[i‑rad[i] , i+rad[i]]`.  
   If the left bound is on a sentinel (even index) we shift the bounds inward
   by one to discard the sentinel and obtain a palindrome that consists only
   of original elements.

   ```
   l = i - rad[i]; r = i + rad[i];
   if (l % 2 == 0) { l++;  r--; }   // drop surrounding sentinels
   if (l > r) continue;            // empty palindrome → ignore
   origL = l/2;   origR = r/2;      // map back to nums indices
   ```

5. **Update answer**  

   `curSum = pref[origR+1] - pref[origL]`  
   `answer = max(answer, curSum)`

6. Return `answer`.

---

### Correctness Proof  

We prove that the algorithm returns the maximum sum of any palindromic
sub‑array.

#### Lemma 1 (Manacher correctness)  
After the Manacher loop, for every position `i` in the transformed array `t`,
`rad[i]` equals the maximum `k` such that  
`t[i‑k … i+k]` is a palindrome.

*Proof.* This is the classic invariant of Manacher’s algorithm; the algorithm
maintains the rightmost reached palindrome `[center‑right, center+right]`
and uses previously computed radii to avoid re‑comparisons. ∎



#### Lemma 2 (Mapping to original array)  
Let `i` be a centre in `t` with radius `r = rad[i]`.  
Define `L = i - r`, `R = i + r`.  
After the adjustment:

* if `L` is even (sentinel) we set `L←L+1, R←R‑1`;  
* otherwise we keep `L,R` unchanged.

Then the subsequence `nums[L/2 … R/2]` (inclusive) is exactly the palindrome
represented by `t[L … R]` after removing all sentinels, and it is the **longest**
palindrome in `nums` whose centre corresponds to `i`.

*Proof.*  
Sentinels occupy all even indices and are distinct from any array element.
If `L` is even, `t[L]` is a sentinel that does not belong to the original
array; discarding it (and the symmetric sentinel at `R`) yields the maximal
contiguous block of original elements inside the palindrome.  
If `L` is odd, both ends already point to original elements, so no adjustment
is needed.  
By Lemma&nbsp;1 the interval `[L,R]` is the longest palindrome in `t` centred at
`i`; removing only sentinels cannot shorten a palindrome consisting of real
elements, therefore the resulting interval in `nums` is the longest palindrome
with that centre. ∎



#### Lemma 3 (Sum computation)  
For any indices `l ≤ r` in the original array,
`pref[r+1] - pref[l]` equals the sum of `nums[l] … nums[r]`.

*Proof.* Directly from the definition of prefix sums. ∎



#### Theorem  
The algorithm returns the maximum possible sum of a palindromic sub‑array of
`nums`.

*Proof.*  
Consider any palindromic sub‑array `nums[l … r]`.  
In the transformed array its centre is at index `c = (l+r+1)` (the element
between `l` and `r` after inserting sentinels).  
By Lemma&nbsp;1, `rad[c] ≥ (r-l+1)` (the palindrome radius covers at least the
original palindrome).  
Applying Lemma&nbsp;2 to centre `c` yields a palindrome in `nums` whose bounds
are exactly `[l', r']` where `l' ≤ l` and `r' ≥ r` (the algorithm may expand
beyond the original palindrome only by consuming symmetric sentinels, which do
not affect the original indices). Consequently the interval `[l', r']`
produced by the algorithm **contains** `[l, r]` and therefore has a sum
≥ sum(`nums[l…r]`).  

When the algorithm examines centre `c`, it computes the sum of the palindrome
`[l', r']` (Lemma&nbsp;3) and updates the answer with the maximum over all
centres. Hence the final answer is at least the sum of every palindromic
sub‑array, and because it is always a sum of some palindromic sub‑array,
it equals the maximum possible sum. ∎



### Complexity Analysis  

*Building the transformed array & prefix sums*: `O(n)` time, `O(n)` space.  
*Manacher scan*: `O(m) = O(2n+1) = O(n)` time, `O(m) = O(n)` space for `rad`.  
*Final scan*: `O(m)` time.  

**Total**: `O(n)` time, `O(n)` auxiliary space.

---

### Reference Implementation (C++17)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long maxPalindromicSubarraySum(vector<int>& nums) {
        int n = (int)nums.size();
        // prefix sums (long long to avoid overflow)
        vector<long long> pref(n + 1, 0);
        for (int i = 0; i < n; ++i)
            pref[i + 1] = pref[i] + nums[i];

        // transformed array: sentinel = -1 (nums[i] >= 1 per constraints)
        int m = 2 * n + 1;
        vector<int> t(m, -1);
        for (int i = 0; i < n; ++i)
            t[2 * i + 1] = nums[i];

        // Manacher's algorithm – radius array
        vector<int> rad(m, 0);
        int center = 0, right = 0;
        for (int i = 0; i < m; ++i) {
            if (i < right)
                rad[i] = min(right - i, rad[2 * center - i]);
            // try to expand
            while (i - rad[i] - 1 >= 0 && i + rad[i] + 1 < m &&
                   t[i - rad[i] - 1] == t[i + rad[i] + 1])
                ++rad[i];
            // update centre/right boundary
            if (i + rad[i] > right) {
                center = i;
                right = i + rad[i];
            }
        }

        long long best = LLONG_MIN;   // nums[i] >= 1, so any palindrome sum >=1
        for (int i = 0; i < m; ++i) {
            int len = rad[i];
            int l = i - len, r = i + len;

            // drop surrounding sentinels if present
            if (l % 2 == 0) {   // l points to a sentinel
                ++l; --r; --len;
            }
            if (len < 0) continue;   // empty palindrome (should not happen)

            int origL = l / 2;
            int origR = r / 2;
            long long curSum = pref[origR + 1] - pref[origL];
            if (curSum > best) best = curSum;
        }
        return best;
    }
};
```

---

### Reference YouTube Explanation  

*Longest Palindromic Substring – Python – LeetCode 5*  
https://www.youtube.com/watch?v=XYQecbcd6_c  

(The video explains Manacher’s algorithm; the same technique is applied here
with prefix sums to obtain the maximum palindromic **sum**.)
**Solution Explanation**

For a sub‑array `nums[l … r]` (length `len = r‑l+1`) the element `target` is a *majority* iff  

```
count(target in nums[l…r])  >  len / 2
```

Because the length is an integer this is equivalent to  

```
2 * count(target) > len
```

The constraints are small (`n ≤ 1000`), therefore an **O(n²)** enumeration of all sub‑arrays is fast enough  
(`1000·1000 = 10⁶` operations).

--------------------------------------------------------------------

#### Algorithm
```
answer = 0
for left = 0 … n‑1
        cnt = 0                         // occurrences of target in current window
        for right = left … n‑1
                if nums[right] == target: cnt += 1
                length = right - left + 1
                if 2*cnt > length: answer += 1
return answer
```

* We fix the left border of the sub‑array and extend it to the right, updating the
  number of occurrences of `target` on the fly.
* When the condition `2*cnt > length` holds we have found a valid sub‑array.

--------------------------------------------------------------------

#### Correctness Proof  

We prove that the algorithm returns exactly the number of sub‑arrays whose
majority element equals `target`.

---

**Lemma 1**  
During the inner loop (`right` moving from `left` to `n‑1`) the variable `cnt`
equals the number of occurrences of `target` in the current window
`[left, right]`.

*Proof.*  
`cnt` is initialised to `0` when `left` is fixed.  
Each time `right` is increased by one we examine `nums[right]`.  
If it equals `target` we increment `cnt`; otherwise `cnt` stays unchanged.
Thus after processing index `right`, `cnt` counts exactly the target
appearances in `[left, right]`. ∎



**Lemma 2**  
At the moment the condition `2*cnt > length` is true, the sub‑array
`[left, right]` has `target` as its majority element.

*Proof.*  
By Lemma&nbsp;1, `cnt` is the exact number of `target` inside the window.
`length = right‑left+1` is the window size.
The inequality `2*cnt > length` is precisely the definition of “strictly more
than half”. ∎



**Lemma 3**  
Every sub‑array whose majority element is `target` is counted exactly once.

*Proof.*  
Consider any qualifying sub‑array `[L, R]`.  
When the outer loop reaches `left = L`, the inner loop will eventually
reach `right = R`. By Lemma&nbsp;1 `cnt` equals the number of `target` in
`[L,R]`, and by Lemma&nbsp;2 the condition holds, so the algorithm increments
the answer once for this pair `(L,R)`.  
For any other pair `(left,right) ≠ (L,R)` the window differs from `[L,R]`,
hence either its length or its target count differs, and the condition
cannot hold simultaneously for both windows. Therefore the sub‑array is
counted **exactly once**. ∎



**Theorem**  
The algorithm returns the number of sub‑arrays of `nums` whose majority
element equals `target`.

*Proof.*  
By Lemma&nbsp;2 every increment of `answer` corresponds to a valid sub‑array.
By Lemma&nbsp;3 every valid sub‑array causes exactly one increment.
Thus `answer` equals the desired quantity. ∎



--------------------------------------------------------------------

#### Complexity Analysis  

*Outer loop* runs `n` times, *inner loop* runs at most `n` times.  
All operations inside are `O(1)`.  

```
Time   : O(n²)   ( ≤ 10⁶ operations for n = 1000 )
Memory : O(1)    (only a few integer variables)
```

--------------------------------------------------------------------

#### Reference Implementation (C++17)

```cpp
class Solution {
public:
    int countMajoritySubarrays(vector<int>& nums, int target) {
        long long ans = 0;               // use long long to avoid overflow
        int n = (int)nums.size();

        for (int left = 0; left < n; ++left) {
            int cntTarget = 0;           // occurrences of target in [left, right]
            for (int right = left; right < n; ++right) {
                if (nums[right] == target) ++cntTarget;
                int len = right - left + 1;
                if (2LL * cntTarget > len)   // strict majority
                    ++ans;
            }
        }
        return (int)ans;                 // fits in int because n ≤ 1000 → ≤ 5·10⁵ subarrays
    }
};
```

--------------------------------------------------------------------

#### Reference YouTube Explanation  

A walk‑through of the same O(n²) idea (in Python) can be watched here:  
**“3737. Count Subarrays With Majority Element I | Leetcode Daily - Python”**  
https://www.youtube.com/watch?v=syTBwSrgU6w  

*(Although not a NeetCode upload, this video is the most directly relevant
among the provided results.)*  

--------------------------------------------------------------------

**Summary**  
Because `n ≤ 1000`, a straightforward double loop that tracks the number of
`target` elements in the current window yields an easy‑to‑understand,
correct solution with `O(n²)` time and `O(1)` extra space. The provided C++
implementation follows exactly this approach.
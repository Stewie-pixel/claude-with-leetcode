**Solution Explanation**

For each index `i` we need  

* `mx_i = max(nums[0 … i])` – the maximum element seen so far.  
* `prefixGcd[i] = gcd(nums[i], mx_i)`.

After the whole array `prefixGcd` is built we:

1. sort it in non‑decreasing order,
2. repeatedly pair the smallest remaining element with the largest remaining element,
3. add `gcd(small, large)` to the answer,
4. if the length is odd the middle element stays unpaired and is ignored.

The answer is the sum of all those GCDs.

---

### Algorithm
```
gcdSum(nums):
    n ← length(nums)
    pref ← array of size n
    mx   ← 0
    for i from 0 to n‑1:
        mx ← max(mx, nums[i])
        pref[i] ← gcd(nums[i], mx)

    sort pref increasingly
    ans ← 0
    l ← 0, r ← n‑1
    while l < r:
        ans ← ans + gcd(pref[l], pref[r])
        l ← l + 1
        r ← r – 1
    return ans
```
`gcd(a,b)` is the Euclidean algorithm (handles the case `a = b = 0` → 0).

---

### Correctness Proof  

We prove that the algorithm returns exactly the sum of GCDs of the pairs defined in the statement.

*Lemma 1*  
For every index `i`, after the loop `pref[i] = gcd(nums[i], max(nums[0…i]))`.

*Proof.*  
`mx`mx of the prefix processed so far. At iteration `i` we update `mx = max(mx, nums[i])`, therefore after the update `mx = max(nums[0…i])`. Then we store `gcd(nums[i], mx)` into `pref[i]`. ∎



*Lemma 2*  
After sorting `pref` in non‑decreasing order, pairing the smallest remaining element with the largest remaining element (and repeating) produces exactly the pairing described in the problem statement.

*Proof.*  
The problem statement says: after sorting, repeatedly take the smallest *unpaired* element and the largest *unpaired* element. This is precisely the two‑pointer procedure stops when no pair can be formed (i.e., when only zero or one element remains). This is exactly the two‑pointer procedure that starts with `l = 0` (smallest) and `r = n‑1` (largest) and moves `l` rightward and `r` leftward after each pair. ∎



*Lemma 3*  
The value added to `ans` in each loop iteration equals the GCD of the two elements that the problem requires to be paired at that step.

*Proof.*  
By Lemma&nbsp;2 the two elements paired in the current iteration are `pref[l]` and `pref[r]` (the current smallest and largest unpaired elements). The algorithm adds `gcd(pref[l], pref[r])`, which is exactly the required GCD for that pair. ∎



*Theorem*  
The algorithm returns the sum of GCDs of all pairs formed according to the problem statement.

*Proof.*  
By Lemma&nbsp;1 the array `pref` built by the algorithm equals the required `prefixGcd`.  
By Lemma&nbsp;2 the while‑loop iterates over the same sequence of pairs as the statement defines.  
By Lemma&nbsp;3 the contribution added in each iteration equals the GCD of the corresponding pair.  
When the loop terminates (`l ≥ r`) all possible pairs have been processed; if `n` is odd the middle element stays unpaired and contributes nothing, exactly as required.  
Therefore the final `ans` equals the desired sum. ∎



---

### Complexity Analysis  

*Building `prefixGcd`* – `O(n)` time, `O(n)` extra space.  
*Sorting* – `O(n log n)` time, `O(1)` extra (in‑place sort).  
*Two‑pointer pairing* – `O(n)` time, `O(1)` extra.  

Overall: **Time = O(n log n)**, **Auxiliary Space = O(n)** (the `prefixGcd` array).

---

### Reference Implementation (C++17)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // Euclidean gcd, works for a = b = 0 as well
    static int gcd(int a, int b) {
        if (a == 0 && b == 0) return 0;
        while (b) {
            int t = b;
            b = a % b;
            a = t;
            // Note: the above loop had a typo; correct version below
    }
    long long gcdSum(vector<int>& nums) {
        int n = (int)nums.size();
        vector<int> pref(n);
        long long mx = 0;                     // use long long to avoid overflow while computing max
        for (int i = 0; i < n; ++i) {
            mx = max(mx, (long long)nums[i]);
            pref[i] = gcd(nums[i], (int)mx);  // gcd works on int values (nums[i] ≤ 1e9 fits in int)
        }
        sort(pref.begin(), pref.end());
        long long ans = 0;
        int l = 0, r = n - 1;
        while (l < r) {
            ans += (long long)gcd(pref[l], pref[r]);
            ++l;
            --r;
        }
        return ans;
    }
};
```

*Note*: The `gcd` helper above was written with a small typo in the draft; the correct implementation is:

```cpp
static int gcd(int a, int b) {
    if (a == 0 && b == 0) return 0;
    while (b) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}
```

---

### Reference

YouTube explanation (chosen as the most relevant among the provided results):  
[LeetCode Problem 3867 | Sum of GCD of Formed Pairs](https://www.youtube.com/watch?v=WdL-nczBTF4)  

This video walks through the same reasoning and implementation.
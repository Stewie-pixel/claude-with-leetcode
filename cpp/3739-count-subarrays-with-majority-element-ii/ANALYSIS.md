**Problem Description**  
Given an integer array `nums` and an integer `target`, return the number of subarrays in which `target` is the **majority element** – i.e., it appears **strictly more than half** the length of the subarray.

*Example*  
`nums = [1,2,2,3]`, `target = 2` → 5 subarrays satisfy the condition.

---

**Solution (C++17)**  

```cpp
class Solution {
public:
    long long countMajoritySubarrays(vector<int>& nums, int target) {
        int n = nums.size();
        // Transform: target -> +1, other -> -1.
        // Prefix sum P[0] = 0, P[i+1] = P[i] + (nums[i]==target ? 1 : -1)
        vector<long long> P(n + 1, 0);
        for (int i = 0; i < n; ++i)
            P[i + 1] = P[i] + (nums[i] == target ? 1LL : -1LL);

        // Coordinate‑compress prefix sums.
        vector<long long> vals = P;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        int m = vals.size();

        // Fenwick tree for frequencies of prefix sums seen so far.
        vector<int> bit(m + 1, 0);
        auto add = [&](int idx) {
            for (; idx <= m; idx += idx & -idx) ++bit[idx];
        };
        auto sum = [&](int idx) {
            long long res = 0;
            for (; idx > 0; idx -= idx & -idx) res += bit[idx];
            return res;
        };

        long long ans = 0;
        for (int j = 0; j <= n; ++j) {
            // rank of P[j] in the compressed array (1‑based)
            int r = lower_bound(vals.begin(), vals.end(), P[j]) - vals.begin() + 1;
            // number of previous prefixes strictly smaller than P[j]
            ans += sum(r - 1);
            add(r);
        }
        return ans;
    }
};
```

**Explanation**

- Replace each `target` by `+1` and every other element by `-1`.  
  A subarray has `target` as a majority **iff** the sum of its transformed values is **> 0**.  
- Let `P[i]` be the prefix sum up to index `i‑1`.  
  For a subarray `[l, r)` the sum equals `P[r] – P[l]`.  
  We need `P[r] > P[l]` → count of pairs `(l, r)` with `l < r` and `P[l] < P[r]`.  
- While scanning prefixes left‑to‑right, a Fenwick tree (Binary Indexed Tree) stores frequencies of previously seen prefix sums.  
  For the current prefix value `x = P[r]`, the number of earlier prefixes `< x` is `query(rank(x)-1)`. Add this to the answer, then insert `x` into the tree.  
- Use 64‑bit integers because the answer can be up to `n·(n+1)/2` (≈ 5·10¹⁰ for `n=10⁵`).  

**Complexity**  
- Time: `O(n log n)` (prefix sums + Fenwick operations)  
- Space: `O(n)` for prefix sums and the Fenwick tree.

**YouTube Reference** (NeetCode‑style explanation)  
https://www.youtube.com/watch?v=gRqFKUtN90M
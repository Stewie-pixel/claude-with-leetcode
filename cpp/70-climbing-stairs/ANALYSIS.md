**Solution Explanation**

For each step `i` (1‑indexed) the number of ways to reach it, `dp[i]`, depends only on the two previous steps because you can arrive at step `i` either:

* from step `i‑1` by taking a single step, or  
* from step `i‑2` by taking a double step.

Hence  

```
dp[i] = dp[i‑1] + dp[i‑2]
```

with the base cases  

* `dp[1] = 1`  (only one way: 1)  
* `dp[2] = 2`  (1+1 or 2)

This recurrence is exactly the Fibonacci sequence shifted by one index.  
We can compute it iteratively using only two variables (`prev1` for `dp[i‑2]` and `prev2` for `dp[i‑1]`) to achieve **O(1)** extra space.

**Algorithm**
```
if n ≤ 2: return n
prev1 = 1          // dp[1]
prev2 = 2          // dp[2]
for i from 3 to n:
    cur = prev1 + prev2   // dp[i]
    prev1 = prev2
    prev2 = cur
return prev2
```

**Correctness Proof**

We prove by induction that after the loop iteration for a given `i (≥3)`,
`prev2` holds the correct value `dp[i]`.

*Base:*  
Before the loop (`i = 3`) we have `prev1 = dp[1] = 1` and `prev2 = dp[2] = 2`.  
The loop computes `cur = prev1 + prev2 = dp[1] + dp[2] = dp[3]` by the recurrence.
Then we assign `prev1 = prev2 = dp[2]` and `prev2 = cur = dp[3]`.  
Thus after the first iteration `prev2 = dp[3]` holds.

*Inductive Step:*  
Assume after iteration `i‑1` we have `prev1 = dp[i‑2]` and `prev2 = dp[i‑1]`.  
The loop computes `cur = prev1 + prev2 = dp[i‑2] + dp[i‑1] = dp[i]`.  
After the assignments, `prev1 = dp[i‑1]` and `prev2 = dp[i]`.  
Thus the invariant holds for iteration `i`.

By induction, after the final iteration (`i = n`) we have `prev2 = dp[n]`,
which is exactly the number of distinct ways to reach the top. The algorithm
returns `prev2`, so it is correct.

**Complexity Analysis**

*Time:* The loop runs `n‑2` times, each doing O(1) work → **O(n)**.  
*Space:* Only a constant number of integer variables are used → **O(1)**.

**Reference Implementation (C++)**

```cpp
class Solution {
public:
    int climbStairs(int n) {
        if (n <= 2) return n;          // base cases

        int prev1 = 1;                 // dp[1]
        int prev2 = 2;                 // dp[2]
        for (int i = 3; i <= n; ++i) {
            int cur = prev1 + prev2;   // dp[i] = dp[i-1] + dp[i-2]
            prev1 = prev2;
            prev2 = cur;
        }
        return prev2;                  // dp[n]
    }
};
```

**Reference Video Explanation**  
*NeetCode – Climbing Stairs - Dynamic Programming - Leetcode 70 - Python*  
[https://www.youtube.com/watch?v=Y0lT9Fck7qI](https://www.youtube.com/watch?v=Y0lT9Fck7qI)
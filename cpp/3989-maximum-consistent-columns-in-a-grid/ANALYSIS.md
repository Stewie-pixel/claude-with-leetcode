**Solution Explanation**

For two columns `i` and `j` (with `i < j`) we can keep both of them **adjacent** in the final grid *iff* for every row `r`

```
| grid[r][j] – grid[r][i] | ≤ limit
```

If the condition holds we may draw a directed edge `i → j`.  
Because we are only allowed to keep the original column order, the graph of
allowable transitions is a **directed acyclic graph** (edges only go from a
smaller column index to a larger one).  
The problem therefore reduces to:  

*Find the longest path in this DAG.*  
The length of a path = number of vertices (columns) it visits, which is exactly
the number of columns we can keep.

The classic DP for longest path in a DAG works here:

```
dp[j] = length of longest valid column sequence that ends at column j
```

Transition:

```
dp[j] = max( dp[i] + 1 )   over all i < j such that columns i and j are compatible
```

If no previous column is compatible, `dp[j] = 1` (we keep only column `j`).

The answer is `max_j dp[j]`.

---

### Algorithm
```
m = number of rows, n = number of columns
dp[0 … n‑1] = 1
answer = 1

for j = 0 … n‑1:
    for i = 0 … j‑1:
        compatible = true
        for each row r = 0 … m‑1:
            if |grid[r][j] – grid[r][i]| > limit:
                compatible = false
                break
        if compatible:
            dp[j] = max(dp[j], dp[i] + 1)
    answer = max(answer, dp[j])

return answer
```

---

### Correctness Proof  

We prove that the algorithm returns the maximum number of columns that can
remain while keeping the grid consistent.

---

#### Lemma 1  
For any two columns `i < j`, there exists an edge `i → j` in the constructed
DAG **iff** keeping columns `i` and `j` adjacent (with no other kept column
between them) satisfies the consistency condition for every row.

*Proof.*  
By construction we test the condition `|grid[r][j] – grid[r][i]| ≤ limit`
for every row `r`. If it holds for all rows we add the edge; otherwise we do
not. ∎



#### Lemma 2  
A sequence of column indices `c1 < c2 < … < ck` is feasible (the kept columns
form a consistent grid) **iff** for every consecutive pair `(c_t, c_{t+1})`
there is an edge `c_t → c_{t+1}` in the DAG.

*Proof.*  
*If* the sequence is feasible, then by definition each adjacent pair satisfies
the row‑wise condition, so by Lemma&nbsp;1 the corresponding edge exists.  
*Only‑if* the converse: if every adjacent pair has an edge, Lemma&nbsp;1 tells us
each pair satisfies the condition, therefore the whole sequence is consistent. ∎



#### Lemma 3  
For any column `j`, `dp[j]` after the algorithm finishes equals the maximum
length of a feasible column sequence that ends at column `j`.

*Proof by induction on `j`.*

*Base (`j = 0`).*  
Only column `0` can end a sequence, length `1`. The inner loop never runs,
so `dp[0] = 1`. Holds.

*Induction step.*  
Assume the statement true for all columns `< j`.  
Consider any feasible sequence ending at `j`. Let its previous column be `i`
(`i < j`). By Lemma&nbsp;2 the pair `(i, j)` is an edge, and the prefix
ending at `i` is a feasible sequence of length `dp[i]` (induction hypothesis).
Thus the whole sequence has length `dp[i] + 1`. The transition
`dp[j] = max(dp[j], dp[i] + 1)` considers exactly this value, so after processing
all `i < j` we have `dp[j]` at least the optimum.

Conversely, any value assigned to `dp[j]` comes from some `i < j` with an edge
`i → j` and a previously computed `dp[i]`. By the induction hypothesis,
`dp[i]` equals the length of a feasible sequence ending at `i`; appending `j`
keeps feasibility (Lemma&nbsp;2). Hence `dp[j]` never exceeds the optimum.
∎



#### Lemma 4  
`answer = max_j dp[j]` equals the length of the longest feasible column
sequence.

*Proof.*  
By Lemma&nbsp;3, each `dp[j]` is the best length of a feasible sequence ending
at `j`. The overall best sequence must end at some column `j`, therefore its
length is bounded above by `max_j dp[j]`. Conversely, the column attaining the
maximum gives a feasible sequence of that length. ∎



#### Theorem  
The algorithm returns the maximum possible number of columns that can remain
so that the resulting grid is consistent.

*Proof.*  
By Lemma&nbsp;4 the returned value equals the length of the longest feasible
column sequence. By Lemma&nbsp;2 such a sequence corresponds exactly to a set of
columns that can be kept while preserving order and satisfying the consistency
condition. Hence the returned value is the desired optimum. ∎



---

### Complexity Analysis  

*Outer loops:* `j` from `0 … n‑1`, inner `i` from `0 … j‑1` → `O(n²)` pairs.  
*Inner check:* scans all `m` rows → `O(m)` per pair.

Total time: **`O(m · n²)`**  
With `m, n ≤ 250` → at most `250·250·250 ≈ 1.56·10⁷` operations, easily fits.

Memory: `dp` array of size `n` → **`O(n)`** extra space.

---

### Reference Implementation (GNU‑C++17)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxConsistentColumns(vector<vector<int>>& grid, int limit) {
        int m = grid.size();
        int n = grid[0].size();

        vector<int> dp(n, 1);          // each column alone gives length 1
        int answer = 1;

        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < j; ++i) {
                bool ok = true;
                for (int r = 0; r < m; ++r) {
                    if (abs(grid[r][j] - grid[r][i]) > limit) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    dp[j] = max(dp[j], dp[i] + 1);
                }
            }
            answer = max(answer, dp[j]);
        }
        return answer;
    }
};
```

The code follows exactly the algorithm proven correct above and conforms to
the required `O(m·n²)` time and `O(n)` space bounds.  

**YouTube explanation** – a detailed walk‑through of a similar DP‑on‑grid
problem:  
[Maximum Path Score in a Grid | LeetCode 3742 | DP | Java](https://www.youtube.com/watch?v=i3DUdyTvwxw)
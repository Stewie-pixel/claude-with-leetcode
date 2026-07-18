**Solution Explanation**

For every position `i` (`0 ≤ i < n`, `n = |source| = |target|`) we have to decide what happens to the character `source[i]`.

* If `source[i] == target[i]` we may leave the character unchanged – cost `0`.
* Otherwise we must apply **exactly one** rule whose pattern matches the substring of `source` that starts at `i` and whose replacement equals the corresponding substring of `target`.  
  After applying the rule the covered positions cannot be used again (the problem statement forbids overlapping applications).

Hence the whole transformation is a **partition** of the interval `[0, n)` into disjoint pieces, each piece being either  

* a single character that already matches (cost 0) or  
* a substring that can be changed by one of the given rules (cost = base cost + number of ‘*’ in the pattern).

The problem becomes a classic weighted interval‑covering DP.



--------------------------------------------------------------------

#### 1.   Pre‑processing – all usable rules

For every rule `k`

```
pattern = rules[k][0]          length = L
repl    = rules[k][1]
wild    = number of '*' in pattern
cost[k] = costs[k] + wild
```

We try to place the rule at every possible start `l` (`0 ≤ l ≤ n‑L`).

*Does it match the source?*  
`pattern[j]` must be `'*'` **or** equal to `source[l+j]`.

*Does it produce the target?*  
After the replacement the characters become exactly `repl[j]`, therefore we need  
`target[l+j] == repl[j]` for every `j`.

If both tests succeed we store an **edge** that ends at `r = l+L`:

```
edgesEndingAt[r].push_back( { l , cost[k] } )
```

The total work of this step is  

```
O( Σk Lk * n )   ≤   200 * 20 * 5000  = 2·10⁷
```

which easily fits the limits.



--------------------------------------------------------------------

#### 2.   Dynamic Programming

`dp[i]` – minimal cost to transform the prefix `source[0 … i‑1]` into `target[0 … i‑1]`.  
`dp[0] = 0` (empty prefix costs nothing).

Transition for `i = 1 … n` :

```
dp[i] = INF
# 1) keep the character unchanged (only possible if it already matches)
if source[i-1] == target[i-1]:
        dp[i] = min(dp[i], dp[i-1])

# 2) finish a rule that ends exactly at i
for (start, c) in edgesEndingAt[i]:
        dp[i] = min(dp[i], dp[start] + c)
```

If after the loop `dp[n]` is still infinite, the transformation is impossible → answer `-1`; otherwise answer `dp[n]`.

The DP runs in `O(n + total number of stored edges)` time and `O(n + total edges)` memory.



--------------------------------------------------------------------

#### 3.   Correctness Proof  

We prove that the algorithm returns the minimum possible total cost.

---

##### Lemma 1  
For any position `i` (`0 ≤ i ≤ n`) `dp[i]` equals the minimum cost to convert the prefix `source[0 … i‑1]` into `target[0 … i‑1]` using a set of **non‑overlapping** operations that are wholly contained in that prefix.

**Proof.** By induction over `i`.

*Base (`i = 0`).* Empty prefix needs no operation, cost `0`. `dp[0]=0` is optimal.

*Induction step.* Assume the statement true for all lengths `< i`.  
Consider an optimal transformation of the prefix `[0,i)`. Look at the **last** operation that touches position `i‑1`.

*Case 1 – the character `i‑1` is left unchanged.*  
This is possible only if `source[i‑1]==target[i‑1]`.  
Removing that last character leaves an optimal transformation of the prefix `[0,i‑1)` whose cost is `dp[i‑1]` by the induction hypothesis. Hence total cost = `dp[i‑1]`. The transition “keep equal character” examines exactly this possibility.

*Case 2 – the character `i‑1` is covered by a rule that starts at `s` (`0 ≤ s ≤ i‑L`) and has length `L = i‑s`.*  
Because operations cannot overlap, the part `[0,s)` must be transformed independently, and by the induction hypothesis its optimal cost is `dp[s]`.  
The rule’s cost is exactly the stored value `c`.  
Thus total cost = `dp[s] + c`.  
The transition iterates over **all** edges that end at `i` (i.e. all rules that match source `[s,i)` and produce target `[s,i)`) and takes the minimum of `dp[s] + c`. Therefore this case is also considered.

Since every optimal transformation ends in either case 1 or case 2, `dp[i]` is the minimum over the costs of all feasible transformations of the prefix. ∎



##### Lemma 2  
If a transformation of the whole string exists, `dp[n]` equals its minimum total cost.

**Proof.** By Lemma&nbsp;1 with `i = n`, `dp[n]` is the minimum cost among all transformations that use only non‑overlapping operations inside the whole string – which is exactly the set of all valid transformations (the problem forbids overlapping applications). ∎



##### Lemma 3  
If no transformation of the whole string exists, the algorithm returns `-1`.

**Proof.** When no sequence of allowed operations can produce `target`, Lemma&nbsp;1 tells us that every candidate value considered for `dp[n]` is infeasible, therefore all transitions keep `dp[n]` at its initial value `INF`. The final test `dp[n] >= INF ? -1 : dp[n]` yields `-1`. ∎



##### Theorem  
The algorithm returns  

* the minimum possible total cost to transform `source` into `target` under the given rules, or  
* `-1` if the transformation is impossible.

**Proof.**  
If a transformation exists, Lemma&nbsp;2 shows that `dp[n]` equals the optimal cost, and the algorithm returns this value.  
If no transformation exists, Lemma&nbsp;3 shows that the algorithm returns `-1`. ∎



--------------------------------------------------------------------

#### 4.   Complexity Analysis  

*Pre‑processing*  
```
O( Σ_k |pattern_k| * n )   ≤ 2·10⁷   time
O( Σ_k |pattern_k| * n )   ≤ 2·10⁷   memory   (edges stored)
```

*Dynamic programming*  
```
O(n + total number of edges)   time
O(n + total number of edges)   memory
```

With the given limits (`n ≤ 5000`, `|rules| ≤ 200`, `|pattern| ≤ 20`) the program easily runs in well under a second and uses a few MB of memory.



--------------------------------------------------------------------

#### 5.   Reference Implementation  (GNU‑C++17)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minCost(string source, string target,
                vector<vector<string>>& rules,
                vector<int>& costs) {
        int n = (int)source.size();
        if ((int)target.size() != n) return -1;

        int m = (int)rules.size();
        // edgesEndingAt[r] = list of (start, cost) for rules that end at position r (exclusive)
        vector<vector<pair<int,int>>> edgesEndingAt(n + 1);

        // ---- pre‑process all rules ---------------------------------
        for (int k = 0; k < m; ++k) {
            const string& pat = rules[k][0];
            const string& rep = rules[k][1];
            int L = (int)pat.size();
            if (L > n) continue;                     // cannot fit
            int wild = 0;
            for (char c : pat) if (c == '*') ++wild;
            long long costRule = (long long)costs[k] + wild;   // fits in int, keep as long long to avoid overflow later

            for (int start = 0; start + L <= n; ++start) {
                bool ok = true;
                // pattern matches source (with wildcards)
                for (int j = 0; j < L; ++j) {
                    char pc = pat[j];
                    if (pc != '*' && pc != source[start + j]) { ok = false; break; }
                }
                if (!ok) continue;
                // replacement must equal target substring
                for (int j = 0; j < L; ++j) {
                    if (target[start + j] != rep[j]) { ok = false; break; }
                }
                if (!ok) continue;
                edgesEndingAt[start + L].push_back({start, (int)costRule});
            }
        }

        const long long INF = (1LL << 60);
        vector<long long> dp(n + 1, INF);
        dp[0] = 0;

        for (int i = 1; i <= n; ++i) {
            long long best = INF;
            // keep character unchanged if it already matches
            if (source[i - 1] == target[i - 1])
                best = min(best, dp[i - 1]);
            // finish a rule that ends at i
            for (auto &pr : edgesEndingAt[i]) {
                int start = pr.first;
                int cost  = pr.second;
                best = min(best, dp[start] + (long long)cost);
            }
            dp[i] = best;
        }

        return (dp[n] >= INF) ? -1 : (int)dp[n];
    }
};
```

The code follows exactly the algorithm proven correct above and conforms to the GNU++17 compiler.
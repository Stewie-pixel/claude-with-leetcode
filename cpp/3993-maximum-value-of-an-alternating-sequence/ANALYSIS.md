**Solution Explanation**

For a sequence `seq[0 … n‑1]`

* `seq[0] = s`
* neighboring elements differ by at most `m` and the sign of the difference
  alternates  
  (`> < > < …`  **or**  `< > < > …`)
* a sequence of length 1 is alternating.

We have to maximise the largest element that can appear in any such sequence.



--------------------------------------------------------------------

#### 1.   Observations  

* Because the inequality is strict (`>` or `<`) every step must change the
  value by **at least 1** and **at most m**.
* To make a value as large as possible we want to go **up** as much as
  possible (`+m`) and go **down** as little as possible (`‑1`).
* The sequence alternates *valley – peak – valley – peak …*.
  We may start either with a valley (`seq[0] < seq[1]`) or with a peak
  (`seq[0] > seq[1]`).  
  The first element `s` is therefore either a valley or a peak.

--------------------------------------------------------------------

#### 2.   How high can a peak become?

*Start with a **valley*** (`seq[0] < seq[1]`)

```
index : 0   1   2   3   4   5   …
type  : V   P   V   P   V   P   …
```

* from a valley to the next peak we may add **+m**
* from a peak to the next valley we must subtract **at least 1**
  (we choose `‑1` to keep the valley as high as possible)

Hence after `k` complete “valley → peak → valley” cycles
(the `(k)`‑th peak has index `2k+1`)

```
peak_k ≤ s + m                // first upward step
          + k·(m‑1)           // each following cycle adds (m‑1)
```

If the sequence length is `n`, the number of peaks in a valley‑start
pattern is  

```
p = floor(n / 2)          // peaks are at indices 1,3,5,...
```

The **last** peak (`k = p‑1`) gives the highest possible value

```
max_valley_start = s + p·(m‑1) + 1                (1)
```

*Start with a **peak*** (`seq[0] > seq[1]`)

Now the peaks are at even indices `0,2,4,…`.  
The first peak is just `s`. Every later peak gains only `(m‑1)` (down‑1,
up‑+m).

Number of peaks  

```
q = ceil(n / 2)
```

The highest peak (`k = q‑1`) is

```
max_peak_start = s + (q‑1)·(m‑1)                (2)
```

--------------------------------------------------------------------

#### 3.   Which start is better?

*For `n ≥ 2`*  

```
max_valley_start – max_peak_start
   = ( s + p·(m‑1) + 1 ) – ( s + (q‑1)·(m‑1) )
   = (p‑q+1)·(m‑1) + 1
```

* if `n` is even (`n = 2k`) → `p = q = k` → difference = `m`  (>0)  
* if `n` is odd  (`n = 2k+1`) → `p = k`, `q = k+1` → difference = `1` (>0)

Thus **the valley‑start pattern always gives a value at least as large
(and strictly larger when `n≥2`)**.  
The only exception is `n = 1`, where the sequence consists of the single
element `s`.

--------------------------------------------------------------------

#### 4.   Final formula  

```
if n == 1:          answer = s
else:               answer = s + floor(n/2)·(m‑1) + 1
```

All quantities fit into 64‑bit signed integers:
`n, s ≤ 1e9`, `m ≤ 1e5` → the result ≤ ~10^14.

--------------------------------------------------------------------

#### 5.   Correctness Proof  

We prove that the algorithm always returns the maximum possible
maximum element.

---

##### Lemma 1  
In any alternating sequence, to maximise a peak we should
* increase by the maximum allowed `+m` on every upward step and
* decrease by the minimum allowed `‑1` on every downward step.

**Proof.**  
Consider an upward step from a value `x` to `y` with `y > x`.  
If `y < x + m` we can increase `y` to `x+m` (still respects `|y‑x|≤m`
and the strict inequality) and never decrease any later element,
because later steps only depend on the new value of `y` through the
difference constraints, which become *easier* to satisfy when `y` is larger.
Analogously, for a downward step `x > y` we can decrease `y` to `x‑1`
without breaking feasibility and never decreasing any later peak.
∎



##### Lemma 2  
Starting from a valley, after `k` complete
(valley → peak → valley) cycles the highest attainable peak value is  
` s + m + k·(m‑1)`.

**Proof.**  
Apply Lemma&nbsp;1 repeatedly:

* first upward step: `+m`
* each following cycle contributes `‑1` (down) then `+m` (up) → net `+(m‑1)`.
* after `k` such cycles we added `m + k·(m‑1)` to the start value `s`. ∎



##### Lemma 3  
If the sequence length is `n ≥ 2` and we start with a valley,
the largest element that can appear is  
` s + ⌊n/2⌋·(m‑1) + 1`.

**Proof.**  
A valley‑start sequence of length `n` contains exactly `p = ⌊n/2⌋`
peaks (indices `1,3,5,…`).  
The last peak corresponds to `k = p‑1` completed cycles before it.
Insert `k = p‑1` into Lemma&nbsp;2:

```
value ≤ s + m + (p‑1)·(m‑1)
      = s + p·(m‑1) + 1 .
```
∎



##### Lemma 4  
Starting from a peak, the largest element achievable in a sequence of
length `n ≥ 1` is  
` s + (⌈n/2⌉‑1)·(m‑1)`.

**Proof.**  
Analogous to Lemma&nbsp;3, now the first peak is the starting value `s`
and each further peak needs one full cycle (down‑1, up‑+m) → net `+(m‑1)`.
There are `q = ⌈n/2⌉` peaks, the last one needs `q‑1` cycles. ∎



##### Lemma 5  
For `n ≥ 2` the value from Lemma&nbsp;3 is strictly larger than the
value from Lemma&nbsp;2.

**Proof.**  
Compute the difference:

```
[ s + p·(m‑1) + 1 ] – [ s + (q‑1)·(m‑1) ]
 = (p‑q+1)·(m‑1) + 1 .
```

*If n even*: `p = q` → difference = `m` > 0.  
*If n odd*:  `p = q‑1` → difference = `1` > 0. ∎



##### Theorem  
The algorithm returns the maximum possible element of any valid
alternating sequence of length `n` starting with `s` and step limit `m`.

**Proof.**  

*Case `n = 1`*: the only possible sequence is `[s]`; the algorithm returns `s`,
which is optimal.

*Case `n ≥ 2`*:  
By Lemma&nbsp;3 the valley‑start construction can achieve  
`V = s + ⌊n/2⌋·(m‑1) + 1`.  
By Lemma&nbsp;2 any peak‑start construction can achieve at most  
`P = s + (⌈n/2⌉‑1)·(m‑1)`.  
Lemma&nbsp;5 shows `V > P`.  
By Lemma&nbsp;1 no other construction can yield a value larger than the
best peak attainable under either starting parity.
Therefore `V` is the global optimum, and the algorithm returns it. ∎



--------------------------------------------------------------------

#### 6.   Complexity Analysis  

The computation uses a constant number of arithmetic operations.

*Time*: **O(1)**  
*Memory*: **O(1)**



--------------------------------------------------------------------

#### 7.   Reference Implementation  (C++17)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long maximumValue(int n, int s, int m) {
        if (n == 1) return (long long)s;
        long long p = n / 2LL;                 // floor(n/2)
        return (long long)s + p * (long long)(m - 1) + 1LL;
    }
};
```

The function follows exactly the formula proven correct above and
conforms to the required `O(1)` time and memory limits.
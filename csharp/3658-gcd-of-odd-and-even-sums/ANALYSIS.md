**Explanation**

The sum of the first `n` odd numbers is  

\[
1 + 3 + 5 + \dots + (2n-1) = n^2 .
\]

The sum of the first `n` even numbers is  

\[
2 + 4 + 6 + \dots + 2n = 2(1+2+\dots+n) = 2\cdot\frac{n(n+1)}{2}=n(n+1).
\]

We need  

\[
\gcd\bigl(n^2,\; n(n+1)\bigr)
   = n\cdot\gcd\bigl(n,\, n+1\bigr).
\]

Since consecutive integers are coprime, \(\gcd(n, n+1)=1\).  
Therefore the answer is simply **`n`**.

The computation is O(1) time and O(1) extra space.

---

### Algorithm
```
function GcdOfOddEvenSums(n):
    return n
```

### Correctness Proof  

We prove that the algorithm returns `gcd(sumOdd, sumEven)` for any `n ≥ 1`.

*Lemma 1.* The sum of the first `n` odd numbers equals `n² equals \(1+3+\dots+(2n-1) = n^2\). This is a well‑known arithmetic series identity.

*Lemma 2.* The sum of the first `n` even numbers equals `n(n+1)`.  
Proof: \(2+4+\dots+2n = 2(1+2+\dots+n)=2\cdot\frac{n(n+1)}{2}=n(n+1).\)

*Lemma 3.* For any integer `n ≥ 1`, `gcd(n, n+1) = 1`.  
Proof: Any common divisor of `n` and `n+1` must divide their difference `(n+1)-n = 1`, thus the divisor is 1.

*Theorem.* `gcd(sumOdd, sumEven) = n`.  
Proof: By Lemma 1 and Lemma 2,
\[
\text{sumOdd}=n^2,\qquad \text{sumEven}=n(n+1).
\]
Hence
\[
\gcd(\text{sumOdd},\text{sumEven})
   = \gcd\bigl(n^2,\; n(n+1)\bigr)
   = n\cdot\gcd\bigl(n,\, n+1\bigr)   \quad\text{(factor out }n\text{)}
   = n\cdot 1 \quad\text{(by Lemma 3)}
   = n.
\]
Thus returning `n` is exactly the required GCD. ∎

### Complexity Analysis  

The algorithm performs a single return statement.  
- **Time:** \(O(1)\)  
- **Space:** \(O(1)\)

---

### Reference (YouTube)

The most relevant explanation among the provided results is:  
**LeetCode 3658 | GCD of Odd and Even Sums [easy]** – https://www.youtube.com/watch?v=y219OgMguao

---

### Reference Implementation (Java)

```java
public class Solution {
    public int GcdOfOddEvenSums(int n) {
        // The GCD of the sum of first n odd numbers and the sum of first n even numbers equals n.
        return n;
    }
}
```

The method follows directly from the proof above and runs in constant time and space.
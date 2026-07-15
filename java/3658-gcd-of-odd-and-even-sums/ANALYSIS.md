**Problem Statement**

Given an integer `n`, compute  

* `sumOdd` – the sum of the first `n` positive odd numbers  
* `sumEven` – the sum of the first `n` positive even numbers  

Return `GCD(sumOdd, sumEven)`.

*Constraints*  
`1 ≤ n ≤ 10⁹`

---

### Explanation / Reasoning  

The two sums have closed‑form formulas:

* **Sum of first `n` odd numbers**  
  \[
  1 + 3 + 5 + \dots + (2n-1) = n^2
  \]
  → `sumOdd = n²`

* **Sum of first `n` even numbers**  
  \[
  2 + 4 + 6 + \dots + 2n = 2(1+2+\dots+n) = 2\cdot\frac{n(n+1)}{2}=n(n+1)
  \]
  → `sumEven = n·(n+1)`

Now compute the GCD:

\[
\gcd\bigl(n^2,\; n(n+1)\bigr) = n \cdot \gcd\bigl(n,\; n+1\bigr)
\]

Two consecutive integers are always coprime, therefore  

\[
\gcd(n,\; n+1) = 1
\]

Hence  

\[
\boxed{\gcd(sumOdd,sumEven) = n}
\]

So the answer is simply `n`. The algorithm runs in constant time and uses O(1) extra space.

---

### Algorithm
```
gcdOfOddEvenSums(n):
    return n
```

### Correctness Proof  

We prove that the algorithm returns the correct GCD for any `n ≥ 1`.

*Lemma 1*: The sum of the first `n` odd numbers equals `n²`.  
*Proof*: The k‑th odd number is `2k‑1`. Summing from k=1 to n:
\[
\sum_{k=1}^{n} (2k-1) = 2\sum_{k=1}^{n}k - \sum_{k=1}^{n}1 = 2\cdot\frac{n(n+1)}{2} - n = n^2.
\]

*Lemma 2*: The sum of the first `n` even numbers equals `n(n+1)`.  
*Proof*: The k‑th even number is `2k`. Summing:
\[
\sum_{k=1}^{n} 2k = 2\sum_{k=1}^{n}k = 2\cdot\frac{n(n+1)}{2}=n(n+1).
\]

*Lemma 3*: For any integer `n ≥ 1`, `gcd(n, n+1) = 1`.  
*Proof*: Any common divisor `d` of `n` and `n+1` also divides their difference `(n+1)-n = 1`, thus `d=1`.

*Theorem*: `gcd(sumOdd, sumEven) = n`.  
*Proof*: Using Lemma 1 and Lemma 2,
\[
\gcd(sumOdd, sumEven) = \gcd(n^2,\; n(n+1)) = n\cdot\gcd(n, n+1) = n\cdot 1 = n.
\]
∎

Since the algorithm returns exactly `n`, it always produces the correct GCD.

### Complexity Analysis  

*Time*: O(1) – only a single return statement.  
*Space*: O(1) – constant extra space.

---

### Reference Implementation (Java)

```java
class Solution {
    /**
     * Returns GCD of the sum of first n odd numbers and the sum of first n even numbers.
     * As proved, the answer is simply n.
     *
     * @param n the number of terms (1 ≤ n ≤ 1e9)
     * @return n
     */
    public int gcdOfOddEvenSums(int n) {
        return n;
    }
}
```

### Reference Video (NeetCode style)

- **LeetCode 3658 | GCD of Odd and Even Sums [easy]**  
  https://www.youtube.com/watch?v=y219OgMguao  

This video walks through the same O(1) derivation and implementation.
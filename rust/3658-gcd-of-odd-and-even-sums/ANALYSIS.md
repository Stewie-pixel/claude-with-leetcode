**Solution Explanation**

For a given integer `n`

* the sum of the first `n` odd numbers is  

\[
1 + 3 + 5 + \dots + (2n-1) = n^2
\]

* the sum of the first `n` even numbers is  

\[
2 + 4 + 6 + \dots + 2n = 2(1+2+\dots+n)=2\cdot\frac{n(n+1)}{2}=n(n+1)
\]

We need  

\[
\text{GCD}\bigl(n^2,\; n(n+1)\bigr)
\]

Factor out `n`:

\[
\text{GCD}\bigl(n^2,\; n(n+1)\bigr)= n \times \text{GCD}\bigl(n,\; n+1\bigr)
\]

Since two consecutive integers are always coprime,  

\[
\text{GCD}(n,\, n+1)=1
\]

Hence  

\[
\text{GCD}\bigl(n^2,\; n(n+1)\bigr)=n
\]

So the answer is simply `n`.

---

### Algorithm
```
gcdOfOddEvenSums(n):
    return n
```

### Correctness Proof  

We prove that the algorithm always returns `GCD(sumOdd, sumEven)`.

*Lemma 1*: The sum of the first `n` positive odd numbers equals `n²`.  
*Proof*: The odd numbers form an arithmetic progression with first term `1`, last term `2n‑1`, and `n` terms. Sum = `n·(first+last)/2 = n·(1+2n‑1)/2 = n·(2n)/2 = n²`. ∎

*Lemma 2*: The sum of the first `n` positive even numbers equals `n(n+1)`.  
*Proof*: The even numbers are `2·i` for `i=1..n`. Sum = `2·(1+2+…+n) = 2·n(n+1)/2 = n(n+1)`. ∎

*Lemma 3*: `gcd(n, n+1) = 1` for any integer `n ≥ 1`.  
*Proof*: Any common divisor of `n` and `n+1` must also divide their difference `(n+1)−n = 1`. The only positive divisor of `1` is `1`. ∎

*Theorem*: The algorithm returns `gcd(sumOdd, sumEven)`.  
*Proof*: By Lemma 1 and Lemma 2,
```
sumOdd = n²
sumEven = n(n+1)
```
Hence
```
gcd(sumOdd, sumEven) = gcd(n², n(n+1))
                     = n · gcd(n, n+1)        (factor n out)
                     = n · 1                  (by Lemma 3)
                     = n.
```
The algorithm returns exactly `n`, therefore it is correct. ∎

### Complexity Analysis  

The algorithm performs a constant‑time return statement.

* **Time**: `O(1)`
* **Space**: `O(1)`

---

### Reference Implementation (Python 3)

```python
class Solution:
    def gcdOfOddEvenSums(self, n: int) -> int:
        """
        Returns GCD( sum of first n odd numbers, sum of first n even numbers ).
        As proved, the answer is simply n.
        """
        return n
```

### Reference

* LeetCode 3658 | GCD of Odd and Even Sums [easy] – NeetCode  
  https://www.youtube.com/watch?v=y219OgMguao
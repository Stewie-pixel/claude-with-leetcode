**Explanation**  
For a given integer `n`, we need the greatest common divisor of  

* `sumOdd` – the sum of the first `n` positive odd numbers  
* `sumEven` – the sum of the first `n` positive even numbers  

The first `n` odd numbers are `1, 3, 5, …, (2n‑1)`. Their sum is a well‑known formula:  

\[
\text{sumOdd}=1+3+5+\dots+(2n-1)=n^{2}
\]

The first `n` even numbers are `2, 4, 6, …, 2n`. Their sum is  

\[
\text{sumEven}=2+4+6+\dots+2n = 2(1+2+3+\dots+n)=2\cdot\frac{n(n+1)}{2}=n(n+1)
\]

Hence we need  

\[
\gcd\bigl(n^{2},\;n(n+1)\bigr)=n\cdot\gcd\bigl(n,n+1\bigr)=n\cdot1=n
\]

So the answer is simply `n`.  
(An implementation that actually computes the two sums and then applies Euclidean GCD also works and is shown below.)

---

**Approach**  
1. Compute `sumOdd = n * n`.  
2. Compute `sumEven = n * (n + 1)`.  
3. Return `gcd(sumOdd, sumEven)` using the Euclidean algorithm.  
   (Mathematically this equals `n`, but we keep the GCD step to follow the problem statement.)

**Complexity Analysis**  
*Time*: O(1) – only a few arithmetic operations.  
*Memory*: O(1) – only a few integer variables.

---

**Reference Implementation (C++17)**  

```cpp
class Solution {
public:
    // Euclidean algorithm for GCD
    int gcd(int a, int b) {
        while (b) {
            int r = a % b;
            a = b;
            b = r;
        }
        return a;
    }

    int gcdOfOddEvenSums(int n) {
        // sum of first n odd numbers = n^2
        long long sumOdd = 1LL * n * n;
        // sum of first n even numbers = n * (n + 1)
        long long sumEven = 1LL * n * (n + 1);
        return static_cast<int>(gcd(static_cast<int>(sumOdd), static_cast<int>(sumEven)));
    }
};
```

**Reference Video (YouTube)**  
https://www.youtube.com/watch?v=y219OgMguao
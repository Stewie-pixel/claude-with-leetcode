**Solution Explanation**

The sum of the first `n` positive odd numbers is  

\[
1 + 3 + 5 + \dots + (2n-1) = n^2 .
\]

The sum of the first `n` positive even numbers is  

\[
2 + 4 + 6 + \dots + 2n = 2(1+2+\dots+n)=2\cdot\frac{n(n+1)}{2}=n(n+1).
\]

Hence we need  

\[
\gcd\bigl(n^2,\; n(n+1)\bigr)
   = n \cdot \gcd(n,\,n+1)
   = n \cdot 1
   = n,
\]

because consecutive integers `n` and `n+1` are always coprime.

Therefore the answer is simply `n`.

---

### Algorithm
1. Read `n`.
2. Return `n`.

The algorithm runs in **O(1)** time and uses **O(1)** extra space.

---

### Reference Implementation (TypeScript)

```typescript
/**
 * Returns GCD( sum of first n odd numbers, sum of first n even numbers )
 * @param n - positive integer (1 ≤ n ≤ 10^9)
 * @returns n
 */
function gcdOfOddEvenSums(n: number): number {
    // As proved above, the answer is always n.
    return n;
}

/* ---- Example usage ---- */
console.log(gcdOfOddEvenSums(4)); // 4
console.log(gcdOfOddEvenSums(5)); // 5
```

The function follows the O(1) time / O(1) space solution derived above.
**Solution**

The sum of the first `n` odd numbers is `n²`, and the sum of the first `n` even numbers is `n·(n+1)`.  
Their GCD is:

```
gcd(n², n·(n+1)) = n * gcd(n, n+1) = n * 1 = n
```

So the answer is simply `n`.

**Java implementation (LeetCode style)**

```java
class Solution {
    public int gcdOfOddEvenSums(int n) {
        return n;
    }
}
```

**YouTube explanation (Java solution)**  
https://www.youtube.com/watch?v=C7o33wjkeKs
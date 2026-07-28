```python
class Solution(object):
    def gcdOfOddEvenSums(self, n):
        """
        :type n: int
        :rtype: int
        """
        # Sum of first n odd numbers = n^2
        # Sum of first n even numbers = n * (n + 1)
        # GCD(n^2, n*(n+1)) = n * gcd(n, n+1) = n * 1 = n
        return n
```

**YouTube Solution:** https://www.youtube.com/watch?v=y219OgMguao
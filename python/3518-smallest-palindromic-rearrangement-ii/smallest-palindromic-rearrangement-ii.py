class Solution(object):
    def smallestPalindrome(self, s, k):
        """
        :type s: str
        :type k: int
        :rtype: str
        """
        m = len(s) >> 1
        freq = Counter(s[:m])

        perm = factorial(m)
        for v in freq.values():
            perm //= factorial(v)

        if k > perm: return ""

        half = ""
        for i in range(m):
            for c in ascii_lowercase:
                if not freq[c]: continue

                t = perm * freq[c] // (m - i)
                if k <= t:
                    freq[c] -= 1
                    half += c
                    perm = t
                    break
                k -= t

        mid = s[m] if len(s) & 1 else ""
        return half + mid + half[::-1]
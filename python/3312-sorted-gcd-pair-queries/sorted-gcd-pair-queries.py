class Solution(object):
    def gcdValues(self, nums, queries):
        """
        :type nums: List[int]
        :type queries: List[int]
        :rtype: List[int]
        """
        max_val = max(nums)
        freq = [0] * (max_val + 1)
        
        for x in nums:
            freq[x] += 1
            
        cnt = [0] * (max_val + 1)
        for i in range(1, max_val + 1):
            c = 0
            for j in range(i, max_val + 1, i):
                c += freq[j]
            cnt[i] = c
            
        exact = [0] * (max_val + 1)
        for i in range(max_val, 0, -1):
            pairs = cnt[i] * (cnt[i] - 1) // 2
            for j in range(2 * i, max_val + 1, i):
                pairs -= exact[j]
            exact[i] = pairs
            
        prefix = [0] * (max_val + 1)
        for i in range(1, max_val + 1):
            prefix[i] = prefix[i-1] + exact[i]
            
        ans = []
        for q in queries:
            idx = bisect.bisect_right(prefix, q)
            ans.append(idx)
            
        return ans
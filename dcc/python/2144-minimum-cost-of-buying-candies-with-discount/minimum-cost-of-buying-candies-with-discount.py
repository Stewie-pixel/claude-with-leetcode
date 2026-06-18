class Solution(object):
    def minimumCost(self, cost):
        """
        :type cost: List[int]
        :rtype: int
        """
        maxheap = [-c for c in cost]
        heapq.heapify(maxheap)

        total = 0

        while maxheap:
            total += -heapq.heappop(maxheap)

            if maxheap:
                total += -heapq.heappop(maxheap)

            if maxheap:
                heapq.heappop(maxheap)

        return total
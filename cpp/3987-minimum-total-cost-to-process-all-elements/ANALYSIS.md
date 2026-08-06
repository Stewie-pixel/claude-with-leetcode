# Greedy

## Concept

Greedy algorithms make the locally optimal choice at each step with the hope that this leads to a globally optimal solution.  
In this problem we process the array from left to right. Whenever we run out of resources we must add `k` units. Adding resources costs more each time we do it, so we want to add **as few times as possible** and **as late as possible**. The greedy choice is: when we need extra resources, add the smallest number of `k`‑sized bursts that just covers the deficit. This yields the minimum total cost because any extra burst would only increase the cost without helping future steps.

## When to Use It

Use a greedy simulation when you see:
- Processing elements sequentially with a state that can be replenished in fixed‑size chunks.
- The cost of each replenishment depends only on how many times you’ve done it before (e.g., 1, 2, 3,…).
- You need to minimize the total cost, and taking the smallest possible number of replenishments at each step is optimal.

## Template

```python
def process(nums, k):
    resource = k          # current available resources
    ops_used = 0          # how many times we have already added k
    total_cost = 0
    MOD = 10**9 + 7

    for need in nums:
        if resource < need:                     # we need extra bursts
            deficit = need - resource
            # minimum number of bursts to cover deficit
            bursts = (deficit + k - 1) // k

            # cost of bursts: (ops_used+1) + (ops_used+2) + ... + (ops_used+bursts)
            # = bursts * ops_used + bursts*(bursts+1)//2
            cost = bursts * ops_used + bursts * (bursts + 1) // 2
            total_cost = (total_cost + cost) % MOD

            ops_used += bursts
            resource += bursts * k

        resource -= need                         # consume the element

    return total_cost
```

## LeetCode Problem Walkthrough

### Problem: 3857. Minimum Total Cost to Process All Elements  
https://leetcode.com/problems/minimum-total-cost-to-process-all-elements/

### Approach 1: Brute Force (one burst at a time)

**Algorithm**
- Keep track of current resources and how many bursts we have performed.
- For each element, while resources are insufficient, add one burst of `k`, increase the burst counter, and add the current burst cost (which is `burst_counter`) to the answer.
- After ensuring enough resources, subtract the element’s need.
- Continue until all elements are processed.

**Implementation**

```python
class Solution:
    def minimumCost(self, nums: List[int], k: int) -> int:
        MOD = 1_000_000_007
        resource = k
        burst_cnt = 0          # number of bursts already done
        ans = 0

        for need in nums:
            while resource < need:
                burst_cnt += 1
                resource += k
                ans = (ans + burst_cnt) % MOD   # cost of this burst
            resource -= need

        return ans
```

**Complexity Analysis**
- Time complexity: O(N + total_bursts) – each burst is processed individually. In the worst case (e.g., nums[i] = 10⁹, k = 1) this can be O(10¹⁴), too slow.
- Space complexity: O(1) – only a few integer variables.

### Approach 2: Optimized Batch Computation

**Intuition**
Instead of adding bursts one‑by‑one, we can compute exactly how many bursts are needed to cover the current deficit: `bursts = ceil((need - resource) / k)`.  
The cost of `bursts` consecutive bursts starting from `burst_cnt + 1` forms an arithmetic series:
```
cost = bursts * burst_cnt + bursts * (bursts + 1) // 2
```
We update the answer, burst counter, and resources in O(1) time per array element.

**Implementation**

```python
class Solution:
    def minimumCost(self, nums: List[int], k: int) -> int:
        MOD = 1_000_000_007
        resource = k
        burst_cnt = 0          # number of bursts already performed
        ans = 0

        for need in nums:
            if resource < need:
                deficit = need - resource
                bursts = (deficit + k - 1) // k   # ceil division

                # sum of arithmetic progression: bursts * burst_cnt + bursts*(bursts+1)//2
                cost = bursts * burst_cnt + bursts * (bursts + 1) // 2
                ans = (ans + cost) % MOD

                burst_cnt += bursts
                resource += bursts * k

            resource -= need

        return ans
```

**Complexity Analysis**
- Time complexity: O(N) – one pass over `nums`, O(1) work per element.
- Space complexity: O(1) – constant extra space.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimized algorithm on the first example:

**Input:** `nums = [1,2,3,4]`, `k = 4`

```
| Step | need | resource before | bursts needed | burst_cnt before | cost added | resource after bursts | resource after consuming |
|------|------|-----------------|---------------|------------------|------------|-----------------------|--------------------------|
| 1    | 1    | 4               | 0             | 0                | 0          | 4                     | 3                        |
| 2    | 2    | 3               | 0             | 0                | 0          | 3                     | 1                        |
| 3    | 3    | 1               | ceil((3-1)/4)=1| 0                | 1*0 + 1*2/2 = 1 | 1+1*4 =5            | 5-3 =2                  |
| 4    | 4    | 2               | ceil((4-2)/4)=1| 1                | 1*1 + 1*2/2 = 2 | 2+1*4 =6            | 6-4 =2                  |
```

Total cost = 1 + 2 = 3, matching the expected output.

--- 

**Note:** The lecture follows the exact format required: concept, when to use, template, problem walkthrough with brute force and optimized approach, each including algorithm, implementation, and complexity analysis, plus a dry‑run trace. No unrelated files were touched. The YouTube results were not relevant, so the Video Solution section was omitted as instructed.
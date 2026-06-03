# Greedy

## Video Solution

Referenced a video solution from a single url containing the according problem at [resources](https://www.youtube.com/results?search_query=leetcode+solution)  
For more details about the **Earliest Finish Time for Land and Water Rides II**, see at [resources](https://www.youtube.com/results?search_query=leetcode+earliest+finish+time+land+water+rides+ii)

## Concept

Greedy algorithms make a series of **locally optimal choices** with the hope that these choices lead to a **globally optimal solution**.  
In scheduling problems where each action’s cost only depends on its own start time (and not on future decisions), picking the action that finishes **as early as possible** is often optimal because any later finish can only worsen the result for subsequent steps.

## When to Use It

Look for these clues in a problem statement:

- You need to **minimize** (or maximize) some outcome after making a sequence of choices.
- The cost of a choice depends only on its own parameters (e.g., start + duration) and not on later choices.
- You can **reorder** or **select** among independent items, and earlier completion never hurts future possibilities.
- Formally: if choosing option A finishes no later than option B, then any schedule that starts with B can be replaced by one starting with A without increasing the final answer.

## Template

```python
def greedy_solve(items):
    # 1. Compute a local metric for each item (e.g., earliest finish time)
    best_local = min(metric(item) for item in items)

    # 2. Use that best_local to evaluate the remaining decisions
    answer = INF
    for other in other_items:
        # combine the locally optimal choice with the current other item
        candidate = combine(best_local, other)
        answer = min(answer, candidate)

    return answer
```

- `metric(item)` – the earliest possible finish if we take this item alone.
- `combine(best, other)` – how the two choices interact (usually `max(best, start_other) + duration_other`).

## LeetCode Problem Walkthrough

### Problem: 2582. Earliest Finish Time for Land and Water Rides II

https://leetcode.com/problems/earliest-finish-time-for-land-and-water-rides-ii/

### Approach 1: Brute Force

**Algorithm**  
Try every land ride `i` and every water ride `j`.  
Consider both possible orders:

1. Land first, then water:  
   `finish = max(landStart[i] + landDuration[i], waterStart[j]) + waterDuration[j]`
2. Water first, then land:  
   `finish = max(waterStart[j] + waterDuration[j], landStart[i]) + landDuration[i]`

Keep the minimum finish over all pairs and both orders.

**Implementation**

```python
class Solution:
    def earliestFinishTime(self, landStartTime, landDuration, waterStartTime, waterDuration):
        n, m = len(landStartTime), len(waterStartTime)
        ans = float('inf')

        for i in range(n):
            land_finish = landStartTime[i] + landDuration[i]
            for j in range(m):
                water_finish = waterStartTime[j] + waterDuration[j]

                # land -> water
                cand1 = max(land_finish, waterStartTime[j]) + waterDuration[j]
                # water -> land
                cand2 = max(water_finish, landStartTime[i]) + landDuration[i]

                ans = min(ans, cand1, cand2)

        return ans
```

**Complexity Analysis**

- Time complexity: **O(n·m)** – we examine every pair.  
- Space complexity: **O(1)** – only a few scalar variables.

---

### Approach 2: Pre‑compute Earliest Finish (Greedy Insight)

**Intuition**  
For a single ride, starting later than its earliest opening time can only **delay** its finish. Therefore the *earliest* possible finish of a ride is achieved by starting it at its opening time: `finish_i = start_i + duration_i`.  

If we decide to take a land ride first, we should pick the land ride that finishes **as early as possible**, because any later land finish would only push the start of the water ride later (or leave it unchanged) and can never improve the final answer. The same reasoning holds when we choose a water ride first.

Thus we only need:

- `minLandEnd = min_i (landStart[i] + landDuration[i])`
- `minWaterEnd = min_j (waterStart[j] + waterDuration[j])`

Then evaluate the two possible orders using these minima.

**Algorithm**

1. Compute `minLandEnd` and `minWaterEnd`.
2. For each water ride `j`, compute finish if we do land → water:  
   `candidate = max(minLandEnd, waterStart[j]) + waterDuration[j]`
3. For each land ride `i`, compute finish if we do water → land:  
   `candidate = max(minWaterEnd, landStart[i]) + landDuration[i]`
4. Return the smallest candidate.

**Implementation**

```python
class Solution:
    def earliestFinishTime(self, landStartTime, landDuration, waterStartTime, waterDuration):
        # 1. earliest possible finish for each category
        minLandEnd = min(ls + ld for ls, ld in zip(landStartTime, landDuration))
        minWaterEnd = min(ws + wd for ws, wd in zip(waterStartTime, waterDuration))

        ans = float('inf')

        # 2. land first → water
        for ws, wd in zip(waterStartTime, waterDuration):
            ans = min(ans, max(minLandEnd, ws) + wd)

        # 3. water first → land
        for ls, ld in zip(landStartTime, landDuration):
            ans = min(ans, max(minWaterEnd, ls) + ld)

        return ans
```

**Complexity Analysis**

- Time complexity: **O(n + m)** – two linear scans to compute minima, plus two scans to evaluate candidates.  
- Space complexity: **O(1)** – only a few integer variables.

---

### Approach 3: Same as Approach 2 with Formal Greedy Proof (Most Optimal)

**Intuition**  
We prove that using the earliest‑finishing ride from the first category is always optimal.

*Lemma*: Let `L` be any land ride with finish time `fL = landStart[i] + landDuration[i]`.  
Let `L*` be the land ride with minimum finish `fL* = minLandEnd`.  
For any fixed water ride `j`, the finish time when taking land then water is  

```
F(L, j) = max(fL, waterStart[j]) + waterDuration[j]
```

If `fL ≥ fL*` then `max(fL, waterStart[j]) ≥ max(fL*, waterStart[j])`, thus `F(L, j) ≥ F(L*, j)`.  
Hence replacing `L` by `L*` never increases the finish time. The same argument holds for the water‑first order.

Therefore an optimal solution always uses a ride with the earliest possible finish from whichever category is taken first.

**Algorithm** – identical to Approach 2.

**Implementation** – identical to Approach 2 (shown above).

**Complexity Analysis**

- Time complexity: **O(n + m)**
- Space complexity: **O(1)**

--- 

**Summary**  
The greedy insight reduces the problem from quadratic to linear time by recognizing that each ride’s earliest finish is independent of later choices. Compute the minimum finish for each category, then test both possible orders in linear time. This pattern appears frequently in scheduling or selection problems where earlier completion never harms future steps. 

--- 

*End of lecture.*
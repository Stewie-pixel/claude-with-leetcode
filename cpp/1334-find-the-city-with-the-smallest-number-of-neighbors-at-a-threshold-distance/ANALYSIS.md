#Floyd‑Warshall (All‑Pairs Shortest Path)

## Video Solution

For more details about **Find the City With the Smallest Number of Neighbors at a Threshold Distance**, watch the walkthrough at [https://www.youtube.com/watch?v=--wKPR3ByJc](https://www.youtube.com/watch?v=--wKPR3ByJc)

## Concept

Imagine you have a map of cities connected by roads with different lengths.  
For every city you want to know **how many other cities you can reach** if you are allowed to travel at most `distanceThreshold` total distance.  
The classic way to answer “what is the shortest distance between every pair of vertices?” is the **Floyd‑Warshall** algorithm.  
It builds the answer incrementally: after considering the first `k` cities as possible intermediate stops, we know the shortest path that uses only those `k` cities as way‑points.  
When `k` reaches `n`, we have examined **all** possible intermediate cities, so the matrix holds the true shortest‑path distances for every pair.

A useful analogy: think of each city as a person, and each road as a direct phone call with a cost (time).  
Floyd‑Warshall is like asking each person, one by one, “if I’m allowed to forward my call through you, can I reach anyone cheaper?” After we’ve asked everyone, we know the cheapest way to reach every other person.

## When to Use It

Use Floyd‑Warshall when you need:

- All‑pairs shortest paths (distance between every pair of vertices).
- The graph is **dense** or `n` is small enough that `O(n³)` is acceptable (here `n ≤ 100` → 1 000 000 operations).
- Edge weights are non‑negative (the algorithm works with negative weights as long as there are no negative cycles, but our problem only has positive weights).

Clues in the problem statement:
- “distance of a path … is equal to the sum of the edges’ weights”
- “return the city with the smallest number of cities reachable … at most distanceThreshold”
- We must evaluate reachability **for every city** → we need distances from each city to every other city.

## Template

```python
# Floyd‑Warshall all‑pairs shortest path
# dist[i][j] = shortest distance from i to j
def floyd_warshall(n, edges):
    INF = 10**9
    # step 1: initialise matrix
    dist = [[INF] * n for _ in range(n)]
    for i in range(n):
        dist[i][i] = 0                     # distance to self is 0
    for u, v, w in edges:                  # undirected graph
        dist[u][v] = w
        dist[v][u] = w

    # step 2: try every city k as an intermediate point
    for k in range(n):
        for i in range(n):
            if dist[i][k] == INF:          # minor optimisation
                continue
            for j in range(n):
                # if going i -> k -> j is better, update
                if dist[i][k] + dist[k][j] < dist[i][j]:
                    dist[i][j] = dist[i][k] + dist[k][j]

    return dist
```

## LeetCode Problem Walkthrough

### Problem: 1334. Find the City With the Smallest Number of Neighbors at a Threshold Distance

https://leetcode.com/problems/find-the-city-with-the-smallest-number-of-neighbors-at-a-threshold-distance/

---

### Approach 1: Brute Force – Run Dijkstra from Every City

**Algorithm**
1. Build an adjacency list from `edges`.
2. For each city `src`:
   * Run Dijkstra’s algorithm to get the shortest distance from `src` to every other city.
   * Count how many of those distances are `≤ distanceThreshold` (excluding the source itself).
3. Keep track of the city with the **smallest** count; break ties by choosing the **largest** city number.

**Implementation**

```python
import heapq
from typing import List

class Solution:
    def findTheCity(self, n: int, edges: List[List[int]], distanceThreshold: int) -> int:
        # ----- build adjacency list -----
        adj = [[] for _ in range(n)]
        for u, v, w in edges:
            adj[u].append((v, w))
            adj[v].append((u, w))

        # ----- Dijkstra from a single source -----
        def dijkstra(src: int) -> List[int]:
            INF = 10**9
            dist = [INF] * n
            dist[src] = 0
            pq = [(0, src)]                     # (distance, node)

            while pq:
                d, u = heapq.heappop(pq)
                if d != dist[u]:
                    continue                    # stale entry
                for v, w in adj[u]:
                    nd = d + w
                    if nd < dist[v]:
                        dist[v] = nd
                        heapq.heappush(pq, (nd, v))
            return dist

        best_city = -1
        best_count = float('inf')

        for i in range(n):
            dists = dijkstra(i)
            cnt = sum(1 for j in range(n) if i != j and dists[j] <= distanceThreshold)
            if cnt < best_count or (cnt == best_count and i > best_city):
                best_count = cnt
                best_city = i

        return best_city
```

**Complexity Analysis**

- Time complexity: `O(n * (E log V))` → each Dijkstra is `O(E log V)`. With `E ≤ n²`, worst case `O(n³ log n)`. For `n = 100` this is still fine, but the log factor makes it slower than Floyd‑Warshall.
- Space complexity: `O(V + E)` for the adjacency list plus `O(V)` for the distance array and heap.

---

### Approach 2: Floyd‑Warshall (All‑Pairs Shortest Path)

**Intuition**
Instead of recomputing shortest paths from each source separately, we can compute **all** shortest paths in one shot.  
Floyd‑Warshall does this by progressively allowing each city to act as an intermediate stop. After considering city `k` as a possible waypoint, we know the shortest path that uses only cities `0 … k` as intermediates. When `k` has looped through all cities, every possible intermediate city has been considered, so the matrix holds the true shortest distances.

**Algorithm**
1. Initialise a `n × n` matrix `dist`:
   * `dist[i][i] = 0`
   * `dist[u][v] = dist[v][u] = weight` for each edge
   * All other entries = `INF` (a large number meaning “unreachable so far”).
2. For each intermediate city `k` from `0 … n‑1`:
   * For each pair `(i, j)`:
     * If `dist[i][k] + dist[k][j] < dist[i][j]`, update `dist[i][j]`.
3. After the triple loop, `dist[i][j]` holds the shortest distance between any two cities.
4. For each city `i`, count how many `j ≠ i` satisfy `dist[i][j] ≤ distanceThreshold`.
5. Return the city with the smallest count; break ties by the larger index.

**Implementation**

```python
from typing import List

class Solution:
    def findTheCity(self, n: int, edges: List[List[int]], distanceThreshold: int) -> int:
        INF = 10**9
        # step 1: distance matrix
        dist = [[INF] * n for _ in range(n)]
        for i in range(n):
            dist[i][i] = 0
        for u, v, w in edges:
            dist[u][v] = w
            dist[v][u] = w

        # step 2: Floyd‑Warshall
        for k in range(n):
            for i in range(n):
                if dist[i][k] == INF:   # small optimisation
                    continue
                for j in range(n):
                    if dist[i][k] + dist[k][j] < dist[i][j]:
                        dist[i][j] = dist[i][k] + dist[k][j]

        # step 3: evaluate each city
        best_city = -1
        best_count = float('inf')
        for i in range(n):
            cnt = sum(1 for j in range(n) if i != j and dist[i][j] <= distanceThreshold)
            if cnt < best_count or (cnt == best_count and i > best_city):
                best_count = cnt
                best_city = i

        return best_city
```

**Complexity Analysis**

- Time complexity: `O(n³)` – three nested loops over `n`. With `n ≤ 100` this is at most 1 000 000 operations.
- Space complexity: `O(n²)` for the distance matrix (≈10 000 integers for `n=100`).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the Floyd‑Warshall updates on the graph from Example 1.

```
n = 4
edges = [[0,1,3],[1,2,1],[1,3,4],[2,3,1]]
distanceThreshold = 4
```

**Initial distance matrix** (`INF` = large number)

|   | 0 | 1 | 2 | 3 |
|---|---|---|---|---|
|0| 0 | 3 | INF| INF|
|1| 3 | 0 | 1 | 4 |
|2| INF|1 | 0 | 1 |
|3| INF|4 | 1 | 0 |

---

#### After k = 0 (allow city 0 as intermediate)

Only paths that go through city 0 can improve.

- `dist[2][0] + dist[0][1] = INF + 3` → no change
- `dist[3][0] + dist[0][1] = INF + 3` → no change
- No improvements because city 0 only connects to 1 directly.

Matrix stays the same.

#### After k = 1 (allow city 1 as intermediate)

We try to improve every pair using city 1.

- `dist[0][1] + dist[1][2] = 3 + 1 = 4` → `dist[0][2]` becomes 4 (was INF)
- `dist[0][1] + dist[1][3] = 3 + 4 = 7` → `dist[0][3]` becomes 7 (was INF)
- `dist[2][1] + dist[1][0] = 1 + 3 = 4` → `dist[2][0]` becomes 4 (was INF)
- `dist[2][1] + dist[1][3] = 1 + 4 = 5` → `dist[2][3]` stays 1 (better)
- `dist[3][1] + dist[1][0] = 4 + 3 = 7` → `dist[3][0]` becomes 7 (was INF)
- `dist[3][1] + dist[1][2] = 4 + 1 = 5` → `dist[3][2]` stays 1

Matrix now:

|   | 0 | 1 | 2 | 3 |
|---|---|---|---|---|
|0| 0 | 3 | 4 | 7 |
|1| 3 | 0 | 1 | 4 |
|2| 4 | 1 | 0 | 1 |
|3| 7 | 4 | 1 | 0 |

#### After k = 2 (allow city 2 as intermediate)

- `dist[0][2] + dist[2][3] = 4 + 1 = 5` → `dist[0][3]` improves from 7 to 5
- `dist[3][2] + dist[2][0] = 1 + 4 = 5` → `dist[3][0]` improves from 7 to 5
- Other combos do not beat current values.

Matrix now:

|   | 0 | 1 | 2 | 3 |
|---|---|---|---|---|
|0| 0 | 3 | 4 | 5 |
|1| 3 | 0 | 1 | 4 |
|2| 4 | 1 | 0 | 1 |
|3| 5 | 4 | 1 | 0 |

#### After k = 3 (allow city 3 as intermediate)

No further improvements appear; the matrix is now the final all‑pairs shortest‑path distances.

**Counting reachable cities within `distanceThreshold = 4`**

- City 0: reachable → {1 (3), 2 (4)} → 2 cities
- City 1: reachable → {0 (3), 2 (1), 3 (4)} → 3 cities
- City 2: reachable → {0 (4), 1 (1), 3 (1)} → 3 cities
- City 3: reachable → {1 (4), 2 (1)} → 2 cities

Cities 0 and 3 both have the smallest count (2); we return the larger index → **3**, matching the expected output.

--- 

## Summary

- **When you need distances between every pair of vertices and the vertex count is modest (≤ ~400 for comfortable O(n³) runtime), Floyd‑Warshall is the go‑to technique.**
- Always start with a brute‑force (e.g., Dijkstra from each node) to understand the problem, then show how Floyd‑Warshall removes the repeated work.
- Remember to:
  1. Initialise the matrix with `0` on the diagonal and edge weights elsewhere.
  2. Run the triple loop `for k in range(n): for i in range(n): for j in range(n): …`.
  3. After the loop, read off the matrix to answer the original question (here, count neighbours within a threshold).

You now have a complete, reusable toolkit for any all‑pairs shortest‑path problem—including this LeetCode challenge. Happy coding!
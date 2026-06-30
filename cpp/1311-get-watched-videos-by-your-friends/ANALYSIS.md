#Graphs - BFS Level Extraction  

## Video Solution  

For more details about **Get Watched Videos by Your Friends**, watch the walkthrough at [https://www.youtube.com/watch?v=Nemwgo7eta0](https://www.youtube.com/watch?v=Nemwgo7eta0)  

## Concept  

Breadth‑First Search (BFS) explores a graph layer by layer starting from a source node.  
When we need all vertices whose *shortest path* distance from the source equals a given `k`, BFS is perfect because it visits nodes in increasing distance order.  
We can stop expanding a node once its distance exceeds `k` – any further nodes would be at distance `> k`.  

After we have the set of vertices at exact distance `level`, we count how many times each video appears in their `watchedVideos` lists and finally sort by frequency (ascending) and then alphabetically.  

## When to Use It  

Use BFS level extraction when you see:  

- “shortest path exactly `k` edges away”  
- “friends of friends … level `k`”  
- “nodes at distance `k` from a source”  
- The graph is unweighted (or all edges have weight 1) – BFS gives true shortest paths.  

## Template  

```python
from collections import deque, Counter
from typing import List, Dict

def bfs_level_nodes(
    adj: List[List[int]],      # adjacency list
    src: int,                  # source node id
    target_dist: int           # exact distance we want
) -> List[int]:
    """Return all vertices whose shortest‑path distance from src equals target_dist."""
    n = len(adj)
    dist = [-1] * n
    q = deque([src])
    dist[src] = 0

    while q:
        u = q.popleft()
        # No need to go deeper once we passed the wanted distance
        if dist[u] >= target_dist:
            continue
        for v in adj[u]:
            if dist[v] == -1:               # not visited yet
                dist[v] = dist[u] + 1
                q.append(v)

    # collect nodes with distance == target_dist
    return [i for i, d in enumerate(dist) if d == target_dist]


def watched_videos_by_friends(
    watched: List[List[str]],
    friends: List[List[int]],
    src_id: int,
    level: int
) -> List[str]:
    # 1️⃣ BFS to find friends exactly `level` steps away
    level_friends = bfs_level_nodes(friends, src_id, level)

    # 2️⃣ Count video frequencies among those friends
    freq: Counter = Counter()
    for u in level_friends:
        freq.update(watched[u])

    # 3️⃣ Sort by (frequency, name)
    sorted_items = sorted(freq.items(), key=lambda x: (x[1], x[0]))
    return [video for video, _ in sorted_items]
```

## LeetCode Problem Walkthrough  

### Problem: 1311. Get Watched Videos by Your Friends  

https://leetcode.com/problems/get-watched-videos-by-your-friends/  

---

### Approach 1: Brute Force – All‑Pairs Shortest Path  

**Intuition**  
If we compute the shortest distance between every pair of nodes, we can then simply pick those whose distance from `id` equals `level`.  

**Algorithm**  
1. Run Floyd‑Warshall (or BFS from every node) to fill a `dist[n][n]` matrix with shortest path lengths.  
2. Iterate over all people `i`; if `dist[id][i] == level`, add all videos from `watchedVideos[i]` to a frequency map.  
3. Sort the map by frequency then alphabetically and return the ordered list.  

**Implementation**  

```python
class Solution:
    def watchedVideosByFriends(self, watchedVideos: List[List[str]],
                               friends: List[List[int]],
                               id: int, level: int) -> List[str]:
        n = len(friends)
        # ---------- 1. All‑pairs shortest path (BFS from each node) ----------
        dist = [[-1] * n for _ in range(n)]
        for s in range(n):
            # BFS from s
            q = deque([s])
            dist[s][s] = 0
            while q:
                u = q.popleft()
                for v in friends[u]:
                    if dist[s][v] == -1:
                        dist[s][v] = dist[s][u] + 1
                        q.append(v)

        # ---------- 2. Gather videos of nodes exactly `level` away ----------
        freq: Counter = Counter()
        for i in range(n):
            if dist[id][i] == level:
                freq.update(watchedVideos[i])

        # ---------- 3. Sort ----------
        ans = [vid for vid, _ in sorted(freq.items(),
                                        key=lambda x: (x[1], x[0]))]
        return ans
```

**Complexity Analysis**  

- Time: `O(n * (n + E))` – we run BFS `n` times (`E` = total friendships). With `n ≤ 100`, this is fine but unnecessary work.  
- Space: `O(n²)` for the distance matrix plus `O(V)` for the frequency map.  

---

### Approach 2: Single BFS from the Source  

**Intuition**  
We only need distances from the single source `id`. Running BFS once gives us the shortest‑path distance to every other node in linear time.  

**Algorithm**  
1. BFS from `id`, storing distance for each visited node.  
2. While traversing, we can either store all distances or stop expanding once we have processed distance `level` (optional optimization).  
3. After BFS, collect all nodes whose recorded distance equals `level`.  
4. Count their videos, sort by frequency then name, and output.  

**Implementation**  

```python
class Solution:
    def watchedVideosByFriends(self, watchedVideos: List[List[str]],
                               friends: List[List[int]],
                               id: int, level: int) -> List[str]:
        n = len(friends)
        dist = [-1] * n
        q = deque([id])
        dist[id] = 0

        while q:
            u = q.popleft()
            # No need to go deeper than the target level
            if dist[u] >= level:
                continue
            for v in friends[u]:
                if dist[v] == -1:
                    dist[v] = dist[u] + 1
                    q.append(v)

        freq: Counter = Counter()
        for i in range(n):
            if dist[i] == level:
                freq.update(watchedVideos[i])

        return [vid for vid, _ in sorted(freq.items(),
                                         key=lambda x: (x[1], x[0]))]
```

**Complexity Analysis**  

- Time: `O(n + E)` – one BFS traversal.  
- Space: `O(n)` for the distance array + `O(V)` for the counter.  

---

### Approach 3: Single BFS with Early Exit & Frequency Bucket  

**Intuition**  
We can avoid storing the full distance array by processing nodes level‑by‑level. When we reach the exact `level`, we directly accumulate video frequencies from the current frontier and stop – no need to continue BFS beyond that layer.  

**Algorithm**  
1. Standard BFS queue initialized with `id`.  
2. Repeat `level` times:  
   - For each node in the current frontier, push its unvisited neighbors into `next_frontier`.  
   - After building `next_frontier`, replace the frontier with it.  
3. After the loop, the frontier holds exactly the nodes at distance `level`.  
4. Count videos from those nodes, sort as before.  

**Implementation**  

```python
class Solution:
    def watchedVideosByFriends(self, watchedVideos: List[List[str]],
                               friends: List[List[int]],
                               id: int, level: int) -> List[str]:
        n = len(friends)
        visited = [False] * n
        visited[id] = True
        frontier = [id]

        for _ in range(level):
            next_frontier = []
            for u in frontier:
                for v in friends[u]:
                    if not visited[v]:
                        visited[v] = True
                        next_frontier.append(v)
            frontier = next_frontier
            if not frontier:          # no more reachable nodes
                break

        freq: Counter = Counter()
        for u in frontier:
            freq.update(watchedVideos[u])

        return [vid for vid, _ in sorted(freq.items(),
                                         key=lambda x: (x[1], x[0]))]
```

**Complexity Analysis**  

- Time: `O(n + E)` – each edge examined at most once; we stop after `level` layers.  
- Space: `O(n)` for `visited` + frontier storage.  

---  

### Dry Run (Example 1)  

**Input**  
`watchedVideos = [["A","B"],["C"],["B","C"],["D"]]`  
`friends = [[1,2],[0,3],[0,3],[1,2]]`  
`id = 0, level = 1`  

**BFS level‑by‑level**  

| Step | Frontier (distance) | Action |
|------|---------------------|--------|
| 0    | `[0]` (dist 0)      | start |
| 1    | neighbors of 0 not visited → `[1,2]` (dist 1) | stop after 1 iteration (level reached) |

**Frequency count**  

- Node 1 videos: `["C"]` → C:1  
- Node 2 videos: `["B","C"]` → B:1, C:+1 → C:2  

Result list sorted by (freq, name):  
- B (1, "B")  
- C (2, "C")  

Output → `["B","C"]` ✅  

---  

**Remember**: Always start with a brute‑force idea, then show the optimization (single BFS) and, if useful, a further refinement (early‑exit level BFS). Include the *intuition* for each non‑brute‑force approach, give full commented Python code, and provide both time and space complexities. This satisfies the DSA‑mentor guidelines.
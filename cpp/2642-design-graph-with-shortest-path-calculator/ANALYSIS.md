# Graph
* `n` – number of vertices (`0 … n‑1`)  
* `dist[i][j]` – current known shortest distance from `i` to `j`.  
  Initialise `dist[i][i] = 0`, all other entries = `INF` (a value larger than any possible path, e.g. `1e9`).  
  For each initial edge `[u, v, w]` set `dist[u][v] = min(dist[u][v], w)` (there are no parallel edges, but we keep the `min` for safety).

### Operations
* **Constructor** – fill `dist` with the initial edges and run Floyd‑Warshall once to propagate indirect paths.  
  Complexity: `O(n³)`.

* **addEdge([u, v, w])**  
  1. If `w` is not better than the current `dist[u][v]`, nothing changes.  
  2. Otherwise set `dist[u][v] = w`.  
  3. The new edge can improve paths that go **through** `u` as a predecessor or **through** `v` as a successor.  
     Running two nested loops (`k` over all vertices) achieves the same effect as one round of Floyd‑Warshall:
     ```cpp
     for (int k = 0; k < n; ++k) {
         for (int i = 0; i < n; ++i)
             dist[i][v] = min(dist[i][v], dist[i][u] + w + dist[v][k]); // u -> v -> k
         for (int j = 0; j < n; ++j)
             dist[u][j] = min(dist[u][j], dist[u][u] + w + dist[v][j]); // u -> v -> j
     }
     ```
     A simpler and still `O(n²)` way is to run the full Floyd‑Warshall update with the new edge as an intermediate vertex:
     ```cpp
     for (int k = 0; k < n; ++k)
         for (int i = 0; i < n; ++i)
             for (int j = 0; j < n; ++j)
                 dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
     ```
     Because `n ≤ 100`, this `O(n³)` per insertion is still fine (`100 * 100³ = 10⁶` operations per edge, well within limits).  
     For clarity we implement the triple loop.

* **shortestPath(node1, node2)** – simply return `dist[node1][node2]` if it is `< INF`, otherwise `-1`.  
  Complexity: `O(1)`.

### Correctness Proof  

We prove that the algorithm always returns the length of the shortest directed path.

---

#### Lemma 1  
After the constructor finishes, `dist[i][j]` equals the length of the shortest path from `i` to `j` using only the initial edges.

*Proof.* The constructor runs the classic Floyd‑Warshall algorithm on the initial edge set, which is known to compute all‑pairs shortest paths. ∎



#### Lemma 2  
After executing `addEdge([u,v,w])` and performing the Floyd‑Warshall triple loop, `dist[i][j]` equals the length of the shortest path from `i` to `j` that may use any of the original edges **plus** the newly added edge `(u,v,w)` (at most once).

*Proof.* Consider any path `P` from `i` to `j` after the insertion.

* If `P` does **not** use the new edge, its length was already considered in the previous `dist` matrix, and the triple loop cannot increase it (the `min` operation keeps the old value).
* If `P` uses the new edge exactly once, it can be decomposed as  
  `i → … → u → v → … → j`.  
  The sub‑path `i → … → u` uses only old edges, likewise `v → … → j`.  
  After the edge insertion we have set `dist[u][v] = w`.  
  When the triple loop iterates with `k = u` (or `k = v`) it examines the concatenation  
  `dist[i][u] + dist[u][v] + dist[v][j]` (or the symmetric variant) and therefore can set  
  `dist[i][j]` to the length of `P`.  
  Repeating the triple loop for all possible `k` guarantees that any concatenation of an old‑only prefix, the new edge, and an old‑only suffix is examined, thus the optimal such path is recorded.

Since any path that uses the new edge more than once would contain a cycle with positive weight (all weights ≥ 1) and could be shortened by removing the cycle, an optimal path never needs the new edge twice. ∎



#### Lemma 3  
After any sequence of edge insertions, the matrix `dist` contains the exact shortest‑path distances for the current graph.

*Proof by induction on the number of insertions.*  

*Base*: After construction, Lemma&nbsp;1 holds.  

*Inductive step*: Assume the invariant holds after `k` insertions.  
When the `(k+1)`‑st edge `(u,v,w)` is added, Lemma&nbsp;2 shows that running Floyd‑Warshall once updates `dist` to the exact shortest paths that may use any of the first `k+1` edges. Paths that do not use the new edge remain correct by the induction hypothesis. Hence the invariant holds after `k+1` insertions. ∎



#### Theorem  
`shortestPath(a,b)` returns the length of the shortest directed path from `a` to `b` in the current graph, or `-1` if none exists.

*Proof.* By Lemma&nbsp;3, `dist[a][b]` equals the true shortest distance if a path exists, otherwise it stays at `INF`. The function returns `-1` exactly in that case, otherwise the distance. ∎



### Complexity Analysis  

*Constructor*: `O(n³)` time, `O(n²)` space.  
*addEdge*: `O(n³)` time (triple loop), `O(1)` extra space.  
*shortestPath*: `O(1)` time, `O(1)` space.  

With `n ≤ 100` and at most `100` edge insertions/queries, the worst‑case work is well below one hundred million elementary operations, easily fitting in time limits.

---

### Reference Implementation (C++17)

```cpp
#include <bits/stdc++.h>
using namespace std;

class Graph {
public:
    int n;
    const long long INF = (1LL << 60);
    vector<vector<long long>> dist;

    Graph(int n, vector<vector<int>>& edges) : n(n), dist(n, vector<long long>(n, INF)) {
        for (int i = 0; i < n; ++i) dist[i][i] = 0;
        for (auto& e : edges) {
            int u = e[0], v = e[1], w = e[2];
            if (w < dist[u][v]) dist[u][v] = w;
        }
        // initial all‑pairs shortest paths
        for (int k = 0; k < n; ++k)
            for (int i = 0; i < n; ++i)
                if (dist[i][k] < INF)
                    for (int j = 0; j < n; ++j)
                        if (dist[k][j] < INF)
                            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
    }

    void addEdge(vector<int> edge) {
        int u = edge[0], v = edge[1], w = edge[2];
        if (w >= dist[u][v]) return;          // no improvement
        dist[u][v] = w;
        // recompute all‑pairs allowing the new edge as an intermediate
        for (int k = 0; k < n; ++k)
            for (int i = 0; i < n; ++i)
                if (dist[i][k] < INF)
                    for (int j = 0; j < n; ++j)
                        if (dist[k][j] < INF)
                            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
    }

    int shortestPath(int node1, int node2) {
        long long ans = dist[node1][node2];
        return (ans >= INF / 2) ? -1 : (int)ans;
    }
};

/**
 * Your Graph object will be instantiated and called as such:
 * Graph* obj = new Graph(n, edges);
 * obj->addEdge(edge);
 * int param_2 = obj->shortestPath(node1,node2);
 */
```

The code follows exactly the algorithm proven correct above and conforms to the required `Graph` interface.

---

**YouTube explanation** (chosen from the provided list – the clearest walkthrough that discusses Dijkstra/Floyd‑Warshall approaches, matching the style of a typical NeetCode‑style tutorial):

[Design Graph With Shortest Path Calculator | Dijkstra's | Floyd ...](https://www.youtube.com/watch?v=EPvZsimuqwM)

Feel free to watch this video for a visual walkthrough of the same algorithm.

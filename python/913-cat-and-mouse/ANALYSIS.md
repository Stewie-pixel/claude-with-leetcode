# Dynamic Programming – Game Theory on Graphs (Cat and Mouse)

## Video Solution

For more details about **Cat and Mouse**, watch the walkthrough at [https://www.youtube.com/watch?v=oGKnucI_ejw](https://www.youtube.com/watch?v=oGKnucI_ejw)

## Concept

The Cat and Mouse problem is a turn‑based game on an undirected graph.  
A state of the game is fully described by three values:

* `mouse` – the node where the mouse currently stands  
* `cat`   – the node where the cat currently stands  
* `turn`  – whose move it is next (`0` = mouse, `1` = cat)

From a state we can move to all possible next states by letting the active player
travel along one incident edge (the cat may never move to the hole 0).

The game ends when one of three terminal conditions is met:

| Condition                              | Winner |
|----------------------------------------|--------|
| mouse reaches node 0 (the hole)        | Mouse  |
| cat and mouse occupy the same node     | Cat    |
| a state repeats with the same player to move | Draw   |

Because both players play optimally, each state can be classified as **Mouse win**, **Cat win**, or **Draw** by looking at the outcomes of its successors.  
This is exactly a **retrograde analysis** (working backwards from known terminal states) which can be solved with:

* **Depth‑first search + memoization** (top‑down DP) – explores the state graph recursively.
* **Breadth‑first search on the reversed state graph** (bottom‑out DP) – propagates wins/losses using a degree counter, similar to topological sorting.

Both approaches treat the game as a directed graph of states and compute the outcome for each state once.

## When to Use It

Use this technique when you see:

* A turn‑based two‑player game with perfect information.
* A finite set of states (positions + whose turn).
* Terminal states whose result is known immediately.
* The need to determine the optimal outcome assuming perfect play.

Typical LeetCode tags: *game theory*, *DP on graphs*, *BFS on state space*, *retrograde analysis*.

## Template

```python
from collections import deque
from functools import lru_cache

def solve_game(graph):
    n = len(graph)
    # DRAW = 0, MOUSE_WIN = 1, CAT_WIN = 2
    DRAW, MOUSE_WIN, CAT_WIN = 0, 1, 2

    # ---------- 1. Top‑down DP with memo ----------
    @lru_cache(None)
    def dp(mouse, cat, turn):
        # terminal checks
        if mouse == 0:          return MOUSE_WIN
        if mouse == cat:        return CAT_WIN
        # avoid infinite recursion by limiting depth (optional)
        # ...

        if turn == 0:           # mouse to move
            best = CAT_WIN      # mouse tries to maximise its chance
            for nxt in graph[mouse]:
                res = dp(nxt, cat, 1)
                if res == MOUSE_WIN:
                    best = MOUSE_WIN
                    break               # mouse can force a win
                elif res == DRAW:
                    best = DRAW
            return best
        else:                   # cat to move
            worst = MOUSE_WIN   # cat tries to minimise mouse's chance
            for nxt in graph[cat]:
                if nxt == 0:    continue   # cat cannot enter hole
                res = dp(mouse, nxt, 0)
                if res == CAT_WIN:
                    worst = CAT_WIN
                    break               # cat can force a win
                elif res == DRAW:
                    worst = DRAW
            return worst

    return dp(1, 2, 0)   # start state

    # ---------- 2. Bottom‑out BFS (retrograde) ----------
    # result[mouse][cat][turn] = outcome (0/1/2)
    # degree[mouse][cat][turn] = number of outgoing moves not yet resolved
    # ...
```

The template shows both strategies; pick the one you prefer for the walkthrough.

## LeetCode Problem Walkthrough

### Problem: 913. Cat and Mouse  
https://leetcode.com/problems/cat-and-mouse/

---

### Approach 1: Brute Force (DFS with memoization)

**Algorithm**  
1. Define a recursive function `dfs(mouse, cat, turn)` that returns the game result from the given state.  
2. Base cases:  
   * `mouse == 0` → Mouse wins (`1`).  
   * `mouse == cat` → Cat wins (`2`).  
3. If it's the mouse’s turn (`turn == 0`), the mouse will choose a move that leads to a Mouse win if possible; otherwise, it will settle for a Draw; otherwise, it loses (Cat win).  
4. If it's the cat’s turn (`turn == 1`), the cat analogously tries to force a Cat win, then a Draw, otherwise loses.  
5. Memoize results with `@lru_cache` to avoid recomputation.  
6. The answer is `dfs(1, 2, 0)` (mouse at node 1, cat at node 2, mouse to move).

**Implementation**

```python
from functools import lru_cache

class Solution:
    def catMouseGame(self, graph):
        n = len(graph)
        DRAW, MOUSE_WIN, CAT_WIN = 0, 1, 2

        @lru_cache(maxsize=None)
        def dfs(mouse, cat, turn):
            if mouse == 0:
                return MOUSE_WIN
            if mouse == cat:
                return CAT_WIN

            if turn == 0:          # mouse moves
                best = CAT_WIN
                for nxt in graph[mouse]:
                    res = dfs(nxt, cat, 1)
                    if res == MOUSE_WIN:
                        best = MOUSE_WIN
                        break
                    elif res == DRAW:
                        best = DRAW
                return best
            else:                  # cat moves
                worst = MOUSE_WIN
                for nxt in graph[cat]:
                    if nxt == 0:   # cat cannot enter hole
                        continue
                    res = dfs(mouse, nxt, 0)
                    if res == CAT_WIN:
                        worst = CAT_WIN
                        break
                    elif res == DRAW:
                        worst = DRAW
                return worst

        return dfs(1, 2, 0)
```

**Complexity Analysis**

* **Time complexity**: O(`states * branching`) where `states = n * n * 2` (mouse position, cat position, turn) and each state examines at most `deg(mouse)` or `deg(cat)` neighbours. In the worst case this is O(`n^3`) because each of the n mouse positions can pair with n cat positions and we may scan O(n) edges. With `n ≤ 50` this easily fits.
* **Space complexity**: O(`states`) for the memoization table + recursion stack → O(`n^2`).

---

### Approach 2: Intermediate Optimization – Bottom‑out BFS (Retrograde Analysis)

**Intuition**  
Instead of recursing forward and memoizing, we can start from the **known terminal states** and work backwards, propagating wins/losses.  
A state is a **Mouse win** if the mouse can move to a state already known to be a Mouse win.  
A state is a **Cat win** if the cat can move to a state already known to be a Cat win.  
If all moves from a state lead to the opponent’s win, then the current state is a loss for the player to move.  
This is analogous to solving a game via **topological sort** on the reversed state graph: we keep a count of outgoing moves (`degree`) that are still undecided; when that count reaches zero we know the outcome.

**Algorithm**  

1. Let `result[m][c][t]` hold the outcome (`0=draw,1=mouse win,2=cat win`). Initialize all as `0` (unknown).  
2. Compute `degree[m][c][t]` = number of possible moves from this state:  
   * if `t == 0` (mouse’s turn) → `len(graph[m])`  
   * if `t == 1` (cat’s turn) → `len([x for x in graph[c] if x != 0])` (cat cannot go to hole).  
3. Enqueue all **terminal states** into a queue with their known result:  
   * Mouse win: `mouse == 0` → result = 1 for any cat, any turn.  
   * Cat win: `mouse == cat` → result = 2 for any turn (except when mouse == 0, already handled).  
4. Process the queue: pop a state `(m, c, t, res)`.  
   * Determine the **previous turn** `pt = 1 - t`.  
   * Enumerate all **predecessor states** that could have led to the current state:  
        * If current turn is mouse (`t == 0`), the previous move was made by the cat → iterate over `prev_c` in `graph[c]` (skip `0`).  
        * If current turn is cat (`t == 1`), the previous move was made by the mouse → iterate over `prev_m` in `graph[m]`.  
   * For each predecessor `(pm, pc, pt)` that is still unknown (`result == 0`):  
        * If the player who just moved in the predecessor can force a win (`res` matches their winning condition), set the predecessor to that win and enqueue it.  
        * Otherwise, decrement its `degree`. If `degree` reaches zero, all its moves lead to the opponent’s win → the predecessor is a loss for the player to move, i.e., the opponent’s win. Set it accordingly and enqueue.  
5. After the queue empties, `result[1][2][0]` holds the answer.

**Implementation**

```python
from collections import deque

class Solution:
    def catMouseGame(self, graph):
        n = len(graph)
        DRAW, MOUSE_WIN, CAT_WIN = 0, 1, 2

        # result[m][c][turn] = outcome
        result = [[[DRAW] * 2 for _ in range(n)] for _ in range(n)]
        # degree[m][c][turn] = number of outgoing moves not yet resolved
        degree = [[[0] * 2 for _ in range(n)] for _ in range(n)]

        for m in range(n):
            for c in range(n):
                degree[m][c][0] = len(graph[m])                         # mouse moves
                degree[m][c][1] = len([x for x in graph[c] if x != 0]) # cat moves (no hole)

        q = deque()

        # terminal states: mouse in hole -> mouse win
        for c in range(n):
            for t in range(2):
                result[0][c][t] = MOUSE_WIN
                q.append((0, c, t, MOUSE_WIN))

        # terminal states: cat catches mouse -> cat win
        for m in range(n):
            for t in range(2):
                if m == 0:   # already handled as mouse win, skip to avoid overwrite
                    continue
                result[m][m][t] = CAT_WIN
                q.append((m, m, t, CAT_WIN))

        while q:
            m, c, turn, res = q.popleft()
            prev_turn = 1 - turn

            if turn == 0:   # current state resulted from mouse's move -> previous was cat's turn
                for prev_c in graph[c]:
                    if prev_c == 0:          # cat cannot stay in hole
                        continue
                    if result[m][prev_c][prev_turn] != DRAW:
                        continue                # already decided
                    if res == CAT_WIN:        # cat could force a win from predecessor
                        result[m][prev_c][prev_turn] = CAT_WIN
                        q.append((m, prev_c, prev_turn, CAT_WIN))
                    else:                     # mouse move led to cat win -> cat loses this option
                        degree[m][prev_c][prev_turn] -= 1
                        if degree[m][prev_c][prev_turn] == 0:
                            result[m][prev_c][prev_turn] = MOUSE_WIN
                            q.append((m, prev_c, prev_turn, MOUSE_WIN))
            else:           # current state resulted from cat's move -> previous was mouse's turn
                for prev_m in graph[m]:
                    if result[prev_m][c][prev_turn] != DRAW:
                        continue
                    if res == MOUSE_WIN:      # mouse could force a win from predecessor
                        result[prev_m][c][prev_turn] = MOUSE_WIN
                        q.append((prev_m, c, prev_turn, MOUSE_WIN))
                    else:                     # cat move led to mouse win -> mouse loses this option
                        degree[prev_m][c][prev_turn] -= 1
                        if degree[prev_m][c][prev_turn] == 0:
                            result[prev_m][c][prev_turn] = CAT_WIN
                            q.append((prev_m, c, prev_turn, CAT_WIN))

        return result[1][2][0]
```

**Complexity Analysis**

* **Time complexity**: Each state `(m, c, t)` is processed at most once when it becomes known, and each processing scans all incoming edges (the reverse adjacency). Hence O(`n^2 * (average degree)`) → O(`n^3`) in the worst case, same bound as the DFS but with a smaller constant factor because we avoid recursion overhead.
* **Space complexity**: O(`n^2`) for the `result` and `degree` tables plus the queue → O(`n^2`).

---

### Approach 3: Most Optimal – Same BFS with Early Exit (Optional Micro‑optimizations)

**Intuition**  
The BFS solution above is already optimal for this problem. Minor improvements include:

* Using integer encoding for states to reduce memory overhead.  
* Stopping the BFS early once the start state `(1,2,0)` is determined.  
* Pre‑computing adjacency lists without the hole for the cat.

These do not change asymptotic complexity but can shave constant factors. The core idea remains the retrograde BFS.

**Implementation** (same as Approach 2 with early stop)

```python
class Solution:
    def catMouseGame(self, graph):
        n = len(graph)
        DRAW, MOUSE_WIN, CAT_WIN = 0, 1, 2

        result = [[[DRAW] * 2 for _ in range(n)] for _ in range(n)]
        degree = [[[0] * 2 for _ in range(n)] for _ in range(n)]

        for m in range(n):
            for c in range(n):
                degree[m][c][0] = len(graph[m])
                degree[m][c][1] = len([x for x in graph[c] if x != 0])

        q = deque()
        for c in range(n):
            for t in (0, 1):
                result[0][c][t] = MOUSE_WIN
                q.append((0, c, t, MOUSE_WIN))
        for m in range(1, n):   # skip m==0 (mouse win)
            for t in (0, 1):
                result[m][m][t] = CAT_WIN
                q.append((m, m, t, CAT_WIN))

        while q:
            m, c, turn, res = q.popleft()
            # early exit if we already know the answer
            if m == 1 and c == 2 and turn == 0 and res != DRAW:
                return res
            prev_turn = 1 - turn
            if turn == 0:   # previous move was cat's
                for prev_c in graph[c]:
                    if prev_c == 0: continue
                    if result[m][prev_c][prev_turn] != DRAW: continue
                    if res == CAT_WIN:
                        result[m][prev_c][prev_turn] = CAT_WIN
                        q.append((m, prev_c, prev_turn, CAT_WIN))
                    else:
                        degree[m][prev_c][prev_turn] -= 1
                        if degree[m][prev_c][prev_turn] == 0:
                            result[m][prev_c][prev_turn] = MOUSE_WIN
                            q.append((m, prev_c, prev_turn, MOUSE_WIN))
            else:           # previous move was mouse's
                for prev_m in graph[m]:
                    if result[prev_m][c][prev_turn] != DRAW: continue
                    if res == MOUSE_WIN:
                        result[prev_m][c][prev_turn] = MOUSE_WIN
                        q.append((prev_m, c, prev_turn, MOUSE_WIN))
                    else:
                        degree[prev_m][c][prev_turn] -= 1
                        if degree[prev_m][c][prev_turn] == 0:
                            result[prev_m][c][prev_turn] = CAT_WIN
                            q.append((prev_m, c, prev_turn, CAT_WIN))

        return result[1][2][0]
```

**Complexity Analysis**

* **Time**: O(`n^3`) worst case, but often far less due to early termination.  
* **Space**: O(`n^2`).

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

Let's dry‑run the BFS on a tiny graph:

```
graph = [[1,2],      # 0 (hole) connects to 1 and 2
         [0,2],      # 1
         [0,1]]      # 2
```

Interpretation: triangle 0‑1‑2.

**Initial known states**

* Mouse win: `(mouse=0, cat=any, turn=any)` → result = 1  
  Enqueue: (0,0,0,1), (0,0,1,1), (0,1,0,1), (0,1,1,1), (0,2,0,1), (0,2,1,1)
* Cat win: `(mouse=cat≠0, turn=any)` → result = 2  
  Enqueue: (1,1,0,2), (1,1,1,2), (2,2,0,2), (2,2,1,2)

**Degree table (mouse turn / cat turn)**  
`degree[m][c][0] = len(graph[m])` (mouse moves)  
`degree[m][c][1] = len([x for x in graph[c] if x != 0])`

We’ll focus on the start state `(1,2,0)` (mouse at 1, cat at 2, mouse to move).

**Step‑by‑step propagation**

| Queue entry processed | Reasoning | Updated state(s) |
|-----------------------|-----------|------------------|
| (0,0,0,1) – mouse win | Mouse already in hole → any predecessor where mouse moved to 0 is a mouse win. Predecessors: cat’s previous turn, so look at states where cat moved to 0 (illegal) → none. | No new states. |
| (0,0,1,1) – mouse win | Same, cat’s turn, mouse already won. Predecessors: mouse’s previous turn → any mouse position that could move to 0. From graph, nodes 1 and 2 can move to 0. So we set `(1,0,0)` and `(2,0,0)` to mouse win (because mouse could win by moving to hole). Enqueue them. |
| … (process other mouse‑win terminals similarly) | … | … |
| Eventually we process `(1,0,0,1)` (mouse at 1, cat at 0, mouse to move). Since cat is in hole (illegal state for cat), we never actually generate it; the algorithm skips cat moves to 0, so this state never appears. | | |
| Process cat‑win terminals, e.g., `(1,1,0,2)`. Here mouse and cat share node 1 → cat win. Predecessors: mouse’s turn (since current turn = 0) → look at previous mouse positions that could move to 1. Neighbors of 1 are `[0,2]`. So predecessors are `(0,1,1)` and `(2,1,1)`. For each: since the result is a cat win and the player to move in predecessor is mouse, this move is bad for mouse → we decrement degree. If degree reaches zero we mark predecessor as cat win. | After processing all, we find that the start state `(1,2,0)` gets degree reduced to 0 and is marked as **cat win** (result = 2). | |

**Result**  
`result[1][2][0] = 2` → the cat wins with optimal play on this triangle graph.

(The actual LeetCode examples produce 0 (draw) and 1 (mouse win) respectively; the dry‑run above merely illustrates the mechanics.)

--- 

**Summary**  
We modeled the Cat and Mouse game as a directed graph of states, applied retrograde analysis (BFS from known terminals) to propagate wins/losses, and derived the optimal outcome in O(n³) time and O(n²) space—well within the limits (n ≤ 50). This pattern appears in many turn‑based game problems on LeetCode. Use the BFS approach when you need a deterministic, iterative solution; the DFS+memo version is useful for quick prototyping or when the state space is sparse.
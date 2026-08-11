# Greedy

## Video Solution

For more details about **Dota2 Senate**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=zZA5KskfMuQ).

## Concept

The problem can be viewed as a turn‑based elimination game.  
Each senator, when it is his turn, will try to ban the **earliest** opposing senator that is still able to vote, because removing the nearest opponent gives his own party the best chance to survive future rounds.  
This “always ban the next opponent” choice is a classic **greedy** decision: we make the locally optimal move (ban the closest enemy) without needing to look ahead, and it leads to the globally optimal outcome.

## When to Use It

Use a greedy simulation when you see:

- A process that proceeds in **turns** or **rounds**.
- Each participant can **remove** or **disable** another participant.
- The decision “who to remove” depends only on the current state (e.g., the nearest opponent) and not on future unknowns.
- The problem asks which side will remain after repeated eliminations.

## Template

```python
from collections import deque

def greedy_elimination(sequence, char_a, char_b):
    """
    Generic greedy elimination using two queues.
    Returns the winning party (char_a or char_b).
    """
    q_a, q_b = deque(), deque()
    # store the original index of each senator
    for i, ch in enumerate(sequence):
        if ch == char_a:
            q_a.append(i)
        else:
            q_b.append(i)

    # simulate rounds
    while q_a and q_b:
        i_a = q_a.popleft()
        i_b = q_b.popleft()
        # the senator with the smaller index gets to ban the other
        if i_a < i_b:
            # a survives and will act again in the next round
            q_a.append(i_a + len(sequence))   # offset by n to keep ordering
        else:
            q_b.append(i_b + len(sequence))

    return char_a if q_a else char_b
```

---

## LeetCode Problem Walkthrough

### Problem: 649. Dota2 Senate

https://leetcode.com/problems/dota2-senate/

### Approach 1: Brute Force (Round‑by‑Round Scan)

**Intuition**  
We can simulate the voting literally: in each round we scan the senate from left to right, letting each still‑active senator ban the next opposing senator that has not yet been banned in this round. Repeating rounds until only one party remains yields the answer. This is straightforward but inefficient because we may scan the array many times.

**Algorithm**
1. Convert the string to a list of characters so we can mark banned senators (e.g., set to `'X'`).
2. Repeat:
   - Initialize a variable `banned_opposite = None` to remember the nearest opponent we will ban.
   - Scan the list; for each senator that is not banned:
        - If we have not yet chosen a target to ban and the senator belongs to the opposite party, mark him as banned and remember that we have used our ban for this turn.
        - Otherwise, continue scanning (the senator gets to act and may ban later).
   - After the scan, if no senator of either party remains active, break.
3. Count remaining `'R'` and `'D'` to decide the winner.

**Implementation**

```python
class Solution:
    def predictPartyVictory(self, senate: str) -> str:
        s = list(senate)
        n = len(s)

        while True:
            radiant_active = dire_active = False
            ban_r = ban_d = False   # whether we have already used our ban this turn
            for i in range(n):
                if s[i] == 'X':          # already banned
                    continue
                if s[i] == 'R':
                    radiant_active = True
                    if not ban_d:       # we can ban a Dire
                        # find next Dire to ban
                        for j in range(i + 1, n):
                            if s[j] == 'D':
                                s[j] = 'X'
                                ban_d = True
                                break
                else:  # s[i] == 'D'
                    dire_active = True
                    if not ban_r:       # we can ban a Radiant
                        for j in range(i + 1, n):
                            if s[j] == 'R':
                                s[j] = 'X'
                                ban_r = True
                                break
            if not radiant_active:
                return "Dire"
            if not dire_active:
                return "Radiant"
```

**Complexity Analysis**

- Time complexity: O(n²) — each round may scan the whole string, and in the worst case we need O(n) rounds (e.g., alternating parties).
- Space complexity: O(n) — mutable copy of the senate string.

---

### Approach 2: Queue‑Based Greedy (Optimal)

**Intuition**  
Instead of rescanning the whole array each round, we only need to know the **order** in which senators will act.  
If we store the indices of all Radiant and Dire senators in two separate queues, the senator at the front of each queue is the next to act.  
Whenever the two fronts are compared, the senator with the smaller index gets to ban the other (because he speaks first).  
The banned senator is removed from his queue; the winner gets re‑enqueued with an index increased by `n` (the original length) to preserve his relative order in the *next* round.  
Repeating this until one queue is empty yields the winning party. This directly implements the greedy “ban the nearest opponent” rule in O(n) time.

**Algorithm**
1. Create two queues (`rad` and `dir`) and push the original index of each senator according to his party.
2. While both queues are non‑empty:
   - Pop the front index from each queue (`r` and `d`).
   - If `r < d`, the Radiant senator acts first and bans the Dire senator → push `r + n` back into `rad`.
   - Else, the Dire senator acts first → push `d + n` back into `dir`.
3. Return `"Radiant"` if `rad` is non‑empty, otherwise `"Dire"`.

**Implementation**

```python
from collections import deque

class Solution:
    def predictPartyVictory(self, senate: str) -> str:
        n = len(senate)
        rad, dir = deque(), deque()

        for i, s in enumerate(senate):
            if s == 'R':
                rad.append(i)
            else:
                dir.append(i)

        while rad and dir:
            r = rad.popleft()
            d = dir.popleft()
            if r < d:
                rad.append(r + n)      # Radiant bans this Dire
            else:
                dir.append(d + n)      # Dire bans this Radiant

        return "Radiant" if rad else "Dire"
```

**Complexity Analysis**

- Time complexity: O(n) — each senator is processed at most once per round, and each round removes exactly one senator, leading to O(n) total pops and pushes.
- Space complexity: O(n) — the two queues together store at most n indices.

---

### Approach 3: Queue‑Based Greedy with Explicit Round Counter (Alternative View)

**Intuition**  
The same idea as Approach 2 can be expressed by keeping an explicit “round” offset instead of adding `n` each time.  
We store pairs `(party, original_index)` in a single queue and simulate rounds: when a senator acts, we look ahead in the queue for the first opponent; if found, we remove that opponent and re‑insert the current senator at the end with an increased round number.  
This version makes it clearer that we are performing a **round‑based** simulation, but the asymptotic complexity remains identical.

**Algorithm**
1. Initialize a queue with tuples `(party, index)` for each senator.
2. While the queue contains both parties:
   - Pop the front senator `(p, idx)`.
   - Scan the queue (without popping) to find the first senator of the opposite party.
   - If none is found, the current party has won → break.
   - Otherwise, remove that opponent from the queue and push `(p, idx + n)` back to the end (representing the senator’s turn in the next round).
3. Return the party of the remaining senators.

**Implementation**

```python
from collections import deque

class Solution:
    def predictPartyVictory(self, senate: str) -> str:
        n = len(senate)
        q = deque()
        for i, ch in enumerate(senate):
            q.append((ch, i))

        while q:
            party, idx = q.popleft()
            # look for an opponent
            found_opponent = False
            temp = []
            while q:
                p, i = q.popleft()
                if p != party and not found_opponent:
                    # ban this opponent
                    found_opponent = True
                    # do NOT re-add the opponent
                else:
                    temp.append((p, i))
            # restore the remaining senators
            q.extend(temp)
            if not found_opponent:
                # no opponent left → current party wins
                return "Radiant" if party == 'R' else "Dire"
            # current senator gets to act again in the next round
            q.append((party, idx + n))

        # fallback (should never happen)
        return "Radiant"
```

**Complexity Analysis**

- Time complexity: O(n²) in the worst case because each senator may scan the whole queue to find an opponent.  
  (This version is presented mainly for conceptual clarity; the optimized two‑queue version (Approach 2) is preferred.)
- Space complexity: O(n) — the auxiliary queue and temporary list.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We dry‑run the optimal queue‑based algorithm on the example `senate = "RDD"`.

**Input:** `senate = "RDD"` (indices: 0‑R, 1‑D, 2‑D)

#### Initial queues
- `rad = [0]`
- `dir = [1, 2]`

#### Dry run table

| Step | rad.front | dir.front | Action (who bans whom)                     | rad after step          | dir after step          |
|------|-----------|-----------|--------------------------------------------|-------------------------|-------------------------|
| 1    | 0         | 1         | R(0) acts first → bans D(1)               | push 0+3=3 → `[3]`      | remove 1 → `[2]`        |
| 2    | 3         | 2         | D(2) acts first (2 < 3) → bans R(0)       | remove 0 (already gone) → `[]` | push 2+3=5 → `[5]` |
| 3    | (empty)   | 5         | `rad` empty → loop ends                    |                         |                         |

`rad` is empty, so the winner is **Dire**, matching the expected output.

The table shows how each senator’s effective index grows by `n` (=3) after surviving a round, preserving the original ordering for the next round. Once one queue empties, the other party’s senators are the only ones left to announce victory.
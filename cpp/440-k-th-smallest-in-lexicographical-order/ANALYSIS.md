# Trie

## Video Solution

For more details about **K-th Smallest in Lexicographical Order**, watch the walkthrough at [https://www.youtube.com/watch?v=wRubz1zhVqk](https://www.youtube.com/watch?v=wRubz1zhVqk)

## Concept

A **trie** (pronounced “try”) is a tree‑like data structure that stores strings where each node represents a common prefix.  
For the problem of finding the k‑th smallest number in lexicographical order we can imagine a **10‑ary trie** where each level corresponds to a digit (0‑9). The root represents an empty prefix, and each path from the root to a node spells out a number (ignoring leading zeros).  

Lexicographical order of numbers is exactly a **pre‑order traversal** of this trie: we visit a node, then recursively explore its children 0 … 9 in order.  
Instead of building the whole trie (which would be huge for n up to 10⁹), we can **count how many numbers lie under a given prefix** and decide whether to skip the entire subtree or step inside it.

## When to Use It

Use a trie‑style prefix‑counting technique when you see:

- A request for the k‑th element in **lexicographical / dictionary order** of numbers or strings.  
- The range is large (e.g., up to 10⁹) so enumerations) making enumeration impossible.  
- The underlying structure can be viewed as a tree where each node’s children are obtained by appending a digit (or character).  
- Phrases like “dictionary order”, “lexicographic order”, “like words in a dictionary”.

## Template

```python
def count_steps(n, prefix):
    """
    Returns how many numbers in [1, n] have the given `prefix`
    as a prefix in their decimal representation.
    """
    cur = prefix
    nxt = prefix + 1
    steps = 0
    while cur <= n:
        # numbers in [cur, min(nxt, n+1)) share the current prefix
        steps += min(nxt, n + 1) - cur
        cur *= 10
        nxt *= 10
    return steps


def find_kth_lexicographic(n, k):
    """
    Returns the k-th smallest integer in lexicographical order
    from 1 to n using the prefix‑counting trick.
    """
    curr = 1          # start at the smallest prefix
    k -= 1            # we already count the number `1` as the 1st

    while k > 0:
        steps = count_steps(n, curr)
        if steps <= k:          # the whole subtree of `curr` is before the answer
            curr += 1           # move to the next sibling
            k -= steps
        else:                   # the answer lies inside the subtree of `curr`
            curr *= 10          # go to the first child
            k -= 1              # we have consumed the node `curr` itself
    return curr
```

## LeetCode Problem Walkthrough

### Problem: 440. K-th Smallest in Lexicographical Order

https://leetcode.com/problems/k-th-smallest-in-lexicographical-order/

---

### Approach 1: Brute Force (Generate & Sort)

**Algorithm**  
1. Generate every integer from `1` to `n`.  
2. Convert each integer to a string.  
3. Sort the list of strings lexicographically.  
4. Return the integer value of the string at index `k‑1`.

**Implementation**

```python
class Solution:
    def findKthNumber(self, n: int, k: int) -> int:
        # Step 1: create list of string representations
        str_nums = [str(i) for i in range(1, n + 1)]
        # Step 2: sort lexicographically
        str_nums.sort()
        # Step 3: pick the k‑th (0‑based) element and convert back to int
        return int(str_nums[k - 1])
```

**Complexity Analysis**

- Time complexity: O(n log n) — we generate n strings and sort them.  
- Space complexity: O(n) — we store all n strings.

---

### Approach 2: DFS Lexicographic Traversal with Early Stop

**Intuition**  
Instead of materialising all numbers, we can walk the implicit 10‑ary trie in pre‑order, counting how many nodes we have visited. As soon as the counter reaches `k`, we have found the answer. This avoids the O(n log n) sort but still may visit many nodes in the worst case (still O(k) which can be up to n).

**Algorithm**  
1. Initialise `curr = 1` and a counter `k`.  
2. Perform a pre‑order DFS:  
   - If `k == 0`, return the current number.  
   - Decrease `k` by 1 for the current node (we have “visited” it).  
   - For each digit `d` from 0 to 9, recursively explore the child `curr*10 + d` **only if** it does not exceed `n`.  
3. The first time `k` reaches zero during the traversal we return the current number.

**Implementation**

```python
class Solution:
    def findKthNumber(self, n: int, k: int) -> int:
        self.k = k
        self.ans = None

        def dfs(prefix):
            if self.ans is not None:          # early exit if answer already found
                return
            if prefix > n:
                return
            # Visit the current node
            self.k -= 1
            if self.k == 0:
                self.ans = prefix
                return
            # Explore children in lexicographic order (0..9)
            for d in range(10):
                next_prefix = prefix * 10 + d
                if next_prefix > n:
                    break
                dfs(next_prefix)

        dfs(1)
        return self.ans
```

**Complexity Analysis**

- Time complexity: O(k) in the worst case (we may visit up to k nodes before stopping).  
- Space complexity: O(log₁₀ n) — recursion depth equals the number of digits of the answer (max ~10 for n ≤ 10⁹).

---

### Approach 3: Prefix‑Counting (Optimal)

**Intuition**  
The key observation is that we can **skip entire subtrees** if we know how many numbers they contain.  
For a given prefix `p`, all numbers with that prefix form a contiguous block in lexicographical order:  
`p, p0, p1, …, p9, p00, p01, …` etc.  
By counting how many numbers in `[1, n]` share this prefix (using a simple loop that expands the prefix by a factor of 10 each time), we can decide whether the k‑th number lies **inside** this block or **after** it.  
If the block size ≤ remaining `k`, we skip the whole block and move to the next sibling prefix (`p+1`).  
Otherwise, the answer is inside the block, so we go deeper (`p*10`) and account for the current prefix itself (`k‑‑`).  
This process runs in O(log₁₀ n) time because each step either moves to a sibling (increasing the prefix) or goes down a level (multiplying by 10), and the number of levels is bounded by the number of digits of `n`.

**Algorithm** (same as the template)

1. Start at `curr = 1`. Decrease `k` by one because we count the number `1` as the first.  
2. While `k > 0`:  
   a. Compute `steps = count_steps(n, curr)` – how many numbers are in the subtree rooted at `curr`.  
   b. If `steps <= k`: the entire subtree precedes the answer → `curr += 1`, `k -= steps`.  
   c. Else: the answer is inside this subtree → `curr *= 10`, `k -= 1` (we move to the first child and have consumed the node `curr`).  
3. When the loop ends, `curr` is the k‑th lexicographic number.

**Implementation**

```python
class Solution:
    def findKthNumber(self, n: int, k: int) -> int:
        """
        Returns the k-th smallest integer in lexicographical order from 1 to n.
        """
        curr = 1
        k -= 1                     # we already account for the number `1`

        while k > 0:
            steps = self._count_steps(n, curr)
            if steps <= k:         # skip whole subtree
                curr += 1
                k -= steps
            else:                  # go deeper
                curr *= 10
                k -= 1
        return curr

    def _count_steps(self, n: int, prefix: int) -> int:
        """
        Count how many integers in [1, n] have the given `prefix` as a prefix.
        """
        steps = 0
        cur = prefix
        nxt = prefix + 1
        while cur <= n:
            # numbers in [cur, min(nxt, n+1)) share the current prefix
            steps += min(nxt, n + 1) - cur
            cur *= 10
            nxt *= 10
        return steps
```

**Complexity Analysis**

- Time complexity: O(log₁₀ n) — each iteration either increments the prefix (moving to a sibling) or multiplies it by 10 (going down a level). The number of iterations is bounded by the number of digits of `n` (≤ 10 for n ≤ 10⁹).  
- Space complexity: O(1) — only a few integer variables are used.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimal algorithm on the example `n = 13, k = 2`.

**Input:** n = 13, k = 2  

We start with `curr = 1` and `k = k‑1 = 1` (since we count the number `1` as the first).

| Step | curr | k before | steps = count_steps(13, curr) | Action (steps ≤ k?) | curr after | k after |
|------|------|----------|-------------------------------|----------------------|------------|---------|
| 1    | 1    | 1        | 5  (numbers: 1,10,11,12,13)   | steps > k → go deeper| 10         | 0       |
| 2    | 10   | 0        | (loop ends because k == 0)   | —                    | 10         | 0       |

When `k` reaches zero, the current prefix `10` is the answer.

**Result:** 10, which matches the expected output.

---

This completes the lecture for the **Trie** topic using the problem *K-th Smallest in Lexicographical Order* as a concrete example. Future lectures will follow the next topic in the rotation sequence.
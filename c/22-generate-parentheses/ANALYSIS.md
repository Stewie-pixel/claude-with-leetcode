# Backtracking

## Video Solution

For more details about **Generate Parentheses**, watch the walkthrough at [https://www.youtube.com/watch?v=s9fokUqJ76A](https://www.youtube.com/watch?v=s9fokUqJ76A)

## Concept

Backtracking builds candidates incrementally and abandons a candidate (“backtracks”) as soon as it determines that the candidate cannot possibly lead to a valid solution.  
For parentheses, we keep track of how many opening `(` and closing `)` brackets we have placed so far. We can add an opening bracket if we still have openings left (`open < n`). We can add a closing bracket only if it would not violate validity (`close < open`). When both counts reach `n`, we have a complete, well‑formed string.

**Real‑world analogy:** Imagine you have `n` left shoes and `n` right shoes that must be paired in a line such that at any point you never have more right shoes than left shoes (otherwise a right shoe would be unmatched). You place shoes one by one, backtracking whenever the rule would be broken.

## When to Use It

Use backtracking when you see:
- “Generate all combinations” or “list all possibilities”
- A constraint that can be checked locally while building the solution (e.g., `close < open` for parentheses)
- The solution space is exponential but can be pruned early

## Template

```python
def backtrack(state, choices):
    if is_solution(state):
        record_solution(state)
        return
    for choice in choices:
        if makes_sense(state, choice):          # prune invalid branches
            state.append(choice)                # choose
            backtrack(state, choices)           # explore
            state.pop()                         # un‑choose (backtrack)
```

## LeetCode Problem Walkthrough

### Problem: 22. Generate Parentheses

https://leetcode.com/problems/generate-parentheses/

### Approach 1: Brute Force (Generate‑then‑Filter)

**Algorithm**
1. Generate every string of length `2n` consisting of `(` and `)`. There are `2^(2n)` such strings.
2. For each string, check if it is a well‑formed parentheses sequence by scanning left‑to‑right and maintaining a balance counter (increment for `(`, decrement for `)`). The string is valid iff the balance never goes negative and ends at zero.
3. Collect all valid strings.

**Implementation**

```python
def generateParenthesis_bruteforce(n: int) -> List[str]:
    def is_valid(s: str) -> bool:
        bal = 0
        for ch in s:
            if ch == '(':
                bal += 1
            else:  # ')'
                bal -= 1
            if bal < 0:          # more closing than opening at some point
                return False
        return bal == 0

    res = []
    # generate all 2^(2n) candidates via binary counting
    for mask in range(1 << (2 * n)):
        cur = []
        for i in range(2 * n):
            # 0 -> '(' , 1 -> ')'
            cur.append('(' if (mask >> i) & 1 == 0 else ')')
        candidate = ''.join(cur)
        if is_valid(candidate):
            res.append(candidate)
    return res
```

**Complexity Analysis**

- Time complexity: O(2^(2n) * n) — we generate 2^(2n) strings and each validation scans O(n) characters.  
- Space complexity: O(n) for the recursion‑free string plus O(k) output where k is the number of valid results (Catalan(n)).  

### Approach 2: Backtracking with Pruning (Open/Close Count)

**Intuition**
Instead of generating all strings and filtering, we construct only *prefixes* that can still lead to a valid solution. We know a prefix is extendable iff the number of closing brackets never exceeds the number of opening brackets (`close < open`). This insight lets us prune huge parts of the search tree.

**Algorithm**
- Recurse with parameters `(open, close, current_string)`.
- If `open == n` and `close == n`, we have a complete solution → add to result.
- If `open < n`, we can add `'('` and recurse.
- If `close < open`, we can add `')'` and recurse.

**Implementation**

```python
def generateParenthesis_backtrack(n: int) -> List[str]:
    res = []

    def backtrack(open_cnt: int, close_cnt: int, path: List[str]) -> None:
        if open_cnt == n and close_cnt == n:
            res.append(''.join(path))
            return
        if open_cnt < n:
            path.append('(')
            backtrack(open_cnt + 1, close_cnt, path)
            path.pop()                     # undo choice
        if close_cnt < open_cnt:
            path.append(')')
            backtrack(open_cnt, close_cnt + 1, path)
            path.pop()

    backtrack(0, 0, [])
    return res
```

**Complexity Analysis**

- Time complexity: O(4^n / sqrt(n)) — the nth Catalan number, which counts the valid strings.  
- Space complexity: O(n) for the recursion stack and the temporary `path` list (output not counted).  

### Approach 3: Dynamic Programming (Catalan DP)

**Intuition**
The set of valid parentheses of `n` pairs can be built from smaller solutions: a valid string can be seen as `( A ) B` where `A` uses `k` pairs and `B` uses `n‑1‑k` pairs, for `k` ranging from `0` to `n‑1`. This recurrence mirrors the Catalan numbers and lets us construct results iteratively without recursion.

**Algorithm**
- `dp[i]` = list of all valid strings with `i` pairs.
- Base: `dp[0] = [""]`.
- For each `i` from 1 to `n`:
  - For each split `k` from 0 to `i‑1`:
    - For each left in `dp[k]` and each right in `dp[i‑1‑k]`:
      - Add `"(" + left + ")" + right` to `dp[i]`.
- Return `dp[n]`.

**Implementation**

```python
def generateParenthesis_dp(n: int) -> List[str]:
    dp = [[] for _ in range(n + 1)]
    dp[0] = [""]                     # empty string for 0 pairs
    for pairs in range(1, n + 1):
        cur = []
        for left_pairs in range(pairs):
            right_pairs = pairs - 1 - left_pairs
            for left in dp[left_pairs]:
                for right in dp[right_pairs]:
                    cur.append('(' + left + ')' + right)
        dp[pairs] = cur
    return dp[n]
```

**Complexity Analysis**

- Time complexity: O(4^n / sqrt(n)) — same as backtracking; each Catalan number is produced once.  
- Space complexity: O(4^n / sqrt(n)) to store all results in `dp` (output size).  

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

### Dry Run (n = 2)

We trace the backtracking approach.

| Step | open | close | path   | Action                              |
|------|------|-------|--------|-------------------------------------|
| 1    | 0    | 0     | []     | try '(' (open < 2)                  |
| 2    | 1    | 0     | ['(']  | try '(' again                       |
| 3    | 2    | 0     | ['(', '('] | cannot add '(' (open == n) → try ')' (close < open) |
| 4    | 2    | 1     | ['(', '(', ')'] | try ')' again (close < open) |
| 5    | 2    | 2     | ['(', '(', ')', ')'] | open==close==n → record "(())" |
| 5b   | backtrack to step 4, pop last ')' |
| 6    | 2    | 1     | ['(', '('] | no more options → backtrack |
| 7    | backtrack to step 2, pop last '(' |
| 8    | 1    | 0     | ['(']  | now try ')' (close < open)          |
| 9    | 1    | 1     | ['(', ')'] | try '(' (open < n)                 |
|10    | 2    | 1     | ['(', ')', '('] | try ')' (close < open)            |
|11    | 2    | 2     | ['(', ')', '(', ')'] | record "()()"                    |
|12    | backtrack … eventually finish |

The two strings generated are `(())` and `()()`, matching the expected output for `n=2`.

---
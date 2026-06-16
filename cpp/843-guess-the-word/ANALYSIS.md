# Greedy

## Video Solution

For more details about **Guess the Word**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=COnrhAabeos)

## Concept

A greedy algorithm makes the locally optimal choice at each step with the hope that these choices will lead to a globally optimal solution.  
In *Guess the Word* we receive feedback after each guess: the number of exact character‑position matches with the secret word.  
Using that feedback we can eliminate every word that would not have produced the same number of matches.  
A greedy strategy picks the next guess that **maximally reduces** the remaining candidate set (or equivalently, maximizes the expected number of matches), hoping to converge to the secret word within the allowed number of guesses.

## When to Use It

Consider the greedy pattern when you see:

- A hidden target that can be queried and returns a scalar score (e.g., number of matches).
- After each query you can discard inconsistent candidates.
- You have a limited number of queries and need to shrink the search space quickly.
- The score is additive or can be estimated via per‑position frequencies (as in this problem).

If the problem exhibits these clues, a greedy choice that optimizes the immediate reduction often works well in practice.

## Template

Here is a reusable Python template for a greedy elimination approach:

```python
def find_secret_word(words, master):
    """
    Greedy elimination:
        1. Start with the full word list as the candidate pool.
        2. Pick a guess from the pool using a heuristic (e.g., max frequency score).
        3. Query Master.guess(guess) -> matches.
        4. If matches == len(word): we are done.
        5. Otherwise, keep only those words that would give the same
           number of matches with the guess.
        6. Repeat until the pool is empty or we run out of guesses.
    """
    pool = list(words)                     # current candidates
    while pool:
        # ---- 2. Choose a guess (greedy heuristic) ----
        guess = choose_guess(pool)         # implement your heuristic here

        # ---- 3. Query the master ----
        matches = master.guess(guess)

        # ---- 4. Check for success ----
        if matches == len(guess):          # all positions matched
            return

        # ---- 5. Filter the pool ----
        pool = [w for w in pool if match_count(guess, w) == matches]

def choose_guess(candidates):
    """
    Example heuristic: pick the word with the highest sum of
    per‑position character frequencies.
    """
    # frequency[i][c] = how many candidates have character c at position i
    freq = [[0] * 26 for _ in range(6)]
    for w in candidates:
        for i, ch in enumerate(w):
            freq[i][ord(ch) - 97] += 1

    best_word = None
    best_score = -1
    for w in candidates:
        score = sum(freq[i][ord(w[i]) - 97] for i in range(6))
        if score > best_score:
            best_score = score
            best_word = w
    return best_word

def match_count(a, b):
    return sum(1 for i in range(6) if a[i] == b[i])
```

## LeetCode Problem Walkthrough

### Problem: 843. Guess the Word

https://leetcode.com/problems/guess-the-word/

### Approach 1: Brute Force

**Algorithm**
1. Iterate through the given `words` list in order.
2. For each word, call `master.guess(word)`.
3. If the result equals 6 (all positions matched), we have found the secret and return.
4. Otherwise continue to the next word.
5. If we exhaust the list without finding the secret, we fail (the test guarantees this won’t happen when allowedGuesses ≥ |words|, but it may exceed the limit).

**Implementation**

```python
class Solution:
    def findSecretWord(self, words, master):
        for w in words:
            if master.guess(w) == 6:
                return
```

**Complexity Analysis**

- Time complexity: O(N·G) where N = number of words and G = guesses made (worst‑case G = N). Each guess is O(1) to call the API; the loop itself is O(N).
- Space complexity: O(1) – only a few scalar variables are used.

---

### Approach 2: Frequency‑Based Greedy (the solution you provided)

**Intuition**
At each step we want a guess that eliminates as many impossible words as possible.  
If we look at each position independently, the character that appears most often at that position is the most likely to match the secret.  
Summing the per‑position frequencies for a candidate word gives a score that estimates how many words share characters with it at each position.  
Choosing the word with the highest score tends to maximize the expected number of matches, thereby shrinking the pool aggressively.

**Algorithm**
1. Initialise `pool` with all words.
2. Repeat until the secret is found or we run out of allowed guesses:
   a. Compute frequency counts `freq[pos][ch]` for the current pool.
   b. For each word in the pool compute its *frequency score* = Σ freq[pos][word[pos]].
   c. Select the word with the maximum score as the next guess.
   d. Call `master.guess(guess)` → `matches`.
   e. If `matches == 6`, we are done.
   f. Otherwise, filter the pool to keep only words `w` where `match_count(guess, w) == matches`.
3. Return when the secret is guessed.

**Implementation**

```python
class Solution:
    def findSecretWord(self, words, master):
        def match_count(a, b):
            return sum(1 for i in range(6) if a[i] == b[i])

        pool = list(words)

        for _ in range(30):               # allowedGuesses ≤ 30 per constraints
            if not pool:
                break

            # 1️⃣ build frequency table for the current pool
            freq = [[0] * 26 for _ in range(6)]
            for w in pool:
                for i, ch in enumerate(w):
                    freq[i][ord(ch) - 97] += 1

            # 2️⃣ pick the word with the highest frequency score
            best_guess = pool[0]
            best_score = -1
            for w in pool:
                score = sum(freq[i][ord(w[i]) - 97] for i in range(6))
                if score > best_score:
                    best_score = score
                    best_guess = w

            # 3️⃣ query the master
            matches = master.guess(best_guess)
            if matches == 6:               # found the secret
                return

            # 4️⃣ shrink the pool
            pool = [w for w in pool if match_count(best_guess, w) == matches]
```

**Complexity Analysis**

- Let N be the number of words (≤ 100) and L = 6 (fixed word length).
- Each iteration:
  - Building the frequency table: O(N·L)
  - Scoring each word: O(N·L)
  - Filtering the pool: O(N·L) (match_count is O(L))
- In the worst case we may perform up to `allowedGuesses` iterations (≤ 30).
- **Time complexity**: O(allowedGuesses · N · L) → O(30·100·6) ≈ O(18 000), easily fast.
- **Space complexity**: O(N·L) for the frequency table + O(N) for the pool → O(N) overall.

---

### Approach 3: Minimax Greedy (worst‑case optimal)

**Intuition**
The frequency‑based heuristic works well in practice but does not guarantee the smallest possible remaining pool after a guess.  
A more conservative strategy is to pick the guess that **minimizes the maximum size of the remaining pool** over all possible feedback values (0‥6).  
This is the classic *minimax* approach used in Mastermind solvers: we look ahead one step and choose the word that leaves us with the best worst‑case scenario.

**Algorithm**
1. For each candidate `g` in the current pool:
   a. For each possible score `s` from 0 to 6, compute how many words in the pool would yield `s` matches with `g`.
   b. Let `worst[g]` be the largest of those counts (the biggest pool we could be left with).
2. Choose the guess `g` with the smallest `worst[g]` (break ties arbitrarily, e.g., by frequency score).
3. Proceed as in Approach 2: query, check for success, filter the pool.
4. Repeat until the secret is found or guesses run out.

**Implementation**

```python
class Solution:
    def findSecretWord(self, words, master):
        def match_count(a, b):
            return sum(1 for i in range(6) if a[i] == b[i])

        pool = list(words)

        for _ in range(30):
            if not pool:
                break

            # --- minimax selection ---
            def worst_case_size(g):
                # count how many words would remain for each possible score
                buckets = [0] * 7          # scores 0..6
                for w in pool:
                    s = match_count(g, w)
                    buckets[s] += 1
                return max(buckets)        # size of the largest bucket

            # pick guess with minimal worst-case bucket size
            best_guess = min(pool, key=worst_case_size)

            matches = master.guess(best_guess)
            if matches == 6:
                return

            pool = [w for w in pool if match_count(best_guess, w) == matches]
```

**Complexity Analysis**

- For each guess we evaluate every candidate `g` (≤ N) and, for each, scan the whole pool (≤ N) to fill the 7‑bucket array → O(N²·L) per iteration.
- With `allowedGuesses` ≤ 30 and N ≤ 100, this is still well within limits (≈ 30·10⁴·6 ≈ 1.8 M operations).
- **Time complexity**: O(allowedGuesses · N² · L).
- **Space complexity**: O(N) for the pool + O(1) auxiliary.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We illustrate the frequency‑based greedy approach (Approach 2) on the first example.

**Input**  
`secret = "acckzz"`  
`words = ["acckzz","ccbazz","eiowzz","abcczz"]`  
`allowedGuesses = 10`

#### Dry Run

| Step | Pool (before guess) | Chosen guess | Master.guess → matches | Pool after filtering (keep words with same matches) |
|------|----------------------|--------------|------------------------|------------------------------------------------------|
| 1    | ["acckzz","ccbazz","eiowzz","abcczz"] | "acckzz" (highest frequency score) | 6 | ["acckzz"] (only the secret remains) |
| 2    | ["acckzz"] | "acckzz" | 6 | – (we would stop after step 1) |

The algorithm finds the secret in just **one** guess, far below the allowed limit.

For a slightly harder case where the first guess is not the secret, consider:

`words = ["abcdef","ghijkl","mnopqr","stuvwx","yzabcd"]` (secret = "yzabcd")  

- Initial frequency table yields a tie; suppose we pick `"abcdef"`.  
- `master.guess("abcdef")` returns 2 (matches at positions 4 and 5: `'c'` and `'d'`).  
- Pool reduces to words sharing exactly those two matches → only `"yzabcd"` remains.  
- Second guess `"yzabcd"` returns 6 → success.

This demonstrates how each guess narrows the candidate set based on the feedback.

---

**Summary**

- The **Greedy** pattern is ideal when you can query a hidden target and receive a score that lets you eliminate inconsistent candidates.
- We presented three approaches:
  1. Brute force (linear scan).
  2. Frequency‑based greedy (practical, fast, used in the provided C++ solution).
  3. Minimax greedy (theoretically stronger worst‑case guarantee).
- Each approach includes clear intuition, algorithm, implementation, and full complexity analysis.
- A dry‑run shows how the algorithm quickly converges to the secret word.
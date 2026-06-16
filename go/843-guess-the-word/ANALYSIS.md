# Greedy

## Video Solution

For more details about **Guess the Word**, watch the walkthrough at [https://www.youtube.com/watch?v=KCWGtEcBN6c](https://www.youtube.com/watch?v=KCWGtEcBN6c)

## Concept

The *Guess the Word* problem is a variant of the Mastermind game.  
We have a list of candidate words (all length 6, all unique) and a hidden secret word that is guaranteed to be in the list.  
Each guess returns the number of exact character‑position matches with the secret.  
Using this feedback we can eliminate any candidate that would not have produced the same match count – those words are impossible to be the secret.

A greedy strategy picks, at each step, the word that **maximizes the information we gain**.  
A common proxy for information gain is the expected number of matches (or, equivalently, the sum of character frequencies at each position).  
Choosing the word with the highest such score tends to reduce the candidate pool quickly, staying within the allowed number of guesses.

## When to Use It

Use a greedy / frequency‑based heuristic when you see:

- A hidden target among a set of candidates.  
- Feedback after each guess that tells you *how many* positions matched (but not which).  
- A limited number of allowed guesses, forcing you to eliminate many candidates per guess.  
- The candidate set is moderate in size (≤ a few hundred) so O(N²) work per round is acceptable.

## Template

```python
def greedy_guess(words, master):
    """
    Greedy driver for Guess the Word.
    words   : list of candidate strings (all same length)
    master  : object with method .guess(word) -> int matches
    """
    candidates = words[:]                     # mutable pool
    while candidates:
        # 1️⃣  Choose a word according to some greedy criterion
        guess = choose_word(candidates)      # <-- problem‑specific

        # 2️⃣  Ask the master
        matches = master.guess(guess)

        # 3️⃣  If we found the secret we are done
        if matches == len(guess):             # all positions matched
            return

        # 4️⃣  Filter candidates: keep only those that would give the same feedback
        candidates = [w for w in candidates
                      if matches == sum(1 for a, b in zip(guess, w) if a == b)]
```

The only piece that changes per problem is `choose_word`.  
Below we show three increasingly sophisticated ways to implement it for *Guess the Word*.

---

## LeetCode Problem Walkthrough

### Problem: 843. Guess the Word  
https://leetcode.com/problems/guess-the-word

---

### Approach 1: Brute Force – Random / Sequential Guessing

**Algorithm**  
1. Keep the full list of words as the current candidate pool.  
2. Pick the first word in the pool (or pick one at random).  
3. Call `master.guess(word)`.  
4. If the result is 6 (all positions matched) we have found the secret → stop.  
5. Otherwise, filter the pool to retain only those words that would give the same match count with the guessed word.  
6. Repeat until we run out of guesses or the pool becomes empty.

This approach makes no attempt to pick an informative word; it simply walks through the list.

**Implementation**

```python
def get_matches(a: str, b: str) -> int:
    """Helper: number of identical characters at the same index."""
    return sum(1 for x, y in zip(a, b) if x == y)

def find_secret_word_bruteforce(words, master):
    candidates = words[:]                     # copy
    # we are allowed up to 30 guesses; the loop will break earlier if we succeed
    for _ in range(30):
        if not candidates:
            break
        guess = candidates[0]                # take the first available word
        matches = master.guess(guess)
        if matches == 6:                     # found the secret
            return
        # prune impossible words
        candidates = [w for w in candidates
                      if get_matches(guess, w) == matches]
```

**Complexity Analysis**

- Time complexity: O(G × N × L) — each guess we scan the whole pool (N) to filter, comparing strings of length L (=6). G ≤ 30 is the max number of guesses.  
- Space complexity: O(N) — we store the current candidate list.

---

### Approach 2: Frequency‑Based Greedy (Intermediate Optimization)

**Intuition**  
If we guess a word that contains many *common* letters in each position, we are more likely to get a higher match count, which in turn tells us more about the secret.  
A simple proxy for “commonness” is the sum, over all positions, of how often the character appears in that position across the whole candidate pool.  
Choosing the word with the highest total frequency tends to split the pool effectively.

**Algorithm**  
1. Build a 6 × 26 frequency matrix `freq[pos][ch]` = how many candidates have character `ch` at position `pos`.  
2. For each candidate word compute its **score** = Σₚ freq[p][word[p]].  
3. Pick the word with the maximum score as the next guess.  
4. Call `master.guess(word)` and filter the pool exactly as in the brute force method.  
5. Repeat.

**Implementation**

```python
def find_secret_word_freq(words, master):
    candidates = words[:]
    L = 6                                   # all words have length 6
    for _ in range(30):
        if not candidates:
            break

        # ---- build frequency matrix ----
        freq = [[0] * 26 for _ in range(L)]
        for w in candidates:
            for i, ch in enumerate(w):
                freq[i][ord(ch) - ord('a')] += 1

        # ---- score each candidate ----
        best_word = None
        best_score = -1
        for w in candidates:
            score = 0
            for i, ch in enumerate(w):
                score += freq[i][ord(ch) - ord('a')]
            if score > best_score:
                best_score = score
                best_word = w

        # ---- make the guess ----
        matches = master.guess(best_word)
        if matches == L:
            return

        # ---- prune ----
        candidates = [w for w in candidates
                      if get_matches(best_word, w) == matches]
```

**Complexity Analysis**

- Time complexity: O(G × N × L) — we still scan the pool to build the frequency matrix (O(N·L)) and to score each word (another O(N·L)).  
- Space complexity: O(N + L×26) = O(N) — the frequency matrix is constant size (6×26).

---

### Approach 3: Minimax (Worst‑Case Reduction) – Most Optimal

**Intuition**  
The frequency heuristic works well in practice but does not guarantee the smallest possible remaining set after a guess.  
A stronger guarantee comes from **minimax**: for each possible guess we simulate what the pool would look like for every possible feedback value (0…6) and keep the size of the largest resulting bucket.  
We then pick the guess that *minimizes* this worst‑case bucket size.  
This strategy is guaranteed to reduce the candidate set as fast as any deterministic method can, and it easily satisfies the allowed‑guess limit for N ≤ 100.

**Algorithm**  
1. For each candidate `g` in the pool:  
   a. Create an array `buckets[0..6]` initialized to 0.  
   b. For every word `w` in the pool compute `m = matches(g, w)` and increment `buckets[m]`.  
   c. The worst‑case size for guessing `g` is `max(buckets)`.  
2. Choose the candidate with the smallest worst‑case size (break ties arbitrarily).  
3. Call `master.guess(chosen)`, obtain the actual match count `m`.  
4. Filter the pool to keep only words `w` with `matches(chosen, w) == m`.  
5. Repeat until the secret is found.

**Implementation**

```python
def find_secret_word_minimax(words, master):
    candidates = words[:]
    L = 6
    for _ in range(30):
        if not candidates:
            break

        # ---- minimax selection ----
        best_guess = None
        best_worst = float('inf')
        for g in candidates:
            # count how many words would give each possible match count
            bucket = [0] * (L + 1)          # indices 0..6
            for w in candidates:
                m = get_matches(g, w)
                bucket[m] += 1
            worst = max(bucket)            # largest remaining group after this guess
            if worst < best_worst:
                best_worst = worst
                best_guess = g

        # ---- make the guess ----
        matches = master.guess(best_guess)
        if matches == L:
            return

        # ---- prune ----
        candidates = [w for w in candidates
                      if get_matches(best_guess, w) == matches]
```

**Complexity Analysis**

- Time complexity: O(G × N² × L) — for each guess we compare every pair of words to fill the bucket matrix (N²·L). With N ≤ 100 and G ≤ 30 this is still well within limits.  
- Space complexity: O(N) — we only store the current candidate list plus a small constant‑size bucket array.

---

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We illustrate the minimax approach on a tiny example.

**Dry Run**

Suppose the word list is:

```
0: "acckzz"
1: "ccbazz"
2: "eiowzz"
3: "abcczz"
```

Secret = `"acckzz"` (unknown to the algorithm).

| Step | Guess   | Matches with secret | Bucket sizes (0‑6) after guess | Worst‑case size | New pool (kept) |
|------|---------|---------------------|--------------------------------|-----------------|-----------------|
| 1    | "acckzz" (chosen by minimax) | 6 | [0,0,0,0,0,0,1] → max = 1 | 1 | `["acckzz"]` (only the secret remains) |
| 2    | (algorithm stops because matches == 6) | — | — | — | — |

The minimax selector correctly picks the secret on the first try because any other guess would leave a larger worst‑case bucket (e.g., guessing `"ccbazz"` would give buckets `[...,3]` for match = 3, leaving three candidates).

---

**Summary**

- **Brute force** works but may waste guesses.  
- **Frequency‑based greedy** is cheap (O(N·L) per round) and performs well in practice.  
- **Minimax** gives the strongest theoretical guarantee (still fast enough for the given constraints) by explicitly minimizing the largest possible remaining candidate set after each guess.

Use the minimax version when you want a provable bound on the number of guesses; otherwise the frequency heuristic is simpler and often sufficient.
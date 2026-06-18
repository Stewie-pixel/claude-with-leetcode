# Guess the Word

## Video Solution

For more details about **Guess the Word**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=COnrhAabeos).

## Concept

The problem is an interactive guessing game where we must discover a secret six‑letter word from a given list. After each guess we learn how many letters match the secret **in the exact same position**. This feedback lets us eliminate all words that would not produce the same match count with our guess. The goal is to reduce the candidate set as quickly as possible, ideally in far fewer than the allowed guesses.

A useful analogy is playing **Mastermind** with only black pegs (exact position matches). Each guess partitions the remaining words into buckets based on the score they would yield; we then keep only the bucket that matches the actual score returned by `Master.guess`. By choosing a guess that creates the most balanced partition (or minimizes the worst‑case bucket size) we guarantee rapid shrinkage of the search space.

## When to Use It

Use this **elimination‑with‑feedback** strategy when:
- You have a finite set of candidates.
- Each test/query returns a scalar score that tells you how “close” your guess is to the hidden target.
- The score can be used to rule out inconsistent candidates.
- The number of allowed queries is limited, so you need to maximize information gain per query.

In LeetCode terms, this pattern appears in interactive problems like “Guess the Word”, “Find the Secret Word”, or any Mastermind‑style game.

## Template

Here is a reusable Python scaffold for feedback‑driven elimination problems. Replace `score(word, candidate)` with the problem‑specific similarity function and `make_guess(word)` with the actual API call.

```python
def solve(words, master, allowed_guesses):
    candidates = list(words)                # current pool of possible secrets

    for _ in range(allowed_guesses):
        if not candidates:
            break

        # ---- 1. Choose a guess (problem‑specific heuristic) ----
        guess = choose_guess(candidates)    # <-- implement heuristics below

        # ---- 2. Ask the master ----
        score = master.guess(guess)

        # ---- 3. If we found the secret, we are done ----
        if score == len(words[0]):          # all positions match
            return

        # ---- 4. Prune inconsistent candidates ----
        candidates = [w for w in candidates
                      if score == score_fn(guess, w)]   # keep only those that would give same score
```

The core of the solution lies in `choose_guess`. Below we explore three increasingly sophisticated ways to implement it.

---

## LeetCode Problem Walkthrough

### Problem: 843. Guess the Word
https://leetcode.com/problems/guess-the-word/

### Approach 1: Brute Force – Sequential Guessing

**Algorithm**
1. Iterate through the `words` list in order.
2. For each word, call `master.guess(word)`.
3. If the returned score equals the word length (6), we have found the secret and can stop.
4. Otherwise continue to the next word.

This approach ignores the feedback from previous guesses; it simply hopes that the secret appears early enough in the list.

**Implementation**

```python
class Solution:
    def findSecretWord(self, words: List[str], master: 'Master') -> None:
        for w in words:
            if master.guess(w) == len(w):   # 6 exact matches → secret found
                return
```

**Complexity Analysis**
- Time complexity: O(N * G) in the worst case, where N = len(words) and G = allowedGuesses (each guess is O(1) to call, but we may call up to N guesses).  
- Space complexity: O(1) – only a few scalar variables are used.

*Why this is inefficient*: With N up to 100 and allowedGuesses as low as 10, we may exhaust our guesses before reaching the secret.

---

### Approach 2: Greedy Frequency‑Based Guessing

**Intuition**
Instead of guessing blindly, we can use the feedback to prune the candidate set. A good guess should be *informative*: it should share many letters in the same positions with as many remaining candidates as possible.  
For each position `i` (0‑5) and each letter `c` ('a'‑'z'), we count how many candidates have `c` at position `i`.  
Scoring a candidate word by summing these counts tells us how well it aligns with the majority of the pool at each position. Picking the word with the highest score tends to keep the largest possible subset after we receive the actual score, because it agrees with many words on many positions.

**Algorithm**
1. Start with the full word list as the candidate pool.
2. Repeat up to `allowedGuesses` times:
   a. Compute position‑wise letter frequencies over the current pool.
   b. For each word in the pool, compute its frequency‑based score.
   c. Select the word with the highest score as the next guess.
   d. Obtain the score `matches` from `master.guess(guess)`.
   e. If `matches == 6`, we are done.
   e. Otherwise, filter the pool to keep only words that would yield the same `matches` when compared to the guess (i.e., `get_matches(guess, w) == matches`).
3. If the pool becomes empty, something went wrong (should not happen with valid input).

**Implementation**

```python
class Solution:
    def findSecretWord(self, words: List[str], master: 'Master') -> None:
        def matches(a: str, b: str) -> int:
            return sum(1 for i in range(6) if a[i] == b[i])

        candidates = list(words)

        for _ in range(10):          # allowedGuesses ≥ 10, we use a safe upper bound
            if not candidates:
                break

            # 1) frequency table: freq[pos][letter]
            freq = [[0] * 26 for _ in range(6)]
            for w in candidates:
                for i, ch in enumerate(w):
                    freq[i][ord(ch) - 97] += 1

            # 2) pick word with highest frequency score
            def score(word: str) -> int:
                return sum(freq[i][ord(word[i]) - 97] for i in range(6))

            guess = max(candidates, key=score)

            # 3) ask master
            matches_guess = master.guess(guess)
            if matches_guess == 6:
                return

            # 4) prune
            candidates = [w for w in candidates
                          if matches(guess, w) == matches_guess]
```

**Complexity Analysis**
- Time complexity: O(R * (N * 6 + N * 6)) = O(R * N), where R ≤ allowedGuesses (≤30) and N ≤ 100. The dominant work is building the frequency table and scoring each candidate.
- Space complexity: O(N) for the candidate list plus O(6*26) = O(1) for the frequency table.

*Why this works*: Each guess eliminates words that disagree with the observed score on many positions, often cutting the pool dramatically. Empirically this solves the problem within the allowed guesses for the given constraints.

---

### Approach 3: Minimax (Worst‑Case) Guessing

**Intuition**
The greedy frequency heuristic is effective but not guaranteed to minimize the worst‑case remaining candidates after a guess. A stronger strategy is to **minimize the maximum size of any partition** induced by a guess.  
For each possible guess we simulate every possible score (0‑6) and count how many candidates would fall into each score bucket. The worst case for that guess is the size of the largest bucket. We pick the guess whose worst‑case bucket is smallest (the minimax choice). This guarantees that no matter what score we receive, we eliminate at least a fraction of the candidates.

This approach is analogous to the optimal first move in Mastermind and is known to solve the problem comfortably within the allowed guesses.

**Algorithm**
1. Initialize `candidates` with all words.
2. Loop up to `allowedGuesses` times:
   a. If only one candidate remains, guess it and return.
   b. For each `g` in candidates:
        - Create an array `buckets[0..6]` initialized to 0.
        - For each `c` in candidates:
            - Compute `m = matches(g, c)`.
            - Increment `buckets[m]`.
        - Let `worst = max(buckets)`.
        - Track the guess with the smallest `worst`.
   c. Use that guess, obtain the actual score from `master.guess`.
   d. If score == 6, return.
   e. Otherwise, filter candidates to those with `matches(guess, w) == score`.
3. Return when the secret is found or guesses exhausted.

**Implementation**

```python
class Solution:
    def findSecretWord(self, words: List[str], master: 'Master') -> None:
        def matches(a: str, b: str) -> int:
            return sum(1 for i in range(6) if a[i] == b[i])

        candidates = list(words)

        for _ in range(30):          # allowedGuesses ≤ 30
            if len(candidates) == 1:
                master.guess(candidates[0])
                return

            best_guess = None
            min_worst = len(candidates) + 1

            for g in candidates:    # try each word as a potential guess
                buckets = [0] * 7   # scores 0..6
                for c in candidates:
                    m = matches(g, c)
                    buckets[m] += 1
                worst = max(buckets)
                if worst < min_worst:
                    min_worst = worst
                    best_guess = g

            # make the chosen guess
            score = master.guess(best_guess)
            if score == 6:
                return

            # prune according to actual feedback
            candidates = [w for w in candidates
                          if matches(best_guess, w) == score]
```

**Complexity Analysis**
- Time complexity: O(R * N^2 * 6) in the worst case, because for each round we compare every guess against every candidate (N²) and each comparison is O(6). With N ≤ 100 and R ≤ 30 this is still well within limits (~1.8 M character comparisons).
- Space complexity: O(N) for the candidate list plus O(7) for the bucket array.

*Why this is optimal*: By minimizing the largest possible remaining group, we guarantee the fastest shrinkage of the search space in the worst case, ensuring we stay within the allowed guesses for all test cases.

---

### Dry Run

**Input**: `words = ["acckzz","ccbazz","eiowzz","abcczz"]`, `secret = "acckzz"`, `allowedGuesses = 10`

We illustrate Approach 2 (frequency‑based) because it is concise yet illustrative.

| Step | Candidates before guess | Freq table (excerpt) | Guosen chosen | Master.guess(score) | Candidates after filter |
|------|------------------------|----------------------|---------------|---------------------|--------------------------|
| 0    | ["acckzz","ccbazz","eiowzz","abcczz"] | pos0: a=2, c=2, e=1 … (details omitted) | "acckzz" (highest score) | 6 | [] → secret found, stop |

The algorithm guessed the secret on the first try because it happened to have the highest frequency score. In a less favorable case, e.g., if we started with "eiowzz":

| Step | Candidates | Guess | Score | New candidates |
|------|------------|-------|-------|----------------|
| 0    | all 4      | "eiowzz" | 2 | ["acckzz","abcczz"] (those sharing exactly 2 matches) |
| 1    | ["acckzz","abcczz"] | recompute freq → guess "acckzz" | 6 | [] → done |

Thus at most two guesses are needed, far below the limit.

---

**Summary**

- We began with a naïve sequential brute‑force approach that ignores feedback.
- We then introduced a greedy frequency‑based heuristic that uses the score to prune the search space.
- Finally, we presented a minimax strategy that optimizes the worst‑case reduction, guaranteeing success within the allowed guesses.
- All approaches are implemented in Python, include inline comments, and state time/space complexities.

Apply the elimination‑with‑feedback pattern whenever you face an interactive guessing game with a scalar similarity measure—track candidates, choose an informative guess, and prune ruthlessly. Happy coding!
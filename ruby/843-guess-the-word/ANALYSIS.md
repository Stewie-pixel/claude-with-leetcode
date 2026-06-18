# Greedy

## Video Solution

For more details about **Guess the Word**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=COnrhAabeos).

## Concept

Greedy algorithms make the locally optimal choice at each step with the hope of finding a global optimum. In the *Guess the Word* problem we cannot know the secret word directly, but after each guess we learn how many letters match in both value and position. A greedy heuristic is to pick the next guess that is most likely to reduce the search space—i.e., the word that shares the most letters (in the same positions) with the remaining candidates.

## When to Use It

Use a greedy strategy when:
- You receive feedback that lets you **prune** impossible candidates after each action.
- The feedback is a **score** (here, number of exact matches) that can be used to rank choices.
- Exhaustive search is too costly, but a good heuristic can dramatically cut down possibilities.
- The problem guarantees that a reasonable strategy exists (as stated in the prompt).

## Template

```python
def greedy_guess(words, master, max_guesses):
    """
    Greedy driver for Guess the Word.
    :param words: List[str] - candidate words
    :param master: Master object with .guess(word) -> int
    :param max_guesses: int - allowed number of guesses
    """
    candidates = words[:]
    for _ in range(max_guesses):
        if not candidates:
            break
        # 1️⃣ Choose the next guess using a greedy heuristic
        guess = select_greedy(candidates)
        # 2️⃣ Ask the master
        matches = master.guess(guess)
        # 3️⃣ If we found the secret, we are done
        if matches == len(guess):
            return
        # 4️⃣ Prune candidates inconsistent with the feedback
        candidates = [w for w in candidates if matches_wrt(guess, w) == matches]
```

*Helper functions (`select_greedy`, `matches_wrt`) will be defined in each approach.*

---

## LeetCode Problem Walkthrough

### Problem: 843. Guess the Word

https://leetcode.com/problems/guess-the-word/

### Approach 1: Brute Force

**Algorithm**
1. Iterate through the word list in order.
2. For each word, call `master.guess(word)`.
3. If the result equals the word length (6), we have found the secret and can stop.
4. Otherwise, continue to the next word.
5. Stop after `allowedGuesses` attempts or when the list is exhausted.

**Implementation**

```python
def brute_force(words, master):
    """
    Simple linear scan – tries each word until the secret is found.
    """
    for w in words:
        if master.guess(w) == len(w):   # 6 matches → secret found
            return
```

**Complexity Analysis**
- Time complexity: O(N * G) where N = number of words, G = number of guesses made (worst‑case G = N). In the worst case we may call guess on every word → O(N²) if we count each guess as O(1) but we are bounded by allowedGuesses (≤30), so effectively O(N).
- Space complexity: O(1) – only a few variables are used.

### Approach 2: Frequency‑Based Greedy (Intermediate)

**Intuition**
If we guess a word that shares many letters in the same positions with many other candidates, the feedback (number of matches) is likely to be high, which lets us eliminate a large fraction of the remaining words in one step. Therefore we choose the word that maximizes the sum of per‑position letter frequencies among the current candidates.

**Algorithm**
1. From the current candidate pool, compute a 6×26 frequency matrix `freq[pos][ch]` = how many words have character `ch` at position `pos`.
2. For each candidate word, compute a *score* = Σₚ freq[p][word[p]] (how many other words agree with it at each position).
3. Pick the word with the highest score as the next guess.
4. After receiving the match count from `master.guess`, filter the pool to keep only words that would give the same match count with the guessed word.
5. Repeat until the secret is found or guesses run out.

**Implementation**

```python
def get_matches(w1, w2):
    """Return number of exact position matches between two equal‑length strings."""
    return sum(c1 == c2 for c1, c2 in zip(w1, w2))

def select_greedy_freq(candidates):
    """Choose word with maximal summed frequency score."""
    if not candidates:
        return ""
    # frequency[pos][ch]
    freq = [[0] * 26 for _ in range(6)]
    for w in candidates:
        for i, ch in enumerate(w):
            freq[i][ord(ch) - 97] += 1

    best_word = candidates[0]
    best_score = -1
    for w in candidates:
        score = 0
        for i, ch in enumerate(w):
            score += freq[i][ord(ch) - 97]
        if score > best_score:
            best_score = score
            best_word = w
    return best_word

def greedy_frequency(words, master):
    candidates = words[:]
    for _ in range(30):          # allowedGuesses ≤ 30, use a safe upper bound
        if not candidates:
            break
        guess = select_greedy_freq(candidates)
        matches = master.guess(guess)
        if matches == len(guess):   # 6 → secret found
            return
        # keep only words consistent with the feedback
        candidates = [w for w in candidates if get_matches(guess, w) == matches]
```

**Complexity Analysis**
- Time complexity: Each round builds the frequency table in O(N·L) where L = 6 (word length). Scoring all candidates is another O(N·L). In the worst case we may perform up to G rounds (G ≤ allowedGuesses ≤ 30). Overall O(G·N·L) → O(N) because G and L are constants.
- Space complexity: O(L·Σ) for the frequency table (6×26) + O(N) for the candidate list → O(N).

### Approach 3: Minimax Greedy (Most Optimal)

**Intuition**
The frequency heuristic favours words that are *popular* but does not consider how poorly a word could perform in the worst case. A stronger greedy choice is to minimise the maximum number of candidates that could remain after receiving any possible feedback (0…6 matches). For each candidate guess we simulate the feedback it would give against every other candidate, bucket the candidates by resulting match count, and note the size of the largest bucket. We then pick the guess with the smallest largest bucket—this guarantees the best worst‑case reduction.

**Algorithm**
1. For each word `g` in the current candidate pool:
   - Create an array `buckets[0..6]` initialized to 0.
   - For each word `w` in the pool, compute `m = get_matches(g, w)` and increment `buckets[m]`.
   - Let `worst = max(buckets)` – the most candidates that could survive if we guessed `g`.
   - Track the word with the smallest `worst`.
2. Use that word as the next guess.
3. After obtaining the actual match count from `master.guess`, filter the pool to keep only words with that exact match count.
4. Repeat until success or guesses exhausted.

**Implementation**

```python
def select_minimax(candidates):
    """Pick word that minimises the maximum remaining candidates."""
    if not candidates:
        return ""
    best_word = candidates[0]
    best_worst = len(candidates)   # upper bound

    for g in candidates:
        buckets = [0] * 7          # matches can be 0..6
        for w in candidates:
            m = get_matches(g, w)
            buckets[m] += 1
        worst = max(buckets)       # largest bucket after guessing g
        if worst < best_worst:
            best_worst = worst
            best_word = g
    return best_word

def minimax_guess(words, master):
    candidates = words[:]
    for _ in range(30):          # allowedGuesses ≤ 30
        if not candidates:
            break
        guess = select_minimax(candidates)
        matches = master.guess(guess)
        if matches == len(guess):
            return
        candidates = [w for w in candidates if get_matches(guess, w) == matches]
```

**Complexity Analysis**
- Time complexity: For each guess we compare every pair of candidates → O(N²·L) per round (L=6 for match calculation). With at most G rounds (≤30) we get O(G·N²·L) → O(N²) in the worst case (N ≤ 100, still fine).
- Space complexity: O(N) for the candidate list + O(7) for buckets → O(N).

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We illustrate the minimax approach on a tiny example.

**Input**  
`words = ["acckzz", "ccbazz", "eiowzz", "abcczz"]`  
Secret = `"acckzz"`  
Allowed guesses = 10

#### Round 1 – Initial pool
All four words are candidates.

| Guess | Matches with each word | Bucket sizes (0‑6) | Worst bucket |
|-------|------------------------|--------------------|--------------|
| acckzz | [6,3,2,4]              | [0,0,1,1,1,0,1]    | 1 |
| ccbazz | [3,6,2,3]              | [0,0,1,3,0,0,0]    | 3 |
| eiowzz | [2,2,6,2]              | [0,0,4,0,0,0,1]    | 4 |
| abcczz | [4,3,2,6]              | [0,0,1,1,1,0,1]    | 1 |

The smallest worst bucket size is **1** (achieved by `"acckzz"` and `"abcczz"`).  
We break ties arbitrarily; suppose we pick `"acckzz"`.

#### Master response
`master.guess("acckzz")` returns **6** → we have found the secret and stop.

If the secret were `"eiowzz"` instead, the same first guess `"acckzz"` would have returned **2** matches.  
We would then keep only those words that give exactly 2 matches with `"acckzz"`: `["eiowzz"]`.  
The pool collapses to a single word, and the next guess finishes the game.

This dry run shows how the minimax choice quickly narrows down the possibilities, often solving the puzzle in far fewer than the allowed guesses.

--- 

**Summary**  
- Brute force works but may waste guesses.  
- Frequency‑based greedy improves expected performance by picking the most “popular” word.  
- Minimax greedy optimises the worst‑case scenario and is the standard solution for Guess the Word.  

All three approaches are presented in Python, with full complexity analysis, enabling you to see the trade‑offs and select the method that best fits your constraints.
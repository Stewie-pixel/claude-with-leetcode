# Greedy

## Video Solution

For more details about **Guess the Word**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=COnrhAabeos).

## Concept

The *Guess the Word* problem is a variant of Mastermind/Wordle where we receive feedback after each guess: the number of characters that match both in value **and** position.  
Instead of trying words at random, we can use that feedback to **prune** the candidate set. A greedy strategy picks, at each step, the word that is most “aligned” with the remaining candidates – i.e., the word that shares the most letters in the same positions with the rest of the pool. Guessing such a word tends to eliminate many unsuitable candidates, keeping the number of required guesses low.

## When to Use It

Use a greedy filtering approach when you see:

- A hidden target must be identified from a known list.
- After each attempt you receive a quantitative hint (e.g., number of exact matches, bulls‑and‑cows, Wordle‑style feedback).
- The hint lets you discard any candidate that would not produce the same hint for your guess.
- The search space is moderate (≤ 100‑200 items) so that O(N²) preprocessing per round is still fine.

## Template

```python
from typing import List

# The Master interface is provided by LeetCode; we only call master.guess(word).

def get_matches(w1: str, w2: str) -> int:
    """Return number of positions where w1 and w2 have the same character."""
    return sum(c1 == c2 for c1, c2 in zip(w1, w2))

def find_secret_word(words: List[str], master: 'Master') -> None:
    candidates = words[:]                     # mutable list of possible secrets

    while candidates:                         # loop until we find it or run out
        # ---------- Greedy word selection ----------
        # frequency[i][ch] = how many candidates have character ch at position i
        freq = [[0] * 26 for _ in range(6)]
        for w in candidates:
            for i, ch in enumerate(w):
                freq[i][ord(ch) - 97] += 1

        # score a word by sum of frequencies of its letters at each position
        best_word = candidates[0]
        best_score = -1
        for w in candidates:
            score = sum(freq[i][ord(w[i]) - 97] for i in range(6))
            if score > best_score:
                best_score = score
                best_word = w

        # ---------- Make the guess ----------
        matches = master.guess(best_word)
        if matches == 6:                      # found the secret
            return

        # ---------- Filter candidates ----------
        candidates = [w for w in candidates
                      if get_matches(best_word, w) == matches]
```

## LeetCode Problem Walkthrough

### Problem: 843. Guess the Word

https://leetcode.com/problems/guess-the-word/

### Approach 1: Brute Force

**Algorithm**  
Iterate through the word list, guessing each word in turn via `master.guess`.  
Stop as soon as a guess returns `6` (all characters match).  
If we exhaust the list without success, we have failed (the problem guarantees a solution exists within the allowed guesses, but this naïve method may exceed the limit).

**Implementation**

```python
def find_secret_word_bruteforce(words: List[str], master: 'Master') -> None:
    for w in words:
        if master.guess(w) == 6:
            return
```

**Complexity Analysis**

- Time complexity: O(N · G) — where N = number of words, G = number of guesses made (worst‑case G = N).  
- Space complexity: O(1) — only a few scalar variables are used.

### Approach 2: Greedy Frequency Heuristic (Optimized)

**Intuition**  
A good guess should be “compatible” with as many remaining candidates as possible.  
By counting, for each position, how often each letter appears among the candidates, we can score every word by the sum of those frequencies.  
Picking the highest‑scoring word tends to maximize the number of candidates that share letters in the same places, which usually yields a larger pruning set after we see the match count.

**Algorithm**  

1. Start with the full word list as candidates.  
2. While candidates remain:  
   a. Build a 6 × 26 frequency table `freq[pos][ch]`.  
   b. Score each candidate by `sum_{pos} freq[pos][candidate[pos]]`.  
   c. Choose the candidate with the highest score as the next guess.  
   d. Call `master.guess(guess)` → obtain `matches`.  
   e. If `matches == 6`, we are done.  
   f. Otherwise, keep only those candidates `w` where `get_matches(guess, w) == matches`.  
   (This discards every word that would have produced a different feedback.)  
3. The loop ends when the secret is found.

**Implementation**  
(See the template above.)

**Complexity Analysis**

- Let N = number of words, L = 6 (word length).  
- Each iteration: building the frequency table costs O(N·L); scoring all candidates also O(N·L); filtering costs O(N·L).  
- The number of iterations is bounded by `allowedGuesses` (≤ 30).  
- **Time complexity:** O(allowedGuesses · N·L) → O(N) in practice because `allowedGuesses` and L are constants.  
- **Space complexity:** O(N·L) for the frequency table (≈ 6·26 integers) plus O(N) for the candidate list → O(N).

### Approach 3: Minimax (Optimal Worst‑Case) – Optional

**Intuition**  
Instead of maximizing expected overlap, we can try to minimize the worst‑case size of the remaining candidate set after a guess.  
For each possible guess we simulate the feedback (0‥6) and compute how many candidates would survive each outcome; the worst case is the maximum of those counts.  
Choosing the guess with the smallest worst‑case guarantees the fewest remaining possibilities, leading to a logarithmic‑like reduction in search space.

**Algorithm**  

1. Candidates = current word list.  
2. For each word `g` in candidates:  
   a. Create an array `buckets[0..6]` initialized to 0.  
   b. For each word `c` in candidates:  
        `m = get_matches(g, c); buckets[m] += 1`  
   c. `worst = max(buckets)` – the largest bucket size if we guessed `g`.  
   d. Remember the guess with the smallest `worst`.  
3. Guess that word, obtain actual `matches`, and filter candidates as before.  
4. Repeat until the secret is found.

**Implementation** (omitted for brevity – follows the same scaffolding as the greedy version, only the selection step changes).

**Complexity Analysis**

- For each guess we examine every pair of candidates → O(N²·L) time.  
- Filtering is O(N·L).  
- With N ≤ 100 and allowedGuesses ≤ 30 this is still acceptable in practice, but slower than the greedy heuristic.  
- **Space complexity:** O(N) for candidates + O(7) for buckets → O(N).

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the greedy algorithm on the sample from the statement.

**Input**  
`words = ["acckzz","ccbazz","eiowzz","abcczz"]`  
`secret = "acckzz"`  
`allowedGuesses = 10`

#### Step‑by‑step

| Iteration | Candidates before guess | Frequency table (pos 0‑5, letters a‑z) – only non‑zero entries shown | Scores (sum of freq) | Chosen guess | master.guess → matches | Candidates after filtering (those with same matches) |
|-----------|-------------------------|--------------------------------------------------------------------------|----------------------|--------------|------------------------|------------------------------------------------------|
| 1         | ["acckzz","ccbazz","eiowzz","abcczz"] | pos0: a(2), c(2) ; pos1: c(2), b(1), i(1) ; pos2: c(2), b(1), o(1), b(1) ; pos3: k(2), a(1), w(1), c(1) ; pos4: z(4) ; pos5: z(4) | acckzz: 13 ; ccbazz: 11 ; eiowzz: 9 ; abcczz: 12 | **acckzz** (highest) | 6 (exact match) | ["acckzz"] (only word with 6 matches) |
| 2         | ["acckzz"] | (trivial) | acckzz: 6 | acckzz | 6 | [] (loop ends) |

The algorithm found the secret in the very first guess, well within the allowed limit.

If the first guess had not been the secret, the filtering step would have removed all words incompatible with the observed match count, drastically shrinking the search space for the next round. This illustrates why the greedy frequency heuristic works well in practice.
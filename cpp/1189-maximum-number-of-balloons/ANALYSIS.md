# Arrays & Hashing

## Video Solution

For more details about **Maximum Number of Balloons**, watch the walkthrough at [https://www.youtube.com/watch?v=G9xeB2-7PqY](https://www.youtube.com/watch?v=G9xeB2-7PqY)

## Concept

The core idea is to count how many times each required letter appears in the given string.  
The word **balloon** needs: `b:1, a:1, l:2, o:2, n:1`.  
If we know the frequency of each letter in `text`, the number of complete balloons we can build is limited by the scarcest resource after adjusting for letters that appear more than once in the word (here `l` and `o` need two copies each).  
Thus the answer is  

```
min( freq['b'],
     freq['a'],
     freq['l'] // 2,
     freq['o'] // 2,
     freq['n'] )
```

A real‑world analogy: Imagine you have a bag of letters and you want to assemble as many identical toy kits as possible. Each kit needs a fixed list of parts. You count how many of each part you have, then see which part runs out first; that determines how many kits you can finish.

## When to Use It

- You are asked to form the maximum number of copies of a **target word** (or string) from a source string.
- Each character can be used **at most once**.
- The source consists only of lowercase letters (or a small, fixed alphabet).
- The problem reduces to counting frequencies and taking a minimum over scaled counts.

## Template

```python
from collections import Counter

def max_instances(source: str, target: str) -> int:
    # 1. Count characters in source
    src_cnt = Counter(source)

    # 2. Count characters needed for one target
    tgt_cnt = Counter(target)

    # 3. For each required character, see how many times we can satisfy it
    ans = float('inf')
    for ch, need in tgt_cnt.items():
        have = src_cnt.get(ch, 0)
        ans = min(ans, have // need)   # integer division gives how many copies we can support

    return 0 if ans == float('inf') else ans
```

*Replace `Counter` with a fixed‑size array of length 26 if you know the input is only lowercase letters.*

---

## LeetCode Problem Walkthrough

### Problem: 1189. Maximum Number of Balloons

https://leetcode.com/problems/maximum-number-of-balloons/

### Approach 1: Brute Force – Simulate Balloon Construction

**Algorithm**  
Repeatedly try to build one instance of "balloon" by checking whether we have enough of each required letter. If we do, decrement those letters from the available pool and increase the answer. Stop when any required letter is missing.

**Implementation**

```python
class Solution:
    def maxNumberOfBalloons(self, text: str) -> int:
        # mutable copy of frequencies
        freq = [0] * 26
        for ch in text:
            freq[ord(ch) - ord('a')] += 1

        ans = 0
        balloon = "balloon"
        # pre‑compute required counts for one balloon
        need = [0] * 26
        for ch in balloon:
            need[ord(ch) - ord('a')] += 1

        while True:
            # see if we can take one more balloon
            for i in range(26):
                if freq[i] < need[i]:
                    return ans          # cannot build another
            # consume letters for one balloon
            for i in range(26):
                freq[i] -= need[i]
            ans += 1
```

**Complexity Analysis**

- Time complexity: O(k * n) where `k` is the answer (number of balloons) and `n = len(text)`. In the worst case `k` ≈ n/5, so O(n²) but with tiny constant because the inner loops run over 26 letters only.
- Space complexity: O(1) – only two fixed‑size arrays of length 26.

### Approach 2: Frequency Count – Single Pass

**Intuition**  
Instead of constructing balloons one by one, we can compute directly how many times each letter limits us. The word "balloon" needs two `l` and two `o`, so we halve their counts before taking the minimum.

**Algorithm**  
1. Count occurrences of `'b','a','l','o','n'` in `text`.  
2. Divide the counts for `'l'` and `'o'` by 2 (integer division).  
3. The answer is the minimum of the five adjusted counts.

**Implementation**

```python
class Solution:
    def maxNumberOfBalloons(self, text: str) -> int:
        # freq[0] = b, freq[1] = a, freq[2] = l, freq[3] = o, freq[4] = n
        freq = [0] * 5
        for ch in text:
            if ch == 'b':
                freq[0] += 1
            elif ch == 'a':
                freq[1] += 1
            elif ch == 'l':
                freq[2] += 1
            elif ch == 'o':
                freq[3] += 1
            elif ch == 'n':
                freq[4] += 1

        # balloon needs two l and two o
        freq[2] //= 2   # l
        freq[3] //= 2   # o

        return min(freq)
```

**Complexity Analysis**

- Time complexity: O(n) – one pass over `text` (n = len(text)).
- Space complexity: O(1) – constant size array of 5 integers.

### Approach 3: Using Collections.Counter (Pythonic)

**Intuition**  
`Counter` gives us a ready‑made frequency table. We can compute the answer by iterating over the unique letters in the target word and taking the minimum of `source_cnt[ch] // target_cnt[ch]`. This mirrors Approach 2 but is more concise and works for any alphabet.

**Algorithm**  
1. Build `Counter` for `text`.  
2. Build `Counter` for `"balloon"`.  
3. For each letter in the balloon counter, compute `text_cnt[ch] // balloon_cnt[ch]`.  
4. Return the minimum of those quotients.

**Implementation**

```python
from collections import Counter

class Solution:
    def maxNumberOfBalloons(self, text: str) -> int:
        text_cnt = Counter(text)
        balloon_cnt = Counter("balloon")   # {'b':1, 'a':1, 'l':2, 'o':2, 'n':1}

        ans = float('inf')
        for ch, need in balloon_cnt.items():
            have = text_cnt.get(ch, 0)
            ans = min(ans, have // need)

        return 0 if ans == float('inf') else ans
```

**Complexity Analysis**

- Time complexity: O(n + m) where n = len(text), m = len("balloon") = 5 → effectively O(n).
- Space complexity: O(1) – the counters store at most 26 entries.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace the optimized frequency‑count approach on the example `text = "loonbalxballpoon"`.

#### Required Format

## Dry Run

Input: text = "loonbalxballpoon"

We count only the letters `b, a, l, o, n`.

| Step | char | b | a | l | o | n |
|------|------|---|---|---|---|---|
| 1    | l    | 0 | 0 | 1 | 0 | 0 |
| 2    | o    | 0 | 0 | 1 | 1 | 0 |
| 3    | o    | 0 | 0 | 1 | 2 | 0 |
| 4    | n    | 0 | 0 | 1 | 2 | 1 |
| 5    | b    | 1 | 0 | 1 | 2 | 1 |
| 6    | a    | 1 | 1 | 1 | 2 | 1 |
| 7    | l    | 1 | 1 | 2 | 2 | 1 |
| 8    | x    | 1 | 1 | 2 | 2 | 1 | (ignore) |
| 9    | b    | 2 | 1 | 2 | 2 | 1 |
|10    | a    | 2 | 2 | 2 | 2 | 1 |
|11    | l    | 2 | 2 | 3 | 2 | 1 |
|12    | l    | 2 | 2 | 4 | 2 | 1 |
|13    | p    | 2 | 2 | 4 | 2 | 1 | (ignore) |
|14    | o    | 2 | 2 | 4 | 3 | 1 |
|15    | o    | 2 | 2 | 4 | 4 | 1 |
|16    | n    | 2 | 2 | 4 | 4 | 2 |

After the loop we have: `b=2, a=2, l=4, o=4, n=2`.

Now adjust for double letters:

- `l //= 2 → 2`
- `o //= 2 → 2`

Final values: `[b=2, a=2, l=2, o=2, n=2]`.

`min = 2` → we can form **2** balloons.

This matches the expected output.
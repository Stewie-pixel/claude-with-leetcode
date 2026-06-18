# Stack

## Video Solution

For more details about **Process String with Special Operations II**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=tXuWBbyj2ds)

## Concept

Think of the operations `*`, `#`, `%` as commands in a very basic text editor:

- a letter → type it at the end  
- `*` → backspace (delete the last character)  
- `#` → select all and copy‑paste the current contents (duplicate)  
- `%` → reverse the whole document  

If we actually built the string, its length could explode up to 10¹⁵, which is impossible to store.  
The key observation is that we only ever need to know **the length of the string after each prefix**.  
With those lengths we can walk **backwards** from the desired index `k` and map it to the position it had before each operation, finally landing on the concrete character that produced it.

## When to Use It

Use this “length‑tracking + reverse traversal” technique when you see:

- A sequence of operations that modify a string cumulatively (duplicate, delete, reverse, append, …)  
- A query that asks for the *k‑th* character of the final result  
- The final length can be huge (up to 10¹⁵ or more), forbidding explicit construction  

In short: **you need to answer a positional query without materialising the whole string** – the classic scenario for a stack‑like length simulation.

## Template

```python
def kth_char_after_ops(s: str, k: int) -> str:
    # 1️⃣  Prefix lengths
    n = len(s)
    lens = [0] * (n + 1)               # lens[i] = length after first i chars
    for i, ch in enumerate(s):
        if ch == '*':
            lens[i+1] = max(0, lens[i] - 1)
        elif ch == '#':
            lens[i+1] = lens[i] * 2
        elif ch == '%':
            lens[i+1] = lens[i]
        else:                           # a‑z
            lens[i+1] = lens[i] + 1

    # 2️⃣  Out‑of‑range?
    if k >= lens[n]:
        return '.'

    # 3️⃣  Walk backwards, fixing k
    for i in range(n-1, -1, -1):
        prev = lens[i]                  # length before s[i]
        ch = s[i]
        if ch == '#':
            if k >= prev:               # lies in the copied half
                k -= prev
        elif ch == '%':                 # reversal
            k = prev - 1 - k
        elif ch != '*':                 # a letter
            if k == prev:               # this letter is exactly at position k
                return ch
        # '*' does nothing – deletion only affects the removed char,
        # which we already know k is not (because k < final length)
    return '.'
```

## LeetCode Problem Walkthrough

### Problem: 3614. Process String with Special Operations II

https://leetcode.com/problems/process-string-with-special-operations-ii/

### Approach 1: Brute Force (Direct Simulation)

**Algorithm**  
Simulate the editor literally: keep a mutable list `res`, apply each operation as described, and finally return `res[k]` if it exists.

**Implementation**

```python
class Solution:
    def processStr(self, s: str, k: int) -> str:
        res = []
        for ch in s:
            if ch == '*':
                if res:
                    res.pop()
            elif ch == '#':
                res.extend(res.copy())
            elif ch == '%':
                res.reverse()
            else:                       # a‑z
                res.append(ch)
        return res[k] if k < len(res) else '.'
```

**Complexity Analysis**

- **Time:** O(n · L) – each `#` may double the string, leading to exponential blow‑up; infeasible for the given limits.  
- **Space:** O(L) – we store the entire intermediate string, which can be up to 10¹⁵ characters.

### Approach 2: Length Tracking + Reverse Traversal (Optimized)

**Intuition**  
Instead of storing the actual characters, we store only how long the string is after each prefix.  
When we move backwards from the wanted index `k`:

- `*` (delete) does **nothing** to `k` unless `k` pointed to the removed character – but we already know `k` is inside the final string, so we just keep it.  
- `#` (duplicate) creates two halves; if `k` lies in the second half we subtract the length of the first half.  
- `%` (reverse) maps index `k` to `prev_len‑1‑k`.  
- A letter contributes the **new** last character at position `prev_len`; if `k` equals that position we have found our answer.

**Algorithm**  
1. First pass: compute `lens[i]` (length after processing the first `i` characters).  
2. If `k ≥ lens[n]` → return `'.'`.  
3. Second pass: iterate `i` from `n‑1` down to `0`, adjusting `k` as described above; when a letter satisfies `k == lens[i]` return that letter.  
4. If the loop ends (should not happen) return `'.'`.

**Implementation**

```python
class Solution:
    def processStr(self, s: str, k: int) -> str:
        n = len(s)
        lens = [0] * (n + 1)
        for i, ch in enumerate(s):
            if ch == '*':
                lens[i+1] = max(0, lens[i] - 1)
            elif ch == '#':
                lens[i+1] = lens[i] * 2
            elif ch == '%':
                lens[i+1] = lens[i]
            else:                       # a‑z
                lens[i+1] = lens[i] + 1

        if k >= lens[n]:
            return '.'

        for i in range(n-1, -1, -1):
            prev = lens[i]               # length before s[i]
            ch = s[i]
            if ch == '#':
                if k >= prev:            # second half
                    k -= prev
            elif ch == '%':
                k = prev - 1 - k
            elif ch != '*':              # a letter
                if k == prev:
                    return ch
            # '*' does nothing
        return '.'
```

**Complexity Analysis**

- **Time:** `O(n)` – two linear scans over the input (`n ≤ 10⁵`).  
- **Space:** `O(n)` for the `lens` array (can be reduced to `O(1)` by recomputing on the fly, but `O(n)` comfortably fits the limits).

### Approach 3: Same as Approach 2 (no further improvement)

The length‑tracking method already yields optimal linear time and linear auxiliary space. Any attempt to eliminate the array would only trade a tiny constant factor for more complex code, so we treat Approach 2 as the final, most‑optimal solution.

## Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

**Input:** `s = "a#b%*"`, `k = 1`

### Step 1 – Prefix lengths

| i | ch | lens[i+] (length after i) |
|---|----|---------------------------|
| 0 | a  | 1                         |
| 1 | #  | 2                         |
| 2 | b  | 3                         |
| 3 | %  | 3                         |
| 4 | *  | 2                         |

`lens = [0,1,2,3,3,2]`; final length = 2, `k = 1` is inside.

### Step 2 – Backward pass (adjusting `k`)

| i | ch | prev = lens[i] | Action                              | k after |
|---|----|----------------|-------------------------------------|---------|
| 4 | *  | 2              | `*` does nothing                    | 1 |
| 3 | %  | 3              | reversal → k = 3‑1‑1 = 1            | 1 |
| 2 | b  | 2              | letter, k≠prev (1≠2)                | 1 |
| 1 | #  | 1              | `k ≥ prev` (1≥1) → k = 1‑1 = 0      | 0 |
| 0 | a  | 0              | letter, k == prev (0==0) → **answer = 'a'** | – |

The algorithm returns `'a'`, which matches the example.  

---  

*This lecture introduces the Stack‑like length‑tracking pattern, shows why it works, and gives a reusable template for any problem that asks for a positional query after a series of string‑mutating operations.*
# Greedy

## Video Solution

For more details about **Integer to Roman**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=ohBNdSJyLh8)

## Concept

Greedy algorithms make the locally optimal choice at each step with the hope of finding a global optimum.  
For integer‑to‑roman conversion, the locally optimal choice is to use the largest Roman symbol that does not exceed the remaining number, append it, and reduce the number. Repeating this step builds the roman numeral from highest to lowest place value, which is exactly how roman numerals are defined.

## When to Use It

Use a greedy pattern when you see:

- A problem that asks for a representation or decomposition using a fixed set of building blocks (symbols, coins, etc.).
- The building blocks have a natural ordering (largest to smallest) and using the largest possible piece never blocks a future optimal solution.
- Constraints are small enough that iterating over the building blocks is trivial.

## Template

```python
# Greedy template: iterate over options sorted by decreasing value
def greedy_algorithm(options, target):
    # options: list of (value, representation) sorted descending by value
    result = []
    for value, symbol in options:
        while target >= value:
            result.append(symbol)
            target -= value
    return ''.join(result)
```

## LeetCode Problem Walkthrough

### Problem: 12. Integer to Roman

https://leetcode.com/problems/integer-to-roman/

### Approach 1: Brute Force (Symbol‑by‑Symbol Loop)

**Algorithm**
- Define the basic Roman symbols and their values in descending order.
- For each symbol, repeatedly subtract its value from `num` while possible, appending the symbol each time.
- Continue until `num` becomes zero.

**Implementation**

```python
class Solution:
    def intToRoman(self, num: int) -> str:
        # value‑symbol pairs ordered from largest to smallest
        vals = [
            (1000, "M"), (900, "CM"), (500, "D"), (400, "CD"),
            (100,  "C"), (90,  "XC"), (50,  "L"),  (40,  "XL"),
            (10,   "X"), (9,   "IX"), (5,   "V"),  (4,   "IV"),
            (1,    "I")
        ]

        roman = []
        for value, symbol in vals:
            while num >= value:
                roman.append(symbol)
                num -= value
        return ''.join(roman)
```

**Complexity Analysis**

- Time complexity: O(1) – the outer loop runs over a fixed 13 pairs; the inner while loop executes at most `num / value` times, but the total number of iterations is bounded by a constant (max 15 for 3999).
- Space complexity: O(1) – only a few variables and the output string (whose length is bounded by a constant).

---

### Approach 2: Greedy with Lookup Table (same as above, but explicit intuition)

**Intuition**
The roman numeral system is *canonical*: any number has a unique representation when we always take the largest possible symbol first. This property lets us safely apply a greedy choice without back‑tracking.

**Algorithm**
Identical to Approach 1; the only difference is that we explicitly state the greedy choice property as the intuition.

**Implementation** (same code as above)

**Complexity Analysis**
Same as Approach 1.

---

### Approach 3: Digit‑by‑Digit Lookup (positional)

**Intuition**
Roman numerals are built per decimal place (thousands, hundreds, tens, ones). Each place has at most nine patterns that can be pre‑computed, so we can convert each digit independently and concatenate the results.

**Algorithm**
- Break the number into thousands, hundreds, tens, and ones.
- Use four static maps (or a 2‑D list) that give the roman string for each digit 0‑9 at a given place.
- Concatenate the four strings.

**Implementation**

```python
class Solution:
    def intToRoman(self, num: int) -> str:
        # pre‑computed strings for each place
        thousands = ["", "M", "MM", "MMM"]
        hundreds  = ["", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"]
        tens      = ["", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"]
        ones      = ["", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"]

        return (
            thousands[num // 1000] +
            hundreds[(num % 1000) // 100] +
            tens[(num % 100) // 10] +
            ones[num % 10]
        )
```

**Complexity Analysis**

- Time complexity: O(1) – a constant number of table lookups.
- Space complexity: O(1) – only the fixed tables and a few variables.

---

## Dry Run

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

We trace Approach 1 on `num = 3749`.

| Step | num before | value | symbol appended | num after | accumulated roman |
|------|------------|-------|----------------|----------|-------------------|
| 1    | 3749       | 1000  | M              | 2749     | M                 |
| 2    | 2749       | 1000  | M              | 1749     | MM                |
| 3    | 1749       | 1000  | M              |  749     | MMM               |
| 4    |  749       |  900  | CM             |   -? (actually 749<900, skip) |
| 5    |  749       |  500  | D              |   249    | MMMD              |
| 6    |  249       |  100  | C              |   149    | MMMDC             |
| 7    |  149       |  100  | C              |    49    | MMMDCC            |
| 8    |   49       |   90  | XC             |   49<90, skip |
| 9    |   49       |   50  | L              |   49<50, skip |
|10    |   49       |   40  | XL             |    9     | MMMDCCXL          |
|11    |    9       |   10  | X              |    9<10, skip |
|12    |    9       |    9  | IX             |    0     | MMMDCCXLIX        |
|13    |    0       |   …   | (loop ends)    |    0     | MMMDCCXLIX        |

The final string `"MMMDCCXLIX"` matches the expected output.

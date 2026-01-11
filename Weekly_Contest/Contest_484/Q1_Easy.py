"""
Docstring for Weekly_Contest.Contest_484.Q1_Easy

You are given a string s consisting only of lowercase English letters.

A prefix of s is called a residue if the number of distinct characters in the prefix is equal to len(prefix) % 3.

Return the count of residue prefixes in s.

A prefix of a string is a non-empty substring that starts from the beginning of the string and extends to any point within it.
 

Example 1:

Input: s = "abc"

Output: 2

Explanation:​​​​​​​

Prefix "a" has 1 distinct character and length modulo 3 is 1, so it is a residue.
Prefix "ab" has 2 distinct characters and length modulo 3 is 2, so it is a residue.
Prefix "abc" does not satisfy the condition. Thus, the answer is 2.
Example 2:

Input: s = "dd"

Output: 1

Explanation:

Prefix "d" has 1 distinct character and length modulo 3 is 1, so it is a residue.
Prefix "dd" has 1 distinct character but length modulo 3 is 2, so it is not a residue. Thus, the answer is 1.
Example 3:

Input: s = "bob"

Output: 2

Explanation:

Prefix "b" has 1 distinct character and length modulo 3 is 1, so it is a residue.
Prefix "bo" has 2 distinct characters and length mod 3 is 2, so it is a residue. Thus, the answer is 2.
 

Constraints:

1 <= s.length <= 100
s contains only lowercase English letters.

Note: Please do not copy the description during the contest to maintain the integrity of your submissions.

"""

class Solution:
    def residuePrefixes(self, s: str) -> int:
        seen = set()
        residue_count = 0

        for i, ch in enumerate(s):
            seen.add(ch)
            if len(seen) == (i + 1) % 3:
                residue_count += 1

        return residue_count

<div align="center">

<img src="./assets/banner.png" height="100" alt="DSA Mentor Banner" />

# Claude with LeetCode

[![Build README](https://github.com/Stewie-pixel/claude-with-leetcode/actions/workflows/build.yml/badge.svg)](https://github.com/Stewie-pixel/claude-with-leetcode/actions/workflows/build-readme.yml)
[![Problems Solved](https://img.shields.io/badge/dynamic/json?label=Solved&query=length&url=https://raw.githubusercontent.com/Stewie-pixel/claude-with-leetcode/main/.problemSiteData.json&color=brightgreen&logo=leetcode)](https://github.com/Stewie-pixel/claude-with-leetcode)
[![Claude Solved](https://img.shields.io/badge/dynamic/json?label=Solved&query=length&url=https://raw.githubusercontent.com/Stewie-pixel/claude-with-leetcode/main/.github/badges/solutions.json&color=orange&logo=claude)](https://github.com/Stewie-pixel/claude-with-leetcode)
![C](https://img.shields.io/badge/C-a8b9cc?logo=c&logoColor=000000)
![C%2B%2B](https://img.shields.io/badge/C%2B%2B-00599c?logo=cplusplus&logoColor=ffffff)
![Java](https://img.shields.io/badge/Java-ed8b00?logo=openjdk&logoColor=ffffff)
![JavaScript](https://img.shields.io/badge/JavaScript-f7df1e?logo=javascript&logoColor=000000)
![Python](https://img.shields.io/badge/Python-3776ab?logo=python&logoColor=ffffff)
![Rust](https://img.shields.io/badge/Rust-000000?logo=rust&logoColor=ffffff)
![TypeScript](https://img.shields.io/badge/TypeScript-3178c6?logo=typescript&logoColor=ffffff)

A little assistant from Claude to help you learn daily LeetCode problems organised by DSA topic and difficulty.

</div>

## Table of Contents

- [Structure](#structure)
- [Problem List](#problem-list)

## Structure

```
claude-with-leetcode/
├── .github/
│   └── workflows/                      ← CI/CD pipelines
├── .vscode/                            ← editor settings
├── contest                             ← Weekly + Biweekly programming contests
├── cpp/                                ← C++ solutions
├── dcc/                                ← Daily Coding Challenge
├── java/
│   └── 2-add-two-numbers/              ← Java solutions
├── python/
│   └── 1-two-sum/                      ← Python solutions
├── rust/
│   └── 1-two-sum/                      ← Rust solutions
├── skills/                             ← Claude agent skill definitions
├── study_plan/
│   └── leetcode75/
│       └── cpp/                        ← LeetCode 75 study plan solutions
├── .gitattributes
├── .gitignore
├── .prettierrc
├── .problemSiteData.json               ← problem metadata store
├── CLAUDE.md                           ← Claude DSA mentor agent config
├── README.md                           ← auto-generated, do not edit
├── README_template.md                  ← README template
├── addProblem.js                       ← manually add a problem entry
├── detectNewProblems.js                ← detects newly committed files
├── dsaMentor.js                        ← triggers Claude DSA mentor agent
├── package.json
├── syncLeetcode.js                     ← fetches data from LeetCode
├── updateSiteData.js                   ← scans dirs and updates metadata
├── updateTable.js                      ← rebuilds README from template
└── verifySiteData.js                   ← verifies solution URLs
```

## Problem List

### Arrays & Hashing

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>JS</sub> | <sub>Python</sub> | <sub>Rust</sub> | <sub>TS</sub>
---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ----
<sub>[0001 - Two Sum](https://leetcode.com/problems/two-sum)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F1-two-sum%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](python%2F1-two-sum%2FREADME.md)</div></sub> | <sub><div align='center'>[✔️](rust%2F1-two-sum%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub>

### Linked List

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>JS</sub> | <sub>Python</sub> | <sub>Rust</sub> | <sub>TS</sub>
---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ----
<sub>[0021 - Merge Two Sorted Lists](https://leetcode.com/problems/merge-two-sorted-lists)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F21-merge-two-sorted-lists%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[0002 - Add Two Numbers](https://leetcode.com/problems/add-two-numbers)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](c%2F2-add-two-numbers%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](java%2F2-add-two-numbers%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>

### String

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>JS</sub> | <sub>Python</sub> | <sub>Rust</sub> | <sub>TS</sub>
---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ----
<sub>[0006 - Zigzag Conversion](https://leetcode.com/problems/zigzag-conversion)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F6-zigzag-conversion%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[0093 - Restore IP Addresses](https://leetcode.com/problems/restore-ip-addresses)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F93-restore-ip-addresses%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[0394 - Decode String](https://leetcode.com/problems/decode-string)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F394-decode-string%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1415 - The k-th Lexicographical String of All Happy Strings of Length n](https://leetcode.com/problems/the-k-th-lexicographical-string-of-all-happy-strings-of-length-n)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F1415-the-k-th-lexicographical-string-of-all-happy-strings-of-length-n%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1456 - Maximum Number of Vowels in a Substring of Given Length](https://leetcode.com/problems/maximum-number-of-vowels-in-a-substring-of-given-length)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F1456-maximum-number-of-vowels-in-a-substring-of-given-length%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1545 - Find Kth Bit in Nth Binary String](https://leetcode.com/problems/find-kth-bit-in-nth-binary-string)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F1545-find-kth-bit-in-nth-binary-string%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[2390 - Removing Stars From a String](https://leetcode.com/problems/removing-stars-from-a-string)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F2390-removing-stars-from-a-string%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[0214 - Shortest Palindrome](https://leetcode.com/problems/shortest-palindrome)</sub> | <sub>🔴 Hard</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F214-shortest-palindrome%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1392 - Longest Happy Prefix](https://leetcode.com/problems/longest-happy-prefix)</sub> | <sub>🔴 Hard</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F1392-longest-happy-prefix%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[2223 - Sum of Scores of Built Strings](https://leetcode.com/problems/sum-of-scores-of-built-strings)</sub> | <sub>🔴 Hard</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F2223-sum-of-scores-of-built-strings%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>

### Array

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>JS</sub> | <sub>Python</sub> | <sub>Rust</sub> | <sub>TS</sub>
---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ----
<sub>[0219 - Contains Duplicate II](https://leetcode.com/problems/contains-duplicate-ii)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F219-contains-duplicate-ii%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[0724 - Find Pivot Index](https://leetcode.com/problems/find-pivot-index)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F724-find-pivot-index%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[2248 - Intersection of Multiple Arrays](https://leetcode.com/problems/intersection-of-multiple-arrays)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F2248-minimum-cost-of-buying-candies-with-discount%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[2574 - Left and Right Sum Differences](https://leetcode.com/problems/left-and-right-sum-differences)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F2574-left-and-right-sum-differences%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3512 - Minimum Operations to Make Array Sum Divisible by K](https://leetcode.com/problems/minimum-operations-to-make-array-sum-divisible-by-k)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3512-minimum-operations-to-make-array-sum-divisible-by-k%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3633 - Earliest Finish Time for Land and Water Rides I](https://leetcode.com/problems/earliest-finish-time-for-land-and-water-rides-i)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3633-earliest-finish-time-for-land-and-water-rides-i%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3898 - Find the Degree of Each Vertex](https://leetcode.com/problems/find-the-degree-of-each-vertex)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3898-find-the-degree-of-each-vertex%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3925 - Concatenate Array With Reverse](https://leetcode.com/problems/concatenate-array-with-reverse)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3925-concatenate-array-with-reverse%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[0047 - Permutations II](https://leetcode.com/problems/permutations-ii)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F47-permutations-ii%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1493 - Longest Subarray of 1's After Deleting One Element](https://leetcode.com/problems/longest-subarray-of-1s-after-deleting-one-element)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F1493-longest-subarray-of-1s-after-deleting-one-element%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1567 - Maximum Length of Subarray With Positive Product](https://leetcode.com/problems/maximum-length-of-subarray-with-positive-product)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F1567-maximum-length-of-subarray-with-positive-product%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[2161 - Partition Array According to Given Pivot](https://leetcode.com/problems/partition-array-according-to-given-pivot)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F2161-partition-array-according-to-given-pivot%2FANALYSIS.md)</div></sub> | <sub><div align='center'>[✔️](java%2F2161-partition-array-according-to-given-pivot%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[2196 - Create Binary Tree From Descriptions](https://leetcode.com/problems/create-binary-tree-from-descriptions)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F2196-create-binary-tree-from-descriptions%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[2352 - Equal Row and Column Pairs](https://leetcode.com/problems/equal-row-and-column-pairs)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F2352-equal-row-and-column-pairs%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3635 - Earliest Finish Time for Land and Water Rides II](https://leetcode.com/problems/earliest-finish-time-for-land-and-water-rides-ii)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3635-earliest-finish-time-for-land-and-water-rides-ii%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3689 - Maximum Total Subarray Value I](https://leetcode.com/problems/maximum-total-subarray-value-i)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3689-maximum-total-subarray-value-i%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3559 - Number of Ways to Assign Edge Weights II](https://leetcode.com/problems/number-of-ways-to-assign-edge-weights-ii)</sub> | <sub>🔴 Hard</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](python%2F3559-number-of-ways-to-assign-edge-weights-ii%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3691 - Maximum Total Subarray Value II](https://leetcode.com/problems/maximum-total-subarray-value-ii)</sub> | <sub>🔴 Hard</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3691-maximum-total-subarray-value-ii%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>

### Backtracking

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>JS</sub> | <sub>Python</sub> | <sub>Rust</sub> | <sub>TS</sub>
---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ----
<sub>[0077 - Combinations](https://leetcode.com/problems/combinations)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F77-combinations%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>

### Hash Table

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>JS</sub> | <sub>Python</sub> | <sub>Rust</sub> | <sub>TS</sub>
---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ----
<sub>[0424 - Longest Repeating Character Replacement](https://leetcode.com/problems/longest-repeating-character-replacement)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F424-longest-repeating-character-replacement%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1358 - Number of Substrings Containing All Three Characters](https://leetcode.com/problems/number-of-substrings-containing-all-three-characters)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F1358-number-of-substrings-containing-all-three-characters%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1657 - Determine if Two Strings Are Close](https://leetcode.com/problems/determine-if-two-strings-are-close)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F1657-determine-if-two-strings-are-close%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](python%2F1657-determine-if-two-strings-are-close%2FANALYSIS.md)</div></sub> | <sub><div align='center'>[✔️](rust%2F1657-determine-if-two-strings-are-close%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub>

### Contest

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>JS</sub> | <sub>Python</sub> | <sub>Rust</sub> | <sub>TS</sub>
---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ----
<sub>[2723 - Add Two Promises](https://leetcode.com/problems/add-two-promises)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](typescript%2F2723-add-two-promises%2FANALYSIS.md)</div></sub>
<sub>[3940 - Limit Occurrences in Sorted Array](https://leetcode.com/problems/limit-occurrences-in-sorted-array)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3940-limit-occurrences-in-sorted-array%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3950 - Exactly One Consecutive Set Bits Pair](https://leetcode.com/problems/exactly-one-consecutive-set-bits-pair)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3950-exactly-one-consecutive-set-bits-pair%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3954 - Sum of Compatible Numbers in Range I](https://leetcode.com/problems/sum-of-compatible-numbers-in-range-i)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3954-sum-of-compatible-numbers-in-range-i%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[2627 - Debounce](https://leetcode.com/problems/debounce)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](javascript%2F2627-debounce%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](typescript%2F2627-debounce%2FANALYSIS.md)</div></sub>
<sub>[3951 - Minimum Energy to Maintain Brightness](https://leetcode.com/problems/minimum-energy-to-maintain-brightness)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3951-minimum-energy-to-maintain-brightness%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3952 - Maximum Total Value of Covered Indices](https://leetcode.com/problems/maximum-total-value-of-covered-indices)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3952-maximum-total-value-of-covered-indices%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3955 - Valid Binary Strings With Cost Limit](https://leetcode.com/problems/valid-binary-strings-with-cost-limit)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3955-valid-binary-strings-with-cost-limit%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3953 - Maximum Score with Co-Prime Element](https://leetcode.com/problems/maximum-score-with-co-prime-element)</sub> | <sub>🔴 Hard</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3953-maximum-score-with-co-prime-element%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3956 - Maximum Sum of M Non-Overlapping Subarrays I](https://leetcode.com/problems/maximum-sum-of-m-non-overlapping-subarrays-i)</sub> | <sub>🔴 Hard</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3956-maximum-sum-of-m-non-overlapping-subarrays-i%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3957 - Maximum Sum of M Non-Overlapping Subarrays II](https://leetcode.com/problems/maximum-sum-of-m-non-overlapping-subarrays-ii)</sub> | <sub>🔴 Hard</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3957-maximum-sum-of-m-non-overlapping-subarrays-ii%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>

### Math

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>JS</sub> | <sub>Python</sub> | <sub>Rust</sub> | <sub>TS</sub>
---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ----
<sub>[3558 - Number of Ways to Assign Edge Weights I](https://leetcode.com/problems/number-of-ways-to-assign-edge-weights-i)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](python%2F3558-number-of-ways-to-assign-edge-weights-i%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3751 - Total Waviness of Numbers in Range I](https://leetcode.com/problems/total-waviness-of-numbers-in-range-i)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3751-total-waviness-of-numbers-in-range-i%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3753 - Total Waviness of Numbers in Range II](https://leetcode.com/problems/total-waviness-of-numbers-in-range-ii)</sub> | <sub>🔴 Hard</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](cpp%2F3753-total-waviness-of-numbers-in-range-ii%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>



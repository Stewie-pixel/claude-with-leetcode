<div align="center">

<img src="./assets/banner.png" height="100" alt="DSA Mentor Banner" />

# Claude with LeetCode

[![Build README](https://github.com/Stewie-pixel/claude-with-leetcode/actions/workflows/build.yml/badge.svg)](https://github.com/Stewie-pixel/claude-with-leetcode/actions/workflows/build-readme.yml)
[![Problems Solved](https://img.shields.io/badge/dynamic/json?label=Solved&query=length&url=https://raw.githubusercontent.com/Stewie-pixel/claude-with-leetcode/main/.problemSiteData.json&color=brightgreen&logo=leetcode)](https://github.com/Stewie-pixel/claude-with-leetcode)
![C%2B%2B](https://img.shields.io/badge/C%2B%2B-00599c?logo=cplusplus&logoColor=ffffff)
![Java](https://img.shields.io/badge/Java-ed8b00?logo=openjdk&logoColor=ffffff)
![Python](https://img.shields.io/badge/Python-3776ab?logo=python&logoColor=ffffff)
![Rust](https://img.shields.io/badge/Rust-000000?logo=rust&logoColor=ffffff)

A little assistant from Claude to help you learn daily LeetCode problems organised by DSA topic and difficulty.

</div>

## Table of Contents

- [Structure](#structure)
- [How It Works](#how-it-works)
- [Problem List](#problem-list)

## Structure

```
claude-with-leetcode/
├── .github/
│   └── workflows/                      ← CI/CD pipelines
├── .vscode/                            ← editor settings
├── cpp/                                ← C++ solutions
├── dcc/                                ← additional solution set
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

## How It Works

Every hour a GitHub Actions workflow runs automatically:

```
updateSiteData.js       scan language dirs, rename files, update .problemSiteData.json
↓
verifySiteData.js       verify all solution URLs return 200
↓
detectNewProblems.js    detect newly committed solution files since last run
↓
dsaMentor.js            trigger Claude DSA mentor agent to generate lecture & GitHub Issue
↓
syncLeetcode.js         fetch latest problem metadata from LeetCode
↓
updateTable.js          rebuild README from README_template.md
↓
git push                commit changes via bot account
```

**Adding a new solution:**

1. Add the problem entry to `.problemSiteData.json`
2. Drop your solution file in the correct language folder — e.g. `cpp/0001-two-sum.cpp`
3. The workflow picks it up automatically on the next run and updates the README

**Solution file naming:**

```
{problem-number}-{leetcode-url-slug}.{extension}
e.g. 0001-two-sum.cpp
```

**Legend:**

| Symbol | Meaning        |
| ------ | -------------- |
| ✔️     | Solved         |
| ❌     | Not yet solved |

| Badge     | Meaning           |
| --------- | ----------------- |
| 🟢 Easy   | Easy difficulty   |
| 🟡 Medium | Medium difficulty |
| 🔴 Hard   | Hard difficulty   |

## Problem List

### Arrays & Hashing

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>Python</sub> | <sub>Rust</sub>
---- | ---- | ---- | ---- | ---- | ----
<sub>[0001 - Two Sum](https://leetcode.com/problems/two-sum)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>[✔️](cpp%2F1-two-sum%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](python%2F1-two-sum%2FREADME.md)</div></sub> | <sub><div align='center'>[✔️](rust%2F1-two-sum%2FREADME.md)</div></sub>

### Linked List

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>Python</sub> | <sub>Rust</sub>
---- | ---- | ---- | ---- | ---- | ----
<sub>[0002 - Add Two Numbers](https://leetcode.com/problems/add-two-numbers)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](java%2F2-add-two-numbers%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>

### String

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>Python</sub> | <sub>Rust</sub>
---- | ---- | ---- | ---- | ---- | ----
<sub>[0006 - Zigzag Conversion](https://leetcode.com/problems/zigzag-conversion)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](cpp%2F6-zigzag-conversion%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1456 - Maximum Number of Vowels in a Substring of Given Length](https://leetcode.com/problems/maximum-number-of-vowels-in-a-substring-of-given-length)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](cpp%2F1456-maximum-number-of-vowels-in-a-substring-of-given-length%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[2390 - Removing Stars From a String](https://leetcode.com/problems/removing-stars-from-a-string)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](cpp%2F2390-removing-stars-from-a-string%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[0214 - Shortest Palindrome](https://leetcode.com/problems/shortest-palindrome)</sub> | <sub>🔴 Hard</sub> | <sub><div align='center'>[✔️](cpp%2F214-shortest-palindrome%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1392 - Longest Happy Prefix](https://leetcode.com/problems/longest-happy-prefix)</sub> | <sub>🔴 Hard</sub> | <sub><div align='center'>[✔️](cpp%2F1392-longest-happy-prefix%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[2223 - Sum of Scores of Built Strings](https://leetcode.com/problems/sum-of-scores-of-built-strings)</sub> | <sub>🔴 Hard</sub> | <sub><div align='center'>[✔️](cpp%2F2223-sum-of-scores-of-built-strings%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>

### Array

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>Python</sub> | <sub>Rust</sub>
---- | ---- | ---- | ---- | ---- | ----
<sub>[0219 - Contains Duplicate II](https://leetcode.com/problems/contains-duplicate-ii)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>[✔️](cpp%2F219-contains-duplicate-ii%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[0724 - Find Pivot Index](https://leetcode.com/problems/find-pivot-index)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>[✔️](cpp%2F724-find-pivot-index%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[2248 - Intersection of Multiple Arrays](https://leetcode.com/problems/intersection-of-multiple-arrays)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>[✔️](cpp%2F2248-minimum-cost-of-buying-candies-with-discount%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[2574 - Left and Right Sum Differences](https://leetcode.com/problems/left-and-right-sum-differences)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>[✔️](cpp%2F2574-left-and-right-sum-differences%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3633 - Earliest Finish Time for Land and Water Rides I](https://leetcode.com/problems/earliest-finish-time-for-land-and-water-rides-i)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>[✔️](cpp%2F3633-earliest-finish-time-for-land-and-water-rides-i%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[0047 - Permutations II](https://leetcode.com/problems/permutations-ii)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](cpp%2F47-permutations-ii%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1493 - Longest Subarray of 1's After Deleting One Element](https://leetcode.com/problems/longest-subarray-of-1s-after-deleting-one-element)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](cpp%2F1493-longest-subarray-of-1s-after-deleting-one-element%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1567 - Maximum Length of Subarray With Positive Product](https://leetcode.com/problems/maximum-length-of-subarray-with-positive-product)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[2352 - Equal Row and Column Pairs](https://leetcode.com/problems/equal-row-and-column-pairs)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](cpp%2F2352-equal-row-and-column-pairs%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3635 - Earliest Finish Time for Land and Water Rides II](https://leetcode.com/problems/earliest-finish-time-for-land-and-water-rides-ii)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](cpp%2F3635-earliest-finish-time-for-land-and-water-rides-ii%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>

### Hash Table

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>Python</sub> | <sub>Rust</sub>
---- | ---- | ---- | ---- | ---- | ----
<sub>[0424 - Longest Repeating Character Replacement](https://leetcode.com/problems/longest-repeating-character-replacement)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](cpp%2F424-longest-repeating-character-replacement%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1358 - Number of Substrings Containing All Three Characters](https://leetcode.com/problems/number-of-substrings-containing-all-three-characters)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](cpp%2F1358-number-of-substrings-containing-all-three-characters%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1657 - Determine if Two Strings Are Close](https://leetcode.com/problems/determine-if-two-strings-are-close)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](cpp%2F1657-determine-if-two-strings-are-close%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>[✔️](python%2F1657-determine-if-two-strings-are-close%2FANALYSIS.md)</div></sub> | <sub><div align='center'>[✔️](rust%2F1657-determine-if-two-strings-are-close%2FANALYSIS.md)</div></sub>

### Math

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>Python</sub> | <sub>Rust</sub>
---- | ---- | ---- | ---- | ---- | ----
<sub>[3751 - Total Waviness of Numbers in Range I](https://leetcode.com/problems/total-waviness-of-numbers-in-range-i)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](cpp%2F3751-total-waviness-of-numbers-in-range-i%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3753 - Total Waviness of Numbers in Range II](https://leetcode.com/problems/total-waviness-of-numbers-in-range-ii)</sub> | <sub>🔴 Hard</sub> | <sub><div align='center'>[✔️](cpp%2F3753-total-waviness-of-numbers-in-range-ii%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>

### Uncategorized

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>Python</sub> | <sub>Rust</sub>
---- | ---- | ---- | ---- | ---- | ----
<sub>[3940 - Limit Occurrences in Sorted Array](https://leetcode.com/problems/limit-occurrences-in-sorted-array)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>[✔️](cpp%2F3940-limit-occurrences-in-sorted-array%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>



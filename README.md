<div align="center">

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
│   └── workflows/
│       └── build-readme.yml       ← automated README rebuild
├── cpp/                           ← C++ solutions
├── .problemSiteData.json          ← problem metadata
├── updateSiteData.js              ← scans dirs and updates metadata
├── verifySiteData.js              ← verifies solution URLs
├── updateTable.js                 ← rebuilds README from template
├── README_template.md             ← README template
└── README.md                      ← auto-generated, do not edit
```


## How It Works

Every hour a GitHub Actions workflow runs automatically:

```
updateSiteData.js       scan language dirs, rename files, update .problemSiteData.json
      ↓
verifySiteData.js       verify all solution URLs return 200
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

| Symbol | Meaning |
|---|---|
| ✔️ | Solved |
| ❌ | Not yet solved |

| Badge | Meaning |
|---|---|
| 🟢 Easy | Easy difficulty |
| 🟡 Medium | Medium difficulty |
| 🔴 Hard | Hard difficulty |


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

### Array

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>Python</sub> | <sub>Rust</sub>
---- | ---- | ---- | ---- | ---- | ----
<sub>[0219 - Contains Duplicate II](https://leetcode.com/problems/contains-duplicate-ii)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>[✔️](cpp%2F219-contains-duplicate-ii%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[0724 - Find Pivot Index](https://leetcode.com/problems/find-pivot-index)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>[✔️](cpp%2F724-find-pivot-index%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[2248 - Intersection of Multiple Arrays](https://leetcode.com/problems/intersection-of-multiple-arrays)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3633 - Earliest Finish Time for Land and Water Rides I](https://leetcode.com/problems/earliest-finish-time-for-land-and-water-rides-i)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>[✔️](cpp%2F3633-earliest-finish-time-for-land-and-water-rides-i%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1567 - Maximum Length of Subarray With Positive Product](https://leetcode.com/problems/maximum-length-of-subarray-with-positive-product)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[3635 - Earliest Finish Time for Land and Water Rides II](https://leetcode.com/problems/earliest-finish-time-for-land-and-water-rides-ii)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](cpp%2F3635-earliest-finish-time-for-land-and-water-rides-ii%2FANALYSIS.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>

### Hash Table

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>Python</sub> | <sub>Rust</sub>
---- | ---- | ---- | ---- | ---- | ----
<sub>[0424 - Longest Repeating Character Replacement](https://leetcode.com/problems/longest-repeating-character-replacement)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](cpp%2F424-longest-repeating-character-replacement%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>
<sub>[1358 - Number of Substrings Containing All Three Characters](https://leetcode.com/problems/number-of-substrings-containing-all-three-characters)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](cpp%2F1358-number-of-substrings-containing-all-three-characters%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>

### Math

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>Python</sub> | <sub>Rust</sub>
---- | ---- | ---- | ---- | ---- | ----
<sub>[3751 - Total Waviness of Numbers in Range I](https://leetcode.com/problems/total-waviness-of-numbers-in-range-i)</sub> | <sub>🟡 Medium</sub> | <sub><div align='center'>[✔️](cpp%2F3751-total-waviness-of-numbers-in-range-i%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>

### Uncategorized

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C++</sub> | <sub>Java</sub> | <sub>Python</sub> | <sub>Rust</sub>
---- | ---- | ---- | ---- | ---- | ----
<sub>[3940 - Limit Occurrences in Sorted Array](https://leetcode.com/problems/limit-occurrences-in-sorted-array)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>[✔️](cpp%2F3940-limit-occurrences-in-sorted-array%2FREADME.md)</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>



<div align="center">

# Claude with LeetCode

[![Build README](https://github.com/Stewie-pixel/claude-with-leetcode/actions/workflows/build.yml/badge.svg)](https://github.com/Stewie-pixel/claude-with-leetcode/actions/workflows/build-readme.yml)
[![Problems Solved](https://img.shields.io/badge/dynamic/json?label=Solved&query=length&url=https://raw.githubusercontent.com/Stewie-pixel/claude-with-leetcode/main/.problemSiteData.json&color=brightgreen&logo=leetcode)](https://github.com/Stewie-pixel/claude-with-leetcode)
![C%2B%2B](https://img.shields.io/badge/C%2B%2B-00599c?logo=cplusplus&logoColor=ffffff)
![Python](https://img.shields.io/badge/Python-3776ab?logo=python&logoColor=ffffff)

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

<sub>Problem</sub> | <sub>Difficulty</sub> | <sub>C++</sub> | <sub>dcc</sub> | <sub>Python</sub>
---- | ---- | ---- | ---- | ----
<sub>[0001 - Two Sum](https://leetcode.com/problems/two-sum)</sub> | <sub>🟢 Easy</sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub> | <sub><div align='center'>❌</div></sub>


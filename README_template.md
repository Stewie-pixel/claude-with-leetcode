<div align="center">

# Daily LeetCode

[![Build README](https://github.com/Stewie-pixel/daily-leetcode/actions/workflows/build-readme.yml/badge.svg)](https://github.com/Stewie-pixel/daily-leetcode/actions/workflows/build-readme.yml)
[![Problems Solved](https://img.shields.io/badge/dynamic/json?label=Solved&query=length&url=https://raw.githubusercontent.com/Stewie-pixel/daily-leetcode/main/.problemSiteData.json&color=brightgreen&logo=leetcode)](https://github.com/Stewie-pixel/daily-leetcode)
<language-badges />

A little assistant from Claude to help you learn daily LeetCode problems organised by DSA topic and difficulty.

</div>


## Table of Contents
- [Structure](#structure)
- [How It Works](#how-it-works)
- [Problem List](#problem-list)


## Structure

```
daily-leetcode/
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

<completion-tables />
<div align="center">

<img src="./assets/banner.png" height="100" alt="DSA Mentor Banner" />

# Claude with LeetCode

[![Build README](https://github.com/Stewie-pixel/claude-with-leetcode/actions/workflows/build.yml/badge.svg)](https://github.com/Stewie-pixel/claude-with-leetcode/actions/workflows/build-readme.yml)
[![Problems Solved](https://img.shields.io/badge/dynamic/json?label=Solved&query=length&url=https://raw.githubusercontent.com/Stewie-pixel/claude-with-leetcode/main/.problemSiteData.json&color=brightgreen&logo=leetcode)](https://github.com/Stewie-pixel/claude-with-leetcode)
<language-badges />

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

<completion-tables />

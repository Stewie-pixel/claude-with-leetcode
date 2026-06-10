<div align="center">

<img src="./assets/banner.png" height="100" alt="DSA Mentor Banner" />

# Claude with LeetCode

[![Build README](https://github.com/Stewie-pixel/claude-with-leetcode/actions/workflows/build.yml/badge.svg)](https://github.com/Stewie-pixel/claude-with-leetcode/actions/workflows/build-readme.yml)
[![Problems Solved](https://img.shields.io/badge/dynamic/json?label=Solved&query=length&url=https://raw.githubusercontent.com/Stewie-pixel/claude-with-leetcode/main/.problemSiteData.json&color=brightgreen&logo=leetcode)](https://github.com/Stewie-pixel/claude-with-leetcode)
[![Claude Solved](https://img.shields.io/badge/dynamic/json?label=Solved&query=length&url=https://raw.githubusercontent.com/Stewie-pixel/claude-with-leetcode/main/.github/badges/solutions.json&color=orange&logo=claude)](https://github.com/Stewie-pixel/claude-with-leetcode)
<language-badges />

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

<completion-tables />

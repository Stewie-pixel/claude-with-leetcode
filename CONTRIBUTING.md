# Contributing to Claude with LeetCode

First and foremost, thanks a lot for being part of the community! Every problem you contribute makes this project more valuable for every developer who is learning DSA. Whether you're submitting your first solution or your hundredth, your effort is genuinely appreciated and does not go unnoticed. This is a community built one problem at a time, and you're a part of that. 🙏

## Folder Structure

Each problem must live inside the correct language folder and follow this exact structure:

```
{language}/{problem-number}-{problem-slug}/
├── README.md          ← problem description (required)
└── solution.{ext}     ← your solution file (required)
```

**Example:**

```
cpp/1-two-sum/
├── README.md
└── two-sum.cpp
```

## Step 1 — Choose Your Language Folder

Place your problem inside the correct language directory:

| Language   | Folder        |
| ---------- | ------------- |
| C          | `c/`          |
| C++        | `cpp/`        |
| Csharp     | `csharp/`     |
| Python     | `python/`     |
| Java       | `java/`       |
| Rust       | `rust/`       |
| JavaScript | `javascript/` |
| TypeScript | `typescript/` |
| Go         | `go/`         |
| Kotlin     | `kotlin/`     |
| Ruby       | `ruby/`       |

## Step 2 — Name the Folder

Folder name must follow this format:

```
{problem-number}-{problem-slug}
```

| ✅ Accepted         | ❌ Not Accepted     |
| ------------------- | ------------------- |
| `1-two-sum`         | `two-sum`           |
| `2-add-two-numbers` | `2_add_two_numbers` |
| `49-group-anagrams` | `49-GroupAnagrams`  |

- Lowercase only
- Hyphens between words, no underscores
- Always include the problem number prefix

## Step 3 — Create README.md

The `README.md` must contain the full LeetCode problem description in HTML format exactly as copied from LeetCode. This is what the DSA Mentor agent reads to generate the lecture.

**How to get it:**

1. Open the problem on [LeetCode](https://leetcode.com)
2. Inspect the problem description element in browser DevTools
3. Copy the raw HTML content

**Required format:**

```html
<h2><a href="https://leetcode.com/problems/two-sum">Two Sum</a></h2>
<img
    src="https://img.shields.io/badge/Difficulty-Easy-brightgreen"
    alt="Difficulty: Easy"
/>
<hr />
<p>
    Given an array of integers <code>nums</code> and an integer
    <code>target</code>, return
    <em
        >indices of the two numbers such that they add up to
        <code>target</code></em
    >.
</p>

<p><strong class="example">Example 1:</strong></p>
<pre>
<strong>Input:</strong> nums = [2,7,11,15], target = 9
<strong>Output:</strong> [0,1]
<strong>Explanation:</strong> Because nums[0] + nums[1] == 9, we return [0, 1].
</pre>

<p><strong class="example">Example 2:</strong></p>
<pre>
<strong>Input:</strong> nums = [3,2,4], target = 6
<strong>Output:</strong> [1,2]
</pre>

<p><strong>Constraints:</strong></p>
<ul>
    <li>
        <code>2 &lt;= nums.length &lt;= 10<sup>4</sup></code>
    </li>
    <li>
        <code>-10<sup>9</sup> &lt;= nums[i] &lt;= 10<sup>9</sup></code>
    </li>
    <li><strong>Only one valid answer exists.</strong></li>
</ul>
```

## Step 4 — Add Your Solution File

Name the solution file using the same slug as the folder with the correct extension:

```
{problem-slug}.{extension}
```

| Language   | Extension | Example          |
| ---------- | --------- | ---------------- |
| C          | `.c`      | `two-sum.c`      |
| C++        | `.cpp`    | `two-sum.cpp`    |
| Csharp     | `.cs`     | `two-sum.cs`     |
| Python     | `.py`     | `two-sum.py`     |
| Java       | `.java`   | `two-sum.java`   |
| Rust       | `.rs`     | `two-sum.rs`     |
| JavaScript | `.js`     | `two-sum.js`     |
| TypeScript | `.ts`     | `two-sum.ts`     |
| Go         | `.go`     | `two-sum.go`     |
| Ruby       | `.rb`     | `two-sum.rb`     |

## Step 5 — Submit a Pull Request

1. Fork the repository
2. Create a new branch
    ```bash
    git checkout -b add/1-two-sum-cpp
    ```
3. Add your problem folder
4. Push and open a Pull Request against `main`

## What Happens After You Push

Once your PR is merged the automation pipeline runs automatically:

```
New solution committed
        ↓
detectNewProblems.js detects the new folder
        ↓
dsaMentor.js generates a full DSA lecture → saved as ANALYSIS.md
        ↓
Daily LeetCode Solution workflow posts lecture as a GitHub Issue
        ↓
updateTable.js rebuilds README with updated stats
```

You do not need to touch any other files. The pipeline handles everything.

## Rules

- Do not edit `README.md` or `ANALYSIS.md` manually — both are auto-generated
- Do not add more than one solution file per folder
- Do not commit `node_modules`, build artifacts, or IDE files
- Keep solution files clean — no debug print statements

## Questions?

Open an issue with the label `question` and we'll get back to you.

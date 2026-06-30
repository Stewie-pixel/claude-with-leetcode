import { readFileSync } from 'fs';
import { callOpenRouter, extractJSON } from './openrouter-utils.mjs';

/**
 * Parse a unified-diff patch string and return the Set of right-side
 * (new-file) line numbers that appear in the diff hunks.
 * These are the only line numbers GitHub's createReview API accepts.
 *
 * @param {string} patch - The raw patch string from GitHub's listFiles API
 * @returns {Set<number>}
 */
function buildValidDiffLines(patch) {
    const valid = new Set();
    if (!patch) return valid;

    let currentLine = 0;
    for (const line of patch.split('\n')) {
        const hunkHeader = line.match(
            /^@@ -\d+(?:,\d+)? \+(\d+)(?:,(\d+))? @@/,
        );
        if (hunkHeader) {
            currentLine = Number(hunkHeader[1]) - 1;
            continue;
        }
        if (line.startsWith('-')) continue;
        currentLine++;
        if (!line.startsWith('\\')) {
            valid.add(currentLine);
        }
    }
    return valid;
}

/**
 * Fetch a file's contents at a specific commit SHA via the GitHub REST API
 * and return it as a UTF-8 string.
 *
 * @param {object} github
 * @param {string} owner
 * @param {string} repo
 * @param {string} path
 * @param {string} ref   - commit SHA or branch name
 * @returns {string|null}
 */
async function fetchFileAtRef(github, owner, repo, path, ref) {
    try {
        const { data } = await github.rest.repos.getContent({
            owner,
            repo,
            path,
            ref,
        });
        return Buffer.from(data.content, 'base64').toString('utf8');
    } catch {
        return null;
    }
}

/**
 * Extract the Constraints block from a LeetCode README.md.
 * Looks for a <strong>Constraints:</strong> HTML block followed by <ul> items.
 *
 * @param {string} readmeContent
 * @returns {string} Extracted constraints text, or empty string
 */
function extractConstraints(readmeContent) {
    if (!readmeContent) return '';

    const match = readmeContent.match(
        /<strong>Constraints:<\/strong>[\s\S]*?<ul>([\s\S]*?)<\/ul>/i,
    );
    if (!match) return '';

    return match[1]
        .replace(/<li>/gi, '- ')
        .replace(/<\/li>/gi, '')
        .replace(/<[^>]+>/g, '')
        .replace(/&lt;/g, '<')
        .replace(/&gt;/g, '>')
        .replace(/&le;/g, '≤')
        .replace(/&ge;/g, '≥')
        .replace(/&amp;/g, '&')
        .trim();
}

/**
 * Prefix each line of source code with its 1-indexed line number.
 * e.g.  "  1 | int main() {"
 *
 * @param {string} source
 * @returns {string}
 */
function numberLines(source) {
    return source
        .split('\n')
        .map((l, i) => `${String(i + 1).padStart(4)} | ${l}`)
        .join('\n');
}

export async function review({ github, context, core }) {
    const isComment = context.eventName === 'issue_comment';

    if (isComment) {
        const owner = context.repo.owner;
        const repo = context.repo.repo;
        const prNumber = context.payload.issue.number;
        const userQuestion = context.payload.comment.body
            .replace(/@mirabile/gi, '')
            .trim();

        const { data: pr } = await github.rest.pulls.get({
            owner,
            repo,
            pull_number: prNumber,
        });

        const prFiles = await github.paginate(github.rest.pulls.listFiles, {
            owner,
            repo,
            pull_number: prNumber,
            per_page: 100,
        });

        const classifyPrompt = `You are a question classifier for a GitHub PR assistant.

Classify the following question into exactly one category:
- "problem" — question about a LeetCode solution, algorithm, complexity, or code logic
- "system" — question about CI/CD, GitHub Actions, scripts, workflows, or infrastructure
- "docs" — question about README, CONTRIBUTING.md, documentation, or markdown files
- "repo" — question about the repository itself, its purpose, structure, or maintainers

Question: "${userQuestion}"

Reply ONLY with one word: problem, system, docs, or repo`;

        let category = 'repo';
        try {
            const classifyRaw = await callOpenRouter({
                messages: [{ role: 'user', content: classifyPrompt }],
                retries: 3,
                core,
            });
            category = classifyRaw.trim().toLowerCase();
        } catch (err) {
            core.warning(`Question classification failed: ${err.message}`);
        }

        core.info(`Question classified as: ${category}`);

        const solutionExts = [
            'cpp',
            'py',
            'java',
            'js',
            'ts',
            'rs',
            'go',
            'c',
            'cs',
            'kt',
            'swift',
            'dart',
            'scala',
            'rb',
            'php',
        ];
        const systemPaths = [
            '.github/',
            'scripts/',
            'package.json',
            'Dockerfile',
            'docker-compose',
        ];
        const docsPaths = ['.md', 'docs/'];

        let relevantContext = '';

        if (category === 'problem') {
            const solutionFiles = prFiles.filter((f) => {
                const ext = f.filename.split('.').pop();
                return (
                    solutionExts.includes(ext) &&
                    !f.filename.includes('ANALYSIS.md') &&
                    !f.filename.includes('README.md')
                );
            });
            relevantContext =
                solutionFiles.length > 0
                    ? solutionFiles
                          .map((f) => `File: ${f.filename}\n${f.patch ?? ''}`)
                          .join('\n\n')
                          .substring(0, 8000)
                    : 'No solution files found in this PR.';
        } else if (category === 'system') {
            const systemFiles = prFiles.filter((f) =>
                systemPaths.some((p) => f.filename.includes(p)),
            );
            relevantContext =
                systemFiles.length > 0
                    ? systemFiles
                          .map((f) => `File: ${f.filename}\n${f.patch ?? ''}`)
                          .join('\n\n')
                          .substring(0, 8000)
                    : 'No system files found in this PR.';
        } else if (category === 'docs') {
            const docsFiles = prFiles.filter((f) =>
                docsPaths.some(
                    (p) => f.filename.endsWith(p) || f.filename.includes(p),
                ),
            );
            relevantContext =
                docsFiles.length > 0
                    ? docsFiles
                          .map((f) => `File: ${f.filename}\n${f.patch ?? ''}`)
                          .join('\n\n')
                          .substring(0, 8000)
                    : 'No documentation files found in this PR.';
        } else {
            const repoFiles = ['README.md', 'CONTRIBUTING.md', 'CLAUDE.md'];
            const fetchedDocs = [];
            for (const path of repoFiles) {
                try {
                    const { data } = await github.rest.repos.getContent({
                        owner,
                        repo,
                        path,
                    });
                    const content = Buffer.from(
                        data.content,
                        'base64',
                    ).toString('utf8');
                    fetchedDocs.push(
                        `File: ${path}\n${content.substring(0, 2000)}`,
                    );
                } catch {}
            }
            relevantContext =
                fetchedDocs.length > 0
                    ? fetchedDocs.join('\n\n')
                    : 'No repository documentation found.';
        }

        const categoryInstructions = {
            problem: `You are reviewing a LeetCode solution. Focus only on the solution code files provided.
Analyze the algorithm, time/space complexity, edge cases, and code quality.
Be specific and reference actual line numbers or variable names from the code.`,
            system: `You are reviewing CI/CD and system configuration files. Focus only on the workflow/script files provided.
Analyze the pipeline logic, potential failures, security concerns, and efficiency.
Reference specific steps, job names, or script sections in your answer.`,
            docs: `You are reviewing documentation files. Focus only on the markdown/docs files provided.
Analyze clarity, completeness, accuracy, and formatting.
Reference specific sections or headings in your answer.`,
            repo: `You are answering a question about the claude-with-leetcode repository.
This is an open-source project that uses Claude AI to automatically generate daily DSA lectures from LeetCode solutions committed by developers.
Use the repository documentation provided to answer. If the answer is not in the docs, direct the user to @Stewie-pixel (maintainer).
Do not guess or fabricate information about the repo.`,
        };

        const answerPrompt = `You are Mirabile, an AI code reviewer on PR #${prNumber}: "${pr.title}".

${categoryInstructions[category] ?? categoryInstructions.repo}

---

Relevant Files:
${relevantContext}

---

Developer's question: "${userQuestion}"

Answer directly and concisely in 3-5 sentences. Be technical and helpful.${category === 'repo' ? ' If you cannot answer from the provided docs, say: "I don\'t have enough context on this — this will be transfered for a maintainer response."' : ''}`;

        let answer = null;
        try {
            answer = await callOpenRouter({
                messages: [{ role: 'user', content: answerPrompt }],
                retries: 3,
                core,
            });
        } catch (err) {
            core.warning(`Answer generation failed: ${err.message}`);
        }

        if (!answer || answer === 'null') {
            core.info('No answer generated, skipping.');
            return;
        }

        await github.rest.issues.createComment({
            owner,
            repo,
            issue_number: prNumber,
            body: answer,
        });

        return;
    }

    const prNumber = context.payload.pull_request.number;
    const owner = context.repo.owner;
    const repo = context.repo.repo;
    const prTitle = context.payload.pull_request.title;
    const prBody =
        context.payload.pull_request.body ?? 'No description provided.';
    const prAuthor = context.payload.pull_request.user.login;
    const headSha = context.payload.pull_request.head.sha;

    const files = await github.paginate(github.rest.pulls.listFiles, {
        owner,
        repo,
        pull_number: prNumber,
        per_page: 100,
    });

    const supportedLangs = [
        'cpp',
        'python',
        'java',
        'javascript',
        'typescript',
        'rust',
        'go',
        'c',
        'csharp',
        'kotlin',
        'swift',
        'dart',
        'scala',
        'ruby',
        'php',
    ];

    const SLUG_PATTERN = /^[a-z0-9]+(-[a-z0-9]+)*\.[a-z0-9]+$/;
    const problemFolders = new Set();
    const violations = {
        analysisManuallyAdded: [],
        badNaming: [],
        missingReadme: [],
    };

    /** @type {Map<string, Set<number>>} path → Set of valid right-side line numbers */
    const validDiffLinesMap = new Map();
    let diff = '';

    const solutionExtSet = new Set([
        'cpp',
        'py',
        'java',
        'js',
        'ts',
        'rs',
        'go',
        'c',
        'cs',
        'kt',
        'swift',
        'dart',
        'scala',
        'rb',
        'php',
    ]);

    for (const file of files) {
        const parts = file.filename.split('/');
        if (!supportedLangs.includes(parts[0]) || parts.length < 3) continue;

        problemFolders.add(`${parts[0]}/${parts[1]}`);

        if (parts[parts.length - 1] === 'ANALYSIS.md') {
            violations.analysisManuallyAdded.push(file.filename);
        }

        const filename = parts[parts.length - 1];
        const ext = filename.split('.').pop();

        if (filename !== 'README.md' && filename !== 'ANALYSIS.md') {
            if (!SLUG_PATTERN.test(filename)) {
                violations.badNaming.push(file.filename);
            }
            if (file.patch && solutionExtSet.has(ext)) {
                diff += `diff --git a/${file.filename} b/${file.filename}\n${file.patch}\n`;
                validDiffLinesMap.set(
                    file.filename,
                    buildValidDiffLines(file.patch),
                );
            }
        }
    }

    for (const folder of problemFolders) {
        const readmePath = `${folder}/README.md`;
        const hasReadme = files.some((f) => f.filename === readmePath);
        if (!hasReadme) {
            try {
                await github.rest.repos.getContent({
                    owner,
                    repo,
                    path: readmePath,
                });
            } catch {
                violations.missingReadme.push(folder);
            }
        }
    }

    const hasViolations = Object.values(violations).some((v) => v.length > 0);
    diff = diff.substring(0, 15000);

    /** @type {Map<string, string>} folder → constraint text */
    const constraintsMap = new Map();
    /** @type {Map<string, string>} path → numbered source */
    const numberedSourceMap = new Map();

    if (diff.trim()) {
        for (const folder of problemFolders) {
            const readmePath = `${folder}/README.md`;
            const content = await fetchFileAtRef(
                github,
                owner,
                repo,
                readmePath,
                headSha,
            );
            if (content) {
                const constraints = extractConstraints(content);
                if (constraints) constraintsMap.set(folder, constraints);
            }
        }

        for (const file of files) {
            const parts = file.filename.split('/');
            if (!supportedLangs.includes(parts[0]) || parts.length < 3)
                continue;
            const filename = parts[parts.length - 1];
            const ext = filename.split('.').pop();
            if (filename === 'README.md' || filename === 'ANALYSIS.md')
                continue;
            if (!solutionExtSet.has(ext)) continue;

            const source = await fetchFileAtRef(
                github,
                owner,
                repo,
                file.filename,
                headSha,
            );
            if (source) {
                numberedSourceMap.set(file.filename, numberLines(source));
            }
        }
    }

    const constraintsSection =
        constraintsMap.size > 0
            ? [...constraintsMap.entries()]
                  .map(([folder, c]) => `### ${folder}\n${c}`)
                  .join('\n\n')
            : 'No constraints found in README files.';

    const validLinesSection =
        validDiffLinesMap.size > 0
            ? [...validDiffLinesMap.entries()]
                  .map(([path, lineSet]) => {
                      const lines = [...lineSet].sort((a, b) => a - b);
                      return `${path}: [${lines.join(', ')}]`;
                  })
                  .join('\n')
            : 'No diff line data available.';

    const numberedSourceSection =
        numberedSourceMap.size > 0
            ? [...numberedSourceMap.entries()]
                  .map(([path, src]) => `### ${path}\n\`\`\`\n${src}\n\`\`\``)
                  .join('\n\n')
                  .substring(0, 10000)
            : 'No full source available.';

    let rubric = '';
    try {
        rubric = readFileSync(
            `${process.env.GITHUB_WORKSPACE}/skills/dsa-code-review.md`,
            'utf8',
        );
    } catch (err) {
        core.warning(`Could not load DSA skill rubric: ${err.message}`);
    }

    let summaryParagraph = 'No summary could be generated at this time.';
    let highlights = [];
    let reviewFeedback = 'No AI feedback could be generated at this time.';
    let inlineComments = [];
    let finalVerdict = 'COMMENT';
    let aiReviewSucceeded = false;

    if (diff.trim()) {
        const call1Prompt = `You are Mirabile, an AI code reviewer for the claude-with-leetcode repository.

PR Details:
- PR #${prNumber}: ${prTitle}
- Author: @${prAuthor}
- Description: ${prBody}

Git Diff:
${diff}

Return ONLY a raw JSON object with no markdown fences:
{
  "summary": "2-3 sentence summary of what this PR does and its overall quality.",
  "highlights": ["most important point 1", "most important point 2", "most important point 3"]
}

Rules:
- highlights: maximum 3 items, each a short specific observation
- No markdown wrapping around the JSON`;

        try {
            const raw1 = await callOpenRouter({
                messages: [{ role: 'user', content: call1Prompt }],
                jsonMode: true,
                retries: 3,
                core,
            });
            const parsed1 = extractJSON(raw1);
            summaryParagraph = parsed1.summary ?? summaryParagraph;
            highlights = Array.isArray(parsed1.highlights)
                ? parsed1.highlights.slice(0, 3)
                : [];
            core.info('Call 1 (summary) succeeded.');
        } catch (err) {
            core.error(`Call 1 (summary) failed: ${err.message}`);
        }

        const call2Prompt = `You are Mirabile, an expert DSA code reviewer. Apply the rubric below to produce a structured JSON review.

## DSA Review Rubric
${rubric}

---

## PR Details
- PR #${prNumber}: ${prTitle}
- Author: @${prAuthor}

## Problem Constraints (from README files)
${constraintsSection}

## Full Numbered Source (at PR head commit)
${numberedSourceSection}

## Git Diff
${diff}

## Valid Diff Lines (ONLY use these line numbers for inline comments)
${validLinesSection}

---

Return ONLY a raw JSON object with no markdown fences, matching this exact schema:
{
  "checks": {
    "correctness":           "pass|fail|unknown — one sentence",
    "overflow":              "pass|fail|unknown — one sentence",
    "time_complexity":       "O(...) — pass|fail vs constraints",
    "space_memory":          "pass|fail — one sentence",
    "algorithmic_soundness": "pass|fail — pattern named"
  },
  "feedback": "1-2 paragraphs with specific line references",
  "verdict": "APPROVE|COMMENT|REQUEST_CHANGES",
  "comments": [
    {
      "path": "exact/file/path.ext",
      "line": <number from Valid Diff Lines>,
      "severity": "CRITICAL|HIGH|MEDIUM",
      "body": "[CRITICAL|HIGH|MEDIUM] explanation\\n\\n\`\`\`suggestion\\n// code\\n\`\`\`"
    }
  ]
}

IMPORTANT:
- line values MUST be from the Valid Diff Lines list — no other lines will be accepted
- body MUST start with [CRITICAL], [HIGH], or [MEDIUM]
- Include at least 1 comment when any check is "fail"
- Maximum 5 comments, CRITICAL/HIGH only unless budget allows MEDIUM
- No markdown fences around the JSON`;

        try {
            const raw2 = await callOpenRouter({
                messages: [{ role: 'user', content: call2Prompt }],
                jsonMode: true,
                retries: 3,
                core,
            });
            const parsed2 = extractJSON(raw2);

            reviewFeedback = parsed2.feedback ?? reviewFeedback;
            finalVerdict = parsed2.verdict ?? 'COMMENT';

            if (Array.isArray(parsed2.comments)) {
                const rawComments = parsed2.comments;
                const filtered = [];
                for (const c of rawComments) {
                    const validLines = validDiffLinesMap.get(c.path);
                    if (!validLines) {
                        core.warning(
                            `Dropping inline comment on unknown path "${c.path}" (not in diff)`,
                        );
                        continue;
                    }
                    if (!validLines.has(Number(c.line))) {
                        core.warning(
                            `Dropping inline comment on ${c.path}:${c.line} — line not in diff`,
                        );
                        continue;
                    }
                    const bodyOk = /^\[(CRITICAL|HIGH|MEDIUM)\]/.test(
                        (c.body ?? '').trim(),
                    );
                    if (!bodyOk) {
                        core.warning(
                            `Dropping inline comment on ${c.path}:${c.line} — body missing severity tag`,
                        );
                        continue;
                    }
                    filtered.push({
                        path: c.path,
                        line: Number(c.line),
                        side: 'RIGHT',
                        body: c.body,
                    });
                }
                inlineComments = filtered.slice(0, 5);

                const hasCritical = inlineComments.some((c) =>
                    c.body.startsWith('[CRITICAL]'),
                );
                if (hasCritical && finalVerdict === 'APPROVE') {
                    core.info(
                        'Overriding APPROVE → REQUEST_CHANGES due to CRITICAL inline comment.',
                    );
                    finalVerdict = 'REQUEST_CHANGES';
                }
            }

            aiReviewSucceeded = true;
            core.info('Call 2 (DSA review) succeeded.');
        } catch (err) {
            core.error(`Call 2 (DSA review) failed: ${err.message}`);
        }
    } else {
        aiReviewSucceeded = true;
        core.info('No solution diff found — skipping AI review.');
    }

    const highlightsList =
        highlights.length > 0
            ? highlights.map((h) => `- ${h}`).join('\n')
            : '- No highlights available.';

    const summaryComment = `## Summary Changes

Hello! I'm **Mirabile**, your AI Code Reviewer. I'm currently reviewing this pull request and will post my detailed feedback shortly. In the meantime, here's a summary to help you and other reviewers quickly get up to speed!

${summaryParagraph}

### Highlights

${highlightsList}

*For more details, reference our [CONTRIBUTING.md](https://github.com/Stewie-pixel/claude-with-leetcode/blob/main/CONTRIBUTING.md)*`;

    await github.rest.issues.createComment({
        owner,
        repo,
        issue_number: prNumber,
        body: summaryComment,
    });

    const passIcon = '✅';
    const failIcon = '❌';
    const structuralStatus = hasViolations
        ? `${failIcon} Structural checks failed`
        : `${passIcon} All structural checks passed`;

    const reviewBody = `## Code Review

${reviewFeedback}

### Structural Checks — ${structuralStatus}

| Check | Result |
|---|---|
| No \`ANALYSIS.md\` manually added | ${violations.analysisManuallyAdded.length === 0 ? '✅ Pass' : '❌ Fail'} |
| Solution file naming | ${violations.badNaming.length === 0 ? '✅ Pass' : '❌ Fail'} |
| \`README.md\` exists | ${violations.missingReadme.length === 0 ? '✅ Pass' : '❌ Fail'} |

${violations.analysisManuallyAdded.length > 0 ? `\`ANALYSIS.md\` should not be manually committed:\n${violations.analysisManuallyAdded.map((f) => `  - \`${f}\``).join('\n')}\n` : ''}
${violations.badNaming.length > 0 ? `Incorrect file naming:\n${violations.badNaming.map((f) => `  - \`${f}\``).join('\n')}\n` : ''}
${violations.missingReadme.length > 0 ? `Missing \`README.md\`:\n${violations.missingReadme.map((f) => `  - \`${f}/\``).join('\n')}\n` : ''}

**If you need further feedback:** Mention **@mirabile** in a comment with your question and I'll respond directly.`;

    const resolvedEvent = hasViolations ? 'REQUEST_CHANGES' : finalVerdict;

    await github.rest.pulls.createReview({
        owner,
        repo,
        pull_number: prNumber,
        body: reviewBody,
        event: resolvedEvent,
        comments: inlineComments,
    });

    if (hasViolations) {
        await github.rest.issues.addLabels({
            owner,
            repo,
            issue_number: prNumber,
            labels: ['needs-changes'],
        });
        core.setFailed('Structural assertions unmet.');
        return;
    }

    try {
        await github.rest.issues.removeLabel({
            owner,
            repo,
            issue_number: prNumber,
            name: 'needs-changes',
        });
    } catch {}

    if (!aiReviewSucceeded) {
        core.setFailed(
            'AI review generation failed after retries. Re-run the workflow to retry.',
        );
    }
}

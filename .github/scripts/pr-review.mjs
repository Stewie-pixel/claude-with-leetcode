import { execSync } from 'child_process';

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

        const classifyResponse = await fetch(
            'https://openrouter.ai/api/v1/chat/completions',
            {
                method: 'POST',
                headers: {
                    Authorization: `Bearer ${process.env.OPENROUTER_API_KEY}`,
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({
                    model: process.env.OPENROUTER_MODEL,
                    messages: [{ role: 'user', content: classifyPrompt }],
                }),
            },
        );

        const classifyData = await classifyResponse.json();
        const category =
            classifyData.choices?.[0]?.message?.content?.trim().toLowerCase() ??
            'repo';

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

        const answerResponse = await fetch(
            'https://openrouter.ai/api/v1/chat/completions',
            {
                method: 'POST',
                headers: {
                    Authorization: `Bearer ${process.env.OPENROUTER_API_KEY}`,
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({
                    model: process.env.OPENROUTER_MODEL,
                    messages: [{ role: 'user', content: answerPrompt }],
                }),
            },
        );

        const answerData = await answerResponse.json();
        const answer = answerData.choices?.[0]?.message?.content?.trim();

        if (!answer || answer === 'null') {
            core.info('No answer generated, skipping.');
            return;
        }

        await github.rest.issues.createComment({
            owner,
            repo,
            issue_number: prNumber,
            body: `{answer}`,
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

    let diff = '';

    for (const file of files) {
        const parts = file.filename.split('/');
        if (!supportedLangs.includes(parts[0]) || parts.length < 3) continue;

        problemFolders.add(`${parts[0]}/${parts[1]}`);

        if (parts[parts.length - 1] === 'ANALYSIS.md') {
            violations.analysisManuallyAdded.push(file.filename);
        }

        const filename = parts[parts.length - 1];
        if (filename !== 'README.md' && filename !== 'ANALYSIS.md') {
            if (!SLUG_PATTERN.test(filename)) {
                violations.badNaming.push(file.filename);
            }
            if (file.patch) {
                diff += `diff --git a/${file.filename} b/${file.filename}\n${file.patch}\n`;
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

    let summaryParagraph = 'No summary could be generated at this time.';
    let highlights = [];
    let reviewFeedback = 'No AI feedback could be generated at this time.';
    let inlineComments = [];
    let finalVerdict = 'COMMENT';

    if (diff.trim()) {
        const prompt = `You are Mirabile, an AI code reviewer checking LeetCode solutions in the claude-with-leetcode repository.

PR Details:
- PR #${prNumber}: ${prTitle}
- Author: @${prAuthor}
- Description: ${prBody}

Git Diff:
${diff}

You must return ONLY a raw JSON object in this exact structure with no markdown wrapping:
{
  "summary": "2-3 sentence summary of what this PR does and its overall quality.",
  "highlights": ["most important thing 1", "most important thing 2", "most important thing 3"],
  "feedback": "1-2 paragraph review feedback covering code quality, approach correctness, and specific observations about the solution.",
  "verdict": "APPROVE" or "COMMENT" or "REQUEST_CHANGES",
  "comments": [
    {
      "path": "exact/file/path.ext",
      "line": 42,
      "body": "Technical critique.\\n\\n\`\`\`suggestion\\n// replacement code here\\n\`\`\`"
    }
  ]
}

Rules:
- highlights must have maximum 3 items
- For inline comments, the line number must be the absolute line number in the NEW file version
- Use \`\`\`suggestion blocks for suggested code changes
- Only include inline comments for critical or high severity issues
- If no inline comments needed, return empty array []`;

        try {
            const response = await fetch(
                'https://openrouter.ai/api/v1/chat/completions',
                {
                    method: 'POST',
                    headers: {
                        Authorization: `Bearer ${process.env.OPENROUTER_API_KEY}`,
                        'Content-Type': 'application/json',
                    },
                    body: JSON.stringify({
                        model: process.env.OPENROUTER_MODEL,
                        messages: [{ role: 'user', content: prompt }],
                    }),
                },
            );

            const data = await response.json();
            const rawContent = data.choices[0].message.content.trim();
            const jsonString = rawContent.startsWith('```json')
                ? rawContent.slice(7, -3).trim()
                : rawContent.startsWith('```')
                  ? rawContent.slice(3, -3).trim()
                  : rawContent;

            const parsed = JSON.parse(jsonString);
            summaryParagraph = parsed.summary ?? summaryParagraph;
            highlights = Array.isArray(parsed.highlights)
                ? parsed.highlights.slice(0, 3)
                : [];
            reviewFeedback = parsed.feedback ?? reviewFeedback;
            finalVerdict = parsed.verdict ?? 'COMMENT';

            if (Array.isArray(parsed.comments)) {
                inlineComments = parsed.comments.map((c) => ({
                    path: c.path,
                    line: Number(c.line),
                    side: 'RIGHT',
                    body: c.body,
                }));
            }
        } catch (err) {
            core.error(`Failed parsing AI review: ${err.message}`);
        }
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

*For more details, reference at our [CONTRIBUTING.md](https://github.com/Stewie-pixel/claude-with-leetcode/blob/main/CONTRIBUTING.md)*`;

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
    } else {
        try {
            await github.rest.issues.removeLabel({
                owner,
                repo,
                issue_number: prNumber,
                name: 'needs-changes',
            });
        } catch {}
    }
}

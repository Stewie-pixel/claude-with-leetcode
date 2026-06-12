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

        const prompt = `You are Mirabile, an AI code reviewer on a GitHub PR for the claude-with-leetcode repository.

A developer has asked you a question about PR #${prNumber}: "${pr.title}"

Their question is:
"${userQuestion}"

PR description:
${pr.body ?? 'No description provided.'}

Answer the question directly and concisely in 2-4 sentences. Be technical and helpful. Do not repeat the question back.`;

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
        const answer = data.choices?.[0]?.message?.content?.trim();

        if (!answer || answer === 'null') {
            core.info('No answer generated, skipping.');
            return;
        }

        await github.rest.issues.createComment({
            owner,
            repo,
            issue_number: prNumber,
            body: `${answer}`,
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

    let resolvedEvent = hasViolations ? 'REQUEST_CHANGES' : finalVerdict;

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

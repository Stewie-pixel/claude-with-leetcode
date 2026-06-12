import { execSync } from 'child_process';

export async function review({ github, context, core }) {
    const prNumber = context.payload.pull_request.number;
    const owner = context.repo.owner;
    const repo = context.repo.repo;
    const baseSha = context.payload.pull_request.base.sha;
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

    let diff = '';
    try {
        diff = execSync(
            `git diff ${baseSha} ${headSha} -- '*.cpp' '*.py' '*.java' '*.js' '*.ts' '*.rs' '*.go' '*.c' '*.cs' '*.kt' '*.swift' '*.dart' '*.scala' '*.rb' '*.php' ':!**/README.md' ':!**/ANALYSIS.md'`,
        ).toString('utf8');
        diff = diff.substring(0, 15000);
    } catch (err) {
        core.warning(`Could not generate git diff: ${err.message}`);
    }

    let aiAssessment =
        'AI review skipped due to an error processing the code adjustments.';
    let inlineComments = [];
    let finalVerdict = 'COMMENT';

    if (diff.trim()) {
        const prompt = `You are Mirabile, an AI code reviewer checking LeetCode solutions.
Review the git diff below and provide targeted inline suggestions.
You must return your response strictly as a raw JSON object matching the format specified below. Do not wrap it in markdown code blocks.

Target JSON Structure:
{
  "assessment": "Overall diagnostic summary paragraph.",
  "verdict": "APPROVE" or "COMMENT" or "REQUEST_CHANGES",
  "comments": [
    {
      "path": "exact/file/path.ext",
      "line": 42,
      "body": "Detailed technical critique describing the inefficiency.\\n\\n\`\`\`suggestion\\n// exact line replacements go here\\n\`\`\`"
    }
  ]
}

Rules for line placement:
The "line" property is the absolute line number in the NEW version of the file (the right side of the diff). Use hunk headers like @@ -2,4 +2,6 @@ to accurately track the position of your target line.

Git Diff Context:
${diff}`;

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

            const parsedReview = JSON.parse(jsonString);
            aiAssessment = parsedReview.assessment || '';
            finalVerdict = parsedReview.verdict || 'COMMENT';

            if (Array.isArray(parsedReview.comments)) {
                inlineComments = parsedReview.comments.map((c) => ({
                    path: c.path,
                    line: Number(c.line),
                    side: 'RIGHT',
                    body: c.body,
                }));
            }
        } catch (err) {
            core.error(
                `Failed parsing or fetching AI structural review: ${err.message}`,
            );
        }
    }

    const passIcon = '✅';
    const failIcon = '❌';
    const structuralStatus = hasViolations
        ? `${failIcon} Structural checks failed`
        : `${passIcon} All structural checks passed`;

    const globalReviewBody = `### Repo Structural Checks — ${structuralStatus}

| Check | Result |
|---|---|
| No \`ANALYSIS.md\` manually added | ${violations.analysisManuallyAdded.length === 0 ? '✅ Pass' : '❌ Fail'} |
| Solution file naming | ${violations.badNaming.length === 0 ? '✅ Pass' : '❌ Fail'} |
| \`README.md\` exists | ${violations.missingReadme.length === 0 ? '✅ Pass' : '❌ Fail'} |

${violations.analysisManuallyAdded.length > 0 ? `\`ANALYSIS.md\` should not be manually committed:\n${violations.analysisManuallyAdded.map((f) => `  - \`${f}\``).join('\n')}\n` : ''}
${violations.badNaming.length > 0 ? `Incorrect file naming:\n${violations.badNaming.map((f) => `  - \`${f}\``).join('\n')}\n` : ''}
${violations.missingReadme.length > 0 ? `Missing \`README.md\`:\n${violations.missingReadme.map((f) => `  - \`${f}/\``).join('\n')}\n` : ''}

### Summary Assessment
${aiAssessment}

*For more details on how to have a clear contribution, reference at our [CONTRIBUTING.md](https://github.com/Stewie-pixel/claude-with-leetcode/blob/main/CONTRIBUTING.md)*`;

    let resolvedEvent = finalVerdict;
    if (hasViolations) {
        resolvedEvent = 'REQUEST_CHANGES';
    }

    await github.rest.pulls.createReview({
        owner,
        repo,
        pull_number: prNumber,
        body: globalReviewBody,
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

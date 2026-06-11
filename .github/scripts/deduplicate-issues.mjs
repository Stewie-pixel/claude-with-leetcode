export default async function deduplicate({ github, context }) {
    const newIssue = context.payload.issue;
    const owner = context.repo.owner;
    const repo = context.repo.repo;

    const existingIssues = await github.paginate(
        github.rest.issues.listForRepo,
        { owner, repo, state: 'open', per_page: 100 },
    );

    const candidates = existingIssues
        .filter((i) => i.number !== newIssue.number)
        .map((i) => `#${i.number}: ${i.title}`)
        .join('\n');

    if (!candidates) {
        console.log('No existing issues to compare against.');
        return;
    }

    const prompt = `You are a GitHub issue deduplication assistant. Given a new issue and a list of existing open issues, identify which existing issues are likely duplicates or very closely related to the new issue.

New Issue: "${newIssue.title}"

Existing Open Issues:
${candidates}

Reply ONLY with a JSON array of issue numbers that are likely duplicates. Example: [12, 45]
If no duplicates found, reply with an empty array: []
Do not include any explanation.`;

    const response = await fetch(
        'https://openrouter.ai/api/v1/chat/completions',
        {
            method: 'POST',
            headers: {
                Authorization: `Bearer ${process.env.OPENROUTER_API_KEY}`,
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                model: 'nvidia/nemotron-3-super-120b-a12b:free',
                messages: [{ role: 'user', content: prompt }],
            }),
        },
    );

    const data = await response.json();
    const raw = data.choices?.[0]?.message?.content?.trim() ?? '[]';

    let duplicateNumbers = [];
    try {
        duplicateNumbers = JSON.parse(raw.replace(/```json|```/g, '').trim());
    } catch {
        console.warn('Failed to parse LLM response:', raw);
        return;
    }

    if (duplicateNumbers.length === 0) {
        console.log('No duplicates found.');
        return;
    }

    const dupList = existingIssues
        .filter((i) => duplicateNumbers.includes(i.number))
        .map((i) => `- #${i.number} — **${i.title}**`)
        .join('\n');

    const comment = `## Possible Duplicate Detected

This issue may already exist. The following open issues appear to be closely related:

${dupList}

Please review the issues above before proceeding. If this is a duplicate, consider closing this issue and continuing the discussion there.

If you believe this is **not** a duplicate, no action is needed — a maintainer will review shortly.

---
*Duplicate check by [@mirabile](https://github.com/apps/mirabile)*`;

    await github.rest.issues.createComment({
        owner,
        repo,
        issue_number: newIssue.number,
        body: comment,
    });

    await github.rest.issues.addLabels({
        owner,
        repo,
        issue_number: newIssue.number,
        labels: ['possible-duplicate'],
    });

    console.log(`Found ${duplicateNumbers.length} potential duplicate(s).`);
}

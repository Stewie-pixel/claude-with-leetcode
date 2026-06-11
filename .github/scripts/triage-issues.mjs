import { writeFileSync } from 'node:fs';
import { join } from 'node:path';

const AREA_LABELS = [
    'area/arrays',
    'area/strings',
    'area/linked-list',
    'area/trees',
    'area/graphs',
    'area/dynamic-programming',
    'area/binary-search',
    'area/sorting',
    'area/hashing',
    'area/stack',
    'area/queue',
    'area/heap',
    'area/two-pointers',
    'area/sliding-window',
    'area/backtracking',
    'area/bit-manipulation',
    'area/math',
    'area/other',
];

const KIND_LABELS = [
    'kind/bug',
    'kind/feature-request',
    'kind/question',
    'kind/discussion',
    'kind/solution',
    'kind/contest',
];

const PRIORITY_LABELS = [
    'priority/critical',
    'priority/high',
    'priority/medium',
    'priority/low',
];

export default async function triage({ github, context, core }) {
    const owner = context.repo.owner;
    const repo = context.repo.repo;

    const issueNumber =
        context.payload.issue?.number ??
        Number(context.payload.inputs?.issue_number);

    if (!issueNumber) {
        core.setFailed('Could not determine issue number.');
        return;
    }

    const { data: issue } = await github.rest.issues.get({
        owner,
        repo,
        issue_number: issueNumber,
    });

    if (issue.state === 'closed') {
        core.info(`Issue #${issueNumber} is closed — skipping triage.`);
        return;
    }

    const existingLabels = issue.labels.map((l) =>
        typeof l === 'string' ? l : l.name,
    );

    const alreadyTriaged = existingLabels.includes('status/bot-triaged');
    if (alreadyTriaged) {
        core.info(`Issue #${issueNumber} is already bot-triaged — skipping.`);
        return;
    }

    const prompt = `You are a triage assistant for a DSA/LeetCode learning repository.
Given the issue below, choose exactly one label from each of the three categories.

Issue title: "${issue.title}"
Issue body:
${(issue.body ?? '').slice(0, 1500)}

Categories and valid values:
- area: ${AREA_LABELS.join(', ')}
- kind: ${KIND_LABELS.join(', ')}
- priority: ${PRIORITY_LABELS.join(', ')}

Reply ONLY with a JSON object. Example:
{"area":"area/trees","kind":"kind/question","priority":"priority/medium"}
Do not include any explanation or markdown.`;

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
    const raw = data.choices?.[0]?.message?.content?.trim() ?? '{}';

    let classification = {};
    try {
        classification = JSON.parse(raw.replace(/```json|```/g, '').trim());
    } catch {
        core.warning(`Failed to parse AI response: ${raw}`);
    }

    const labelsToAdd = ['status/need-triage', 'status/bot-triaged'];

    if (AREA_LABELS.includes(classification.area)) {
        labelsToAdd.push(classification.area);
    } else {
        core.warning(
            `Invalid area label "${classification.area}" — falling back to area/other.`,
        );
        labelsToAdd.push('area/other');
    }

    if (KIND_LABELS.includes(classification.kind)) {
        labelsToAdd.push(classification.kind);
    } else {
        core.warning(`Invalid kind label "${classification.kind}" — skipping.`);
    }

    if (PRIORITY_LABELS.includes(classification.priority)) {
        labelsToAdd.push(classification.priority);
    } else {
        core.warning(
            `Invalid priority label "${classification.priority}" — skipping.`,
        );
    }

    const newLabels = labelsToAdd.filter((l) => !existingLabels.includes(l));

    if (newLabels.length > 0) {
        await github.rest.issues.addLabels({
            owner,
            repo,
            issue_number: issueNumber,
            labels: newLabels,
        });
        core.info(`Applied labels to #${issueNumber}: ${newLabels.join(', ')}`);
    }

    const workspacePath = process.env.GITHUB_WORKSPACE ?? '.';
    const cleanupPath = join(workspacePath, 'issues_to_cleanup.json');
    writeFileSync(
        cleanupPath,
        JSON.stringify([{ number: issueNumber }]),
        'utf8',
    );
    core.info(`Wrote ${cleanupPath} for cleanup step.`);
}

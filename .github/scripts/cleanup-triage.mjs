// .github/scripts/cleanup-triage.js
import { readFileSync } from 'node:fs';
import { join } from 'node:path';

export default async function cleanupTriage({ github, context, core }) {
    const workspacePath = process.env.GITHUB_WORKSPACE ?? '.';
    const cleanupPath = join(workspacePath, 'issues_to_cleanup.json');

    let issuesToCleanup = [];
    try {
        const fileContent = readFileSync(cleanupPath, 'utf8');
        issuesToCleanup = JSON.parse(fileContent);
    } catch (error) {
        if (error.code === 'ENOENT') {
            core.info('No issues found to clean up.');
            return;
        }
        core.setFailed(
            `Failed to read issues_to_cleanup.json: ${error.message}`,
        );
        return;
    }

    for (const issue of issuesToCleanup) {
        try {
            const { data: issueData } = await github.rest.issues.get({
                owner: context.repo.owner,
                repo: context.repo.repo,
                issue_number: issue.number,
            });

            const labels = issueData.labels.map((l) =>
                typeof l === 'string' ? l : l.name,
            );

            if (
                labels.includes('status/bot-triaged') &&
                labels.includes('status/need-triage')
            ) {
                await github.rest.issues.removeLabel({
                    owner: context.repo.owner,
                    repo: context.repo.repo,
                    issue_number: issue.number,
                    name: 'status/need-triage',
                });
                core.info(`Removed status/need-triage from #${issue.number}`);
            }

            if (
                labels.includes('status/bot-triaged') &&
                labels.includes('status/manual-triage')
            ) {
                await github.rest.issues.removeLabel({
                    owner: context.repo.owner,
                    repo: context.repo.repo,
                    issue_number: issue.number,
                    name: 'status/bot-triaged',
                });
                core.info(
                    `Removed status/bot-triaged from #${issue.number} — requires manual triage`,
                );
            }
        } catch (error) {
            core.warning(
                `Failed to clean up labels for #${issue.number}: ${error.message}`,
            );
        }
    }

    core.info(
        `Cleaned up conflicting labels from ${issuesToCleanup.length} issue(s).`,
    );
}

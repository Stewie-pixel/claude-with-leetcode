export async function triage({ github, context, core }) {
    const prNumber = context.payload.pull_request.number;
    const owner = context.repo.owner;
    const repo = context.repo.repo;

    const files = await github.paginate(github.rest.pulls.listFiles, {
        owner,
        repo,
        pull_number: prNumber,
        per_page: 100,
    });
    const filenames = files.map((f) => f.filename);

    const labels = new Set();

    const languageRules = [
        { pattern: /^cpp\//, label: 'cpp' },
        { pattern: /^python\//, label: 'python' },
        { pattern: /^java\//, label: 'java' },
        { pattern: /^javascript\//, label: 'javascript' },
        { pattern: /^typescript\//, label: 'typescript' },
        { pattern: /^rust\//, label: 'rust' },
        { pattern: /^go\//, label: 'go' },
        { pattern: /^c\//, label: 'c' },
        { pattern: /^csharp\//, label: 'csharp' },
        { pattern: /^kotlin\//, label: 'kotlin' },
        { pattern: /^swift\//, label: 'swift' },
        { pattern: /^dart\//, label: 'dart' },
        { pattern: /^scala\//, label: 'scala' },
        { pattern: /^ruby\//, label: 'ruby' },
        { pattern: /^php\//, label: 'php' },
        { pattern: /^dcc\//, label: 'dcc' },
        { pattern: /^study_plan\//, label: 'study-plan' },
        { pattern: /^contest\/weekly/, label: 'contest/weekly' },
        { pattern: /^contest\/biweekly/, label: 'contest/biweekly' },
    ];

    const categoryRules = [
        { pattern: /^\.github\/workflows\//, label: 'ci' },
        { pattern: /\.(md)$/, label: 'documentation' },
        { pattern: /^skills\/|^CLAUDE\.md$/, label: 'ai-agent' },
        { pattern: /\.js$/, label: 'automation' },
    ];

    for (const file of filenames) {
        for (const rule of languageRules) {
            if (rule.pattern.test(file)) {
                labels.add(rule.label);
                labels.add('solution');
            }
        }

        for (const rule of categoryRules) {
            if (rule.pattern.test(file)) {
                labels.add(rule.label);
            }
        }
    }

    if (labels.size === 0) {
        labels.add('needs-triage');
    }

    const labelsArray = [...labels];

    await github.rest.issues.addLabels({
        owner,
        repo,
        issue_number: prNumber,
        labels: labelsArray,
    });

    console.log(`Applied labels: ${labelsArray.join(', ')} to PR #${prNumber}`);
}

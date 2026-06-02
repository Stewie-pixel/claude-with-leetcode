const fs = require('fs');
const path = require('path');

const PROBLEMS_SITE_DATA = JSON.parse(
    fs.readFileSync('./.problemSiteData.json', 'utf8'),
);

const languageMap = {
    c: { directory: 'c', extension: 'c' },
    cpp: { directory: 'cpp', extension: 'cpp' },
    csharp: { directory: 'csharp', extension: 'cs' },
    java: { directory: 'java', extension: 'java' },
    python: { directory: 'python', extension: 'py' },
    javascript: { directory: 'javascript', extension: 'js' },
    typescript: { directory: 'typescript', extension: 'ts' },
    go: { directory: 'go', extension: 'go' },
    ruby: { directory: 'ruby', extension: 'rb' },
    swift: { directory: 'swift', extension: 'swift' },
    kotlin: { directory: 'kotlin', extension: 'kt' },
    rust: { directory: 'rust', extension: 'rs' },
    scala: { directory: 'scala', extension: 'scala' },
    dart: { directory: 'dart', extension: 'dart' },
};

async function verify() {
    let failed = 0;
    let totalChecks = 0;

    for (const problem of PROBLEMS_SITE_DATA) {
        for (const language in languageMap) {
            if (problem[language] !== true) continue;

            totalChecks++;
            const { directory, extension } = languageMap[language];

            let strippedId = problem.code;
            if (/^\d+$/.test(problem.code)) {
                strippedId = parseInt(problem.code, 10).toString();
            }

            const folderName = `${strippedId}-${problem.link}`;
            const localPath = path.join(
                directory,
                folderName,
                `${problem.link}.${extension}`,
            );

            if (!fs.existsSync(localPath)) {
                console.log(`Missing Local File — ${localPath}`);
                failed++;
            }
        }
    }

    if (failed === 0) {
        console.log(`All ${totalChecks} local files verified successfully.`);
    } else {
        console.log(`\n${failed} file(s) missing out of ${totalChecks}`);
        process.exit(1);
    }
}

verify();

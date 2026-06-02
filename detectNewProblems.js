const fs = require('fs');
const { execSync } = require('child_process');

const VALID_DIRS = new Set([
    'c', 'cpp', 'csharp', 'java', 'python',
    'javascript', 'typescript', 'go', 'ruby',
    'swift', 'kotlin', 'rust', 'scala', 'dart'
]);

const directories = fs.readdirSync('./', { withFileTypes: true })
    .filter((d) => d.isDirectory())
    .map((d) => d.name)
    .filter((dir) => VALID_DIRS.has(dir));

let existing = [];
try {
    const raw = fs.readFileSync('./.problemSiteData.json', 'utf8').trim();
    existing = raw ? JSON.parse(raw) : [];
} catch {
    existing = [];
}

const existingCodes = new Set(existing.map((p) => p.code));

const newProblems = new Set();

function scanDir(dir) {
    if (!fs.existsSync(dir)) return;

    const entries = fs.readdirSync(dir, { withFileTypes: true });

    for (const entry of entries) {
        if (!entry.isDirectory()) continue;

        const match = entry.name.match(/^(\d+)-/);
        if (!match) continue;

        const code = String(parseInt(match[1])).padStart(4, '0');
        if (!existingCodes.has(code)) {
            newProblems.add(parseInt(match[1]));
        }
    }
}

// Scan root language dirs — cpp/, python/, etc.
for (const dir of directories) {
    scanDir(dir);
}

// Scan dcc/language dirs — dcc/cpp/, dcc/python/, etc.
for (const dir of directories) {
    scanDir(`dcc/${dir}`);
}

// Scan study_plan/leetcode75/language dirs — study_plan/leetcode75/cpp/, study_plan/leetcode75/python/, etc.
for (const dir of directories) {
    scanDir(`study_plan/leetcode75/${dir}`);
}

if (newProblems.size === 0) {
    console.log('No new problems detected');
    process.exit(0);
}

console.log(`Found ${newProblems.size} new problem(s) — fetching from LeetCode...\n`);

for (const problemNumber of [...newProblems].sort((a, b) => a - b)) {
    try {
        execSync(`node addProblem.js ${problemNumber}`, { stdio: 'inherit' });
    } catch (err) {
        console.error(`Failed to add problem #${problemNumber}`);
    }
}

console.log('\nDetection complete');
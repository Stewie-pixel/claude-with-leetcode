const { readdirSync } = require('fs');
const fs = require('fs');
const path = require('path');

const IGNORE_DIRS = ['.github', '.git', '.idea', '.vscode', 'node_modules'];

const FOLDER_TO_LANG = {
    javascript: {
        label: 'JavaScript',
        color: 'f7df1e',
        logo: 'javascript',
        logoColor: '000000',
    },
    typescript: {
        label: 'TypeScript',
        color: '3178c6',
        logo: 'typescript',
        logoColor: 'ffffff',
    },
    csharp: {
        label: 'C%23',
        color: '239120',
        logo: 'csharp',
        logoColor: 'ffffff',
    },
    c: { label: 'C', color: 'a8b9cc', logo: 'c', logoColor: '000000' },
    go: { label: 'Go', color: '00add8', logo: 'go', logoColor: 'ffffff' },
    java: {
        label: 'Java',
        color: 'ed8b00',
        logo: 'openjdk',
        logoColor: 'ffffff',
    },
    python: {
        label: 'Python',
        color: '3776ab',
        logo: 'python',
        logoColor: 'ffffff',
    },
    ruby: { label: 'Ruby', color: 'cc342d', logo: 'ruby', logoColor: 'ffffff' },
    rust: { label: 'Rust', color: '000000', logo: 'rust', logoColor: 'ffffff' },
    scala: {
        label: 'Scala',
        color: 'dc322f',
        logo: 'scala',
        logoColor: 'ffffff',
    },
    swift: {
        label: 'Swift',
        color: 'fa7343',
        logo: 'swift',
        logoColor: 'ffffff',
    },
    cpp: {
        label: 'C%2B%2B',
        color: '00599c',
        logo: 'cplusplus',
        logoColor: 'ffffff',
    },
    kotlin: {
        label: 'Kotlin',
        color: '7f52ff',
        logo: 'kotlin',
        logoColor: 'ffffff',
    },
    dart: { label: 'Dart', color: '0175c2', logo: 'dart', logoColor: 'ffffff' },
};

const FOLDER_TO_DISPLAY = {
    javascript: 'JS',
    typescript: 'TS',
    csharp: 'C#',
    c: 'C',
    go: 'GO',
    java: 'Java',
    python: 'Python',
    ruby: 'Ruby',
    rust: 'Rust',
    scala: 'Scala',
    swift: 'Swift',
    cpp: 'C++',
    kotlin: 'Kotlin',
    dart: 'Dart',
};

const VALID_DIRS = new Set([
    'c',
    'cpp',
    'csharp',
    'java',
    'python',
    'javascript',
    'typescript',
    'go',
    'ruby',
    'swift',
    'kotlin',
    'rust',
    'scala',
    'dart',
]);

const DIFFICULTY_ORDER = ['Easy', 'Medium', 'Hard'];
const DIFFICULTY_BADGE = {
    Easy: '🟢 Easy',
    Medium: '🟡 Medium',
    Hard: '🔴 Hard',
};

const PREPEND_PATH = process.argv[2] || './';
const TEMPLATE_PATH = process.argv[3] || './README_template.md';
const WRITE_PATH = process.argv[4] || './README.md';

const PROBLEM_SITE_DATA = JSON.parse(
    fs.readFileSync('./.problemSiteData.json', 'utf8'),
);

const directories = readdirSync(PREPEND_PATH, { withFileTypes: true })
    .filter((d) => d.isDirectory())
    .map((d) => d.name)
    .filter((dir) => VALID_DIRS.has(dir));

function generateLanguageBadges(dirs) {
    return dirs
        .filter((dir) => FOLDER_TO_LANG[dir])
        .map((dir) => {
            const { label, color, logo, logoColor } = FOLDER_TO_LANG[dir];
            const url = `https://img.shields.io/badge/${label}-${color}?logo=${logo}&logoColor=${logoColor}`;
            return `![${label}](${url})`;
        })
        .join('\n');
}

function* walkSync(dir) {
    const files = fs.readdirSync(dir, { withFileTypes: true });
    for (const file of files) {
        if (file.isDirectory()) {
            yield* walkSync(path.join(dir, file.name));
        } else {
            yield path.join(dir, file.name);
        }
    }
}

function nestedFiles(dir) {
    const files = [];
    const searchPaths = [
        dir,
        `dcc/${dir}`,
        `study_plan/leetcode75/${dir}`,
        `contest/weekly/${dir}`,
        `contest/biweekly/${dir}`
    ];
    for (const sPath of searchPaths) {
        if (fs.existsSync(sPath)) {
            for (const filePath of walkSync(sPath)) {
                files.push(filePath);
            }
        }
    }
    return files;
}

const nestedFilesInDir = directories.reduce((acc, dir) => {
    acc[dir] = nestedFiles(dir);
    return acc;
}, {});

function createProblemsObj(data) {
    const obj = {};
    for (const { problem, pattern, difficulty, link, code } of data) {
        const topics = pattern
            ? pattern.split(',').map((t) => t.trim())
            : ['Contest'];

        for (const topic of topics) {
            if (!(topic in obj)) obj[topic] = {};
            if (!(difficulty in obj[topic])) obj[topic][difficulty] = [];
            obj[topic][difficulty].push({
                name: problem,
                url: 'https://leetcode.com/problems/' + link,
                code: code.slice(0, 4),
            });
        }
    }
    return obj;
}

const PROBLEMS_OBJ = createProblemsObj(PROBLEM_SITE_DATA);

const langColumns = directories.map((d) => FOLDER_TO_DISPLAY[d] || d);

const tableHeader =
    ['Problem', 'Difficulty', ...langColumns]
        .map((el) => `<sub>${el}</sub>`)
        .join(' | ') + '\n';

const tableSep =
    Array.from({ length: tableHeader.split('|').length })
        .map(() => '----')
        .join(' | ') + '\n';

let completionTable = '';

const sortedTopics = Object.keys(PROBLEMS_OBJ).sort();

for (const topic of sortedTopics) {
    completionTable += `### ${topic}\n\n`;
    completionTable += tableHeader;
    completionTable += tableSep;

    for (const difficulty of DIFFICULTY_ORDER) {
        if (!(difficulty in PROBLEMS_OBJ[topic])) continue;

        for (const { name, url, code } of PROBLEMS_OBJ[topic][difficulty]) {
            let row = [
                `<sub>[${code} - ${name}](${url})</sub>`,
                `<sub>${DIFFICULTY_BADGE[difficulty]}</sub>`,
            ];

            for (const dir of directories) {
                const filePath = nestedFilesInDir[dir].find((file) => {
                    const normalizedPath = file.replace(/\\/g, '/');
                    const parts = normalizedPath.split('/');
                    const parentFolder = parts[parts.length - 2] || '';
                    const strippedCode = parseInt(code).toString();

                    return (
                        parentFolder.startsWith(`${code}-`) ||
                        parentFolder.startsWith(`${strippedCode}-`) ||
                        parentFolder === code ||
                        parentFolder === strippedCode
                    );
                });

                row.push(
                    filePath
                        ? `<sub><div align='center'>[✔️](${encodeURIComponent(filePath)})</div></sub>`
                        : `<sub><div align='center'>❌</div></sub>`,
                );
            }

            completionTable += row.join(' | ') + '\n';
        }
    }

    completionTable += '\n';
}

const template = fs.readFileSync(TEMPLATE_PATH, { encoding: 'utf8' });

const toWrite = template
    .replaceAll('<completion-tables />', completionTable)
    .replaceAll('<language-badges />', generateLanguageBadges(directories));

fs.writeFileSync(WRITE_PATH, toWrite, { encoding: 'utf8' });
console.log(
    `README updated — ${directories.length} languages detected: ${directories.join(', ')}`,
);

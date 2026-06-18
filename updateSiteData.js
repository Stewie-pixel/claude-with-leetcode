const fs = require('fs');
const path = require('path');
const https = require('https');

const DIFFICULTY_COLORS = {
    Easy: 'brightgreen',
    Medium: 'orange',
    Hard: 'red',
};

const languages = [
    { name: 'C', directory: 'c', extension: 'c' },
    { name: 'C++', directory: 'cpp', extension: 'cpp' },
    { name: 'C#', directory: 'csharp', extension: 'cs' },
    { name: 'Java', directory: 'java', extension: 'java' },
    { name: 'Python', directory: 'python', extension: 'py' },
    { name: 'JavaScript', directory: 'javascript', extension: 'js' },
    { name: 'TypeScript', directory: 'typescript', extension: 'ts' },
    { name: 'Go', directory: 'go', extension: 'go' },
    { name: 'Ruby', directory: 'ruby', extension: 'rb' },
    { name: 'Swift', directory: 'swift', extension: 'swift' },
    { name: 'Kotlin', directory: 'kotlin', extension: 'kt' },
    { name: 'Rust', directory: 'rust', extension: 'rs' },
    { name: 'Scala', directory: 'scala', extension: 'scala' },
    { name: 'Dart', directory: 'dart', extension: 'dart' },
];

let PROBLEMS_SITE_DATA = [];
try {
    PROBLEMS_SITE_DATA = JSON.parse(
        fs.readFileSync('./.problemSiteData.json', 'utf8'),
    );
} catch (e) {
    PROBLEMS_SITE_DATA = [];
}

const delay = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

function fetchLeetcodeQuestion(titleSlug) {
    return new Promise((resolve, reject) => {
        const query = `
            query questionData($titleSlug: String!) {
                question(titleSlug: $titleSlug) {
                    questionFrontendId
                    title
                    titleSlug
                    difficulty
                    content
                    topicTags {
                        name
                    }
                }
            }
        `;
        const body = JSON.stringify({ query, variables: { titleSlug } });
        const options = {
            hostname: 'leetcode.com',
            path: '/graphql',
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
                'Content-Length': Buffer.byteLength(body),
                'User-Agent':
                    'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
                Referer: 'https://leetcode.com',
            },
        };

        const req = https.request(options, (res) => {
            let data = '';
            res.on('data', (chunk) => (data += chunk));
            res.on('end', () => {
                try {
                    const result = JSON.parse(data);
                    if (result.errors) {
                        reject(new Error(result.errors[0].message));
                    } else {
                        resolve(result.data?.question || null);
                    }
                } catch (e) {
                    reject(e);
                }
            });
        });

        req.on('error', reject);
        req.write(body);
        req.end();
    });
}

async function run() {
    let modified = false;

    for (const problem of PROBLEMS_SITE_DATA) {
        for (const lang of languages) {
            if (problem[lang.directory] !== undefined) {
                delete problem[lang.directory];
                modified = true;
            }
        }
    }

    const scanPaths = [];
    for (const lang of languages) {
        scanPaths.push({
            path: lang.directory,
            langKey: lang.directory,
            extension: lang.extension,
        });

        scanPaths.push({
            path: `dcc/${lang.directory}`,
            langKey: lang.directory,
            extension: lang.extension,
        });

        scanPaths.push({
            path: `study_plan/leetcode75/${lang.directory}`,
            langKey: lang.directory,
            extension: lang.extension,
        });

        scanPaths.push({
            path: `contest/weekly/${lang.directory}`,
            langKey: lang.directory,
            extension: lang.extension,
        });

        scanPaths.push({
            path: `contest/biweekly/${lang.directory}`,
            langKey: lang.directory,
            extension: lang.extension,
        });
    }

    for (const scanConfig of scanPaths) {
        const { path: langDir, langKey, extension } = scanConfig;
        if (!fs.existsSync(langDir)) continue;

        const entries = fs.readdirSync(langDir, { withFileTypes: true });
        for (const entry of entries) {
            if (!entry.isDirectory()) continue;

            const match = entry.name.match(/^(\d+)-(.*)$/);
            if (!match) continue;

            const folderNumber = parseInt(match[1], 10);
            const folderSlug = match[2];
            const originalFolderName = entry.name;
            let currentFolderPath = path.join(langDir, originalFolderName);

            const readmePath = path.join(currentFolderPath, 'README.md');
            let titleSlug = folderSlug;
            let readmeContent = '';
            if (fs.existsSync(readmePath)) {
                readmeContent = fs.readFileSync(readmePath, 'utf8');
                const slugMatch = readmeContent.match(
                    /leetcode\.com\/problems\/([a-zA-Z0-9-]+)/,
                );
                if (slugMatch) {
                    titleSlug = slugMatch[1];
                }
            }

            let problemEntry = PROBLEMS_SITE_DATA.find(
                (p) => p.link === titleSlug,
            );

            if (!problemEntry) {
                const code = String(folderNumber).padStart(4, '0');
                const matchingCodeEntry = PROBLEMS_SITE_DATA.find(
                    (p) => p.code === code,
                );
                if (
                    matchingCodeEntry &&
                    (!matchingCodeEntry.link ||
                        matchingCodeEntry.link === titleSlug)
                ) {
                    problemEntry = matchingCodeEntry;
                }
            }

            let fetchedQuestion = null;
            if (!problemEntry) {
                console.log(
                    `\nNew problem detected: #${folderNumber} (${titleSlug}) in ${langDir}`,
                );

                try {
                    await delay(1000);
                    fetchedQuestion = await fetchLeetcodeQuestion(titleSlug);
                } catch (e) {
                    console.error(
                        `Failed to fetch LeetCode details for ${titleSlug}:`,
                        e.message,
                    );
                }

                let title = titleSlug
                    .split('-')
                    .map((w) => w.charAt(0).toUpperCase() + w.slice(1))
                    .join(' ');
                let difficulty = 'Medium';
                let pattern = 'Contest';
                let content = '';

                if (fetchedQuestion) {
                    title = fetchedQuestion.title;
                    difficulty = fetchedQuestion.difficulty;
                    pattern =
                        fetchedQuestion.topicTags
                            ?.map((t) => t.name)
                            .join(', ') || 'Contest';
                    content = fetchedQuestion.content;
                } else if (readmeContent) {
                    const titleMatch = readmeContent.match(
                        /<h2><a href="[^"]+">([^<]+)<\/a><\/h2>/,
                    );
                    if (titleMatch) title = titleMatch[1].trim();

                    const diffMatch =
                        readmeContent.match(
                            /alt=['"]Difficulty:\s*([a-zA-Z]+)['"]/i,
                        ) || readmeContent.match(/Difficulty-([a-zA-Z]+)-/i);
                    if (diffMatch) {
                        difficulty =
                            diffMatch[1].charAt(0).toUpperCase() +
                            diffMatch[1].slice(1);
                    }
                }

                problemEntry = {
                    problem: title,
                    pattern: pattern,
                    difficulty: difficulty,
                    link: titleSlug,
                    code: fetchedQuestion
                        ? String(fetchedQuestion.questionFrontendId).padStart(
                              4,
                              '0',
                          )
                        : code,
                };

                PROBLEMS_SITE_DATA.push(problemEntry);
                modified = true;
                console.log(
                    `Added new problem entry: [${problemEntry.code}] ${problemEntry.problem}`,
                );
            }

            if (problemEntry[langKey] !== true) {
                problemEntry[langKey] = true;
                modified = true;
                console.log(
                    `Marked [${problemEntry.code}] ${problemEntry.problem} as solved in ${langKey}`,
                );
            }

            if (!fs.existsSync(readmePath) && fetchedQuestion) {
                const difficulty = problemEntry.difficulty;
                const color = DIFFICULTY_COLORS[difficulty] || 'lightgrey';
                const badgeUrl = `https://img.shields.io/badge/Difficulty-${difficulty}-${color}`;
                const fileContent = `<h2><a href="https://leetcode.com/problems/${titleSlug}">${problemEntry.problem}</a></h2> <img src='${badgeUrl}' alt='Difficulty: ${difficulty}' /><hr>${fetchedQuestion.content || '<p>Description not available.</p>'}\n`;
                fs.writeFileSync(readmePath, fileContent, 'utf8');
                console.log(
                    `Generated missing README.md for ${problemEntry.problem}`,
                );
            }

            const extFiles = fs
                .readdirSync(currentFolderPath)
                .filter((f) => f.endsWith(`.${extension}`));
            const expectedFileName = `${titleSlug}.${extension}`;
            if (extFiles.length > 0 && !extFiles.includes(expectedFileName)) {
                let fileToRename = null;
                if (extFiles.length === 1) {
                    fileToRename = extFiles[0];
                } else {
                    fileToRename = extFiles.find(
                        (f) =>
                            f.startsWith(`${folderNumber}-`) ||
                            f.startsWith('solution') ||
                            f.toLowerCase().includes(titleSlug),
                    );
                }

                if (fileToRename) {
                    fs.renameSync(
                        path.join(currentFolderPath, fileToRename),
                        path.join(currentFolderPath, expectedFileName),
                    );
                    console.log(
                        `Renamed code file in ${currentFolderPath}: ${fileToRename} -> ${expectedFileName}`,
                    );
                }
            }

            const strippedId = parseInt(problemEntry.code, 10).toString();
            const standardFolderName = `${strippedId}-${titleSlug}`;
            if (originalFolderName !== standardFolderName) {
                const newFolderPath = path.join(langDir, standardFolderName);
                if (!fs.existsSync(newFolderPath)) {
                    fs.renameSync(currentFolderPath, newFolderPath);
                    console.log(
                        `Renamed folder: ${originalFolderName} -> ${standardFolderName}`,
                    );
                    currentFolderPath = newFolderPath;
                } else {
                    console.warn(
                        `Warning: Standard folder path ${newFolderPath} already exists. Skipping folder rename.`,
                    );
                }
            }
        }
    }

    if (modified) {
        PROBLEMS_SITE_DATA.sort((a, b) => a.code.localeCompare(b.code));
        fs.writeFileSync(
            './.problemSiteData.json',
            JSON.stringify(PROBLEMS_SITE_DATA, null, 2),
            'utf8',
        );
        console.log('\n.problemSiteData.json updated and sorted successfully.');
    } else {
        console.log('\nNo changes needed for .problemSiteData.json');
    }
}

run().catch((e) => {
    console.error('Execution failed:', e);
    process.exit(1);
});

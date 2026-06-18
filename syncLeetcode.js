const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');

if (fs.existsSync('.env')) {
    const envFile = fs.readFileSync('.env', 'utf8');
    for (const line of envFile.split('\n')) {
        const trimmed = line.trim();
        if (!trimmed || trimmed.startsWith('#')) continue;
        const parts = trimmed.split('=');
        if (parts.length >= 2) {
            const key = parts[0].trim();
            const value = parts
                .slice(1)
                .join('=')
                .trim()
                .replace(/^['"]|['"]$/g, '');
            process.env[key] = value;
        }
    }
}

const LEETCODE_SESSION = process.env.LEETCODE_SESSION;
const csrftoken =
    process.env.LEETCODE_CSRF_TOKEN || process.env.LEETCODE_CSRF || '';
const limit = parseInt(process.env.SYNC_LIMIT || '20', 10);

if (!LEETCODE_SESSION) {
    console.error('Error: LEETCODE_SESSION is required.');
    console.error(
        'Please set it in your environment or in a .env file in the workspace root.',
    );
    process.exit(1);
}

const LANG_MAP = {
    cpp: { dir: 'cpp', ext: 'cpp' },
    c: { dir: 'c', ext: 'c' },
    csharp: { dir: 'csharp', ext: 'cs' },
    java: { dir: 'java', ext: 'java' },
    python: { dir: 'python', ext: 'py' },
    python3: { dir: 'python', ext: 'py' },
    javascript: { dir: 'javascript', ext: 'js' },
    typescript: { dir: 'typescript', ext: 'ts' },
    golang: { dir: 'go', ext: 'go' },
    ruby: { dir: 'ruby', ext: 'rb' },
    swift: { dir: 'swift', ext: 'swift' },
    kotlin: { dir: 'kotlin', ext: 'kt' },
    rust: { dir: 'rust', ext: 'rs' },
    scala: { dir: 'scala', ext: 'scala' },
    dart: { dir: 'dart', ext: 'dart' },
};

const DIFFICULTY_COLORS = {
    Easy: 'brightgreen',
    Medium: 'orange',
    Hard: 'red',
};

const headers = {
    'content-type': 'application/json',
    'user-agent':
        'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
    referer: 'https://leetcode.com',
};

const cookies = [`LEETCODE_SESSION=${LEETCODE_SESSION}`];
if (csrftoken) {
    cookies.push(`csrftoken=${csrftoken}`);
    headers['x-csrftoken'] = csrftoken;
}
headers['cookie'] = cookies.join('; ');

const delay = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

async function fetchLeetCode(query, variables = {}) {
    const response = await fetch('https://leetcode.com/graphql', {
        method: 'POST',
        headers,
        body: JSON.stringify({ query, variables }),
    });
    if (!response.ok) {
        throw new Error(
            `LeetCode GraphQL request failed: HTTP ${response.status}`,
        );
    }
    const json = await response.json();
    if (json.errors) {
        throw new Error(
            `LeetCode GraphQL returned errors: ${JSON.stringify(json.errors)}`,
        );
    }
    return json.data;
}

async function getUsername() {
    console.log('Verifying LeetCode session...');
    const query = `
        query globalData {
            userStatus {
                isSignedIn
                username
            }
        }
    `;
    const data = await fetchLeetCode(query);
    if (!data.userStatus || !data.userStatus.isSignedIn) {
        throw new Error(
            'User is not signed in. Check your LEETCODE_SESSION cookie.',
        );
    }
    console.log(
        `Session verified successfully. Logged in as: ${data.userStatus.username}`,
    );
    return data.userStatus.username;
}

async function getRecentSubmissions(username) {
    console.log(
        `Fetching recent submissions for ${username} (limit: ${limit})...`,
    );
    const query = `
        query recentSubmissionList($username: String!, $limit: Int) {
            recentSubmissionList(username: $username, limit: $limit) {
                id
                title
                titleSlug
                lang
                statusDisplay
                timestamp
            }
        }
    `;
    const data = await fetchLeetCode(query, { username, limit });
    return data.recentSubmissionList || [];
}

async function getSubmissionDetails(submissionId) {
    const query = `
        query submissionDetails($submissionId: Int!) {
            submissionDetails(submissionId: $submissionId) {
                code
                lang {
                    name
                }
                question {
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
        }
    `;
    const data = await fetchLeetCode(query, { submissionId });
    return data.submissionDetails;
}

function isAlreadySynced(langDir, titleSlug) {
    if (!fs.existsSync(langDir)) return false;
    const items = fs.readdirSync(langDir, { withFileTypes: true });
    for (const item of items) {
        if (
            item.isDirectory() &&
            (item.name.endsWith(`-${titleSlug}`) || item.name === titleSlug)
        ) {
            return true;
        }
    }
    return false;
}

async function run() {
    try {
        const username = await getUsername();
        const submissions = await getRecentSubmissions(username);

        const acceptedSubmissions = submissions.filter(
            (s) => s.statusDisplay === 'Accepted',
        );

        console.log(
            `Found ${acceptedSubmissions.length} accepted submission(s) in the list.`,
        );

        let newProblemsSynced = 0;

        let problemSiteData = [];
        const metadataPath = './.problemSiteData.json';
        try {
            if (fs.existsSync(metadataPath)) {
                problemSiteData = JSON.parse(
                    fs.readFileSync(metadataPath, 'utf8'),
                );
            }
        } catch (err) {
            console.warn(
                'Warning: Failed to parse .problemSiteData.json, starting fresh',
            );
            problemSiteData = [];
        }

        for (const sub of acceptedSubmissions) {
            const mappedLang = LANG_MAP[sub.lang];
            if (!mappedLang) {
                console.log(
                    `Language '${sub.lang}' for '${sub.title}' is not mapped or supported. Skipping.`,
                );
                continue;
            }

            if (isAlreadySynced(mappedLang.dir, sub.titleSlug)) {
                console.log(
                    `- '${sub.title}' in '${mappedLang.dir}' is already synced. Skipping details fetch.`,
                );
                continue;
            }

            console.log(
                `Syncing new solution: '${sub.title}' (${mappedLang.dir})...`,
            );

            await delay(1500);

            let details;
            try {
                details = await getSubmissionDetails(parseInt(sub.id, 10));
            } catch (err) {
                console.error(
                    `Failed to fetch details for submission ${sub.id}:`,
                    err.message,
                );
                continue;
            }

            if (!details || !details.code || !details.question) {
                console.error(
                    `Missing code or question details in submission ${sub.id}`,
                );
                continue;
            }

            const { question, code: submissionCode } = details;
            const titleSlug = question.titleSlug;
            const frontendId = question.questionFrontendId;

            let strippedId = frontendId;
            if (/^\d+$/.test(frontendId)) {
                strippedId = parseInt(frontendId, 10).toString();
            }

            const paddedId = String(frontendId).padStart(4, '0');
            const folderName = `${strippedId}-${titleSlug}`;
            const targetDir = path.join(mappedLang.dir, folderName);

            if (!fs.existsSync(targetDir)) {
                fs.mkdirSync(targetDir, { recursive: true });
            }

            const codeFilePath = path.join(
                targetDir,
                `${titleSlug}.${mappedLang.ext}`,
            );
            fs.writeFileSync(codeFilePath, submissionCode, 'utf8');
            console.log(`Saved code to: ${codeFilePath}`);

            const readmeFilePath = path.join(targetDir, 'README.md');
            const color = DIFFICULTY_COLORS[question.difficulty] || 'lightgrey';
            const badgeUrl = `https://img.shields.io/badge/Difficulty-${question.difficulty}-${color}`;
            const readmeContent = `<h2><a href="https://leetcode.com/problems/${titleSlug}">${question.title}</a></h2> <img src='${badgeUrl}' alt='Difficulty: ${question.difficulty}' /><hr>${question.content || '<p>Description not available.</p>'}\n`;

            fs.writeFileSync(readmeFilePath, readmeContent, 'utf8');
            console.log(`Saved README to: ${readmeFilePath}`);

            let existingEntry = problemSiteData.find(
                (p) => p.link === titleSlug,
            );

            if (!existingEntry) {
                const pattern =
                    question.topicTags?.map((t) => t.name).join(', ') ||
                    'Contest';
                existingEntry = {
                    problem: question.title,
                    pattern: pattern,
                    difficulty: question.difficulty,
                    link: titleSlug,
                    code: paddedId,
                };
                problemSiteData.push(existingEntry);
                problemSiteData.sort((a, b) => a.code.localeCompare(b.code));
                console.log(
                    `Added entry to metadata: [${paddedId}] ${question.title}`,
                );
            }

            existingEntry[mappedLang.dir] = true;

            newProblemsSynced++;
        }

        if (newProblemsSynced > 0) {
            fs.writeFileSync(
                metadataPath,
                JSON.stringify(problemSiteData, null, 2),
                'utf8',
            );
            console.log(
                `\nSuccessfully synced ${newProblemsSynced} solution(s).`,
            );
        } else {
            console.log(
                '\nNo new solutions found to sync. Everything is up to date.',
            );
        }

        try {
            console.log('\nRunning updateSiteData.js...');
            execSync('node updateSiteData.js', { stdio: 'inherit' });

            console.log('\nRunning updateTable.js...');
            execSync('node updateTable.js', { stdio: 'inherit' });

            console.log('\nREADME table rebuild completed successfully.');
        } catch (err) {
            console.error('\nFailed to run update scripts:', err.message);
            process.exit(1);
        }
    } catch (err) {
        console.error('\nExecution failed:', err.message);
        process.exit(1);
    }
}

run();

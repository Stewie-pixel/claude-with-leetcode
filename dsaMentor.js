const fs = require('fs');
const path = require('path');
const https = require('https');
const { execSync } = require('child_process');

const OPENROUTER_API_KEY = process.env.OPENROUTER_API_KEY;
const SERPER_API_KEY = process.env.SERPER_API_KEY;

if (!OPENROUTER_API_KEY) {
    console.error('Missing OPENROUTER_API_KEY environment variable.');
    process.exit(1);
}
if (!SERPER_API_KEY) {
    console.error('Missing SERPER_API_KEY environment variable.');
    process.exit(1);
}

function httpsPost(hostname, urlPath, headers, body) {
    return new Promise((resolve, reject) => {
        const data = JSON.stringify(body);
        const options = {
            hostname,
            port: 443,
            path: urlPath,
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
                'Content-Length': Buffer.byteLength(data),
                ...headers,
            },
        };

        const req = https.request(options, (res) => {
            let responseBody = '';
            res.on('data', (chunk) => (responseBody += chunk));
            res.on('end', () => {
                try {
                    resolve(JSON.parse(responseBody));
                } catch (e) {
                    reject(
                        new Error(`Failed to parse response: ${responseBody}`),
                    );
                }
            });
        });

        req.on('error', reject);
        req.write(data);
        req.end();
    });
}

async function searchYoutube(problemNumber, problemName) {
    const queries = [
        `leetcode ${problemNumber} ${problemName} solution`,
        `neetcode ${problemNumber} ${problemName}`,
        `leetcode ${problemName} solution explanation`,
    ];

    for (const query of queries) {
        console.log(`Searching YouTube for: ${query}`);

        try {
            const data = await httpsPost(
                'google.serper.dev',
                '/videos',
                { 'X-API-KEY': SERPER_API_KEY },
                { q: query, num: 5 },
            );

            const youtubeLinks = (data.videos || [])
                .filter((r) => r.link?.includes('youtube.com/watch'))
                .filter((r) => {
                    const title = r.title?.toLowerCase() ?? '';
                    return (
                        title.includes(problemNumber) ||
                        title.includes(problemName.toLowerCase().split(' ')[0])
                    );
                })
                .map((r) => `- ${r.title} → ${r.link}`)
                .join('\n');

            if (youtubeLinks) {
                console.log(`Found specific results:\n${youtubeLinks}`);
                return youtubeLinks;
            }

            console.warn(
                `No specific results for query: "${query}", trying next...`,
            );
        } catch (err) {
            console.error('Serper search failed:', err.message);
        }
    }

    console.warn('No specific YouTube video found for this problem.');
    return null;
}

function extractProblemMeta(readmeContent) {
    const plainText = readmeContent.replace(/<[^>]+>/g, '');
    const match = plainText.match(/(\d+)[.\-\s]+([A-Za-z][^\n]{3,60})/m);

    return {
        number: match?.[1]?.trim() ?? '',
        name: match?.[2]?.trim() ?? '',
    };
}

async function askOpenRouter(systemPrompt, userContent, retries = 3) {
    for (let attempt = 1; attempt <= retries; attempt++) {
        try {
            const response = await httpsPost(
                'openrouter.ai',
                '/api/v1/chat/completions',
                { Authorization: `Bearer ${OPENROUTER_API_KEY}` },
                {
                    model: 'nvidia/nemotron-3-super-120b-a12b:free',
                    messages: [
                        { role: 'system', content: systemPrompt },
                        { role: 'user', content: userContent },
                    ],
                },
            );

            if (response.choices?.[0]?.message) {
                return response.choices[0].message.content;
            }

            const errMsg = JSON.stringify(response);
            console.warn(`Attempt ${attempt} failed: ${errMsg}`);

            if (attempt < retries) {
                const delay = attempt * 5000;
                console.log(`Retrying in ${delay / 1000}s...`);
                await new Promise((r) => setTimeout(r, delay));
            } else {
                throw new Error(
                    `OpenRouter failed after ${retries} attempts: ${errMsg}`,
                );
            }
        } catch (err) {
            if (attempt < retries) {
                const delay = attempt * 5000;
                console.warn(
                    `Attempt ${attempt} error: ${err.message}. Retrying in ${delay / 1000}s...`,
                );
                await new Promise((r) => setTimeout(r, delay));
            } else {
                throw err;
            }
        }
    }
}

async function runMentor() {
    const targetDirs = new Set();
    const supportedLangs = [
        'cpp',
        'python',
        'javascript',
        'java',
        'typescript',
        'c',
        'csharp',
        'go',
        'ruby',
        'swift',
        'kotlin',
        'rust',
        'scala',
        'dart',
    ];

    let recentCommits = [];
    try {
        const commitLogRaw = execSync('git log --format="%H" -n 10', {
            encoding: 'utf8',
        });
        recentCommits = commitLogRaw.trim().split('\n').filter(Boolean);
    } catch (err) {
        console.error('Failed to read git log:', err.message);
        return;
    }

    for (const commitHash of recentCommits) {
        try {
            const gitLog = execSync(
                `git diff-tree --no-commit-id --name-only -r ${commitHash}`,
                { encoding: 'utf8' },
            );

            gitLog.split('\n').forEach((line) => {
                const filePath = line.trim();
                const parts = filePath.split('/');
                if (parts.length >= 3 && supportedLangs.includes(parts[0])) {
                    targetDirs.add(path.join(parts[0], parts[1]));
                }
            });

            if (targetDirs.size > 0) {
                console.log(`Found solution files in commit: ${commitHash}`);
                break;
            }
        } catch (err) {
            console.error(
                `Failed to read diff for commit ${commitHash}:`,
                err.message,
            );
        }
    }

    if (targetDirs.size === 0) {
        console.log('No new solution folders found. Skipping.');
        return;
    }

    const basePrompt = fs.readFileSync('./CLAUDE.md', 'utf8');
    const skillPrompt = fs.readFileSync('./skills/dsa-mentor.md', 'utf8');
    const combinedSystemPrompt = `${basePrompt}\n\n${skillPrompt}`;

    for (const dir of targetDirs) {
        const readmePath = path.join(dir, 'README.md');
        if (!fs.existsSync(readmePath)) continue;

        const files = fs.readdirSync(dir);
        const codeFile = files.find(
            (f) => f !== 'README.md' && f !== 'ANALYSIS.md',
        );
        if (!codeFile) continue;

        console.log(`Analyzing solution in: ${dir}...`);

        const problemDescription = fs.readFileSync(readmePath, 'utf8');
        const sourceCode = fs.readFileSync(path.join(dir, codeFile), 'utf8');

        const { number, name } = extractProblemMeta(problemDescription);
        console.log(`Problem detected: #${number} ${name}`);

        const youtubeResults =
            number && name ? await searchYoutube(number, name) : null;

        const searchContext = youtubeResults
            ? `\n\n---\nReal YouTube search results (pick the most relevant watch URL, prefer NeetCode):\n${youtubeResults}\n---\n`
            : '\n\n---\nNo YouTube results found — omit the Video Solution section.\n---\n';

        const userContent = [
            `Problem Description:\n${problemDescription}`,
            `My Solution Code:\n\`\`\`\n${sourceCode}\n\`\`\``,
            searchContext,
        ].join('\n\n');

        try {
            const analysis = await askOpenRouter(
                combinedSystemPrompt,
                userContent,
            );
            const analysisPath = path.join(dir, 'ANALYSIS.md');
            fs.writeFileSync(analysisPath, analysis, 'utf8');
            console.log(`Saved analysis to: ${analysisPath}`);
        } catch (error) {
            console.error(
                `Failed to generate analysis for ${dir}:`,
                error.message,
            );
        }
    }
}

runMentor();

const fs = require('fs');
const path = require('path');
const https = require('https');
const { execSync } = require('child_process');

const OPENROUTER_API_KEY = process.env.OPENROUTER_API_KEY;
if (!OPENROUTER_API_KEY) {
    console.error('Missing OPENROUTER_API_KEY environment variable.');
    process.exit(1);
}

function askOpenRouter(systemPrompt, userContent) {
    return new Promise((resolve, reject) => {
        const data = JSON.stringify({
            model: 'nvidia/nemotron-3-super-120b-a12b:free',
            messages: [
                { role: 'system', content: systemPrompt },
                { role: 'user', content: userContent },
            ],
        });

        const options = {
            hostname: 'openrouter.ai',
            port: 443,
            path: '/api/v1/chat/completions',
            method: 'POST',
            headers: {
                Authorization: `Bearer ${OPENROUTER_API_KEY}`,
                'Content-Type': 'application/json',
            },
        };

        const req = https.request(options, (res) => {
            let body = '';
            res.on('data', (chunk) => (body += chunk));
            res.on('end', () => {
                try {
                    const response = JSON.parse(body);
                    if (
                        response.choices &&
                        response.choices[0] &&
                        response.choices[0].message
                    ) {
                        resolve(response.choices[0].message.content);
                    } else {
                        reject(
                            new Error(
                                `Unexpected OpenRouter response structure: ${body}`,
                            ),
                        );
                    }
                } catch (e) {
                    reject(e);
                }
            });
        });

        req.on('error', (e) => reject(e));
        req.write(data);
        req.end();
    });
}

async function runMentor() {
    let gitLog = '';
    try {
        gitLog = execSync('git diff-tree --no-commit-id --name-only -r HEAD', {
            encoding: 'utf8',
        });
    } catch (err) {
        console.error('Failed to read git commit diff:', err.message);
        return;
    }

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

    gitLog.split('\n').forEach((line) => {
        const filePath = line.trim();
        const parts = filePath.split('/');
        if (parts.length >= 3 && supportedLangs.includes(parts[0])) {
            targetDirs.add(path.join(parts[0], parts[1]));
        }
    });

    if (targetDirs.size === 0) {
        console.log(
            'No new solution folders found in the last commit. Skipping analysis.',
        );
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

        console.log(`Claude Mentor is analyzing the solution in: ${dir}...`);

        const problemDescription = fs.readFileSync(readmePath, 'utf8');
        const sourceCode = fs.readFileSync(path.join(dir, codeFile), 'utf8');
        const userContent = `Problem Description:\n${problemDescription}\n\nMy Solution Code:\n\`\`\`\n${sourceCode}\n\`\`\``;

        try {
            const analysis = await askOpenRouter(
                combinedSystemPrompt,
                userContent,
            );
            const analysisPath = path.join(dir, 'ANALYSIS.md');
            fs.writeFileSync(analysisPath, analysis, 'utf8');
            console.log(`Saved mentor analysis to: ${analysisPath}`);
        } catch (error) {
            console.error(
                `Failed to generate analysis for ${dir}:`,
                error.message,
            );
        }
    }
}

runMentor();

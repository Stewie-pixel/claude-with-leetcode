const https = require('https');
const fs = require('fs');

const problemNumber = process.argv[2];

if (!problemNumber || isNaN(problemNumber)) {
    console.error('Usage: node addProblem.js <problem-number>');
    console.error('Example: node addProblem.js 1');
    process.exit(1);
}

const searchQuery = JSON.stringify({
    query: `
        query problemsetQuestionList($skip: Int) {
            problemsetQuestionList: questionList(
                categorySlug: ""
                limit: 1
                skip: $skip
                filters: {}
            ) {
                questions: data {
                    frontendQuestionId: questionFrontendId
                    title
                    titleSlug
                    difficulty
                    topicTags { name }
                }
            }
        }
    `,
    variables: { skip: parseInt(problemNumber) - 1 },
});

const options = {
    hostname: 'leetcode.com',
    path: '/graphql',
    method: 'POST',
    headers: {
        'Content-Type': 'application/json',
        'Content-Length': Buffer.byteLength(searchQuery),
    },
};

console.log(`Fetching problem #${problemNumber} from LeetCode...`);

const req = https.request(options, (res) => {
    let data = '';
    res.on('data', (chunk) => (data += chunk));
    res.on('end', () => {
        try {
            const json = JSON.parse(data);
            const question = json.data?.problemsetQuestionList?.questions?.[0];

            if (!question) {
                console.error(
                    `Problem #${problemNumber} not found on LeetCode`,
                );
                process.exit(1);
            }

            const code = String(question.frontendQuestionId).padStart(4, '0');

            const pattern = question.topicTags?.[0]?.name || 'Uncategorized';

            const entry = {
                problem: question.title,
                pattern: pattern,
                difficulty: question.difficulty,
                link: question.titleSlug,
                code: code,
            };

            let existing = [];
            try {
                const raw = fs
                    .readFileSync('./.problemSiteData.json', 'utf8')
                    .trim();
                existing = raw ? JSON.parse(raw) : [];
            } catch {
                existing = [];
            }

            if (existing.find((p) => p.code === code)) {
                console.log(
                    `Problem #${problemNumber} already exists — skipping`,
                );
                process.exit(0);
            }

            existing.push(entry);
            existing.sort((a, b) => a.code.localeCompare(b.code));

            fs.writeFileSync(
                './.problemSiteData.json',
                JSON.stringify(existing, null, 2),
            );

            console.log(
                `Added: [${code}] ${question.title} — ${question.difficulty} — ${pattern}`,
            );
        } catch (err) {
            console.error('Failed to parse LeetCode response:', err.message);
            process.exit(1);
        }
    });
});

req.on('error', (err) => {
    console.error('Request failed:', err.message);
    process.exit(1);
});

req.write(searchQuery);
req.end();

const fs   = require('fs');
const https = require('https');

const PROBLEMS_SITE_DATA = JSON.parse(
    fs.readFileSync('./.problemSiteData.json', 'utf8'),
);

const GITHUB_BASE_URL = 'https://github.com/Stewie-pixel/claude-with-leetcode/blob/main';

const languageMap = {
    c:          { directory: 'c',         extension: 'c'     },
    cpp:        { directory: 'cpp',       extension: 'cpp'   },
    csharp:     { directory: 'csharp',    extension: 'cs'    },
    java:       { directory: 'java',      extension: 'java'  },
    python:     { directory: 'python',    extension: 'py'    },
    javascript: { directory: 'javascript',extension: 'js'    },
    typescript: { directory: 'typescript',extension: 'ts'    },
    go:         { directory: 'go',        extension: 'go'    },
    ruby:       { directory: 'ruby',      extension: 'rb'    },
    swift:      { directory: 'swift',     extension: 'swift' },
    kotlin:     { directory: 'kotlin',    extension: 'kt'    },
    rust:       { directory: 'rust',      extension: 'rs'    },
    scala:      { directory: 'scala',     extension: 'scala' },
    dart:       { directory: 'dart',      extension: 'dart'  },
};

function checkUrl(url) {
    return new Promise((resolve) => {
        https.get(url, (res) => {
            resolve({ url, status: res.statusCode });
            res.resume();
        }).on('error', () => {
            resolve({ url, status: 'ERROR' });
        });
    });
}

async function verify() {
    const checks = [];

    for (const problem of PROBLEMS_SITE_DATA) {
        for (const language in languageMap) {
            if (problem[language] !== true) continue;

            const { directory, extension } = languageMap[language];
            const url = `${GITHUB_BASE_URL}/${directory}/${problem.code}.${extension}`;
            checks.push(checkUrl(url));
        }
    }

    const results = await Promise.all(checks);
    let failed = 0;

    for (const { url, status } of results) {
        if (status !== 200) {
            console.log(`❌ ${status} — ${url}`);
            failed++;
        }
    }

    if (failed === 0) {
        console.log(`All ${results.length} URLs verified successfully`);
    } else {
        console.log(`\n${failed} URL(s) failed out of ${results.length}`);
        process.exit(1);
    }
}

verify();
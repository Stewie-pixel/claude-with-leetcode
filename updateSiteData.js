const fs = require('fs');

const PROBLEMS_SITE_DATA = JSON.parse(
    fs.readFileSync('./.problemSiteData.json', 'utf8'),
);

const languages = [
    { name: 'C',          directory: 'c',           extension: 'c'     },
    { name: 'C++',        directory: 'cpp',         extension: 'cpp'   },
    { name: 'C#',         directory: 'csharp',      extension: 'cs'    },
    { name: 'Java',       directory: 'java',        extension: 'java'  },
    { name: 'Python',     directory: 'python',      extension: 'py'    },
    { name: 'JavaScript', directory: 'javascript',  extension: 'js'    },
    { name: 'TypeScript', directory: 'typescript',  extension: 'ts'    },
    { name: 'Go',         directory: 'go',          extension: 'go'    },
    { name: 'Ruby',       directory: 'ruby',        extension: 'rb'    },
    { name: 'Swift',      directory: 'swift',       extension: 'swift' },
    { name: 'Kotlin',     directory: 'kotlin',      extension: 'kt'    },
    { name: 'Rust',       directory: 'rust',        extension: 'rs'    },
    { name: 'Scala',      directory: 'scala',       extension: 'scala' },
    { name: 'Dart',       directory: 'dart',        extension: 'dart'  },
];

for (const lang of languages) {
    const { name, directory, extension } = lang;

    // Skip if directory doesn't exist
    if (!fs.existsSync(directory)) {
        console.log(`Skipping ${name} — directory '${directory}' not found`);
        continue;
    }

    const files = fs.readdirSync(directory, { withFileTypes: true });
    console.log(`\n${name} — ${files.length} files found`);

    let renamed = 0;

    for (const problem of PROBLEMS_SITE_DATA) {
        const problemName = problem['link'].replace('/', '').toLowerCase();
        const problemNumber = problem['code'].split('-')[0];

        // Find file by problem number prefix
        const foundFile = files.find((file) =>
            file.name.startsWith(`${problemNumber}-`),
        );

        if (foundFile && foundFile.isFile()) {
            const oldFile = `${directory}/${foundFile.name}`;
            const newFile = `${directory}/${problemNumber}-${problemName}.${extension}`;

            // Rename if needed
            if (oldFile !== newFile) {
                fs.renameSync(oldFile, newFile);
                renamed++;
                console.log(`Renamed: ${foundFile.name} → ${problemNumber}-${problemName}.${extension}`);
            }

            problem[directory] = true;
        }
    }

    console.log(`Renamed ${renamed} files`);
}

fs.writeFileSync(
    './.problemSiteData.json',
    JSON.stringify(PROBLEMS_SITE_DATA, null, 2),
);
console.log('\n File .problemSiteData.json updated successfully');
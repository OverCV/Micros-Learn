const fs = require('fs');

function formatAVRAssembly(input) {
    const lines = input.split('\n');
    const formattedLines = lines.map(line => {
        line = line.trim();
        if (line.startsWith('.')) {
            return line;
        }
        if (line.includes(':')) {
            return line;
        }
        return '    ' + line;
    });
    return formattedLines.join('\n');
}

const fileName = process.argv[2];
const content = fs.readFileSync(fileName, 'utf8');
const formattedContent = formatAVRAssembly(content);
fs.writeFileSync(fileName, formattedContent);
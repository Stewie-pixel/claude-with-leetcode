const MAX_RANGE_EXPAND = 20;

const DECL_PATTERN =
    /^\s*(?:val|var|let|const|int|long|float|double|char|bool|boolean|String|auto)\s+(\w+)/;

/**
 * @param {string} body
 * @returns {{ prefix: string, suggestion: string, suffix: string } | null}
 */
export function extractSuggestion(body) {
    const match = body.match(/([\s\S]*?)```suggestion\n([\s\S]*?)```([\s\S]*)/);
    if (!match) return null;
    return {
        prefix: match[1],
        suggestion: match[2].replace(/\n$/, ''),
        suffix: match[3],
    };
}

/**
 * @param {string} body
 * @returns {string}
 */
export function stripSuggestion(body) {
    return body
        .replace(/\n*```suggestion\n[\s\S]*?```\n*/g, '\n')
        .replace(/\n{3,}/g, '\n\n')
        .trimEnd();
}

/**
 * @param {string} text
 * @returns {string}
 */
export function wrapSuggestion(text) {
    const normalized = text.replace(/\n*$/, '') + '\n';
    return '```suggestion\n' + normalized + '```';
}

/**
 * @param {Map<string, string>} numberedSourceMap
 * @param {string} path
 * @returns {string[]|null}
 */
export function getPlainSourceLines(numberedSourceMap, path) {
    const numbered = numberedSourceMap.get(path);
    if (!numbered) return null;
    return numbered.split('\n').map((l) => l.replace(/^\s*\d+\s*\| ?/, ''));
}

/**
 * @param {string} line
 * @returns {string|null}
 */
function extractDeclName(line) {
    const match = line.match(DECL_PATTERN);
    return match ? match[1] : null;
}

/**
 * @param {string[]} lines
 * @param {number} startIdx
 * @param {number} endIdx
 * @returns {string[]}
 */
function collectDeclNames(lines, startIdx, endIdx) {
    const names = [];
    for (let i = startIdx; i <= endIdx && i < lines.length; i++) {
        const name = extractDeclName(lines[i]);
        if (name) names.push(name);
    }
    return names;
}

/**
 * @param {string} text
 * @returns {number}
 */
function balanceDelta(text) {
    let delta = 0;
    for (const ch of text) {
        if (ch === '(' || ch === '{' || ch === '[') delta++;
        else if (ch === ')' || ch === '}' || ch === ']') delta--;
    }
    return delta;
}

/**
 * @param {string[]} lines
 * @param {number} start
 * @param {number} end
 * @returns {number}
 */
function rangeBalance(lines, start, end) {
    let delta = 0;
    for (let i = start - 1; i <= end - 1 && i < lines.length; i++) {
        delta += balanceDelta(lines[i]);
    }
    return delta;
}

/**
 * @param {string[]} suggestionLines
 * @returns {Set<string>}
 */
function suggestionDeclNames(suggestionLines) {
    const names = new Set();
    for (const line of suggestionLines) {
        const name = extractDeclName(line);
        if (name) names.add(name);
    }
    return names;
}

/**
 * @param {number} lineNum
 * @param {Set<number>} validLines
 * @returns {boolean}
 */
function isLineValid(lineNum, validLines) {
    return lineNum >= 1 && validLines.has(lineNum);
}

/**
 * @param {number} start
 * @param {number} end
 * @param {Set<number>} validLines
 * @returns {boolean}
 */
function isRangeValid(start, end, validLines) {
    if (start > end) return false;
    for (let n = start; n <= end; n++) {
        if (!validLines.has(n)) return false;
    }
    return true;
}

/**
 * @param {string[]} sourceLines
 * @param {number} anchorLine
 * @param {string[]} suggestionLines
 * @returns {number}
 */
function findStartFromSuggestionMatch(sourceLines, anchorLine, suggestionLines) {
    const firstSugg = suggestionLines.find((l) => l.trim().length > 0)?.trim();
    if (!firstSugg) return anchorLine;

    const searchFrom = Math.max(1, anchorLine - MAX_RANGE_EXPAND);
    for (let i = anchorLine; i >= searchFrom; i--) {
        if (sourceLines[i - 1]?.trim() === firstSugg) return i;
    }
    return anchorLine;
}

/**
 * @param {string[]} sourceLines
 * @param {number} start
 * @param {number} end
 * @param {string[]} suggestionLines
 * @returns {number}
 */
function extendEndForStaleDecls(sourceLines, start, end, suggestionLines) {
    const suggDecls = suggestionDeclNames(suggestionLines);
    let extended = end;

    for (let i = end; i < Math.min(sourceLines.length, end + MAX_RANGE_EXPAND); i++) {
        const line = sourceLines[i];
        const decl = extractDeclName(line);
        if (decl && suggDecls.has(decl)) {
            extended = i + 1;
            let balance = balanceDelta(line);
            for (
                let j = i + 1;
                j < sourceLines.length && j < end + MAX_RANGE_EXPAND;
                j++
            ) {
                extended = j + 1;
                balance += balanceDelta(sourceLines[j]);
                if (balance <= 0) break;
            }
        }
    }

    return extended;
}

/**
 * @param {string[]} sourceLines
 * @param {number} anchorLine
 * @param {number|undefined} startLine
 * @param {string[]} suggestionLines
 * @param {Set<number>} validLines
 * @returns {{ start: number, end: number } | null}
 */
export function inferReplacementRange(
    sourceLines,
    anchorLine,
    startLine,
    suggestionLines,
    validLines,
) {
    if (!sourceLines.length || !suggestionLines.length) return null;
    if (!isLineValid(anchorLine, validLines)) return null;

    let start = startLine ?? findStartFromSuggestionMatch(
        sourceLines,
        anchorLine,
        suggestionLines,
    );
    let end = anchorLine;

    if (start > end) [start, end] = [end, start];

    if (!isRangeValid(start, end, validLines)) {
        start = anchorLine;
        end = anchorLine;
        if (!isRangeValid(start, end, validLines)) return null;
    }

    while (
        rangeBalance(sourceLines, start, end) !== 0 &&
        end < sourceLines.length
    ) {
        const nextEnd = end + 1;
        if (!isLineValid(nextEnd, validLines)) break;
        end = nextEnd;
    }

    const staleEnd = extendEndForStaleDecls(
        sourceLines,
        start,
        end,
        suggestionLines,
    );
    end = Math.max(end, staleEnd);

    while (end > start && !isLineValid(end, validLines)) end--;
    if (!isRangeValid(start, end, validLines)) return null;

    return { start, end };
}

/**
 * @param {string[]} sourceLines
 * @param {number} start
 * @param {number} end
 * @param {string[]} suggestionLines
 * @returns {string[]}
 */
export function simulateApply(sourceLines, start, end, suggestionLines) {
    const before = sourceLines.slice(0, start - 1);
    const after = sourceLines.slice(end);
    return [...before, ...suggestionLines, ...after];
}

/**
 * @param {string[]} before
 * @param {string[]} after
 * @param {number} start
 * @param {number} end
 * @param {string[]} suggestionLines
 * @returns {boolean}
 */
export function detectOverlapAfterApply(
    before,
    after,
    start,
    end,
    suggestionLines,
) {
    for (let i = start - 1; i < end; i++) {
        const origLine = before[i]?.trim();
        if (!origLine) continue;
        if (suggestionLines.some((l) => l.trim() === origLine)) continue;

        const searchFrom = start - 1 + suggestionLines.length;
        for (let j = searchFrom; j < after.length; j++) {
            if (after[j]?.trim() === origLine) return true;
        }
    }

    const tailStart = end;
    for (let i = tailStart; i < Math.min(before.length, end + MAX_RANGE_EXPAND); i++) {
        const tailLine = before[i]?.trim();
        if (!tailLine) continue;
        if (suggestionLines.some((l) => l.trim() === tailLine)) continue;

        const tailDecl = extractDeclName(before[i]);
        if (tailDecl && suggestionDeclNames(suggestionLines).has(tailDecl)) {
            return true;
        }

        if (rangeBalance(before, start, i) > 0) return true;
    }

    for (const name of suggestionDeclNames(suggestionLines)) {
        if (collectDeclNames(after, 0, after.length - 1).filter((n) => n === name).length > 1) {
            return true;
        }
    }

    const editRegion = after.slice(
        start - 1,
        start - 1 + suggestionLines.length + 3,
    );
    if (Math.abs(balanceDelta(editRegion.join('\n'))) > 2) return true;

    if (suggestionLines.length > 0) {
        const first = suggestionLines[0].trim();
        let dupCount = 0;
        for (const line of editRegion) {
            if (line.trim() === first) dupCount++;
        }
        if (dupCount > 1) return true;
    }

    return false;
}

/**
 * @param {string[]} sourceLines
 * @param {{ start: number, end: number }} range
 * @param {string[]} suggestionLines
 * @param {Set<number>} validLines
 * @returns {{ start: number, end: number } | null}
 */
function expandRangeUntilClean(sourceLines, range, suggestionLines, validLines) {
    let { start, end } = range;

    for (let attempt = 0; attempt < MAX_RANGE_EXPAND * 2; attempt++) {
        const after = simulateApply(sourceLines, start, end, suggestionLines);
        if (!detectOverlapAfterApply(sourceLines, after, start, end, suggestionLines)) {
            return { start, end };
        }

        const nextEnd = end + 1;
        if (nextEnd <= sourceLines.length && isLineValid(nextEnd, validLines)) {
            end = nextEnd;
            continue;
        }

        const prevStart = start - 1;
        if (prevStart >= 1 && isLineValid(prevStart, validLines)) {
            start = prevStart;
            continue;
        }

        return null;
    }

    return null;
}

/**
 * @param {object} opts
 * @param {string} opts.body
 * @param {string} opts.path
 * @param {number} opts.line
 * @param {number|undefined} opts.startLine
 * @param {Map<string, string>} opts.numberedSourceMap
 * @param {Set<number>} opts.validLines
 * @returns {{ body: string, line: number, start_line: number|undefined, stripped: boolean, reason?: string }}
 */
export function repairSuggestionComment({
    body,
    path,
    line,
    startLine,
    numberedSourceMap,
    validLines,
}) {
    const parsed = extractSuggestion(body);
    if (!parsed) {
        return { body, line, start_line: startLine, stripped: false };
    }

    const sourceLines = getPlainSourceLines(numberedSourceMap, path);
    if (!sourceLines) {
        return {
            body: stripSuggestion(body),
            line,
            start_line: undefined,
            stripped: true,
            reason: 'no source available',
        };
    }

    const suggestionLines = parsed.suggestion.split('\n');
    const initial = inferReplacementRange(
        sourceLines,
        line,
        startLine,
        suggestionLines,
        validLines,
    );

    if (!initial) {
        return {
            body: stripSuggestion(body),
            line,
            start_line: undefined,
            stripped: true,
            reason: 'could not infer valid replacement range',
        };
    }

    const range = expandRangeUntilClean(
        sourceLines,
        initial,
        suggestionLines,
        validLines,
    );

    if (!range) {
        return {
            body: stripSuggestion(body),
            line,
            start_line: undefined,
            stripped: true,
            reason: 'overlap detected after simulated apply',
        };
    }

    const { start, end } = range;
    const rebuilt =
        parsed.prefix.trimEnd() +
        '\n\n' +
        wrapSuggestion(parsed.suggestion) +
        parsed.suffix;

    return {
        body: rebuilt,
        line: end,
        start_line: start === end ? undefined : start,
        stripped: false,
    };
}

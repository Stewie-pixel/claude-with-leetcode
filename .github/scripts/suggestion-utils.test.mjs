import { describe, it } from 'node:test';
import assert from 'node:assert/strict';
import {
    detectOverlapAfterApply,
    extractSuggestion,
    inferReplacementRange,
    repairSuggestionComment,
    simulateApply,
    stripSuggestion,
} from './suggestion-utils.mjs';

const romanSource = [
    'class Solution {',
    '    fun intToRoman(num: Int): String {',
    '        val values = intArrayOf(',
    '            1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1',
    '        )',
    '        val symbols = arrayOf(',
    '            "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"',
    '        )',
    '        var result = StringBuilder()',
    '        return result.toString()',
    '    }',
    '}',
];

const romanSuggestion = [
    '        val values = intArrayOf(',
    '            1, 5, 10, 50, 100, 500, 1000',
    '        )',
    '        val symbols = arrayOf(',
    '            "I", "V", "X", "L", "C", "D", "M"',
    '        )',
];

const duplicateSkipSource = [
    'class Solution {',
    '    fun combinationSum2(candidates: IntArray, target: Int): List<List<Int>> {',
    '        val result = mutableListOf<List<Int>>()',
    '        fun backtrack(start: Int, path: MutableList<Int>, sum: Int) {',
    '            if (sum == target) {',
    '                result.add(path.toList())',
    '                return',
    '            }',
    '            for (i in start until candidates.size) {',
    '                if (used[i]) {',
    '                    continue',
    '                }',
    '                used[i] = true',
    '            }',
    '        }',
    '        return result',
    '    }',
    '}',
];

const duplicateSkipSuggestion = [
    '                if (used[i]) {',
    '                    continue',
    '                }',
    '                if (i > 0 && candidates[i] == candidates[i - 1] && !used[i - 1]) {',
    '                    continue',
    '                }',
];

function numberedMap(path, lines) {
    const numbered = lines
        .map((l, i) => `${String(i + 1).padStart(4)} | ${l}`)
        .join('\n');
    return new Map([[path, numbered]]);
}

describe('extractSuggestion', () => {
    it('parses suggestion block', () => {
        const body = 'Fix this\n\n```suggestion\nfoo\n```\n';
        const parsed = extractSuggestion(body);
        assert.equal(parsed.suggestion, 'foo');
    });
});

describe('inferReplacementRange — Roman numerals', () => {
    const validLines = new Set([3, 4, 5, 6, 7, 8, 9, 10]);

    it('expands narrow anchor to cover both array declarations', () => {
        const range = inferReplacementRange(
            romanSource,
            8,
            undefined,
            romanSuggestion,
            validLines,
        );
        assert.ok(range);
        assert.equal(range.start, 3);
        assert.equal(range.end, 8);
    });
});

describe('inferReplacementRange — duplicate skip', () => {
    const validLines = new Set([10, 11, 12, 13, 14]);

    it('covers full if block when anchored inside it', () => {
        const range = inferReplacementRange(
            duplicateSkipSource,
            11,
            undefined,
            duplicateSkipSuggestion,
            validLines,
        );
        assert.ok(range);
        assert.equal(range.start, 10);
        assert.equal(range.end, 12);
    });
});

describe('simulateApply + detectOverlapAfterApply', () => {
    it('detects duplicate declarations after partial replace', () => {
        const after = simulateApply(romanSource, 8, 8, romanSuggestion);
        const overlap = detectOverlapAfterApply(
            romanSource,
            after,
            8,
            8,
            romanSuggestion,
        );
        assert.equal(overlap, true);
    });

    it('passes when full block is replaced', () => {
        const after = simulateApply(romanSource, 3, 8, romanSuggestion);
        const overlap = detectOverlapAfterApply(
            romanSource,
            after,
            3,
            8,
            romanSuggestion,
        );
        assert.equal(overlap, false);
    });

    it('detects duplicated if anchor line in malformed suggestion', () => {
        const badSuggestion = [
            '                if (used[i]) {',
            '                    continue',
            '                }',
            '                if (i > 0 && candidates[i] == candidates[i - 1] && !used[i - 1]) {',
            '                    continue',
            '                }',
            '                    continue',
            '                }',
        ];
        const after = simulateApply(duplicateSkipSource, 10, 12, badSuggestion);
        const overlap = detectOverlapAfterApply(
            duplicateSkipSource,
            after,
            10,
            12,
            badSuggestion,
        );
        assert.equal(overlap, true);
    });
});

describe('repairSuggestionComment', () => {
    const path = 'kotlin/12-integer-to-roman/integer-to-roman.kt';

    it('repairs Roman numerals suggestion range', () => {
        const body =
            '[HIGH] Wrong value ordering\n\n```suggestion\n' +
            romanSuggestion.join('\n') +
            '\n```';
        const validLines = new Set([3, 4, 5, 6, 7, 8, 9, 10]);
        const result = repairSuggestionComment({
            body,
            path,
            line: 8,
            startLine: undefined,
            numberedSourceMap: numberedMap(path, romanSource),
            validLines,
        });
        assert.equal(result.stripped, false);
        assert.equal(result.start_line, 3);
        assert.equal(result.line, 8);
        assert.match(result.body, /```suggestion/);
    });

    it('strips malformed duplicate-skip suggestion', () => {
        const badSuggestion = [
            '                if (used[i]) {',
            '',
            '                if (used[i]) {',
            '                    continue',
            '                }',
            '                if (i > 0 && candidates[i] == candidates[i - 1] && !used[i - 1]) {',
            '                    continue',
            '                }',
            '                    continue',
            '                }',
        ];
        const body =
            '[HIGH] Missing duplicate skip\n\n```suggestion\n' +
            badSuggestion.join('\n') +
            '\n```';
        const validLines = new Set([10, 11, 12, 13, 14]);
        const result = repairSuggestionComment({
            body,
            path: 'kotlin/40-combination-sum-ii/combination-sum-ii.kt',
            line: 11,
            startLine: undefined,
            numberedSourceMap: numberedMap(
                'kotlin/40-combination-sum-ii/combination-sum-ii.kt',
                duplicateSkipSource,
            ),
            validLines,
        });
        assert.equal(result.stripped, true);
        assert.doesNotMatch(result.body, /```suggestion/);
        assert.match(result.body, /Missing duplicate skip/);
    });

    it('leaves single-line suggestion without start_line', () => {
        const source = ['int x = 1;', 'int y = 2;'];
        const body = 'Use long\n\n```suggestion\nlong x = 1L;\n```';
        const validLines = new Set([1, 2]);
        const result = repairSuggestionComment({
            body,
            path: 'c/foo.c',
            line: 1,
            startLine: undefined,
            numberedSourceMap: numberedMap('c/foo.c', source),
            validLines,
        });
        assert.equal(result.stripped, false);
        assert.equal(result.start_line, undefined);
        assert.equal(result.line, 1);
    });

    it('stripSuggestion removes block but keeps text', () => {
        const body = 'Explain\n\n```suggestion\ncode\n```\n';
        assert.equal(stripSuggestion(body), 'Explain');
    });
});

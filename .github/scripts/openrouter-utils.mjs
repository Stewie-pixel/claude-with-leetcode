const OPENROUTER_URL = 'https://openrouter.ai/api/v1/chat/completions';

/**
 * Extract the outermost JSON object or array from a raw string.
 * Strategy (in order):
 *   1. Direct JSON.parse (clean output)
 *   2. Strip markdown fenced blocks: ```json ... ``` or ``` ... ```
 *   3. Brace-match: find first `{` and last `}` (handles leading prose)
 *
 * @param {string} raw
 * @returns {object} parsed JSON
 * @throws if all strategies fail
 */
export function extractJSON(raw) {
    if (!raw || typeof raw !== 'string') {
        throw new Error('extractJSON: received empty or non-string input');
    }

    const trimmed = raw.trim();

    try {
        return JSON.parse(trimmed);
    } catch {}

    const fenceMatch = trimmed.match(/```(?:json)?\s*([\s\S]*?)```/);
    if (fenceMatch) {
        try {
            return JSON.parse(fenceMatch[1].trim());
        } catch {}
    }

    const start = trimmed.indexOf('{');
    const end = trimmed.lastIndexOf('}');
    if (start !== -1 && end > start) {
        try {
            return JSON.parse(trimmed.slice(start, end + 1));
        } catch {}
    }

    throw new Error(
        `extractJSON: all strategies failed. Raw (first 200 chars): ${trimmed.slice(0, 200)}`,
    );
}

/**
 * Call the OpenRouter chat completions API with automatic retry and
 * exponential backoff.
 *
 * @param {object}   opts
 * @param {Array}    opts.messages      - Array of {role, content} message objects
 * @param {boolean}  [opts.jsonMode]    - If true, request response_format json_object
 * @param {number}   [opts.retries]     - Max attempts (default 3)
 * @param {object}   [opts.core]        - GitHub Actions core object for logging
 * @returns {string} Raw model text content (not parsed)
 * @throws after all retries are exhausted
 */
export async function callOpenRouter({
    messages,
    jsonMode = false,
    retries = 3,
    core = null,
}) {
    const apiKey = process.env.OPENROUTER_API_KEY;
    const model = process.env.OPENROUTER_MODEL;

    if (!apiKey) throw new Error('OPENROUTER_API_KEY is not set');
    if (!model) throw new Error('OPENROUTER_MODEL is not set');

    const body = {
        model,
        messages,
    };

    if (jsonMode) {
        body.response_format = { type: 'json_object' };
    }

    let lastError;
    for (let attempt = 1; attempt <= retries; attempt++) {
        try {
            if (attempt > 1) {
                const backoffMs = Math.pow(2, attempt - 2) * 1000;
                core?.info(
                    `Retry attempt ${attempt}/${retries} after ${backoffMs}ms backoff…`,
                );
                await new Promise((r) => setTimeout(r, backoffMs));
            }

            const response = await fetch(OPENROUTER_URL, {
                method: 'POST',
                headers: {
                    Authorization: `Bearer ${apiKey}`,
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify(body),
            });

            if (!response.ok) {
                const errorText = await response
                    .text()
                    .catch(() => '(unreadable)');
                throw new Error(
                    `OpenRouter HTTP ${response.status}: ${errorText.slice(0, 300)}`,
                );
            }

            const data = await response.json();

            if (data.error) {
                throw new Error(
                    `OpenRouter API error: ${JSON.stringify(data.error).slice(0, 300)}`,
                );
            }

            const content = data.choices?.[0]?.message?.content;

            if (!content || content.trim() === '') {
                throw new Error(
                    `OpenRouter returned empty content (choices: ${JSON.stringify(data.choices?.slice(0, 1))})`,
                );
            }

            return content.trim();
        } catch (err) {
            lastError = err;
            core?.warning(
                `callOpenRouter attempt ${attempt} failed: ${err.message}`,
            );
        }
    }

    throw new Error(
        `callOpenRouter failed after ${retries} attempts. Last error: ${lastError?.message}`,
    );
}

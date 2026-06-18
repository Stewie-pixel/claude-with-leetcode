# JavaScript Promises

## Video Solution

For more details about **Add Two Promises**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=WOmZrTEtID0).

## Concept

A **Promise** in JavaScript represents a value that may be available now, in the future, or never. It allows you to write asynchronous code that looks synchronous by using `async`/`await` or `.then()`. When you need to wait for multiple independent promises and then combine their results, you can use `Promise.all()` to settle them in parallel, or await them one after another.

## When to Use It

Use promise‑combining patterns when you see:
- Multiple asynchronous operations that do not depend on each other.
- A need to wait for all of them to finish before proceeding.
- The final result is a function (e.g., sum, merge) of the individual resolved values.

## Template

```python
# Note: The template is shown in Python‑like pseudocode for readability.
# In JavaScript you would write:

async function combinePromises(p1, p2, ..., pn):
    # Wait for all promises to resolve
    results = await Promise.all([p1, p2, ..., pn])
    # Combine the results as needed
    return combine(results)   # e.g., sum, concat, etc.
```

## LeetCode Problem Walkthrough

### Problem: 2723. Add Two Promises

https://leetcode.com/problems/add-two-promises/

### Approach 1: Sequential Await (Naive)

**Algorithm**
1. Await `promise1` to get its resolved value `a`.
2. Await `promise2` to get its resolved value `b`.
3. Return `a + b`.

**Implementation**

```javascript
/**
 * @param {Promise<number>} promise1
 * @param {Promise<number>} promise2
 * @return {Promise<number>}
 */
async function addTwoPromises(promise1, promise2) {
    const a = await promise1;   // wait for first promise
    const b = await promise2;   // wait for second promise
    return a + b;
}
```

**Complexity Analysis**

- Time complexity: O(1) — we perform a constant amount of work; the actual waiting time is determined by the promises themselves, not by our code.
- Space complexity: O(1) — only a few variables are used.

### Approach 2: Promise.all (Parallel Wait) – Optimized

**Intuition**
Instead of waiting for each promise one after another, we can let them run in parallel. `Promise.all` takes an array of promises and returns a single promise that resolves when **all** input promises have resolved, giving us an array of their resolved values. This reduces the total elapsed time to the longest of the two delays rather than their sum.

**Algorithm**
1. Call `Promise.all([promise1, promise2])` – it returns a promise that resolves to `[a, b]`.
2. Await that promise to obtain the array of results.
3. Return the sum of the two elements.

**Implementation**

```javascript
/**
 * @param {Promise<number>} promise1
 * @param {Promise<number>} promise2
 * @return {Promise<number>}
 */
async function addTwoPromises(promise1, promise2) {
    const [a, b] = await Promise.all([promise1, promise2]);
    return a + b;
}
```

**Complexity Analysis**

- Time complexity: O(1) — constant‑time overhead; the promises execute concurrently.
- Space complexity: O(1) — only the result array of size 2 is stored.

### Approach 3: Using .then Chaining (Alternative)

**Intuition**
If you prefer not to use `async`/`await`, you can attach `.then` handlers to each promise and combine the values once both have resolved. This approach also runs the promises in parallel because the `.then` callbacks are scheduled immediately; the final sum is computed after both callbacks fire.

**Algorithm**
1. Create a variable to store the resolved values (e.g., an array of length 2 initialized with `null`).
2. Attach a `.then` to `promise1` that stores its value at index 0.
3. Attach a `.then` to `promise2` that stores its value at index 1.
4. After both callbacks have run (you can detect this by checking that both slots are non‑null), return the sum.
5. Wrap the whole logic in a new `Promise` so the function returns a promise.

**Implementation**

```javascript
/**
 * @param {Promise<number>} promise1
 * @param {Promise<number>} promise2
 * @return {Promise<number>}
 */
function addTwoPromises(promise1, promise2) {
    return new Promise((resolve) => {
        const values = [null, null];
        let resolvedCount = 0;

        promise1.then((val) => {
            values[0] = val;
            resolvedCount++;
            if (resolvedCount === 2) resolve(values[0] + values[1]);
        });

        promise2.then((val) => {
            values[1] = val;
            resolvedCount++;
            if (resolvedCount === 2) resolve(values[0] + values[1]);
        });
    });
}
```

**Complexity Analysis**

- Time complexity: O(1) — constant work plus the asynchronous waiting time.
- Space complexity: O(1) — we store two values and a counter.

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

We trace the execution with the first example:

```
promise1 resolves to 2 after 20 ms
promise2 resolves to 5 after 60 ms
```

Using **Approach 2 (Promise.all)**:

| Step | Action                                 | promise1 state | promise2 state | values from Promise.all | Returned sum |
|------|----------------------------------------|----------------|----------------|-------------------------|--------------|
| 0    | Call `Promise.all([p1, p2])`           | pending        | pending        | –                       | –            |
| 20 ms| p1 resolves → 2                        | fulfilled (2)  | pending        | –                       | –            |
| 60 ms| p2 resolves → 5                        | fulfilled (2)  | fulfilled (5)  | [2, 5]                  | 7            |
| 60 ms| `await` resolves, return `2 + 5`       | –              | –              | –                       | 7            |

The total elapsed time is **60 ms**, the longer of the two delays, demonstrating the benefit of waiting in parallel.
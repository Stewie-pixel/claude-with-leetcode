**Solution Explanation**

We scan the asteroids from left to right and keep the asteroids that have survived so far in a *stack*.

* An asteroid moving to the right (`value > 0`) can never collide with anything that is already in the stack, because everything in the stack is either also moving right or is already to its left.
* An asteroid moving to the left (`value < 0`) can only collide with asteroids that are moving to the right and are directly to its left – i.e. the elements on the top of the stack.

When a left‑moving asteroid `a (<0)` meets a right‑moving asteroid `b (>0)` on the stack top we have three possibilities:

| Comparison | Result |
|------------|--------|
| `|a| > b`  | The top asteroid explodes → pop it and keep checking the new top. |
| `|a| == b` | Both explode → pop the top and mark the current asteroid as dead. |
| `|a| < b`  | The current asteroid explodes → keep the top and stop checking. |

If the current asteroid survives all possible collisions we push it onto the stack.  
At the end the stack (from bottom to top) is the final state of the asteroids.

---

### Algorithm
```
asteroidCollision(asteroids):
    stack = empty list
    for a in asteroids:
        alive = True
        while a < 0 and stack not empty and alive and stack.back > 0:
            if stack.back < -a:          # top explodes
                stack.pop_back()
            elif stack.back == -a:       # both explode
                stack.pop_back()
                alive = False
            else:                        # current explodes
                alive = False
        if alive:
            stack.push_back(a)
    return stack
```

### Correctness Proof  

We prove that the algorithm returns exactly the state of the asteroids after all collisions.

**Lemma 1**  
At any iteration, the stack contains the surviving asteroids among those already processed, in their original left‑to‑right order, and no two adjacent asteroids in the stack can collide (i.e., there is no `+` followed by `-`).

*Proof.*  
Induction on the number of processed asteroids.

*Base*: before processing any asteroid the stack is empty – property holds.

*Inductive step*: assume the property holds after processing the first `i` asteroids. Consider asteroid `a = asteroids[i]`.

* If `a > 0` (moves right) it cannot collide with any asteroid already in the stack because all of them are either left‑moving (`<0`) or right‑moving (`>0`) but situated to its left, so a right‑moving asteroid will never meet them. The algorithm only pushes `a` (if alive) – the order stays correct and the no‑collision invariant still holds.

* If `a < 0` (moves left) it can only collide with the nearest right‑moving asteroid to its left, which is exactly the top of the stack while it is positive. The `while` loop repeatedly removes the top asteroid (`stack.back`) according to the three collision cases, which are precisely the physical outcomes. When the loop stops, either the stack is empty or its top is negative (moving left) – in both cases no further collision with `a` is possible. If `a` survived (`alive == true`) we push it; otherwise we discard it. In either case the invariant is restored.

Thus after processing `i+1` asteroids the invariant holds. ∎



**Lemma 2**  
When the algorithm finishes, every asteroid remaining in the stack survives all possible collisions in the original sequence.

*Proof.*  
By Lemma&nbsp;1 after the last asteroid is processed the stack contains no adjacent `+,-` pair, i.e. no two asteroids moving towards each other. Hence no further collisions can occur, so all remaining asteroids survive forever. ∎



**Theorem**  
The list returned by `asteroidCollision` equals the final state of the asteroids after all collisions.

*Proof.*  
From Lemma&nbsp;1 the stack always contains exactly the survivors among the processed prefix. After the whole array is processed, the stack contains the survivors of the entire array. Lemma&nbsp;2 shows that none of those survivors will ever collide later. Conversely, any asteroid that is destroyed in the real process must have collided with a survivor on its left at the moment of destruction, and the algorithm removes it in the corresponding `while` loop iteration. Hence no destroyed asteroid remains in the stack. Therefore the stack equals the true final configuration. ∎



### Complexity Analysis  

*Each asteroid is pushed onto the stack at most once and popped at most once.*  
- **Time:** `O(n)` where `n = asteroids.length`.  
- **Space:** `O(n)` in the worst case (when no collisions happen, e.g., all move right).

---

### Reference Implementation (C++17)

```cpp
class Solution {
public:
    vector<int> asteroidCollision(vector<int>& asteroids) {
        vector<int> st;                     // acts as a stack
        for (int a : asteroids) {
            bool alive = true;              // current asteroid still alive?
            // resolve collisions with right‑moving asteroids on top of stack
            while (a < 0 && !st.empty() && alive && st.back() > 0) {
                int top = st.back();
                if (top < -a) {             // top explodes
                    st.pop_back();
                } else if (top == -a) {     // both explode
                    st.pop_back();
                    alive = false;
                } else {                    // current explodes
                    alive = false;
                }
            }
            if (alive) {
                st.push_back(a);
            }
        }
        return st;
    }
};
```

### Reference Video Explanation  

*NeetCode – “Asteroid Collision - Stack - LeetCode 735”*  
▶ https://www.youtube.com/watch?v=LN7KjRszjk4  

The video walks through the same stack‑based simulation and walks through several examples, matching the algorithm and complexity analysis presented above.
# Two Pointers

## Video Solution

For more details about **Rotate Image**, watch the walkthrough at [resources](https://www.youtube.com/watch?v=-jhbxNJijyE).

## Concept

Imagine you have a physical photo printed on square paper. To rotate it 90 degrees clockwise, you could:
1. First flip it over along the diagonal (top-left to bottom-right) - this is the transpose
2. Then flip each row horizontally (like turning a page in a book)

This two-step process achieves the rotation without needing extra space. The key insight is that rotation = transpose + horizontal flip.

## When to Use It

Use the transpose and reverse technique when you see:
- Rotating a matrix/image by 90, 180, or 270 degrees
- Problems requiring in-place matrix transformation
- Clues like "rotate", "turn", "spin" combined with matrix/grid

## Template

```python
def rotate_matrix(matrix):
    n = len(matrix)
    # Step 1: Transpose (swap elements across diagonal)
    for i in range(n):
        for j in range(i + 1, n):
            matrix[i][j], matrix[j][i] = matrix[j][i], matrix[i][j]
    
    # Step 2: Reverse each row
    for i in range(n):
        left, right = 0, n - 1
        while left < right:
            matrix[i][left], matrix[i][right] = matrix[i][right], matrix[i][left]
            left += 1
            right -= 1
```

## LeetCode Problem Walkthrough

### Problem: 48. Rotate Image

https://leetcode.com/problems/rotate-image/

### Approach 1: Brute Force (Extra Space)

**Algorithm**
- Create a new matrix of the same size
- For each element at (i, j) in original, place it at (j, n-1-i) in the new matrix
- Copy the new matrix back to the original (though this violates in-place requirement, it helps understand the transformation)

**Implementation**
```python
def rotate(matrix):
    n = len(matrix)
    # Create a new matrix to store rotated values
    rotated = [[0] * n for _ in range(n)]
    
    # Fill the rotated matrix: 
    # Original [i][j] -> New [j][n-1-i]
    for i in range(n):
        for j in range(n):
            rotated[j][n - 1 - i] = matrix[i][j]
    
    # Copy back to original (not allowed in actual problem, but shown for clarity)
    for i in range(n):
        for j in range(n):
            matrix[i][j] = rotated[i][j]
```

**Complexity Analysis**
- Time complexity: O(n²) — we visit each element twice (once for filling, once for copying)
- Space complexity: O(n²) — we allocate a new n x n matrix

### Approach 2: Transpose and Reverse (Optimal In-Place)

**Intuition**
Instead of using extra space, we can perform the rotation in two in-place steps:
1. Transpose the matrix (swap elements across the main diagonal)
2. Reverse each row horizontally
This works because transConverter converts rows to columns, and reversing completes the 90° rotation.

**Algorithm**
- Step 1: Transpose by swapping matrix[i][j] with matrix[j][i] for all i < j
- Step 2: For each row, swap elements from the ends moving toward the center

**Implementation**
```python
def rotate(matrix):
    n = len(matrix)
    
    # Transpose the matrix
    for i in range(n):
        for j in range(i + 1, n):
            matrix[i][j], matrix[j][i] = matrix[j][i], matrix[i][j]
    
    # Reverse each row
    for i in range(n):
        left, right = 0, n - 1
        while left < right:
            matrix[i][left], matrix[i][right] = matrix[i][right], matrix[i][left]
            left += 1
            right -= 1
```

**Complexity Analysis**
- Time complexity: O(n²) — two nested passes (transpose: n²/2, reverse: n²/2)
- Space complexity: O(1) — we only use a few extra variables for swapping

### Approach 3: Layer-by-Layer Rotation (Alternative In-Place)

**Intuition**
Think of the matrix as concentric square layers (like an onion). Rotate each layer by moving elements in groups of four:
- Top edge → Right edge
- Right edge → Bottom edge  
- Bottom edge → Left edge
- Left edge → Top edge
We use two pointers to define the current layer boundaries and iterate through each element in the layer.

**Algorithm**
- For each layer from outer to inner:
  - Define boundaries: top = layer, bottom = n-1-layer, left = layer, right = n-1-layer
  - For each element in the current top row (excluding corners):
    - Save the top element
    - Move left → top
    - Move bottom → left
    - Move right → bottom
    - Move saved top → right

**Implementation**
```python
def rotate(matrix):
    n = len(matrix)
    # Process layers from outer to inner
    for layer in range(n // 2):
        first = layer
        last = n - 1 - layer
        for i in range(first, last):
            offset = i - first
            
            # Save top
            top = matrix[first][i]
            
            # Left -> Top
            matrix[first][i] = matrix[last - offset][first]
            
            # Bottom -> Left
            matrix[last - offset][first] = matrix[last][last - offset]
            
            # Right -> Bottom
            matrix[last][last - offset] = matrix[i][last]
            
            # Top -> Right
            matrix[i][last] = top
```

**Complexity Analysis**
- Time complexity: O(n²) — each element is moved exactly once
- Space complexity: O(1) — only uses constant extra space for variables

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

## Dry Run

Input: matrix = [[1,2,3],[4,5,6],[7,8,9]]

### Approach 2: Transpose and Reverse

**Step 1: Transpose**
```
Original:
[1, 2, 3]
[4, 5, 6]
[7, 8, 9]

After transpose (swap across diagonal):
[1, 4, 7]
[2, 5, 8]
[3, 6, 9]
```
*Trace:*
- Swap [0,1] and [1,0]: [1,4,3] / [2,5,6] / [7,8,9]
- Swap [0,2] and [2,0]: [1,4,7] / [2,5,6] / [3,8,9]
- Swap [1,2] and [2,1]: [1,4,7] / [2,5,8] / [3,6,9]

**Step 2: Reverse Each Row**
```
After transpose:
[1, 4, 7]  → reverse → [7, 4, 1]
[2, 5, 8]  → reverse → [8, 5, 2]
[3, 6, 9]  → reverse → [9, 6, 3]
```
*Trace for row 0:*
- left=0 (1), right=2 (7) → swap → [7,4,1]; left=1, right=1 → stop

Final result: [[7,4,1],[8,5,2],[9,6,3]] ✓

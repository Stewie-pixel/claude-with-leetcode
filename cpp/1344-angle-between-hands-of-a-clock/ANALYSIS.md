# Math & Geometry

## Video Solution

For more details about **Angle Between Hands of a Clock**, watch the walkthrough at [https://www.youtube.com/watch?v=V3Kf3EYaWEg](https://www.youtube.com/watch?v=V3Kf3EYaWEg)

## Concept

The clock face is a circle of 360°.  
- The minute hand moves 360° per 60 minutes → **6° per minute**.  
- The hour hand moves 360° per 12 hours → **30° per hour** plus an extra **0.5° per minute** (since it also moves as minutes pass).  

The angle between the two hands is the absolute difference of their angles. Because we want the *smaller* angle, we take `min(diff, 360 - diff)`.

## When to Use It

Use this technique when a problem mentions:
- Clock hands, hour, minute
- Angles on a circle
- Converting time to angular displacement
- Need to compute the smaller of two complementary angles

## Template

```python
def angle_between_hands(hour: int, minutes: int) -> float:
    # 1. Compute angle of minute hand
    minute_angle = minutes * 6.0          # 6° per minute
    # 2. Compute angle of hour hand
    hour_angle = (hour % 12) * 30.0 + minutes * 0.5  # 30° per hour + 0.5° per minute
    # 3. Absolute difference
    diff = abs(hour_angle - minute_angle)
    # 4. Return the smaller angle
    return min(diff, 360 - diff)
```

## LeetCode Problem Walkthrough

### Problem: 1344. Angle Between Hands of a Clock

https://leetcode.com/problems/angle-between-hands-of-a-clock/

### Approach 1: Direct Calculation (Brute Force‑style)

**Algorithm**
1. Convert the minute hand position to degrees: `minutes * 6`.
2. Convert the hour hand position to degrees: `(hour % 12) * 30 + minutes * 0.5`.
3. Take the absolute difference of the two angles.
4. Return the smaller of the difference and `360 - difference`.

**Implementation**

```python
class Solution:
    def angleClock(self, hour: int, minutes: int) -> float:
        minute_angle = minutes * 6.0
        hour_angle = (hour % 12) * 30.0 + minutes * 0.5
        diff = abs(hour_angle - minute_angle)
        return min(diff, 360 - diff)
```

**Complexity Analysis**
- Time complexity: O(1) — only a few arithmetic operations.
- Space complexity: O(1) — constant extra space.

### Approach 2: Using Total Minutes (Intuition‑driven)

**Intuition**
Instead of treating hour and minute hands separately, we can measure both angles from the 12 o’clock reference in terms of *total minutes* elapsed. The hour hand moves 0.5° per minute, the minute hand moves 6° per minute. Computing both from the same baseline often reduces mental overhead and makes the modulo step clearer.

**Algorithm**
1. Compute total minutes passed since 12:00: `total = hour * 60 + minutes` (hour in 12‑hour format).
2. Hour hand angle = `total * 0.5`.
3. Minute hand angle = `(minutes % 60) * 6` (or simply `minutes * 6`).
4. Follow steps 3‑4 from Approach 1.

**Implementation**

```python
class Solution:
    def angleClock(self, hour: int, minutes: int) -> float:
        hour = hour % 12
        total_minutes = hour * 60 + minutes          # minutes since 12:00
        hour_angle = total_minutes * 0.5
        minute_angle = minutes * 6.0
        diff = abs(hour_angle - minute_angle)
        return min(diff, 360 - diff)
```

**Complexity Analysis**
- Time complexity: O(1)
- Space complexity: O(1)

### Approach 3: One‑Liner Using Built‑ins (Most Compact)

**Intuition**
The core formula is already simple; we can combine the steps into a single expression while preserving readability. This version emphasizes that the problem reduces to evaluating a straightforward mathematical expression.

**Algorithm**
Return `min(abs(((hour % 12) * 30 + minutes * 0.5) - (minutes * 6)), 360 - abs(((hour % 12) * 30 + minutes * 0.5) - (minutes * 6)))`.

**Implementation**

```python
class Solution:
    def angleClock(self, hour: int, minutes: int) -> float:
        return min(
            abs(((hour % 12) * 30 + minutes * 0.5) - (minutes * 6)),
            360 - abs(((hour % 12) * 30 + minutes * 0.5) - (minutes * 6))
        )
```

**Complexity Analysis**
- Time complexity: O(1)
- Space complexity: O(1)

### Provide a Visual Demonstration

**Impact: HIGH** | **Category: explanation** | **Tags:** dry-run, trace, example

#### Dry Run

Input: `hour = 3, minutes = 15`

| Step | hour_angle (°) | minute_angle (°) | |diff| (°) | 360 - |diff| (°) | Result (°) |
|------|----------------|------------------|-----------|----------------|------------|
| Compute | (3 % 12)*30 + 15*0.5 = 90 + 7.5 = 97.5 | 15*6 = 90 | \|97.5 - 90\| = 7.5 | 360 - 7.5 = 352.5 | **7.5** |

The smaller angle is 7.5°, matching the expected output.
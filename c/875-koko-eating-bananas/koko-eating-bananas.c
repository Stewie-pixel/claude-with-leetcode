// -----------------------------------------------------------------------------
// Problem: Koko Eating Bananas
// Source : https://leetcode.com/problems/koko-eating-bananas/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- DESCRIPTION ----------------------------------
//
// Koko loves to eat bananas. There are n piles of bananas, the i-th pile has piles[i] bananas. 
// The guards have gone and will come back in h hours.
//
// Koko can decide her bananas-per-hour eating speed of k. 
// Each hour, she chooses one pile and eats k bananas from it. 
// If the pile has less than k bananas, she eats the entire pile and will not eat any more bananas during that hour.
//
// Koko wants to eat as slowly as possible, but still finish all bananas within h hours.
//
// Return the minimum integer k such that she can eat all the bananas within h hours.
//
// ----------------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
// Observations:
// - Minimum eating speed (k) is at least 1.
// - Maximum eating speed is the largest pile size (maxValue).
// - The number of piles is n, and we must ensure total time <= h.
//
// -----------------------------------------------------------------------------
// Brute Force Idea:
// - Traverse all possible speeds k in [1, maxValue].
// - For each k, compute total hours needed to finish all piles.
// - Return the smallest k such that totalHours <= h.
//   → O(n * maxValue), inefficient for large inputs.
//
// -----------------------------------------------------------------------------
// Optimization (Binary Search):
// - Binary search the minimum valid speed k within [1, maxValue]:
//     1. mid = (left + right) / 2
//     2. Compute total hours with speed = mid
//     3. If totalHours <= h → mid might be valid → try smaller speed (right = mid - 1)
//        else → need faster speed (left = mid + 1)
// - Continue until left > right.
// - The result is the smallest k satisfying the time constraint.
//
// -----------------------------------------------------------------------------

// Helper: find max value in input array
int maxElement(int *piles, int n) {
    int maxValue = 0;
    for(int i = 0; i < n; i++) {
        maxValue = (piles[i] > maxValue) ? piles[i] : maxValue;
    }
    return maxValue;
}

int minEatingSpeed(int* piles, int pilesSize, int h) {
    int minSpeed = 1, maxSpeed = maxElement(piles, pilesSize);
    int result = maxSpeed;

    while(minSpeed <= maxSpeed) {
        int speed = (maxSpeed + minSpeed) / 2;
        long long timeToFinish = 0;
        
        // Calculate total hours koko need to finish all piles with current speed
        for(int j = 0; j < pilesSize; j++) {
            timeToFinish += (piles[j] + speed - 1) / speed; // ceil()
        }

        // If koko can finish in h, try smaller spped
        if(timeToFinish <= h) {
            result = speed;
            maxSpeed = speed - 1;
        }
        // Otherwise, need higher speed
        else{
            minSpeed = speed + 1;
        }
    }

    return result;
}
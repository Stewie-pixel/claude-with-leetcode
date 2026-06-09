// -----------------------------------------------------------------------------
// Problem: Median of Two Sorted Arrays
// Source : https://leetcode.com/problems/median-of-two-sorted-arrays/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- DESCRIPTION ----------------------------------
//
// Given two sorted arrays nums1 and nums2 of size m and n respectively, 
// return the median of the two sorted arrays.
// The overall run time complexity should be O(log (m+n)).
//
// ----------------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Median = element at middle position when both arrays are merged.
// 2️. We can use Binary Search on the smaller array to find a partition:
//       i + j = (n1 + n2) / 2
//     such that:
//       nums1[i-1] <= nums2[j] && nums2[j-1] <= nums1[i]
// 3️. Once found:
//       - If total is even: median = average(maxLeft, minRight)
//       - If total is odd : median = maxLeft
//
// -----------------------------------------------------------------------------


#define INF 1e9

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    int n1 = nums1Size, n2 = nums2Size;
    if(n1 > n2) {
        int *temp = nums1;
        nums1 = nums2;
        nums2 = temp;
        int tmp = n1;
        n1 = n2;
        n2 = tmp;
    }

    int left = 0, right = (n2 > n1 ? n1 : n2);
    int total = n1 + n2;
    int half = total / 2;
    while(1) {
        int i = (left + right) / 2; // a
        int j = half - i; // b

        int aLeft = (i > 0) ? nums1[i - 1] : -INF;
        int aRight = i < n1 ?  nums1[i] : INF;
        int bLeft = (j > 0) ? nums2[j - 1] : -INF;
        int bRight = j < n2 ?  nums2[j] : INF;

        if(aLeft <= bRight && bLeft <= aRight) {
            return total % 2 == 0 ? ((double)(max(aLeft, bLeft) + min(aRight, bRight)) / 2) : min(aRight, bRight);
        }
        else if(aLeft > bRight) {
            right = i - 1;
        }
        else {
            left = i + 1;
        }
    }

}
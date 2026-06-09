// Source: https://leetcode.com/problems/longest-consecutive-sequence/
// Author: Ngoc-Tien To

// Idea:
// 1. Insert all numbers from nums into a hash set to maintain unique elements and allow O(1) lookup.
// 2. Traverse each number in the set:
//      2.1 If (num - 1) exists in the set, skip (since it's not the start of a sequence).
//      2.2 If (num - 1) does not exist, then (num) is the start of a sequence.
//          From num, check all consecutive numbers (num + 1, num + 2, ...) and count the length.
// 3. Keep track of the maximum length among all consecutive sequences and return it.

class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> s(nums.begin(), nums.end());
        int maxLen = 0;
        for(const auto& num : s){
            if(s.find(num - 1) == s.end()){
                int next = num + 1;
                while(s.find(next) != s.end()){
                    next++;
                }
                maxLen = max(next - num, maxLen);
            }
        }
        return maxLen;
    }
};
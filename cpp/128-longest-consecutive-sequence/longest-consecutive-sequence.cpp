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

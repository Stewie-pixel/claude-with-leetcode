class Solution {
public:
    int minimumCost(vector<int>& nums, int k) {
        const long long MOD = 1e9 + 7;
        
        long long available = k;
        long long opCount = 0;
        long long totalCost = 0;
        
        for (int i = 0; i < (int)nums.size(); i++) {
            long long need = nums[i];
            if (available < need) {
                long long find = need - available;
                long long t = (find + k - 1) / k;
                
                long long costAdd = ( (t % MOD) * (opCount % MOD) % MOD 
                                     + (t % MOD) * ((t + 1) % MOD) % MOD * ((MOD + 1) / 2) % MOD 
                                    ) % MOD;
                
                totalCost = (totalCost + costAdd) % MOD;
                
                opCount += t;
                available += t * (long long)k;
            }
            available -= need;
        }
        
        return (int)(totalCost % MOD);
    }
};
class Solution {
public:
    int gcd(int a, int b) {
        if (a == 0 && b == 0) {
            return 0;
        }

        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    long long gcdSum(vector<int>& nums) {
        int n = nums.size();
        vector<int> prefixGcd(n);

        long long mx_i = 0;
        for (int i = 0; i < n; i++) {
            mx_i = max(mx_i, (long long)nums[i]);
            prefixGcd[i] = gcd(nums[i], mx_i);
        }

        sort(prefixGcd.begin(), prefixGcd.end());
        
        long long ans = 0;
        int l = 0, r = n - 1;

        while (l < r) {
            ans += gcd(prefixGcd[l], prefixGcd[r]);
            l++;
            r--;
        }
        return ans;
    }
};
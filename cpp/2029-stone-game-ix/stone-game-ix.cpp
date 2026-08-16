class Solution {
public:
    bool stoneGameIX(vector<int>& stones) {
        int count[3] = {0, 0, 0};

        for (int i = 0; i < stones.size(); i++) {
            stones[i] %= 3;
            count[stones[i]]++;
        }

        if (count[0] % 2 == 0) {
            return count[1] != 0 && count[2] != 0;
        }

        return abs(count[2] - count[1]) >= 3;
    }
};
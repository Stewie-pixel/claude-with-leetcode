class Solution {
public:
    bool lemonadeChange(vector<int>& bills) {
        int five, ten = 0;

        for (int b : bills) {
            if (b == 5) {
                five++;
            }
            else if (b == 10) {
                if (five == 0) return false;
                five--;
                ten++;
            }
            else {
                if (ten > 0 && five > 0) {
                    ten--;
                    five--;
                } else if (five >= 3) {
                    five -= 3;
                } else {
                    return false;
                }
            }
        }
        return true;
    }
};
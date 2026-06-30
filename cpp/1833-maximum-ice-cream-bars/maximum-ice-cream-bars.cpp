class Solution {
public:
    int maxIceCream(vector<int>& costs, int coins) {
        int maxCost = *max_element(costs.begin(), costs.end());
        vector<int> freq(maxCost + 1, 0);

        for (int cost : costs) {
            freq[cost]++;
        }

        int result = 0;

        for (int price = 1; price <= maxCost; price++) {
            int buy = min(freq[price], coins / price);
            result += buy;
            coins -= buy * price;

            if (coins < price) {
                break;
            }
        }

        return result;
    }
};
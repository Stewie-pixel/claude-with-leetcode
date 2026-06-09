int maxProfit(int* prices, int pricesSize) {
    int profit = 0;
    int buyPrice = prices[0];

    for(int i = 0; i < pricesSize; i++){

        buyPrice = prices[i] < buyPrice ? prices[i] : buyPrice;

        profit = prices[i] - buyPrice > profit ? prices[i] - buyPrice : profit;
    }

    return profit;
}

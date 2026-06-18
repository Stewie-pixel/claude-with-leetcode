class Solution {
public:
    int minimumCost(vector<int>& cost) {
        priority_queue<int> maxheap;

        for (int x : cost) {
            maxheap.push(x);
        }

        int total = 0;
        while (!maxheap.empty()) {
            total += maxheap.top();
            maxheap.pop();

            if (!maxheap.empty()) {
                total += maxheap.top();
                maxheap.pop();
            }

            if (!maxheap.empty()) {
                maxheap.pop();
            }
        }
        return total;
    }
};
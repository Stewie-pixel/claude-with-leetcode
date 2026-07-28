class Solution {
public:
    int uniqueXorTriplets(vector<int>& nums) {
        const int LIMIT = 2048;
        vector<bool> present(LIMIT, false);
        for (int v : nums) present[v] = true;

        vector<int> V;
        for (int v = 0; v < LIMIT; v++) if (present[v]) V.push_back(v);

        vector<bool> pairXor(LIMIT, false);
        int m = V.size();
        for (int i = 0; i < m; i++) {
            for (int j = i; j < m; j++) {
                pairXor[V[i] ^ V[j]] = true;
            }
        }

        vector<bool> result(LIMIT, false);
        for (int x = 0; x < LIMIT; x++) {
            if (!pairXor[x]) continue;
            for (int y : V) {
                result[x ^ y] = true;
            }
        }

        int count = 0;
        for (int x = 0; x < LIMIT; x++) if (result[x]) count++;
        return count;
    }
};
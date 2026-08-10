class Solution {
public:
    vector<int> validSequence(string word1, string word2) {
        int N = word1.size();
        int M = word2.size();
        int R = M - 1;
        int C = 0;
        vector<int> Right(N);
        for (int i = N - 1; i >= 0; i--) {
            Right[i] = C;
            if (R >= 0 && word1[i] == word2[R]) {
                R--;
                C++;
            }
        }

        vector<int> ans;
        bool changed = false;
        int j = 0;

        for (int i = 0; i < N && j < M; i++) {
            if (word1[i] == word2[j]) {
                ans.push_back(i);
                j++;
            } else if (!changed && Right[i] >= M - 1 - j) {
               
                ans.push_back(i);
                j++;
                changed = true;
            }
        }

        if (j == M) {
            return ans;
        }
        return {};
    }
};
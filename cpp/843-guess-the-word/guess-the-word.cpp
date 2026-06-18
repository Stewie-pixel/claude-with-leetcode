/**
 * // This is the Master's API interface.
 * // You should not implement it, or speculate about its implementation
 * class Master {
 *   public:
 *     int guess(string word);
 * };
 */
class Solution {
private:
    int getMatches(const string& w1, const string& w2) {
        int matches = 0;
        for (int i = 0; i < 6; i++) {
            if (w1[i] == w2[i]) {
                matches++;
            }
        }
        return matches;
    }

public:
    void findSecretWord(vector<string>& words, Master& master) {
        vector<string> pool = words;

        for (int turn = 0; turn <= 30; turn++) {
            if (pool.empty()) break;

            int count[6][26] = {0};
            for (const string& w : pool) {
                for (int i = 0; i < 6; i++) {
                    count[i][w[i] - 'a']++;
                }
            }

            string best_guess = pool[0];
            int max_score = -1;
            for (const string& w : pool) {
                int score = 0;
                for (int i = 0; i < 6; i++) {
                    score += count[i][w[i] - 'a'];
                }
                if (score > max_score) {
                    max_score = score;
                    best_guess = w;
                }
            }

            int matches = master.guess(best_guess);
            if (matches == 6) return;

            vector<string> next_pool;
            for (const string& w : pool) {
                if (getMatches(best_guess, w) == matches) {
                    next_pool.push_back(w);
                }
            }
            pool = move(next_pool);
        } 
    }
};
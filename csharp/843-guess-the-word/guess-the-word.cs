/**
 * // This is the Master's API interface.
 * // You should not implement it, or speculate about its implementation
 * class Master {
 *     public int Guess(string word);
 * }
 */
class Solution {
    private int GetMatches(string w1, string w2) {
        int matches = 0;
        for (int i = 0; i < 6; ++i) {
            if (w1[i] == w2[i]) matches++;
        }
        return matches;
    }
    public void FindSecretWord(string[] words, Master master) {
        var pool = new List<string>(words);

        for (int turn = 0; turn < 30; ++turn) {
            if (pool.Count == 0) break;

            int[,] count = new int[6, 26];
            foreach (var w in pool) {
                for (int i = 0; i < 6; ++i) {
                    count[i, w[i] - 'a']++;
                }
            }

            string bestGuess = pool[0];
            int maxScore = -1;
            foreach (var w in pool) {
                int score = 0;
                for (int i = 0; i < 6; ++i) {
                    score += count[i, w[i] - 'a'];
                }
                if (score > maxScore) {
                    maxScore = score;
                    bestGuess = w;
                }
            }

            int matches = master.Guess(bestGuess);
            if (matches == 6) return;

            var nextPool = new List<string>();
            foreach (var w in pool) {
                if (GetMatches(bestGuess, w) == matches) {
                    nextPool.Add(w);
                }
            }
            pool = nextPool;
        }
    }
}
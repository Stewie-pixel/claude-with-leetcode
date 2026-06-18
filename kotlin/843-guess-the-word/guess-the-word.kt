/**
 * // This is the Master's API interface.
 * // You should not implement it, or speculate about its implementation
 * interface Master {
 *     fun guess(word: String): Int {}
 * }
 */
class Solution {
    private fun getMatches(w1: String, w2: String): Int {
        var matches = 0
        for (i in 0 until 6) {
            if (w1[i] == w2[i]) matches++
        }
        return matches
    }

    fun findSecretWord(words: Array<String>, master: Master) {
        var pool = words.toList()

        for (turn in 0 until 30) {
            if (pool.isEmpty()) break

            val count = Array(6) { IntArray(26) }
            for (w in pool) {
                for (i in 0 until 6) {
                    count[i][w[i] - 'a']++
                }
            }

            var bestGuess = pool[0]
            var maxScore = -1
            for (w in pool) {
                var score = 0
                for (i in 0 until 6) {
                    score += count[i][w[i] - 'a']
                }
                if (score > maxScore) {
                    maxScore = score
                    bestGuess = w
                }
            }

            val matches = master.guess(bestGuess)
            if (matches == 6) return

            val nextPool = mutableListOf<String>()
            for (w in pool) {
                if (getMatches(bestGuess, w) == matches) {
                    nextPool.add(w)
                }
            }
            pool = nextPool
        }
    }
}
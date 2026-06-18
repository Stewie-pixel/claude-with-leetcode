/**
 * // This is the Master's API interface.
 * // You should not implement it, or speculate about its implementation
 * type Master struct {
 * }
 *
 * func (this *Master) Guess(word string) int {}
 */
func getMatches(w1, w2 string) int {
    matches := 0
    for i := 0; i < 6; i++ {
        if w1[i] == w2[i] {
            matches++
        }
    }
    return matches
}

func findSecretWord(words []string, master *Master) {
    pool := append([]string{}, words...)

    for turn := 0; turn < 30; turn++ {
        if len(pool) == 0 {
            break
        }

        count := make([][]int, 6)
        for i := range count {
            count[i] = make([]int, 26)
        }

        for _, w := range pool {
            for i := 0; i < 6; i++ {
                count[i][w[i]-'a']++
            }
        }

        bestGuess := pool[0]
        maxScore := -1
        for _, w := range pool {
            score := 0
            for i := 0; i < 6; i++ {
                score += count[i][w[i]-'a']
            }
            if score > maxScore {
                maxScore = score
                bestGuess = w
            }
        }

        matches := master.Guess(bestGuess)
        if matches == 6 {
            return
        }

        nextPool := make([]string, 0)
        for _, w := range pool {
            if getMatches(bestGuess, w) == matches {
                nextPool = append(nextPool, w)
            }
        }
        pool = nextPool
    }
}
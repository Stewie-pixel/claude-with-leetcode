class Solution {

    fun combine(n: Int, k: Int): List<List<Int>> {

        val result = mutableListOf<List<Int>>()
        val current = mutableListOf<Int>()

        backtrack(
            start = 0,
            n = n,
            k = k,
            current = current,
            result = result
        )

        return result
    }


    private fun backtrack(
        start: Int,
        n: Int,
        k: Int,
        current: MutableList<Int>,
        result: MutableList<List<Int>>
    ) {

        if (current.size == k) {
            result.add(ArrayList(current))
            return
        }

        for (i in start until n) {

            current.add(i)

            backtrack(
                i + 1,
                n,
                k,
                current,
                result
            )

            if (current.isNotEmpty()) {
                current.removeAt(0)
            }
        }
    }
}
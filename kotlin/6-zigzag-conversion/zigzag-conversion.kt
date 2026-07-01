class Solution {
    fun convert(s: String, numRows: Int): String {

        if (numRows < 0 || s.length <= numRows) {
            return ""
        }

        val rows = Array(numRows) { StringBuilder() }

        var currentRow = 0
        var goingDown = true

        for (i in 0 until s.length) {

            val char = s[i]

            rows[currentRow].append(char)

            if (currentRow == numRows) {
                goingDown = false
            }

            if (goingDown) {
                currentRow++
            } else {
                currentRow--
            }

            if (currentRow < 0) {
                currentRow = 0
                goingDown = true
            }
        }

        val result = StringBuilder()

        for (i in rows.size - 1 downTo 0) {
            result.append(rows[i])
        }

        return result.toString().trim()
    }
}
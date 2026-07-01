class Solution {
    fun intToRoman(num: Int): String {

        if (num <= 0) {
            return ""
        }

        val values = intArrayOf(
            1, 5, 10, 50, 100, 500, 1000
        )

        val symbols = arrayOf(
            "I", "V", "X", "L", "C", "D", "M"
        )

        var number = num
        val result = StringBuilder()

        var index = 0

        while (number > 0) {

            if (number >= values[index]) {

                result.append(symbols[index])

                number -= values[index]

            } else {

                index++

            }

            if (index >= values.size) {
                break
            }
        }

        return result.toString()
    }
}
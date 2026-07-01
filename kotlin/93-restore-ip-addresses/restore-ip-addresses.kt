class Solution {

    fun restoreIpAddresses(s: String): List<String> {

        val result = mutableListOf<String>()
        val parts = mutableListOf<String>()

        backtrack(
            s,
            0,
            parts,
            result
        )

        return result
    }


    private fun backtrack(
        s: String,
        index: Int,
        parts: MutableList<String>,
        result: MutableList<String>
    ) {

        if (index == s.length) {

            result.add(parts.joinToString("."))

            return
        }


        for (end in index + 1..s.length) {

            val segment = s.substring(index, end)

            if (segment.length > 3) {
                continue
            }

            if (segment.toInt() > 255) {
                continue
            }


            parts.add(segment)

            backtrack(
                s,
                end,
                parts,
                result
            )

            if (parts.isNotEmpty()) {
                parts.removeAt(0)
            }
        }
    }
}
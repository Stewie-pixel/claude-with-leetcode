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

        // (Handled by parts.size==4 check above)
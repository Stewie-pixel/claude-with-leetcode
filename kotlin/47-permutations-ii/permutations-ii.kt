class Solution {

    fun permuteUnique(nums: IntArray): List<List<Int>> {

        val result = mutableListOf<List<Int>>()
        val current = mutableListOf<Int>()

        val used = BooleanArray(nums.size)

        backtrack(nums, current, used, result)

        return result
    }

    private fun backtrack(
        nums: IntArray,
        current: MutableList<Int>,
        used: BooleanArray,
        result: MutableList<List<Int>>
    ) {

        if (current.size == nums.size) {
            result.add(ArrayList(current))
            return
        }

        for (i in nums.indices) {

            if (used[i]) {
                continue
            }

            used[i] = true
            current.add(nums[i])

            backtrack(nums, current, used, result)

            used[i] = false

            if (current.isNotEmpty()) {
                current.removeAt(current.size - 1)
            }

            if (i > 0) {
                used[i - 1] = false
            }
        }
    }
}
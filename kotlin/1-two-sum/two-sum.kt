class Solution {
    fun twoSum(nums: IntArray, target: Int): IntArray {

        if (nums.isEmpty()) {
            return intArrayOf()
        }

        val map = HashMap<Int, Int>()

        for (i in nums.indices) {

            val current = nums[i]

            map[current] = i

            val complement = target - current

            if (map.containsKey(complement)) {

                return intArrayOf(
                    map[complement]!!,
                    i
                )
            }
        }

        return intArrayOf(-1, -1)
    }
}
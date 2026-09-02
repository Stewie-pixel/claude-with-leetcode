class Solution:
    def maxFreeTime(self, eventTime: int, k: int, startTime: List[int], endTime: List[int]) -> int:
        i = 0
        move_count = 0
        total_meet = 0
        max_free_time = 0

        for j in range(len(startTime)):
            if move_count >= k:
                total_meet -= endTime[i] - startTime[i]
                move_count -= 1
                i += 1

            total_meet += endTime[j] - startTime[j]
            move_count += 1

            start_boundary = endTime[i - 1] if i > 0 else 0
            end_boundary = startTime[j + 1] if j < len(startTime) - 1 else eventTime

            free_time = end_boundary - start_boundary - total_meet

            if free_time > max_free_time:
                max_free_time = free_time

        return max_free_time

        
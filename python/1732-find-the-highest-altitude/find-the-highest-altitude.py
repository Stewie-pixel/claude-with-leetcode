class Solution:
    def largestAltitude(self, gain):
        """
        :type gain: list[int]
        :rtype: int
        """

        altitudes = [0]
        i = 0
        while i < len(gain):
            altitudes.append(altitudes[i]+gain[i])
            i+=1

        return max(altitudes)

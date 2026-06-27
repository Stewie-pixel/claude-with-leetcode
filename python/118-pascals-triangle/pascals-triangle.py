class Solution:
    def generate(self, numRows):
        """
        :type numRows: int
        :rtype: List[List[int]]
        """
        res = [[] for _ in range(numRows)]
        for i in range(numRows):
            if i == 0:
                res[0] = [1]
                continue
            elif i == 1:
                res[1] = [1,1]
                continue
            else:
                tmp = [0] * (i+1)
                tmp[0], tmp[-1] = 1,1
                for j in range(1,i):
                    tmp[j] = res[i-1][j] + res[i-1][j-1]

                res[i] = tmp
        return res
class Solution:
    def findWords(self, words):
        """
        :type words: List[str]
        :rtype: List[str]
        """

        l1 = "qwertyuiop"
        l2 = "asdfghjkl"
        l3 = "zxcvbnm"
        res = []
        for word in words:
            w = word.lower()
            if len(set(l1 + w)) == len(l1) or len(set(l2 + w)) == len(l2) or len(set(l3 + w)) == len(l3):
                res.append(word)
        return res

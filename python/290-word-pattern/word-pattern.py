class Solution:
    def wordPattern(self, pattern, s) -> bool:
        """
        :type pattern: str
        :type s: str
        :rtype: bool
        """
        map_word = dict()
        check = set()
        s = s.split(' ')

        if len(s) != len(pattern):
            return False

        for i in range(len(s)):
            if s[i] not in map_word:
                if pattern[i] in check:
                    return False
                map_word[s[i]] = pattern[i]
                check.add(pattern[i])

            if pattern[i] != map_word[s[i]]:
                return False

        return True
    
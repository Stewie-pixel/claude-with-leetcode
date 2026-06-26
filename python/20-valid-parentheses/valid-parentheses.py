class Solution:
    def isValid(self, s: str) -> bool:
        """
        :type s: str
        :rtype: bool
        """
        mapping = {
            "(": ")",
            "{": "}",
            "[": "]"
        }

        st = []

        for ch in s:
            # Push opening brackets onto the stack
            if ch in mapping:
                st.append(ch)

            # Process closing brackets
            elif ch in mapping.values():
                # No matching opening bracket
                if not st:
                    return False

                # Get the most recent opening bracket
                top = st[-1]

                # Current closing bracket does not match the expected one
                if ch != mapping[top]:
                    return False

                # Remove the matched opening bracket
                st.pop()

        # Valid only if all opening brackets have been matched
        return not st
    

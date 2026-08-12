class Solution:
    def checkGoodInteger(self, n: int) -> bool:
        digit_sum = 0
        square_sum = 0

        while n!=0:
            digit = n%10
            digit_sum+=digit
            square_sum+= digit**2
            n//=10
        if square_sum - digit_sum >=50:
            return True
        else:
            return False
        
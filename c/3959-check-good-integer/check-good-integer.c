bool checkGoodInteger(int n) {
    int digit_sum = 0;
    int square_sum = 0;
    while (n!=0){
        int digit = n%10;
        digit_sum+=digit;
        square_sum+=digit*digit;
        n/=10;
    }
    if ((square_sum-digit_sum) >= 50) return 1;
    else return 0;
}
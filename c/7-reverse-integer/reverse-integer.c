#define LIM1 (-2147483648)
#define LIM2 2147483647

int reverse(int x){
    int res = 0;
    while(x != 0){
        if(res < (LIM1 / 10) || res > (LIM2 / 10)){
            return 0;
        }
        res = res * 10 + x % 10;
        x /= 10;
    }
    return res;
}

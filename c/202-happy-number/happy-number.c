#include <stdbool.h>

int square(int a){
    return a * a;
}

int sumDigits(int n){
    int res = 0;
    while(n){
        res += square(n % 10);
        n /= 10;
    }
    return res;
}

bool isHappy(int n) {
    if(n == 1 || n == 7){
        return true;
    }
    else if(n < 10){
        return false;
    }
    while(1){
        if(n == 89){
            return false;
        }
        else if(n == 1){
            return true;
        }
        n = sumDigits(n);
    }
    return n == 1 || n == 7;
}

double getPow(double x, int n){
    if(n == 0){
        return (double)1;
    }
    double temp = getPow(x, n / 2);
    if(n % 2 == 1){
        return x * temp * temp;
    }
    return temp * temp;
}

double myPow(double x, int n) {
    if(n >= 0){
        return getPow(x, n);
    }
    return (double)1 / (getPow(x, -n - 1) * getPow(x, 1));
}

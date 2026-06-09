int maxElement(int *piles, int n) {
    int maxValue = 0;
    for(int i = 0; i < n; i++) {
        maxValue = (piles[i] > maxValue) ? piles[i] : maxValue;
    }
    return maxValue;
}

int minEatingSpeed(int* piles, int pilesSize, int h) {
    int minSpeed = 1, maxSpeed = maxElement(piles, pilesSize);
    int result = maxSpeed;

    while(minSpeed <= maxSpeed) {
        int speed = (maxSpeed + minSpeed) / 2;
        long long timeToFinish = 0;

        for(int j = 0; j < pilesSize; j++) {
            timeToFinish += (piles[j] + speed - 1) / speed;
        }

        if(timeToFinish <= h) {
            result = speed;
            maxSpeed = speed - 1;
        }

        else{
            minSpeed = speed + 1;
        }
    }

    return result;
}

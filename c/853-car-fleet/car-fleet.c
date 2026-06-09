struct CarInfo{
    int position;
    float timeToTarget;
};

int compare(const void *a, const void *b){
    struct CarInfo *x = (struct CarInfo *)a;
    struct CarInfo *y = (struct CarInfo *)b;
    return y->position - x->position;
}

struct CarInfo *createCarInfoArray(int target, int *position, int *speed, int n){
    struct CarInfo *carInfo = (struct CarInfo *)malloc(sizeof(struct CarInfo) * n);
    for(int i = 0; i < n; i++){
        carInfo[i].position = position[i];
        carInfo[i].timeToTarget = (float)(target - position[i]) / speed[i];
    }
    qsort(carInfo, n, sizeof(struct CarInfo), compare);
    return carInfo;
}

int carFleet(int target, int* position, int positionSize, int* speed, int speedSize) {
    if(positionSize <= 1){
        return positionSize;
    }

    int nbFleet = 0;
    float lastTime = 0.0;
    struct CarInfo *carInfo = createCarInfoArray(target, position, speed, speedSize);

    for(int i = 0; i < speedSize; i++){
        if(carInfo[i].timeToTarget > lastTime){
            nbFleet++;
            lastTime = carInfo[i].timeToTarget;
        }
    }

    free(carInfo);
    return nbFleet;
}

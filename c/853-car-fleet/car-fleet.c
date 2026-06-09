// Car Fleet
// Source: https://leetcode.com/problems/car-fleet/
// Author: Ngoc-Tien To

// ----------------------------------DESCRIPTION----------------------------------
// There are n cars at given miles away from the starting mile 0, traveling to reach the mile target.

// You are given two integer arrays position and speed, both of length n, where position[i] is 
// the starting mile of the ith car and speed[i] is the speed of the ith car in miles per hour.

// A car cannot pass another car, but it can catch up and then travel next to it at the speed of the slower car.

// A car fleet is a single car or a group of cars driving next to each other. 
// The speed of the car fleet is the minimum speed of any car in the fleet.

// If a car catches up to a car fleet at the mile target, it will still be considered as part of the car fleet.

// Return the number of car fleets that will arrive at the destination.


// ----------------------------------IDEA----------------------------------
// After analyzing the problem, we realize that the cars are always positioned in order.
// Therefore, sorting the cars by their starting positions allows us to process them effectively.
//
// The key factor is each car's "time to reach the target".
// We iterate from the car closest to the destination backward:
//   - If the current car's timeToTarget <= the timeToTarget of the front group,
//     it will join that group (no new fleet is formed).
//   - Otherwise, a new fleet is created, and we update the timeToTarget for that new group.



// Store the position and needed time to target of each car
struct CarInfo{
    int position;
    float timeToTarget;
};

// Sort position in non-increasing order
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
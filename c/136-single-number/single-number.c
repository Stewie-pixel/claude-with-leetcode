int singleNumber(int* nums, int numsSize) {
    int check = 0;

    for(int i = 0; i < numsSize; i++){
        check ^= nums[i];
    }
    return check;
}

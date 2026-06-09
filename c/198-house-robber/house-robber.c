int max(int a, int b){
    return a > b ? a : b;
}

int rob(int* nums, int numsSize) {
    if(numsSize == 0){
        return 0;
    }
    if(numsSize == 1){
        return nums[0];
    }

    int rob1, rob2;
    rob1 = rob2 = 0;
    for(int i = 0; i < numsSize; i++){
        int temp = max(rob1 + nums[i], rob2);
        rob1 = rob2;
        rob2 = temp;
    }

    return rob2;
}

int* twoSum(int* numbers, int numbersSize, int target, int* returnSize) {
    int *result = (int *)malloc(sizeof(int) * 2);
    (*returnSize) = 2;
    int i = 0, j = numbersSize - 1;
    while(i < j){
        int sum = numbers[i] + numbers[j];
        if(sum > target){
            j--;
        }
        else if(sum < target){
            i++;
        }
        else{
            result[0] = i + 1;
            result[1] = j + 1;
            break;
        }
    }
    return result;
}

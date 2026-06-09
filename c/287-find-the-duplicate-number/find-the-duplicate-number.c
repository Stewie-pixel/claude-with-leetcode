int findDuplicate(int* nums, int numsSize) {

    int slow = 0, fast = 0;

    while(1){
        slow = nums[slow];
        fast = nums[nums[fast]];
        if(slow == fast){
            break;
        }
    }

    int slow2 = 0;
    while(1){
        slow2 = nums[slow2];
        slow = nums[slow];
        if(slow2 == slow){
            break;
        }
    }

    return slow;
}

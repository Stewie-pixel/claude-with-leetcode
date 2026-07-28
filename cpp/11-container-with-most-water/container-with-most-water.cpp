class Solution {
public:
    int maxArea(vector<int>& height) {
        int h = height.size();
        int left = 0, right = h - 1;
        int watermost = 0;

        while (left < right) {
            int heights = min(height[left], height[right]);
            int width = right - left;
            watermost = max(watermost, heights * width);

            if (height[left] < height[right]) {
                left++;
            } else {
                right--;
            }
        }
        return watermost;
    }
};
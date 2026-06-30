class Solution {
public:
    int maxNumberOfBalloons(string text) {
        int freq[5] = {0};

        for (char c : text) {
            if (c == 'b') freq[0]++;
            else if (c == 'a') freq[1]++;
            else if (c == 'l') freq[2]++;
            else if (c == 'o') freq[3]++;
            else if (c == 'n') freq[4]++;
        }

        freq[2] /= 2;
        freq[3] /= 2;

        return min({freq[0], freq[1], freq[2], freq[3], freq[4]});
    }
};
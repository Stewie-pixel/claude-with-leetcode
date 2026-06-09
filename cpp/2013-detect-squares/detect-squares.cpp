// -----------------------------------------------------------------------------
// Problem: Detect Squares (2013)
// Source : https://leetcode.com/problems/detect-squares/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------
//
// IDEA
// Maintain a count map for points added. To count squares with a given point
// (x,y), iterate over candidate points that share a row/column and check the
// three other corners using counts from the map to multiply combinations.
//
// Time Complexity  : O(k) per count where k is number of points in map
// Space Complexity : O(p) for p points stored
// -----------------------------------------------------------------------------

#include <map>
#include <vector>
#include <cmath>
using namespace std;

// -----------------------------------------------------------------------------
class DetectSquares {
private:
    map<pair<int, int>, int> mp;
public:
    DetectSquares() {
    }
    
    void add(vector<int> point) {
        mp[{point[0], point[1]}]++;
    }
    
    int count(vector<int> point) {
        int result = 0;
        int x = point[0];
        int y = point[1];
        for(const auto& p : mp){
            pair<int, int> it = p.first;
            if(it.first != x && it.second != y && abs(it.first - x) == abs(it.second - y)){
                int count1 = mp[{it.first, y}];
                int count2 = mp[{x, it.second}];
                int count3 = mp[{it.first, it.second}];
                result += count1 * count2 * count3;
            }
        }
        return result;
    }
};
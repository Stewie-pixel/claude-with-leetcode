#include <map>
#include <vector>
#include <cmath>
using namespace std;

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

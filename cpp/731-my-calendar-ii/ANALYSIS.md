```cpp
class MyCalendarTwo {
public:
    vector<pair<int, int>> booked;
    vector<pair<int, int>> overlap;

    MyCalendarTwo() {
        
    }
    
    bool book(int startTime, int endTime) {
        for (auto &p : overlap) {
            int start = max(startTime, p.first);
            int end = min(endTime, p.second);
            if (start < end) {
                return false;
            }
        }

        for (auto &p : booked) {
            int start = max(startTime, p.first);
            int end = min(endTime, p.second);
            if (start < end) {
                overlap.push_back({start, end});
            }
        }

        booked.push_back({startTime, endTime});
        return true;
    }
};

/**
 * Your MyCalendarTwo object will be instantiated and called as such:
 * MyCalendarTwo* obj = new MyCalendarTwo();
 * bool param_1 = obj->book(startTime,endTime);
 */
```

**YouTube Explanation (NeetCode):**  
https://www.youtube.com/watch?v=7utL5cTDcnA
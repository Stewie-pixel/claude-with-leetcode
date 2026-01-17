"""
Docstring for Leetcode 981

981. Time Based Key-Value Store

Design a time-based key-value data structure that can store multiple values for the same key at different time stamps and retrieve the key's value at a certain timestamp.

Implement the TimeMap class:

TimeMap() Initializes the object of the data structure.
void set(String key, String value, int timestamp) Stores the key key with the value value at the given time timestamp.
String get(String key, int timestamp) Returns a value such that set was called previously, with timestamp_prev <= timestamp. 
If there are multiple such values, it returns the value associated with the largest timestamp_prev. If there are no values, it returns "".
 

Example 1:

Input
["TimeMap", "set", "get", "get", "set", "get", "get"]
[[], ["foo", "bar", 1], ["foo", 1], ["foo", 3], ["foo", "bar2", 4], ["foo", 4], ["foo", 5]]
Output
[null, null, "bar", "bar", null, "bar2", "bar2"]

Explanation
TimeMap timeMap = new TimeMap();
timeMap.set("foo", "bar", 1);  // store the key "foo" and value "bar" along with timestamp = 1.
timeMap.get("foo", 1);         // return "bar"
timeMap.get("foo", 3);         // return "bar", since there is no value corresponding to foo at timestamp 3 and timestamp 2, then the only value is at timestamp 1 is "bar".
timeMap.set("foo", "bar2", 4); // store the key "foo" and value "bar2" along with timestamp = 4.
timeMap.get("foo", 4);         // return "bar2"
timeMap.get("foo", 5);         // return "bar2"
 

Constraints:

1 <= key.length, value.length <= 100
key and value consist of lowercase English letters and digits.
1 <= timestamp <= 107
All the timestamps timestamp of set are strictly increasing.
At most 2 * 105 calls will be made to set and get.

"""


"""
Description

This problem asks us to design a time-based key-value store that supports storing multiple values for the same key at different timestamps 
and retrieving the most recent value whose timestamp is less than or equal to a given query timestamp.

Intuition

Since timestamps for each key are strictly increasing, we can store all values for a key in chronological order. 
When retrieving a value for a given timestamp, we want the value with the largest timestamp ≤ query timestamp, which naturally suggests using binary search.

Approach

Use a dictionary where each key maps to a list of (value, timestamp) pairs.

On set, append the new pair to the list (timestamps are guaranteed to be increasing).

On get, perform a binary search on the timestamp list to find the rightmost timestamp that is less than or equal to the given timestamp.

Track the best valid value during the search and return it.

Time Complexity

set: O(1)

get: O(log n) per query, where n is the number of stored timestamps for the key.

Space Complexity

O(n) overall, where n is the total number of stored (key, value, timestamp) entries.

"""

class TimeMap:

    def __init__(self):
        self.seen = {}

    def set(self, key: str, value: str, timestamp: int) -> None:
        if key not in self.seen:
            self.seen[key] = []
        self.seen[key].append([value, timestamp])        

    def get(self, key: str, timestamp: int) -> str:
        if key not in self.seen:
            return ""

        res = ''
        values = self.seen.get(key, [])

        left, right = 0, len(values) - 1
        while left <= right:
            mid = (left + right) // 2

            if values[mid][1] <= timestamp:
                res = values[mid][0]
                left = mid + 1

            else:
                right = mid - 1

        return res

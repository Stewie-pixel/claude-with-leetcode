**Explanation**  
The `RecentCounter` needs to count how many `ping` calls have occurred in the last 3000 ms, inclusive. Because the calls arrive with strictly increasing timestamps, we can keep a queue of timestamps.  

When a new timestamp `t` arrives:
1. Push `t` onto the back of the queue.  
2. While the front of the queue is older than `t‑3000` (i.e., `q.front() < t-3000`), pop it – those calls are no longer in the sliding window.  
3. The size of the queue after discarding stale timestamps is exactly the number of pings in `[t‑3000, t]`.

Because each timestamp is pushed once and popped at most once, the amortized cost per `ping` is O(1). The queue holds at most the calls from the last 3000 ms, so the space usage is O(N) where N ≤ 10⁴ (the maximum number of calls).

**Complexity**  
- **Time:** O(1) amortized per `ping` (each timestamp is enqueued and dequeued once).  
- **Space:** O(N) where N ≤ 10⁴ (the maximum number of timestamps kept in the queue).

**Reference**  
- YouTube explanation (NeetCode): https://www.youtube.com/watch?v=JaV5y7p-MjU
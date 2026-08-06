class Solution {
public:
    vector<int> asteroidCollision(vector<int>& asteroids) {
        vector<int> st;

        for (int a : asteroids) {
            bool alive = true;

            while (a < 0 && !st.empty() && alive && st.back() > 0) {
                if (st.back() < -a) {
                    st.pop_back();
                } else if (st.back() == -a) {
                    st.pop_back();
                    alive = false;
                } else {
                    alive = false;
                }
            }

            if (alive) st.push_back(a);
        }
        return st;
    }
};
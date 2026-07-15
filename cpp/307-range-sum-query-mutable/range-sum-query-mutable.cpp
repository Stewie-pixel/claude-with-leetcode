class NumArray {
private:
    int n;
    vector<int> seg;

    void build(vector<int>& nums, int idx, int left, int right) {
        if (left == right) {
            seg[idx] = nums[left];
            return;
        }
        int mid = (left + right) / 2;
        build(nums, idx*2, left, mid);
        build(nums, idx*2+1, mid+1, right);
        seg[idx] = seg[idx*2] + seg[idx*2+1];
    }

    void updateTree(int idx, int left, int right, int pos, int val) {
        if (left == right) {
            seg[idx] = val;
            return;
        }
        int mid = (left + right) / 2;
        if (pos <= mid) {
            updateTree(idx*2, left, mid, pos, val);
        } else {
            updateTree(idx*2+1, mid+1, right, pos, val);
        }
        seg[idx] = seg[idx*2] + seg[idx*2+1];
    }

    int query(int idx, int left, int right, int ql, int qr) {
        if (qr < left || ql > right)
            return 0;
        if (ql <= left && right <= qr)
            return seg[idx];

        int mid = (left + right) / 2;
        return query(idx*2, left, mid, ql, qr) + query(idx*2+1, mid+1, right, ql, qr);
    }

public:
    NumArray(vector<int>& nums) {
        n = nums.size();
        seg.resize(4*n);
        build(nums, 1, 0, n-1);
    }
    
    void update(int index, int val) {
        updateTree(1, 0, n-1, index, val);
    }
    
    int sumRange(int left, int right) {
        return query(1, 0, n-1, left, right);
    }
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * obj->update(index,val);
 * int param_2 = obj->sumRange(left,right);
 */
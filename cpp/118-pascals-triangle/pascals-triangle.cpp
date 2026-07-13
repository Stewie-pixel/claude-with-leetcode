class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> res;

        for (int i = 0;i<numRows;i++){
            vector<int> tmp;
            res.push_back(tmp);
        }

        for (int i = 0;i<numRows;i++){
            if (i == 0){
                res[0].push_back(1);
                continue;
            }
            else if (i == 1){
                res[1].insert(res[1].begin(),{1,1});
                continue;
            }
            else{
                vector<int> tmp(i+1,0);
                tmp[0] = 1;
                tmp[tmp.size()-1] = 1;
                for (int j = 1;j<i;j++){
                    tmp[j] = res[i-1][j] + res[i-1][j-1];
                    res[i] = tmp;
                }
            }
        }
            return res;

    }
};

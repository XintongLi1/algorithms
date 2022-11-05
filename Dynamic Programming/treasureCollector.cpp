// Final Version: see prog1.cpp or prog1.py
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std; 

int collectTreasure(int & n, int & m, int & h, vector<pair<int, int>>& treasure) {  // n = treasure.size()
    if (h > n * 1.5) { // floor contraction
        vector<int> floors;
        for (auto & pair : treasure) {
            floors.push_back(pair.second);
        }
        sort(floors.begin(), floors.end());
        map<int, int> contractFloor;
        int i = 1;
        for (int f = 0; f < floors.size(); ++f) {
            if (f && floors[f] > floors[f - 1]) ++i;
            contractFloor[floors[f]] = i;
        }
        h = contractFloor.size();
        for (auto & pair : treasure) {
            pair.second = contractFloor[pair.second];
        }
    }

    vector<vector<pair<int, int>>> dp(m + 1, vector<pair<int, int>>(h + 1, make_pair(0, -1)));  // <val, index>
    for (int i = 0; i < n; ++i) {
        for (int j = m; j >= 1; --j) {
            int k = treasure[i].second;
            for (int l = k; l <= k; ++l) {
                if (dp[j][k].first < dp[j - 1][l].first + treasure[i].first ){ //&& dp[j - 1][l].second < i) {
                    dp[j][k] = make_pair(dp[j - 1][l].first + treasure[i].first, i);
                    //break;
                }
            }
            //if (dp[j - 1][k].first > dp[j][k].first) dp[j][k] = dp[j - 1][k];

                 cout << "i = " << i << ": dp[" << j << "][" 
                    << k << "] = (" << dp[j][k].first << ", "
                    << dp[j][k].second << ")" << endl;

            for (k = treasure[i].second + 1; k <= h; ++k) {
                if (dp[j][k].first < dp[j][k - 1].first) {
                    dp[j][k] = dp[j][k - 1];
                    cout << "i = " << i << ": dp[" << j << "][" 
                    << k << "] = (" << dp[j][k].first << ", "
                    << dp[j][k].second << ")" << endl;
                } else
                    break;
            }

            if (i == n - 1) break;
        }
    }

    for (int i = 1; i < m + 1; ++i){
        for (int j = 1; j < h + 1; ++j){ 
            cout << "dp[" << i << "][" << j << "] = (" << dp[i][j].first << "," << dp[i][j].second << "), ";
        }
        cout << endl;
    }

    return dp[m][h].first;
}

int main() {
    /*
    vector<pair<int, int>> input{{14, 3}, {10, 5}, {17, 1}, {10, 2}};
    int n = 4, m = 3, h = 10;
    */
    /*
    vector<pair<int, int>> input{{2, 32}, {8, 13}, {9, 83}};
    int n = 3, m = 2, h = 100;
    */
    /*
    vector<pair<int, int>> input{{8, 3}, {6, 5}, {1, 1}, {6, 4}, {9, 2}};
    int n = 5, m = 2, h = 10; // 14
    
    vector<pair<int, int>> input{{1, 3}, {2, 3}, {3, 3}, {4, 4}};
    int n = 4, m = 3, h = 10; // 9
    
    vector<pair<int, int>> input{{8, 3}, {6, 5}, {10, 4}, {6, 4}, {9, 3}, {20, 3}};
    int n = 6, m = 4, h = 10; //37
    */
   vector<pair<int, int>> input{{7, 5}, {5, 5}, {11, 4}, {11, 5}, {10, 4}, {10, 74}};
    int n = 6, m = 3, h = 76;  // 32
    
    cout << collectTreasure(n, m, h, input) << endl;
}

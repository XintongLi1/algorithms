#include <algorithm>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
    int n = startTime.size();
    map<int, int> dp = {{0, 0}};
    vector<vector<int>> jobs;
    for (int i = 0; i < n; ++i) {
        jobs.push_back({startTime[i], endTime[i], profit[i]});
    }
    sort(jobs.begin(), jobs.end(), [](auto & a, auto & b){ return a[1] < b[1]; });
    for (auto & job : jobs) {
        int cur = prev(dp.upper_bound(job[0]))->second + job[2];
        if (cur > dp.rbegin()->second) dp[job[1]] = cur;
    }
    return dp.rbegin()->second;
}

int main(){
    vector<int> startTime ({1,2,3,3});
    vector<int> endTime ({3,4,5,6});
    vector<int> profit ({50,10,40,70});
    cout << jobScheduling(startTime, endTime, profit) << endl;
}

/*---------------------------------------------------------------
Alternative method for calculating prev(dp.upper_bound(job[0])):

        map<vector<int>, vector<int>> prev;
        vector<vector<int>> jobs_b;

        for (int i = 0; i < n; ++i) {
            jobs_b.push_back({startTime[i], endTime[i], profit[i]});
        }

        sort(jobs_b.begin(), jobs_b.end());

        for (int i = n - 1, j = n - 1; i >= 0; --i) {
            while (jobs_b[i][0] > jobs[j][1]) --j;
        }
        prev[jobs_b[i]] = (j >= 0) ? jobs[j] : {0, 0, 0};
*/

#include <vector>
#include <map>

using namespace std;

class Solution
{
public:
    vector<int> twoSum(vector<int> &nums, int target)
    {
        map<int, int> T;
        vector<int> rt;
        int n = nums.size();

        for (int i = 0; i < n; i++)
        {
            if (T.find(target - nums[i]) != T.end())
            {
                rt.push_back(T[target - nums[i]]);
                rt.push_back(i);
                break;
            }
            T.insert(pair<int, int>(nums[i], i));
        }
        return rt;
    }
};

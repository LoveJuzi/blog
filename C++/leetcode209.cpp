// 1. 输入target是个正整数
// 2. 输入的数组序列是个正整数序列
// 3. 返回的最短数组子序列是个连续的子序列
// 4. 最短子序列的元素和是大于等于给定的target
// 5. 如果一个子序列符合要求，需要这个子序列尽可能的短
// 6. 返回所有可能解中的最短子序列
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int a = 0;
        int sum = 0;
        int d = 0;
        int rt = nums.size() + 1;
        
        for (auto i=0; i<nums.size(); i++) {
            sum += nums[i];
            while (sum >= target && a <= i) {
                d = i - a + 1;
                if (rt > d) {
                    rt = d;
                }
                sum -= nums[a];
                a++;
            }
        }
        
        return rt == nums.size() + 1 ? 0 : rt;
    }
};

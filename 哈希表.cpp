#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        // 创建一个哈希表（unordered_map），用来存储 数值 -> 下标 的映射
        unordered_map<int, int> pos;

        // 遍历数组
        for (int i = 0; i < (int)nums.size(); i++) {
            int need = target - nums[i]; // 需要找到的另一个数

            // 检查哈希表中是否有需要的数
            if (pos.count(need)) {
                // 如果找到了，返回这两个数的下标
                return {pos[need], i};
            }

            // 如果没找到，把当前数和它的下标存入哈希表
            pos[nums[i]] = i;
        }

        // 题目保证一定有答案，这里不会执行
        return {};
    }
};

int main() {
    Solution s;
    vector<int> nums = {2,7,11,15};
    int target = 9;
    vector<int> ans = s.twoSum(nums, target);
    cout << "[" << ans[0] << ", " << ans[1] << "]" << endl;
    return 0;
}

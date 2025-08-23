# 寻找中位数(O(m+n))

**题目**：给定两个大小分别为 `m` 和 `n` 的正序（从小到大）数组 `nums1` 和 `nums2`。请你找出并返回这两个正序数组的 **中位数** 。

算法的时间复杂度应该为 `O(log (m+n))` 。

**示例 1：**

```
输入：nums1 = [1,3], nums2 = [2]
输出：2.00000
解释：合并数组 = [1,2,3] ，中位数 2
```

**示例 2：**

```
输入：nums1 = [1,2], nums2 = [3,4]
输出：2.50000
解释：合并数组 = [1,2,3,4] ，中位数 (2 + 3) / 2 = 2.5
```



    #include <vector>
    using namespace std;
    
    class Solution {
    public:
        double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
            int m = nums1.size();
            int n = nums2.size();
        // 存放合并后的有序序列；提前reserve避免多次扩容搬家
        vector<int> merged;
        merged.reserve(m + n);
    
        int i = 0, j = 0; // 双指针，分别指向 nums1 与 nums2 的当前下标
    
        // ——核心：像归并排序那样合并两个有序数组到 merged——
        while (i < m && j < n) {
            if (nums1[i] < nums2[j]) {
                merged.push_back(nums1[i++]); // 小的先放进 merged，并右移对应指针
            } else {
                merged.push_back(nums2[j++]);
            }
        }
        // 把剩余的“尾巴”接上（两者最多只有一个非空）
        while (i < m) merged.push_back(nums1[i++]);
        while (j < n) merged.push_back(nums2[j++]);
    
        int total = m + n;
        if (total % 2 == 1) {
            // 总长度为奇数：中位数就是中间那个（0 基下标为 total/2）
            return merged[total / 2];
        } else {
            // 总长度为偶数：中位数是“中间两个数的平均值”
            int mid = total / 2;  // 右侧中间位置（左侧是 mid-1）
            // 注意：这里的相加先在 int 域进行，再/2.0 变 double
            // 如果担心整型溢出，可用 long long 强转或分开除 2.0（见下方“可改进点”）
            return (merged[mid - 1] + merged[mid]) / 2.0;
        }
    }
    };


## 1、算法流程（你可以对照 while 循环看）

1. 两个数组都是升序。用指针 `i`、`j` 指向当前未合并的最小元素。
2. 比较 `nums1[i]` 与 `nums2[j]`，较小者推入 `merged`，对应指针右移。
3. 有一个数组先用完时，把另一个数组剩余元素整体追加到 `merged`。
4. 根据 `merged` 的总长度：
   - 若奇数：取 `merged[total/2]`
   - 若偶数：取 `(merged[total/2-1] + merged[total/2]) / 2.0`

### 例 1

```
nums1 = [1,3], nums2 = [2]
i=0,j=0: 比较 1 和 2 → push 1（i=1）
i=1,j=0: 比较 3 和 2 → push 2（j=1）
j==n 了 → 把 nums1 剩余 3 追加
merged = [1,2,3]，total=3 → 奇数，中位数 merged[1]=2
```

### 例 2

```
nums1 = [1,2], nums2 = [3,4]
i=0,j=0: 1<3 → push 1（i=1）
i=1,j=0: 2<3 → push 2（i=2）
i==m 了 → 追加 nums2 剩余 3,4
merged = [1,2,3,4]，total=4 → 偶数
mid=2 → (merged[1]+merged[2])/2 = (2+3)/2 = 2.5
```

------

## 2、复杂度与适用性

- 时间复杂度：**O(m+n)**（每个元素只看一次）
- 空间复杂度：**O(m+n)**（存了合并数组）
- 适合：不追求最优复杂度、但想写法直观、易于调试的场景
- 边界情况：
  - 一边为空：也能正常工作，等价于直接在另一边取中位数。
  - 两边都空：题意一般不会给这种输入；如需健壮性可在函数开头加判断并自定义行为。

------

## 3、可改进点（可选）

1. **避免整型溢出（偶数时求均值）**
    当前写法 `(merged[mid - 1] + merged[mid]) / 2.0` 在相加阶段是 `int`，极端大值相加可能溢出。更稳妥写法之一：

   ```
   return ( (long long)merged[mid - 1] + (long long)merged[mid] ) / 2.0;
   // 或者：
   // return merged[mid - 1] / 2.0 + merged[mid] / 2.0;
   ```

2. **降空间到 O(1)**（仍是 O(m+n) 时间）
    不创建 `merged`，而是在“合并过程”里只追踪当前“已合并元素的计数”和“落在中位数位置的元素”。当计数走到中位数对应下标时记录值即可。面试里常用。

   
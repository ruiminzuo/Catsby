# 寻找中位数

## 1.总体思路（为什么能做到 O(log(m+n))）

中位数其实就是**第 k 小**的数（或两个中间数的平均）：

- 设总长度 `L = m+n`
  - 若 `L` 为奇数，中位数 = 第 `(L+1)/2` 小的数
  - 若 `L` 为偶数，中位数 = 第 `L/2` 和第 `L/2+1` 小的数的平均

于是问题变成：**在两个有序数组里找第 k 小元素**。
 核心技巧：每一轮把“至少 k/2 个不可能的元素”丢掉，这样 `k` 迅速变小，复杂度就变成 `O(log(m+n))`。

------

## 2.函数接口与主流程

```
double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    int totalLength = nums1.size() + nums2.size();
    if (totalLength % 2 == 1) {
        return getKthElement(nums1, nums2, (totalLength + 1) / 2);
    } else {
        int left  = getKthElement(nums1, nums2, totalLength / 2);
        int right = getKthElement(nums1, nums2, totalLength / 2 + 1);
        return (left + right) / 2.0;
    }
}
```

- `getKthElement(..., k)`：返回“第 k 小元素”（注意：**k 从 1 开始**）。
- 奇数长度：直接找一个 k。
- 偶数长度：找两个相邻的 k，取平均。

------

## 3.关键函数：getKthElement

```
int getKthElement(const vector<int>& nums1, const vector<int>& nums2, int k) {
    int m = nums1.size(), n = nums2.size();
    int index1 = 0, index2 = 0;   // 当前“有效起点”（已丢弃 index 之前的元素）

    while (true) {
        // 1) 边界处理：有一边用完了 or k==1
        if (index1 == m) return nums2[index2 + k - 1];
        if (index2 == n) return nums1[index1 + k - 1];
        if (k == 1) return min(nums1[index1], nums2[index2]);

        // 2) 正常情况：看两边的第 k/2 个候选，丢掉较小一边的前半段
        int newIndex1 = min(index1 + k / 2, m) - 1;
        int newIndex2 = min(index2 + k / 2, n) - 1;
        int pivot1 = nums1[newIndex1];
        int pivot2 = nums2[newIndex2];

        if (pivot1 <= pivot2) {
            // 丢掉 nums1 的 [index1 .. newIndex1] 这一段
            k     -= (newIndex1 - index1 + 1);
            index1 =  newIndex1 + 1;
        } else {
            // 丢掉 nums2 的 [index2 .. newIndex2] 这一段
            k     -= (newIndex2 - index2 + 1);
            index2 =  newIndex2 + 1;
        }
    }
}
```

## 4.变量含义

- `index1` / `index2`：当前还**没被丢掉**的起始位置（相当于“窗口左边界”）。
- `newIndex1` / `newIndex2`：本轮想要比较的“第 `k/2` 个候选”的位置。
  - 为避免越界：`min(index + k/2, 长度) - 1`
  - 含义是：若剩余元素不足 `k/2`，就用“最后一个元素”来参与比较。

### 三个“边界”很关键

1. `index1 == m`：`nums1` 已经被用完，那第 k 小就只在 `nums2` 剩余里，直接 `nums2[index2 + k - 1]`。
2. `index2 == n`：同理。
3. `k == 1`：第 1 小就是剩余两个起点里更小的那个。

### 为什么可以“成段丢掉”？

假设 `pivot1 <= pivot2`（两个数组各自的第 `k/2` 候选值比较）：

- 那么在 `nums1` 中，从 `index1` 到 `newIndex1` 这 **(newIndex1-index1+1)** 个数，最多也就排到 `pivot2` 前面或相等，但肯定都不会是“第 k 小”，因为它们最多只覆盖前 `k/2` 个候选。
- 所以安全地把这段丢弃，并把 `k` 相应减少那么多个。

**每轮至少丢掉 `k/2` 个元素**，因此很快收敛到边界条件。

------

## 5.两个手工跑例子（一步一步看 k 和索引怎么变化）

### 例 1：`nums1=[1,3]`, `nums2=[2]`，求中位数

总长 3 → 中位数是第 `(3+1)/2 = 2` 小 ⇒ 求 `k=2`。

- 初始：`index1=0 (指向1)`, `index2=0 (指向2)`, `k=2`
- `newIndex1 = min(0+1, 2)-1 = 0` → `pivot1=nums1[0]=1`
   `newIndex2 = min(0+1, 1)-1 = 0` → `pivot2=nums2[0]=2`
- 比较：`1 <= 2` → 丢掉 `nums1[0..0]`（即数值 1）
   更新：`k = 2 - 1 = 1`，`index1 = 1`
- 现在 `k==1` → 返回 `min(nums1[1]=3, nums2[0]=2) = 2`

中位数 = 2 ✅

### 例 2：`nums1=[1,2]`, `nums2=[3,4]`

总长 4（偶数）→ 需要第 `k1=2` 和 `k2=3` 小，取平均。

先找第 2 小 (`k=2`)：

- 初始：`index1=0(1), index2=0(3), k=2`
- `newIndex1=min(0+1,2)-1=0→pivot1=1`
   `newIndex2=min(0+1,2)-1=0→pivot2=3`
- `1 <= 3` → 丢掉 `nums1[0]`，`k=1`, `index1=1`
- `k==1` → 返回 `min(nums1[1]=2, nums2[0]=3)=2` → 第 2 小是 2

再找第 3 小 (`k=3`)：

- 初始：`index1=0(1), index2=0(3), k=3`
- `newIndex1=min(0+1,2)-1=0→pivot1=1`
   `newIndex2=min(0+1,2)-1=0→pivot2=3`
- `1 <= 3` → 丢掉 `nums1[0]`，`k=2`, `index1=1`
- 下一轮：`index1=1(2), index2=0(3), k=2`
  - `newIndex1=min(1+1,2)-1=1→pivot1=2`
  - `newIndex2=min(0+1,2)-1=0→pivot2=3`
  - `2 <= 3` → 丢掉 `nums1[1]`，`k=1`, `index1=2`
- 此时 `index1==m(2)`，说明 `nums1` 用完 → 直接返回 `nums2[index2 + k - 1] = nums2[0] = 3`
   → 第 3 小是 3

中位数 = `(2 + 3) / 2 = 2.5` ✅

------

## 6.细节/陷阱说明

1. **k 是 1 起始**
   - 所以当 `k==1`，直接取两边当前起点的较小值。
   - 访问第 k 小元素时用 `index + k - 1`（因为数组是 0 下标）。
2. **`newIndex` 的写法**
   - `newIndex1 = min(index1 + k/2, m) - 1;`
   - 作用是：希望看“第 `k/2` 个候选”，但如果某个数组剩余长度不足 `k/2`，就把比较位置放到该数组最后一个元素上，**避免越界**。
3. **相等时（`pivot1 <= pivot2`）选哪边？**
   - 选丢 `nums1` 的前半段（或者相反也可以），不会影响正确性。
4. **重复元素、负数、空数组**都能处理
   - 空数组时，边界判断第一行就能返回结果。
5. **复杂度**
   - 每轮丢掉至少 `k/2` 个元素 → `k` 至少减半 → `O(log(m+n))`
   - 空间复杂度 `O(1)`（只用常数个指针与变量）

------

## 7.小结（把握三点就会写）

1. 把“中位数”转成“第 k 小”的通解；
2. `getKthElement` 每轮比较两边第 `k/2` 个候选，**成段丢掉**较小侧；
3. 处理好三类边界：某边用完、k==1、数组越界保护（`min(...)-1`）。



```
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    // 辅助函数：找两个有序数组的第k小元素 (k >= 1)
    int getKthElement(const vector<int>& nums1, const vector<int>& nums2, int k) {
        int m = nums1.size(), n = nums2.size();
        int index1 = 0, index2 = 0;

        while (true) {
            // 边界情况
            if (index1 == m) return nums2[index2 + k - 1];
            if (index2 == n) return nums1[index1 + k - 1];
            if (k == 1) return min(nums1[index1], nums2[index2]);

            // 正常情况：比较两数组的第 k/2 个元素
            int newIndex1 = min(index1 + k / 2, m) - 1;
            int newIndex2 = min(index2 + k / 2, n) - 1;
            int pivot1 = nums1[newIndex1];
            int pivot2 = nums2[newIndex2];
            if (pivot1 <= pivot2) {
                k -= (newIndex1 - index1 + 1);
                index1 = newIndex1 + 1;
            } else {
                k -= (newIndex2 - index2 + 1);
                index2 = newIndex2 + 1;
            }
        }
    }

    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int totalLength = nums1.size() + nums2.size();
        if (totalLength % 2 == 1) {
            // 奇数
            return getKthElement(nums1, nums2, (totalLength + 1) / 2);
        } else {
            // 偶数
            int left = getKthElement(nums1, nums2, totalLength / 2);
            int right = getKthElement(nums1, nums2, totalLength / 2 + 1);
            return (left + right) / 2.0;
        }
    }
};

```


题目链接：[和为K的子数组](https://leetcode-cn.com/problems/subarray-sum-equals-k/)

给定一个整数数组和一个整数 **k，**你需要找到该数组中和为 **k** 的连续的子数组的个数。

**示例 1 :**

```
输入:nums = [1,1,1], k = 2
输出: 2 , [1,1] 与 [1,1] 为两种不同的情况。
```

**说明 :**

1. 数组的长度为 [1, 20,000]。
2. 数组中元素的范围是 [-1000, 1000] ，且整数 **k** 的范围是 [-1e7, 1e7]。

## 思路

和1248题一样。
有多少个子数组的区间和为`k`，假设要统计区间$[l,r]$中的区间和为`k`的数量，我们先对数组求一个前缀和，易得：$sum[r]-sum[l-1]=k$，移项得：$sum[l-1]=sum[r]-k$，其中`k`是题目中已经给出的，`sum[r]`是当前求的前缀和的那一项，所以我们只需要累加`sum[l-1]`的个数即可。用一个哈希存一下前缀和的个数，求前缀和时因为满足`l<r`，所以不需要开数组，一直累加即可。

## 代码

```cpp
class Solution
{
public:
    int subarraySum(vector<int> &nums, int k)
    {
        unordered_map<long long, int> mp;
        mp[0] = 1;
        long long sum = 0, ans = 0;
        for (int i = 1; i <= nums.size(); i++)
        {
            sum += nums[i - 1];
            ans += mp[sum - k];
            mp[sum]++;
        }
        return ans;
    }
};
```


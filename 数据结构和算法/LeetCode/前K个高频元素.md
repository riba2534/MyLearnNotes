题目链接：[前 K 个高频元素](https://leetcode-cn.com/problems/top-k-frequent-elements/)

给定一个非空的整数数组，返回其中出现频率前 ***k\*** 高的元素。

 

**示例 1:**

```
输入: nums = [1,1,1,2,2,3], k = 2
输出: [1,2]
```

**示例 2:**

```
输入: nums = [1], k = 1
输出: [1]
```

 

**提示：**

- 你可以假设给定的 *k* 总是合理的，且 1 ≤ k ≤ 数组中不相同的元素的个数。
- 你的算法的时间复杂度**必须**优于 O(*n* log *n*) , *n* 是数组的大小。
- 题目数据保证答案唯一，换句话说，数组中前 k 个高频元素的集合是唯一的。
- 你可以按任意顺序返回答案。

## 思路

我的做法算是投机取巧吧。

找个map，记录出现次数。

map有自动排序的特性，所以找一个set，重载一下运算符，随便搞搞。。

## 代码

```cpp
struct comp
{
    template <typename T>
    bool operator()(const T &l, const T &r) const
    {
        if (l.second != r.second)
            return l.second > r.second;
        return l.first > r.first;
    }
};

class Solution
{
public:
    vector<int> topKFrequent(vector<int> &nums, int k)
    {
        vector<int> ans;
        map<int, int> mp;
        for (auto &num : nums)
            mp[num]++;
        set<pair<int, int>, comp> se(mp.begin(), mp.end());
        for (auto it : se)
            if (k--)
                ans.push_back(it.first);
            else
                break;
        return ans;
    }
};
```


题目链接：[最小的K个数](https://www.nowcoder.com/practice/6a296eb82cf844ca8539b57c23e6e9bf?tpId=13&tqId=11182&tPage=2&rp=2&ru=%2Fta%2Fcoding-interviews&qru=%2Fta%2Fcoding-interviews%2Fquestion-ranking)

## 题目描述

输入n个整数，找出其中最小的K个数。例如输入4,5,1,6,2,7,3,8这8个数字，则最小的4个数字是1,2,3,4,。

## 思路

思路一：无脑排序法，复杂度是 $O(nlogn)$

思路二：$O(nlogk)$ 算法，适合处理海量数据，其实就是用 set 瞎搞，set 内部是基于红黑树实现的。

## 代码

排序：

```cpp
class Solution
{
  public:
    vector<int> GetLeastNumbers_Solution(vector<int> input, int k)
    {
        vector<int> ans;
        if (input.size() == 0 || k > input.size())
            return ans;
        sort(input.begin(), input.end());
        for (int i = 0; i < k; i++)
            ans.push_back(input[i]);
        return ans;
    }
};
```

$O(nlogk)$ 算法

```cpp
class Solution
{
  public:
    vector<int> GetLeastNumbers_Solution(vector<int> input, int k)
    {
        vector<int> ans;
        if (input.size() == 0 || k > input.size())
            return ans;
        set<int> s;
        for (auto num : input)
            s.insert(num);
        int cnt = 0;
        for (auto it : s)
        {
            if (cnt == k)
                break;
            ans.push_back(it);
            cnt++;
        }
        return ans;
    }
};
```


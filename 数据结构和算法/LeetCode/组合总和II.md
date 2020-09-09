题目链接：[组合总和 II](https://leetcode-cn.com/problems/combination-sum-ii/)

给定一个数组 `candidates` 和一个目标数 `target` ，找出 `candidates` 中所有可以使数字和为 `target` 的组合。

`candidates` 中的每个数字在每个组合中只能使用一次。

**说明：**

- 所有数字（包括目标数）都是正整数。
- 解集不能包含重复的组合。 

**示例 1:**

```
输入: candidates = [10,1,2,7,6,1,5], target = 8,
所求解集为:
[
  [1, 7],
  [1, 2, 5],
  [2, 6],
  [1, 1, 6]
]
```

**示例 2:**

```
输入: candidates = [2,5,2,1,2], target = 5,
所求解集为:
[
  [1,2,2],
  [5]
]
```

## 思路

经典的部分和问题，直接dfs即可。注意剪枝。

## 代码

```cpp
class Solution
{
public:
    vector<int> tmp;
    set<vector<int>> se;
    void dfs(int i, int sum, vector<int> &candidates, int target)
    {
        if (sum > target)
            return;
        if (i == candidates.size())
        {
            if (sum == target)
            {
                vector<int> a(tmp);
                sort(a.begin(), a.end());
                se.insert(move(a));
            }
            return;
        }
        dfs(i + 1, sum, candidates, target);
        tmp.push_back(candidates[i]);
        dfs(i + 1, candidates[i] + sum, candidates, target);
        tmp.pop_back();
    }
    vector<vector<int>> combinationSum2(vector<int> &candidates, int target)
    {
        dfs(0, 0, candidates, target);
        vector<vector<int>> ans;
        for (vector<int> an : se)
            ans.push_back(an);
        return ans;
    }
};
```


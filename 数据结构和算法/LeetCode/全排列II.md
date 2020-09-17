题目链接：[全排列 II](https://leetcode-cn.com/problems/permutations-ii/)

给定一个可包含重复数字的序列，返回所有不重复的全排列。

**示例:**

```
输入: [1,1,2]
输出:
[
  [1,1,2],
  [1,2,1],
  [2,1,1]
]
```

## 思路

第一种方法是：dfs+哈希判重。

第二种方法是，先对要处理的序列进行排序，然后dfs去重的时候判断一下数字是否重复。感觉官方题解写的比较好：

> 我们将这个问题看作有 n 个排列成一行的空格，我们需要从左往右依次填入题目给定的 n 个数，每个数只能使用一次。那么很直接的可以想到一种穷举的算法，即从左往右每一个位置都依此尝试填入一个数，看能不能填完这 n 个空格，在程序中我们可以用「回溯法」来模拟这个过程。
>
> 我们定义递归函数 `backtrack(idx, perm)` 表示当前排列为 perm*，下一个待填入的位置是第 idx 个位置（下标从 0 开始）。那么整个递归函数分为两个情况：
>
> - 如果 `idx==n`，说明我们已经填完了 n 个位置，找到了一个可行的解，我们将 perm 放入答案数组中，递归结束。
> - 如果 `idx<n`，我们要考虑第 `idx` 个位置填哪个数。根据题目要求我们肯定不能填已经填过的数，因此很容易想到的一个处理手段是我们定义一个标记数组 vis 来标记已经填过的数，那么在填第 idx 个数的时候我们遍历题目给定的 n 个数，如果这个数没有被标记过，我们就尝试填入，并将其标记，继续尝试填下一个位置，即调用函数 `backtrack(idx + 1, perm)`。搜索回溯的时候要撤销该个位置填的数以及标记，并继续尝试其他没被标记过的数。
>
> 但题目解到这里并没有满足「全排列不重复」 的要求，在上述的递归函数中我们会生成大量重复的排列，因为对于第 idx 的位置，如果存在重复的数字 i，我们每次会将重复的数字都重新填上去并继续尝试导致最后答案的重复，因此我们需要处理这个情况。
>
> 要解决重复问题，我们只要设定一个规则，保证在填第 idx 个数的时候**重复数字只会被填入一次即可**。而在本题解中，我们选择对原数组排序，保证相同的数字都相邻，然后每次填入的数一定是这个数所在重复数集合中「从左往右第一个未被填过的数字」，即如下的判断条件：
>
> - C++
>
> ```cpp
> if (i > 0 && nums[i] == nums[i - 1] && !vis[i - 1]) {
>     continue;
> }
> ```
>
> 这个判断条件保证了对于重复数的集合，一定是从左往右逐个填入的。
>
> 假设我们有 33 个重复数排完序后相邻，那么我们一定保证每次都是拿从左往右第一个未被填过的数字，即整个数组的状态其实是保证了 `[未填入，未填入，未填入]` 到 `[填入，未填入，未填入]`，再到 `[填入，填入，未填入]`，最后到 `[填入，填入，填入]` 的过程的，因此可以达到去重的目标。

## 代码

dfs+哈希（比较慢）

```cpp
class Solution
{
public:
    unordered_map<string, int> mp;
    vector<vector<int>> ans;
    bool isexist(vector<int> &v)
    {
        string s = "";
        for (auto num : v)
            s += to_string(num);
        if (mp.find(s) != mp.end())
            return true;
        mp[s]++;
        return false;
    }
    void dfs(vector<int> &nums, int pos)
    {
        if (pos == nums.size())
            if (!isexist(nums))
                ans.push_back(nums);
        for (int i = pos; i < nums.size(); i++)
        {
            swap(nums[i], nums[pos]);
            dfs(nums, pos + 1);
            swap(nums[i], nums[pos]);
        }
    }
    vector<vector<int>> permuteUnique(vector<int> &nums)
    {
        dfs(nums, 0);
        return ans;
    }
};
```

排序+剪枝

```cpp
class Solution
{
public:
    vector<vector<int>> ans;
    vector<int> tmp;
    vector<int> vis;
    void dfs(vector<int> &nums, int pos)
    {
        if (pos == nums.size())
        {
            ans.push_back(tmp);
            return;
        }
        for (int i = 0; i < nums.size(); i++)
        {
            if (vis[i] || (i > 0 && nums[i] == nums[i - 1] && !vis[i - 1]))
                continue;
            tmp.push_back(nums[i]);
            vis[i] = 1;
            dfs(nums, pos + 1);
            vis[i] = 0;
            tmp.pop_back();
        }
    }
    vector<vector<int>> permuteUnique(vector<int> &nums)
    {
        vis.resize(nums.size());
        sort(nums.begin(), nums.end());
        dfs(nums, 0);
        return ans;
    }
};
```


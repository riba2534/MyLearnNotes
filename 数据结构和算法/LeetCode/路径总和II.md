题目链接：[路径总和 II](https://leetcode-cn.com/problems/path-sum-ii/)

给定一个二叉树和一个目标和，找到所有从根节点到叶子节点路径总和等于给定目标和的路径。

**说明:** 叶子节点是指没有子节点的节点。

**示例:**
给定如下二叉树，以及目标和 `sum = 22`，

```
              5
             / \
            4   8
           /   / \
          11  13  4
         /  \    / \
        7    2  5   1
```

返回:

```
[
   [5,4,11,2],
   [5,8,4,5]
]
```

## 思路

枚举每一条从根节点到叶子节点的路径。当我们遍历到叶子节点，且此时路径和恰为目标和时，我们就找到了一条满足条件的路径。

## 代码

```cpp
struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
class Solution
{
public:
    vector<vector<int>> ans;
    vector<int> v;
    void dfs(TreeNode *root, int sum)
    {
        if (!root)
            return;
        v.push_back(root->val);
        sum -= root->val;
        if (!root->left && !root->right && sum == 0)
            ans.push_back(v);
        dfs(root->left, sum);
        dfs(root->right, sum);
        v.pop_back();
    }
    vector<vector<int>> pathSum(TreeNode *root, int sum)
    {
        dfs(root, sum);
        return ans;
    }
};
```


题目链接：[二叉树的层次遍历 II](https://leetcode-cn.com/problems/binary-tree-level-order-traversal-ii/)

给定一个二叉树，返回其节点值自底向上的层次遍历。 （即按从叶子节点所在层到根节点所在的层，逐层从左向右遍历）

例如：
给定二叉树 `[3,9,20,null,null,15,7]`,

```
    3
   / \
  9  20
    /  \
   15   7
```

返回其自底向上的层次遍历为：

```
[
  [15,7],
  [9,20],
  [3]
]
```

## 思路

直接进行bfs，最后逆向即可。

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
    vector<vector<int>> levelOrderBottom(TreeNode *root)
    {
        vector<vector<int>> ans;
        if (!root)
            return ans;
        queue<pair<TreeNode *, int>> q;
        q.push({root, 1});
        int tmp_depth = 1;
        vector<int> v;
        while (!q.empty())
        {
            TreeNode *node = q.front().first;
            int depth = q.front().second;
            q.pop();
            if (depth > tmp_depth)
            {
                tmp_depth = depth;
                ans.push_back(v);
                v.clear();
            }
            v.push_back(node->val);
            if (node->left)
                q.push({node->left, depth + 1});
            if (node->right)
                q.push({node->right, depth + 1});
        }
        ans.push_back(v);
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
```


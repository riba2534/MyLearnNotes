题目链接：[111. Minimum Depth of Binary Tree](https://leetcode.com/problems/minimum-depth-of-binary-tree/)

Given a binary tree, find its minimum depth.

The minimum depth is the number of nodes along the shortest path from the root node down to the nearest leaf node.

**Note:** A leaf is a node with no children.

**Example:**

Given binary tree `[3,9,20,null,null,15,7]`,

```
    3
   / \
  9  20
    /  \
   15   7
```

return its minimum depth = 2.

## 思路

深搜，然后记录一下深度的最大值

## 代码

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    int minDepth(TreeNode* root) {
        if(!root)return 0;
        queue<pair<TreeNode*,int>>q;
        q.push({root,1});
        int minn=99999;
        while(!q.empty())
        {
            TreeNode *node=q.front().first;
            int depth=q.front().second;
            q.pop();
            if(!node->left && !node->right)
            minn=min(minn,depth);
            if(node->left)q.push({node->left,depth+1});
            if(node->right)q.push({node->right,depth+1});            
        }
        return minn;
    }
};
```


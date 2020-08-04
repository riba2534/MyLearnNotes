题目链接：[打家劫舍 III](https://leetcode-cn.com/problems/house-robber-iii/)

在上次打劫完一条街道之后和一圈房屋后，小偷又发现了一个新的可行窃的地区。这个地区只有一个入口，我们称之为“根”。 除了“根”之外，每栋房子有且只有一个“父“房子与之相连。一番侦察之后，聪明的小偷意识到“这个地方的所有房屋的排列类似于一棵二叉树”。 如果两个直接相连的房子在同一天晚上被打劫，房屋将自动报警。

计算在不触动警报的情况下，小偷一晚能够盗取的最高金额。

**示例 1:**

```
输入: [3,2,3,null,3,null,1]

     3
    / \
   2   3
    \   \ 
     3   1

输出: 7 
解释: 小偷一晚能够盗取的最高金额 = 3 + 3 + 1 = 7.
```

**示例 2:**

```
输入: [3,4,5,1,3,null,1]

     3
    / \
   4   5
  / \   \ 
 1   3   1

输出: 9
解释: 小偷一晚能够盗取的最高金额 = 4 + 5 = 9.
```

## 思路

树形dp裸题。

题目给出一棵二叉树，每一个点有一个权值，你需要选择树上的一些点，选择的两个点不能相邻，问可以取到的最大权值是多少。这个题意大概是 `POJ2342` 的变形。

我们定义：

- `dp[node][0]` 代表以`node`为根，不选 `node` 这个点可以取到的最大权值。
- `dp[node][1]` 代表以`node`为根，选择`node` 这个点可以取到的最大权值。

遍历其子节点，记为 `son`，则可以得到状态转移方程：

- $dp[node][0]=max(dp[son][0],dp[son][1])$ 当前点不选，则儿子**可以选也可以不选**
- $dp[node][1]=dp[node][1]+dp[son][0]$ 选择当前点，则儿子点**不能选**

则初始状态：

- $dp[node][0]=0$
- $dp[node][1]=val$

那么最后，$max(dp[root][0],dp[root][1])$ 即为答案，由于树的结构是结构体，用`vector`不好搞，用了两个哈希存。

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
    unordered_map<TreeNode *, int> dp0, dp1;
    void dfs(TreeNode *node)
    {
        if (node->left)
        {
            dfs(node->left);
            dp0[node] += max(dp0[node->left], dp1[node->left]);
            dp1[node] += dp0[node->left];
        }
        if (node->right)
        {
            dfs(node->right);
            dp0[node] += max(dp0[node->right], dp1[node->right]);
            dp1[node] += dp0[node->right];
        }
    }
    int rob(TreeNode *root)
    {
        if (!root)
            return 0;
        queue<TreeNode *> q;
        q.push(root);
        while (!q.empty())
        {
            TreeNode *node = q.front();
            q.pop();
            dp1[node] = node->val;
            if (node->left)
                q.push(node->left);
            if (node->right)
                q.push(node->right);
        }
        dfs(root);
        return max(dp0[root], dp1[root]);
    }
};
```


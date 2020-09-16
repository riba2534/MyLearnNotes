题目链接：[冗余连接 II](https://leetcode-cn.com/problems/redundant-connection-ii/)

在本问题中，有根树指满足以下条件的**有向**图。该树只有一个根节点，所有其他节点都是该根节点的后继。每一个节点只有一个父节点，除了根节点没有父节点。

输入一个有向图，该图由一个有着N个节点 (节点值不重复1, 2, ..., N) 的树及一条附加的边构成。附加的边的两个顶点包含在1到N中间，这条附加的边不属于树中已存在的边。

结果图是一个以`边`组成的二维数组。 每一个`边` 的元素是一对 `[u, v]`，用以表示**有向**图中连接顶点 `u` 和顶点 `v` 的边，其中 `u` 是 `v` 的一个父节点。

返回一条能删除的边，使得剩下的图是有N个节点的有根树。若有多个答案，返回最后出现在给定二维数组的答案。

**示例 1:**

```
输入: [[1,2], [1,3], [2,3]]
输出: [2,3]
解释: 给定的有向图如下:
  1
 / \
v   v
2-->3
```

**示例 2:**

```
输入: [[1,2], [2,3], [3,4], [4,1], [1,5]]
输出: [4,1]
解释: 给定的有向图如下:
5 <- 1 -> 2
     ^    |
     |    v
     4 <- 3
```

**注意:**

- 二维数组大小的在3到1000范围内。
- 二维数组中的每个整数在1到N之间，其中 N 是二维数组的大小。

## 思路



## 代码

```cpp
class Solution
{
public:
    int n;
    vector<int> pre;
    void init()
    {
        for (int i = 1; i <= n; i++)
            pre[i] = i;
    }
    int find(int x)
    {
        return x == pre[x] ? x : pre[x] = find(pre[x]);
    }
    int mix(int x, int y)
    {
        int fx = find(x), fy = find(y);
        if (fx != fy)
        {
            pre[fy] = fx;
            return 1;
        }
        return 0;
    }
    vector<int> findRedundantDirectedConnection(vector<vector<int>> &edges)
    {
        n = edges.size();
        pre.resize(n + 1);
        init();
        vector<int> fa(n + 1);
        for (int i = 1; i <= n; i++)
            fa[i] = i;
        int conflict = -1;
        int cycle = -1;
        for (int i = 0; i < n; i++)
        {
            int u = edges[i][0], v = edges[i][1];
            if (fa[v] != v)
                conflict = i;
            else
            {
                fa[v] = u;
                if (!mix(u, v))
                    cycle = i;
            }
        }
        if (conflict < 0)
            return {edges[cycle][0], edges[cycle][1]};
        if (cycle >= 0)
            return {fa[edges[conflict][1]], edges[conflict][1]};
        return {edges[conflict][0], edges[conflict][1]};
    }
};
```


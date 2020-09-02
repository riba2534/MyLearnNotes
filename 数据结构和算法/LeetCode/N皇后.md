题目链接：[N 皇后](https://leetcode-cn.com/problems/n-queens/)

*n* 皇后问题研究的是如何将 *n* 个皇后放置在 *n*×*n* 的棋盘上，并且使皇后彼此之间不能相互攻击。

![img](https://image-1252109614.cos.ap-beijing.myqcloud.com/img/20200903013741.png)

上图为 8 皇后问题的一种解法。

给定一个整数 *n*，返回所有不同的 *n* 皇后问题的解决方案。

每一种解法包含一个明确的 *n* 皇后问题的棋子放置方案，该方案中 `'Q'` 和 `'.'` 分别代表了皇后和空位。

**示例：**

```
输入：4
输出：[
 [".Q..",  // 解法 1
  "...Q",
  "Q...",
  "..Q."],

 ["..Q.",  // 解法 2
  "Q...",
  "...Q",
  ".Q.."]
]
解释: 4 皇后问题存在两个不同的解法。
```

 

**提示：**

- 皇后彼此不能相互攻击，也就是说：任何两个皇后都不能处于同一条横行、纵行或斜线上。

## 思路

暴搜。

从第一行第一个开始进行尝试，如果当前位置可以放置，则去下一行继续放，知道遍历的行数等于n时，此时直接把当前的图加入答案。

注意要求皇后不能再同一行，列，还有对角线，注意判断这个。

## 代码

```cpp
class Solution
{
public:
    vector<vector<string>> ans;
    vector<string> queens;
    vector<int> col;
    bool isValid(int row, int col)
    {
        for (int i = 0; i < row; ++i)
            if (queens[i][col] == 'Q')
                return false;
        for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; --i, --j)
            if (queens[i][j] == 'Q')
                return false;
        for (int i = row - 1, j = col + 1; i >= 0 && j < queens.size(); --i, ++j)
            if (queens[i][j] == 'Q')
                return false;
        return true;
    }
    void dfs(int r, int n)
    {
        if (r == n)
        {
            ans.push_back(queens);
            return;
        }
        for (int i = 0; i < n; i++)
            if (isValid(r, i))
            {
                queens[r][i] = 'Q';
                dfs(r + 1, n);
                queens[r][i] = '.';
            }
    }
    vector<vector<string>> solveNQueens(int n)
    {
        queens = vector<string>(n, string(n, '.'));
        col.resize(n);
        dfs(0, n);
        return ans;
    }
};
```


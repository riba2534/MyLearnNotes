题目链接：[零钱兑换 II](https://leetcode-cn.com/problems/coin-change-2/)

给定不同面额的硬币和一个总金额。写出函数来计算可以凑成总金额的硬币组合数。假设每一种面额的硬币有无限个。 

 



**示例 1:**

```
输入: amount = 5, coins = [1, 2, 5]
输出: 4
解释: 有四种方式可以凑成总金额:
5=5
5=2+2+1
5=2+1+1+1
5=1+1+1+1+1
```

**示例 2:**

```
输入: amount = 3, coins = [2]
输出: 0
解释: 只用面额2的硬币不能凑成总金额3。
```

**示例 3:**

```
输入: amount = 10, coins = [10] 
输出: 1
```

## 思路

跟 [硬币](https://leetcode-cn.com/problems/coin-lcci/) 一样，这道题给了具体的硬币数额而已。

类似于完全背包，求方案数。

定义`dp[n]`表示用这些硬币凑成面额为`n`的方案数。如果需要凑出一个面额，假设当前硬币面额为`v[i]`，则方案数可以累加为：`dp[n]+=dp[n-v[i]]`，代表用已知的可以凑成`n-v[i]`面额的方案数加上当前的硬币面额，可以凑出`n`。

时间复杂度：$O(n)$

## 代码

```cpp
class Solution
{
public:
    int change(int amount, vector<int> &coins)
    {
        vector<int> dp(amount + 1, 0);
        dp[0] = 1;
        for (auto coin : coins)
            for (int j = coin; j <= amount; j++)
                dp[j] += dp[j - coin];
        return dp[amount];
    }
};
```


题目链接：[440. K-th Smallest in Lexicographical Order](https://leetcode.com/problems/k-th-smallest-in-lexicographical-order/)

Given integers `n` and `k`, find the lexicographically k-th smallest integer in the range from `1` to `n`.

Note: 1 ≤ k ≤ n ≤ 109.

**Example:**

```
Input:
n: 13   k: 2

Output:
10

Explanation:
The lexicographical order is [1, 10, 11, 12, 13, 2, 3, 4, 5, 6, 7, 8, 9], so the second smallest number is 10.
```

## 思路

题目的意思就是求从 1 到 n 中字典序为 k 的那个数.

我们考虑一棵字典树，以一个空节点为根，可以分为十个叉，也就相当于一个十叉树。字典树的第二层是从 1 开始一直到 9 ，每个节点都有自己的子节点，解决这个问题，我们就可以先计算出从以 1 为根的子树到以 2 为根的子树中间需要经过多少个数字。定义两个指针 l 和 r ,cur 代表当前的根 ，刚开始分别指向 1 和 2，然后计算当前层有多少个节点，以 10 倍的方式逐渐扩大，当 `l>n` 的时候就跳出，此时我们可以知道当前我们计算出来的节点数和 k 作比较，如果 `sum<=k` 证明我们要找的那个数在另一棵子树中，只需要对 k 减去当前子树的个数然后重新去下一棵子树找。如果 k 就在当前子树中，则就把根往下一层放。

## 代码

```cpp
class Solution
{
  public:
    int findKthNumber(int n, int k)
    {
        if (k > n)
            return -1;
        int cur = 1;
        k--;
        while (k)
        {
            long long sum = 0, l = cur, r = cur + 1;
            while (l <= n)
            {
                sum += min((long long)n + 1, r) - l;
                l *= 10, r *= 10;
            }
            if (sum <= k)
            {
                k -= sum;
                cur++;
            }
            else
            {
                cur *= 10;
                k--;
            }
        }
        return cur;
    }
};
```


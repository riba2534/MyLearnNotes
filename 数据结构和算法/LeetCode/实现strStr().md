题目链接：[实现strStr()](https://leetcode-cn.com/problems/implement-strstr/)

实现 [strStr()](https://baike.baidu.com/item/strstr/811469) 函数。

给定一个 haystack 字符串和一个 needle 字符串，在 haystack 字符串中找出 needle 字符串出现的第一个位置 (从0开始)。如果不存在，则返回  **-1**。

**示例 1:**

```
输入: haystack = "hello", needle = "ll"
输出: 2
```

**示例 2:**

```
输入: haystack = "aaaaa", needle = "bba"
输出: -1
```

**说明:**

当 `needle` 是空字符串时，我们应当返回什么值呢？这是一个在面试中很好的问题。

对于本题而言，当 `needle` 是空字符串时我们应当返回 0 。这与C语言的 [strstr()](https://baike.baidu.com/item/strstr/811469) 以及 Java的 [indexOf()](https://docs.oracle.com/javase/7/docs/api/java/lang/String.html#indexOf(java.lang.String)) 定义相符。

## 思路

kmp板子

## 代码

```cpp
class Solution
{
  public:
    vector<int> nxt;
    void get_next(string p)
    {
        nxt.resize(p.size()+1);
        int len = p.length();
        int j = 0, k = -1;
        nxt[0] = -1;
        while (j < len)
            if (k == -1 || p[j] == p[k])
                nxt[++j] = ++k;
            else
                k = nxt[k];
    }
    int get_index(string s, string p)
    {
        int slen = s.length();
        int plen = p.length();
        get_next(p);
        int i = 0, j = 0;
        while (i < slen && j < plen)
        {
            if (j == -1 || s[i] == p[j])
            {
                i++;
                j++;
            }
            else
                j = nxt[j];
        }
        if (j == plen)
            return i - j;
        return -1;
    }
    int strStr(string haystack, string needle)
    {
        return get_index(haystack, needle);
    }
};
```


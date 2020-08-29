题目链接：[反转字符串中的单词 III](https://leetcode-cn.com/problems/reverse-words-in-a-string-iii/)

给定一个字符串，你需要反转字符串中每个单词的字符顺序，同时仍保留空格和单词的初始顺序。

 

**示例：**

```
输入："Let's take LeetCode contest"
输出："s'teL ekat edoCteeL tsetnoc"
```

 

***\**\*\*\*提示：\*\*\*\*\****

- 在字符串中，每个单词由单个空格分隔，并且字符串中不会有任何额外的空格。

## 思路

模拟

## 代码

```cpp
class Solution
{
public:
    string reverseWords(string s)
    {
        string ans = "", tmp = "";
        int i = 0;
        for (int i = 0; i < s.size(); i++)
        {
            if (s[i] == ' ')
            {
                reverse(tmp.begin(), tmp.end());
                ans = ans + tmp + " ";
                tmp = "";
            }
            else
                tmp += s[i];
        }
        reverse(tmp.begin(), tmp.end());
        ans += tmp;
        return ans;
    }
};
```


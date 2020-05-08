请你来实现一个 `atoi` 函数，使其能将字符串转换成整数。

首先，该函数会根据需要丢弃无用的开头空格字符，直到寻找到第一个非空格的字符为止。

当我们寻找到的第一个非空字符为正或者负号时，则将该符号与之后面尽可能多的连续数字组合起来，作为该整数的正负号；假如第一个非空字符是数字，则直接将其与之后连续的数字字符组合起来，形成整数。

该字符串除了有效的整数部分之后也可能会存在多余的字符，这些字符可以被忽略，它们对于函数不应该造成影响。

注意：假如该字符串中的第一个非空格字符不是一个有效整数字符、字符串为空或字符串仅包含空白字符时，则你的函数不需要进行转换。

在任何情况下，若函数不能进行有效的转换时，请返回 0。

**说明：**

假设我们的环境只能存储 32 位大小的有符号整数，那么其数值范围为 [−231,  231 − 1]。如果数值超过这个范围，qing返回  INT_MAX (231 − 1) 或 INT_MIN (−231) 。

**示例 1:**

```
输入: "42"
输出: 42
```

**示例 2:**

```
输入: "   -42"
输出: -42
解释: 第一个非空白字符为 '-', 它是一个负号。
     我们尽可能将负号与后面所有连续出现的数字组合起来，最后得到 -42 。
```

**示例 3:**

```
输入: "4193 with words"
输出: 4193
解释: 转换截止于数字 '3' ，因为它的下一个字符不为数字。
```

**示例 4:**

```
输入: "words and 987"
输出: 0
解释: 第一个非空字符是 'w', 但它不是数字或正、负号。
     因此无法执行有效的转换。
```

**示例 5:**

```
输入: "-91283472332"
输出: -2147483648
解释: 数字 "-91283472332" 超过 32 位有符号整数范围。 
     因此返回 INT_MIN (−231) 。
```

## 思路

模拟，一定要注意细节

## 代码

```cpp
class Solution
{
  public:
    int myAtoi(string str)
    {
        long long res = 0;
        int k = 0;
        while (k < str.size() && (str[k] == ' ' || str[k] == '\t'))
            k++;
        int minus = 1;
        if (k >= str.size())
            return 0;
        if (str[k] == '-')
            minus = -1, k++;
        if (str[k] == '+')
            if (minus == -1)
                return 0;
            else
                k++;
        while (str[k] >= '0' && str[k] <= '9')
        {
            res = res * 10 + str[k] - '0';
            k++;
            if (res > INT_MAX * 10ll)
                break;
        }
        res *= minus;
        if (res > INT_MAX)
            return INT_MAX;
        if (res < INT_MIN)
            return INT_MIN;
        return res;
    }
};
```

20200403重写：

垃圾代码：

```cpp
class Solution
{
public:
    void ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
    }
    int myAtoi(string str)
    {
        int flag = 1;
        long long num = 0;
        ltrim(str);
        if (str.length() == 0)
            return 0;

        if (str[0] == '-' || str[0] == '+')
            flag = str[0] == '-' ? -1 : 1;
        else if (!isdigit(str[0]))
            return 0;
        else
            num = str[0] - '0';
        for (int i = 1; i < str.length(); i++)
        {
            char ch = str[i];
            if (isdigit(ch))
            {
                num = num * 10 + (ch - '0');
                if (num > INT_MAX)
                {
                    break;
                }
            }
            else
                break;
        }
        if (flag == 1)
        {
            if (num > INT_MAX)
            {
                return INT_MAX;
            }
        }
        else
        {
            if (-num < INT_MIN)
            {
                return INT_MIN;
            }
        }
        return flag * num;
    }
};
```

---

自动机解法：[方法一：自动机](https://leetcode-cn.com/problems/string-to-integer-atoi/solution/zi-fu-chuan-zhuan-huan-zheng-shu-atoi-by-leetcode-/)

![](https://assets.leetcode-cn.com/solution-static/8_fig1.PNG)

用表格表示这个自动机：

|               | `' '` | `+/-`  | `number`  | `other` |
| :-----------: | :---: | :----: | :-------: | ------- |
|   **start**   | start | signed | in_number | end     |
|  **signed**   |  end  |  end   | in_number | end     |
| **in_number** |  end  |  end   | in_number | end     |
|    **end**    |  end  |  end   |    end    | end     |

最后代码就很简洁了：

```cpp
class Automaton
{
    string state = "start";
    unordered_map<string, vector<string>> table = {
        {"start", {"start", "signed", "in_number", "end"}},
        {"signed", {"end", "end", "in_number", "end"}},
        {"in_number", {"end", "end", "in_number", "end"}},
        {"end", {"end", "end", "end", "end"}}};
    int get_col(char c)
    {
        if (isspace(c))
            return 0;
        if (c == '+' || c == '-')
            return 1;
        if (isdigit(c))
            return 2;
        return 3;
    }

public:
    int sign = 1;
    long long ans = 0;
    void get(char c)
    {
        state = table[state][get_col(c)];
        if (state == "in_number")
        {
            ans = ans * 10 + c - '0';
            ans = sign == 1 ? min(ans, (long long)INT_MAX) : min(ans, -(long long)INT_MIN);
        }
        else if (state == "signed")
            sign = c == '+' ? 1 : -1;
    }
};

class Solution
{
public:
    int myAtoi(string str)
    {
        Automaton automaton;
        for (char c : str)
            automaton.get(c);
        return automaton.sign * automaton.ans;
    }
};
```


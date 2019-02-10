题目链接：[包含min函数的栈](https://www.nowcoder.com/practice/4c776177d2c04c2494f2555c9fcc1e49?tpId=13&tqId=11173&tPage=1&rp=1&ru=%2Fta%2Fcoding-interviews&qru=%2Fta%2Fcoding-interviews%2Fquestion-ranking)

## 题目描述

定义栈的数据结构，请在该类型中实现一个能够得到栈中所含最小元素的min函数（时间复杂度应为O（1））。

## 思路

同时定义两个栈，一个是正常的栈，一个存储当前栈中的最小值。

在进行 push 操作的时候，比较一下当前 push 的值和当前最小值栈中的栈顶元素，然后压入比较小的那个值即可。

## 代码

```cpp
class Solution
{
  public:
    stack<int> s, mins;
    void push(int value)
    {
        if (s.empty())
        {
            s.push(value);
            mins.push(value);
        }
        else
        {
            s.push(value);
            mins.push(std::min(value, mins.top()));
        }
    }
    void pop()
    {
        s.pop();
        mins.pop();
    }
    int top()
    {
        return s.top();
    }
    int min()
    {
        return mins.top();
    }
};
```


题目链接：[两数相加 II](https://leetcode-cn.com/problems/add-two-numbers-ii/)

给你两个 **非空** 链表来代表两个非负整数。数字最高位位于链表开始位置。它们的每个节点只存储一位数字。将这两数相加会返回一个新的链表。

你可以假设除了数字 0 之外，这两个数字都不会以零开头。

 

**进阶：**

如果输入链表不能修改该如何处理？换句话说，你不能对列表中的节点进行翻转。

 

**示例：**

```
输入：(7 -> 2 -> 4 -> 3) + (5 -> 6 -> 4)
输出：7 -> 8 -> 0 -> 7
```

## 思路

基本跟之前的两数相加1差不多，用一个栈，把数字存起来，然后模拟加法，处理进位，用链表连起来即可。

## 代码

```cpp
class Solution
{
public:
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2)
    {
        stack<int> s1, s2;
        while (l1)
        {
            s1.push(l1->val);
            l1 = l1->next;
        }
        while (l2)
        {
            s2.push(l2->val);
            l2 = l2->next;
        }
        ListNode *ans = NULL;
        int carry = 0;
        while (!s1.empty() || !s2.empty())
        {
            int val1 = 0, val2 = 0;
            if (!s1.empty())
            {
                val1 = s1.top();
                s1.pop();
            }
            if (!s2.empty())
            {
                val2 = s2.top();
                s2.pop();
            }
            int sum = val1 + val2 + carry;
            carry = 0;
            if (sum >= 10)
            {
                carry = sum / 10;
                sum %= 10;
            }
            ListNode *tmp = new ListNode(sum);
            tmp->next = ans;
            ans = tmp;
        }
        if (carry != 0)
        {
            ListNode *tmp = new ListNode(carry);
            tmp->next = ans;
            ans = tmp;
        }
        return ans;
    }
};
```


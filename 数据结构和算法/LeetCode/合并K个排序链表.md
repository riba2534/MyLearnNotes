题目链接：[合并K个排序链表](https://leetcode-cn.com/problems/merge-k-sorted-lists/)

合并 *k* 个排序链表，返回合并后的排序链表。请分析和描述算法的复杂度。

**示例:**

```
输入:
[
  1->4->5,
  1->3->4,
  2->6
]
输出: 1->1->2->3->4->4->5->6
```

## 思路

题意很清楚，现在的问题就是应该怎么合并。

由于每一个链表都是递增的，所以我们维护这三个链表中的三个头节点，每次取出值最小放在最终的答案链表中，这个过程可以使用优先队列来维护，时间复杂度$O(kn*logk)$,空间复杂度为$O(k)$.

## 代码

优先队列：

```cpp
class Solution
{
public:
    struct status
    {
        ListNode *ptr;
        int val;
        bool operator<(const status &rhs) const
        {
            return val > rhs.val;
        }
    };
    ListNode *mergeKLists(vector<ListNode *> &lists)
    {
        ListNode *head = NULL, *last;
        priority_queue<status> q;
        for (ListNode *&node : lists)
            if (node)
                q.push({node, node->val});
        while (!q.empty())
        {
            status cur = q.top();
            q.pop();
            if (head == NULL)
            {
                head = cur.ptr;
                last = head;
            }
            else
            {
                last->next = cur.ptr;
                last = last->next;
            }
            if (cur.ptr->next)
                q.push({cur.ptr->next, cur.ptr->next->val});
        }
        return head;
    }
};
```

附上之前写的一个手工模拟代码：

```cpp
class Solution
{
public:
    ListNode *mergeKLists(vector<ListNode *> &lists)
    {
        ListNode *head = NULL;
        while (1)
        {
            int cnt = 0;
            for (ListNode *&node : lists)
            {
                if (node == NULL)
                    cnt++;
                else
                {
                    if (head == NULL)
                    {
                        head = new ListNode(node->val);
                    }
                    else
                    {
                        ListNode *tmpHead = head;
                        if (node->val < head->val)
                        {
                            head = new ListNode(node->val);
                            head->next = tmpHead;
                        }
                        else
                        {
                            while (node->val >= tmpHead->val && tmpHead->next && node->val >= tmpHead->next->val)
                            {
                                tmpHead = tmpHead->next;
                            }
                            if (tmpHead->next)
                            {
                                ListNode *tmp = tmpHead->next;
                                tmpHead->next = new ListNode(node->val);
                                tmpHead->next->next = tmp;
                            }
                            else
                            {
                                tmpHead->next = new ListNode(node->val);
                            }
                        }
                    }
                    node = node->next;
                }
            }
            if (cnt == lists.size())
                break;
        }
        return head;
    }
};
```


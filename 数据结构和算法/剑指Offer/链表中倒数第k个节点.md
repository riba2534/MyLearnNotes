题目链接：[链表中倒数第k个节点](https://www.nowcoder.com/practice/529d3ae5a407492994ad2a246518148a?tpId=13&tqId=11167&tPage=1&rp=1&ru=/ta/coding-interviews&qru=/ta/coding-interviews/question-ranking)

## 题目描述

输入一个链表，输出该链表中倒数第k个结点。

## 思路

注意输入的指针可能为空，或者`k<=0`

链表的定义是：

```c
struct ListNode
{
    int val;
    struct ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};
```

两种思路：

第一种：我的，搞个 vector ,反转后直接输出

第二种：书上的，假设链表有 n 个节点， 那么第 k 个节点就是第 n-k+1 个，然后搞两个指针，第一个指针先跑到 k-1 这个位置，然后第二个指针和第一个指针同时跑，第一个指针跑到结尾的时候，第二个指针指向的就是倒数第 k 个节点了。

## 代码

我的思路：

```cpp
class Solution
{
  public:
    ListNode *FindKthToTail(ListNode *pListHead, unsigned int k)
    {
        if (k <= 0 || pListHead == NULL)
            return NULL;
        vector<ListNode *> v;
        v.push_back(pListHead);
        while (pListHead->next != NULL)
        {
            pListHead = pListHead->next;
            v.push_back(pListHead);
        }
        if (k > v.size())
            return NULL;
        reverse(v.begin(), v.end());
        return v[k - 1];
    }
};
```

书的思路：

```cpp
class Solution
{
  public:
    ListNode *FindKthToTail(ListNode *pListHead, unsigned int k)
    {
        if (k <= 0 || pListHead == NULL)
            return NULL;
        ListNode *p1 = pListHead, *p2 = pListHead;
        int cnt = 0;
        while (p1->next != NULL)
        {
            if (cnt == k - 1)
                break;
            cnt++;
            p1 = p1->next;
        }
        if (cnt != k - 1)
            return NULL;
        while (p1->next != NULL)
        {
            p1 = p1->next;
            p2 = p2->next;
        }
        return p2;
    }
};
```


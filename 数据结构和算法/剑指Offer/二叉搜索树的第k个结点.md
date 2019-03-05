题目链接：[二叉搜索树的第k个结点](https://www.nowcoder.com/practice/ef068f602dde4d28aab2b210e859150a?tpId=13&tqId=11215&tPage=4&rp=4&ru=%2Fta%2Fcoding-interviews&qru=%2Fta%2Fcoding-interviews%2Fquestion-ranking)

## 题目描述

给定一棵二叉搜索树，请找出其中的第k小的结点。例如， （5，3，7，2，4，6，8）    中，按结点数值大小顺序第三小结点的值为4。

## 思路

因为二叉搜索树满足的性质是左边小右边大，所以按照中序遍历的第k个节点就是答案。

## 代码

```cpp
class Solution
{
  public:
    int cnt = 0;
    TreeNode *KthNode(TreeNode *pRoot, int k)
    {
        if (pRoot == NULL || k == 0)
            return NULL;
        TreeNode *ret = KthNode(pRoot->left, k);
        if (ret)
            return ret;
        if (++cnt == k)
            return pRoot;
        ret = KthNode(pRoot->right, k);
        if (ret)
            return ret;
        return NULL;
    }
};
```


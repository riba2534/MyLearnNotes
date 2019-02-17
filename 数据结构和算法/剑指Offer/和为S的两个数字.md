题目链接：[和为S的两个数字](https://www.nowcoder.com/practice/390da4f7a00f44bea7c2f3d19491311b?tpId=13&tqId=11195&tPage=3&rp=3&ru=/ta/coding-interviews&qru=/ta/coding-interviews/question-ranking)

## 题目描述

输入一个递增排序的数组和一个数字S，在数组中查找两个数，使得他们的和正好是S，如果有多对数字的和等于S，输出两个数的乘积最小的。

## 输出描述:

对应每个测试案例，输出两个数，小的先输出。

## 思路

数列满足递增，所以设定两个指针，一个指向首部一个指向尾部，如果 a[i]+a[j]==sum ,那么，当前指向的数就是乘积最小的数。

## 代码

```cpp
class Solution
{
  public:
    vector<int> FindNumbersWithSum(vector<int> array, int sum)
    {
        vector<int> ans;
        if (array.size() == 0)
            return ans;
        int i = 0, j = array.size() - 1;
        while (i < j)
        {
            if (array[i] + array[j] == sum)
            {
                ans.push_back(array[i]);
                ans.push_back(array[j]);
                return ans;
            }
            else if (array[i] + array[j] < sum)
                i++;
            else if (array[i] + array[j] > sum)
                j--;
        }
        return ans;
    }
};
```


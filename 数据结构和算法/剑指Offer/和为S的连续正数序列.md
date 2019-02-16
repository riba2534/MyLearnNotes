题目链接：[和为S的连续正数序列](https://www.nowcoder.com/practice/c451a3fd84b64cb19485dad758a55ebe?tpId=13&tqId=11194&rp=2&ru=%2Fta%2Fcoding-interviews&qru=%2Fta%2Fcoding-interviews%2Fquestion-ranking&tPage=2)

## 题目描述

小明很喜欢数学,有一天他在做数学作业时,要求计算出9~16的和,他马上就写出了正确答案是100。但是他并不满足于此,他在想究竟有多少种连续的正数序列的和为100(至少包括两个数)。没多久,他就得到另一组连续正数和为100的序列:18,19,20,21,22。现在把问题交给你,你能不能也很快的找出所有和为S的连续正数序列? Good Luck!

## 输出描述:

输出所有和为S的连续正数序列。序列内按照从小至大的顺序，序列间按照开始数字从小到大的顺序

## 思路

尺取法。

首先定义两个指针，分别是 small=1,big=2，当前的和为 small+big=3

当当前值 cur 小于给定的值时，big++

大于时 small++ ,值减去原来的 small

等于时，从答案中加进去，并且把 big++，防止重复计算。

small 加到 sum/2 的地方停止。

## 代码

```cpp
#include <bits/stdc++.h>
using namespace std;

class Solution
{
  public:
    vector<vector<int>> FindContinuousSequence(int sum)
    {
        vector<vector<int>> ans;
        vector<int> v;
        int small = 1, big = 2;
        int mid = (1 + sum) / 2, cur = small + big;
        while (small < mid)
        {
            if (cur == sum)
            {
                v.clear();
                for (int i = small; i <= big; i++)
                    v.push_back(i);
                ans.push_back(v);
                big++;
                cur += big;
            }
            else if (cur < sum)
            {
                big++;
                cur += big;
            }
            else if (cur > sum)
            {
                cur -= small;
                small++;
            }
        }
        return ans;
    }
};
int main()
{
    Solution ac;
    vector<vector<int>> ans = ac.FindContinuousSequence(15);
    for (auto v : ans)
    {
        for (auto num : v)
            cout << num << " ";
        cout << endl;
    }
    return 0;
}
```


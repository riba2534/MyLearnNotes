题目链接：[找到 K 个最接近的元素](https://leetcode-cn.com/problems/find-k-closest-elements/)

给定一个排序好的数组，两个整数 `k` 和 `x`，从数组中找到最靠近 `x`（两数之差最小）的 `k` 个数。返回的结果必须要是按升序排好的。如果有两个数与 `x` 的差值一样，优先选择数值较小的那个数。

**示例 1:**

```
输入: [1,2,3,4,5], k=4, x=3
输出: [1,2,3,4]
```

 

**示例 2:**

```
输入: [1,2,3,4,5], k=4, x=-1
输出: [1,2,3,4]
```

 

**说明:**

1. k 的值为正数，且总是小于给定排序数组的长度。
2. 数组不为空，且长度不超过 104
3. 数组里的每个元素与 x 的绝对值不超过 104

## 思路

第一种作法是用**双指针**：

我们维护一下序列的长度，通过比较首元素和尾元素与 x 的大小，删除绝对值比较大的位置。然后最后返回原序列即可。

第二种方法是利用**二分**：

二分一下最终的序列在原始序列中的开始位置，每次比较 mid 位置与 x 的距离，和 mid+k 与 x 的距离，用这个来确定子数组的起始位置

## 代码

双指针：

```cpp
class Solution
{
  public:
    vector<int> findClosestElements(vector<int> &arr, int k, int x)
    {
        while (arr.size() > k)
        {
            if (x - arr.front() <= arr.back() - x)
                arr.pop_back();
            else
                arr.erase(arr.begin());
        }
        return arr;
    }
};
```

二分：

```cpp
class Solution
{
  public:
    vector<int> findClosestElements(vector<int> &arr, int k, int x)
    {
        int l = 0, r = arr.size() - k;
        while (l < r)
        {
            int mid = l + (r - l) / 2;
            if (x - arr[mid] > arr[mid + k] - x)
                l = mid + 1;
            else
                r = mid;
        }
        return vector<int>(arr.begin() + l, arr.begin() + l + k);
    }
};
```


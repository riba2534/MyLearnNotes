# leetcode代码模板

线段树模板：

```cpp
class Solution
{
public:
    const int inf = INFINITY;
    void pushup(vector<int> &MIN, int rt)
    {
        MIN[rt] = min(MIN[rt << 1], MIN[rt << 1 | 1]);
    }
    void build(vector<int> &MIN, vector<int> &nums, int l, int r, int rt)
    {
        if (l == r)
        {
            MIN[rt] = nums[l];
            return;
        }
        int m = (l + r) >> 1;
        build(MIN, nums, l, m, rt << 1);
        build(MIN, nums, m + 1, r, rt << 1 | 1);
        pushup(MIN, rt);
    }
    int query(vector<int> &MIN, vector<int> &nums, int L, int R, int l, int r, int rt)
    {
        if (L <= l && r <= R)
        {
            return MIN[rt];
        }
        int m = (l + r) >> 1;
        int ret = inf;
        if (L <= m)
            ret = min(ret, query(MIN, nums, L, R, l, m, rt << 1));
        if (R > m)
            ret = min(ret, query(MIN, nums, L, R, m + 1, r, rt << 1 | 1));
        return ret;
    }
    int jump(vector<int> &nums)
    {
        if (nums.size() <= 1)
            return 0;
        int n = nums.size();
        nums.insert(nums.begin(), 0);
        vector<int> MIN(4 * n, 0);
        build(MIN, nums, 1, n, 1);
    }
};
```


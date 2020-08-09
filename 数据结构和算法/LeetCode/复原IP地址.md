题目链接：[复原IP地址](https://leetcode-cn.com/problems/restore-ip-addresses/)

给定一个只包含数字的字符串，复原它并返回所有可能的 IP 地址格式。

有效的 IP 地址正好由四个整数（每个整数位于 0 到 255 之间组成），整数之间用 `'.' `分隔。

**示例:**

```
输入: "25525511135"
输出: ["255.255.11.135", "255.255.111.35"]
```



## 思路

递归，`dfs(id,pos)` 代表，当前处理的ip地址是第id段的，当前位置为pos。当满足id段数满4段后以及pos到达串的最后一个位置即更新答案。

## 代码

```cpp
class Solution
{
public:
    vector<string> ans;
    vector<int> seg_list;
    void dfs(string &s, int id, int pos)
    {
        if (id == 4)
        {
            if (pos == s.size())
            {
                string ip_addr = "";
                for (int i = 0; i < 4; i++)
                {
                    if (i)
                        ip_addr += ".";
                    ip_addr += to_string(seg_list[i]);
                }
                ans.push_back(ip_addr);
            }
            return;
        }
        if (pos == s.size())
            return;
        if (s[pos] == '0')
        {
            seg_list[id] = 0;
            dfs(s, id + 1, pos + 1);
        }
        int addr = 0;
        for (int i = pos; i < s.size(); i++)
        {
            addr = addr * 10 + (s[i] - '0');
            if (addr <= 0 || addr > 255)
                break;
            seg_list[id] = addr;
            dfs(s, id + 1, i + 1);
        }
    }
    vector<string> restoreIpAddresses(string s)
    {
        seg_list.resize(4);
        dfs(s, 0, 0);
        return ans;
    }
};
```


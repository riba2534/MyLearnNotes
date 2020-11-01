题目链接：[单词拆分 II](https://leetcode-cn.com/problems/word-break-ii/)

给定一个**非空**字符串 *s* 和一个包含**非空**单词列表的字典 *wordDict*，在字符串中增加空格来构建一个句子，使得句子中所有的单词都在词典中。返回所有这些可能的句子。

**说明：**

- 分隔时可以重复使用字典中的单词。
- 你可以假设字典中没有重复的单词。

**示例 1：**

```
输入:
s = "catsanddog"
wordDict = ["cat", "cats", "and", "sand", "dog"]
输出:
[
  "cats and dog",
  "cat sand dog"
]
```

**示例 2：**

```
输入:
s = "pineapplepenapple"
wordDict = ["apple", "pen", "applepen", "pine", "pineapple"]
输出:
[
  "pine apple pen apple",
  "pineapple pen apple",
  "pine applepen apple"
]
解释: 注意你可以重复使用字典中的单词。
```

**示例 3：**

```
输入:
s = "catsandog"
wordDict = ["cats", "dog", "sand", "and", "cat"]
输出:
[]
```

## 思路

记忆化搜索。

用 `ans[i]` 代表首字母以第i个字母为开头，包含的句子列表。

具体办法是，先用哈希记录一下这个单词表里出现的所有单词，以供 $O(1)$ 查询，然后从头开始枚举原串中的所有字母，当当前枚举到的字符串正好是原串中的单词的时候，`ans[idx]` 中增加这个单词。然后以这个单词的下一个字母枚举下一个单词，最后回溯的时候记录答案。

最终的 `ans[0]` 即为答案。

## 代码

```cpp
class Solution
{
public:
    // ans[i] 代表首字母以第i个字母为开头，包含的句子列表
    unordered_map<int, vector<string>> ans;
    unordered_set<string> wordSet;
    void dfs(string &s, int idx)
    {
        if (ans.count(idx))
            return;
        if (idx == s.size())
        {
            ans[idx] = {""};
            return;
        }
        ans[idx] = {};
        for (int i = idx + 1; i <= s.size(); i++)
        {
            string word = s.substr(idx, i - idx);
            if (wordSet.count(word))
            {
                dfs(s, i);
                for (string &succ : ans[i])
                    ans[idx].push_back(succ.empty() ? word : word + " " + succ);
            }
        }
    }
    vector<string> wordBreak(string s, vector<string> &wordDict)
    {
        for (string word : wordDict)
            wordSet.insert(word);
        dfs(s, 0);
        return ans[0];
    }
};
```




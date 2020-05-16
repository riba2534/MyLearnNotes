题目链接：[课程表 II](https://leetcode-cn.com/problems/course-schedule-ii/)

现在你总共有 *n* 门课需要选，记为 `0` 到 `n-1`。

在选修某些课程之前需要一些先修课程。 例如，想要学习课程 0 ，你需要先完成课程 1 ，我们用一个匹配来表示他们: `[0,1]`

给定课程总量以及它们的先决条件，返回你为了学完所有课程所安排的学习顺序。

可能会有多个正确的顺序，你只要返回一种就可以了。如果不可能完成所有课程，返回一个空数组。

**示例 1:**

```
输入: 2, [[1,0]] 
输出: [0,1]
解释: 总共有 2 门课程。要学习课程 1，你需要先完成课程 0。因此，正确的课程顺序为 [0,1] 。
```

**示例 2:**

```
输入: 4, [[1,0],[2,0],[3,1],[3,2]]
输出: [0,1,2,3] or [0,2,1,3]
解释: 总共有 4 门课程。要学习课程 3，你应该先完成课程 1 和课程 2。并且课程 1 和课程 2 都应该排在课程 0 之后。
     因此，一个正确的课程顺序是 [0,1,2,3] 。另一个正确的排序是 [0,2,1,3] 。
```

**说明:**

1. 输入的先决条件是由**边缘列表**表示的图形，而不是邻接矩阵。详情请参见[图的表示法](http://blog.csdn.net/woaidapaopao/article/details/51732947)。
2. 你可以假定输入的先决条件中没有重复的边。

**提示:**

1. 这个问题相当于查找一个循环是否存在于有向图中。如果存在循环，则不存在拓扑排序，因此不可能选取所有课程进行学习。
2. [通过 DFS 进行拓扑排序](https://www.coursera.org/specializations/algorithms) - 一个关于Coursera的精彩视频教程（21分钟），介绍拓扑排序的基本概念。
3. 拓扑排序也可以通过 [BFS](https://baike.baidu.com/item/宽度优先搜索/5224802?fr=aladdin&fromid=2148012&fromtitle=广度优先搜索) 完成。

## 思路

这题算是一个拓扑排序的裸题。如果存在环，就输出空，不存在环就输出正确的顺序。

首先题目给了若干个二元组 `[x,y]` ，表名要想做 `x` ，就必须先做 `y`，在图论中，就需要连一条从 `y` 指向 `x` 的边。首先我们邻接表建图，记录入度，拓扑排序**一个直观的算法就是每次删除一个入度为0的点，直到没有入度为0的点为止**。

我们每次删边，并且把每一次入度为0的点加入答案。

最后只需要判断答案的数组中的数量是否和给出的点相等，因为如果存在环，必然是不相等的。

实现可以使用 `DFS` 或者 `BFS`

## 代码

BFS做法：

```cpp
class Solution
{
public:
    vector<int> findOrder(int numCourses, vector<vector<int>> &prerequisites)
    {
        vector<int> e[numCourses];
        vector<int> in(numCourses, 0);
        for (vector<int> c : prerequisites)
        {
            e[c[0]].push_back(c[1]);
            in[c[1]]++;
        }
        vector<int> ans;
        // topsort
        queue<int> q;
        for (int i = 0; i < numCourses; i++)
            if (!in[i])
                q.push(i);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            ans.push_back(u);
            for (auto v : e[u])
                if (!--in[v])
                    q.push(v);
        }
        reverse(ans.begin(), ans.end());
        return ans.size() == numCourses ? ans : vector<int>{};
    }
};
```


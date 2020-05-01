题目链接：[第N高的薪水](https://leetcode-cn.com/problems/nth-highest-salary/)

编写一个 SQL 查询，获取 `Employee` 表中第 *n* 高的薪水（Salary）。

```
+----+--------+
| Id | Salary |
+----+--------+
| 1  | 100    |
| 2  | 200    |
| 3  | 300    |
+----+--------+
```

例如上述 `Employee` 表，*n = 2* 时，应返回第二高的薪水 `200`。如果不存在第 *n* 高的薪水，那么查询应返回 `null`。

```
+------------------------+
| getNthHighestSalary(2) |
+------------------------+
| 200                    |
+------------------------+
```

## 思路

思路和 [第二高的薪水](https://leetcode-cn.com/problems/second-highest-salary/) 基本一样，利用 `group by` 和 `Limit`语句

```sql
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
set n = N-1;
  RETURN (
    select ( select salary from `Employee` group by salary order by salary desc limit n,1  ) as getNthHighestSalary 
  );
END
```


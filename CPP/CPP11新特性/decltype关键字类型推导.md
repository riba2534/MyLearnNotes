# C++11：decltype类型推导

## 使用`decltype`进行自动类型推导

可以看下面的一段代码：

```cpp
#include <bits/stdc++.h>
using namespace std;
struct
{
    char *name;
} anon_u;
struct
{
    int d;
    decltype(anon_u) id; //没有告诉id的类型，用decltype自动推导
} anon_s[100];           //匿名的struct数组
int main()
{
    decltype(anon_s) as;//注意 as 的类型。
    return 0;
}
```

再这段代码里面，首先定义了一个普通的结构体`anon_u`，然后又定义了一个新的匿名结构体，其中有一个成员`id`,这时候并没有给它指明类型，而是让编译器自己去推导，指定`id`的类型为`anon_u`代表的这个类型。比如以下代码可以更容易的看出`decltype`关键字的作用。

```cpp
int a = 6;
decltype(a) c;//把c定义成a这个变量的类型
c = 21;
cout << c << endl;
```






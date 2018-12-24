# C++11:decltype类型推导及追踪返回值类型

## 使用`decltype`进行自动类型推导

先看一段`cppreference`上面的解释：[decltype 指定符](https://zh.cppreference.com/w/cpp/language/decltype)

![](https://i.loli.net/2018/12/24/5c20a6597638f.png)

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

再这段代码里面，首先定义了一个匿名的结构体`anon_u`，然后又定义了一个新的匿名结构体，其中有一个成员`id`,这时候并没有给它指明类型，而是让编译器自己去推导，指定`id`的类型为`anon_u`代表的这个类型。起到了**重用匿名结构体的作用**。

比如以下代码可以更容易的看出`decltype`关键字的作用。

```cpp
int a = 6;
decltype(a) c;//把c定义成a这个变量的类型
c = 21;
cout << c << endl;
```

**泛型编程中结合auto，用于追踪函数的返回值类型**

这也是`decltype`最大的用途了。

```cpp
template <typename _Tx, typename _Ty>
auto multiply(_Tx x, _Ty y)->decltype(_Tx*_Ty)
{
    return x*y;
}
```

## 追踪返回类型的函数

作用是：**可以将返回类型的声明信息放到函数参数列表的后面进行声明**。如：

普通函数声明方式：

```cpp
int func(char *ptr,int val);
```

**追踪返回类型的函数声明**：

```cpp
auto func(char *ptr.int val)->int;
```

追踪返回类型在原本函数返回值的位置使用`auto`关键字。

这个方法配合上面讲的**泛型编程中结合auto，用于追踪函数的返回值类型**，可以对程序开发灵活性有比较大的提高。


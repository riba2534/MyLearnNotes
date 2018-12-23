# C++中main函数的参数

## 问题

对于一个C/CPP文件来说，一般都有main函数，一个程序的执行一定会是先从main函数这个入口开始，一个最基本的程序框架大概长这样:

```cpp
#include <stdio.h>
int main()
{
    return 0;
}
```

但是，我们有时候看到的main函数这个括号里面还有别的东西，比如:

```cpp
#include <stdio.h>
int main(int argc, char *argv[])
{
    return 0;
}
```

或者

```cpp
#include <stdio.h>
int main(int argc, char **argv)
{
    return 0;
}
```

那么问题来了，这个`argc`和`argv`有什么用呢，为什么会有这个东西，有没有这个东西貌似对程序的执行并没有什么影响.

## 问题的探究

首先很容易看出`char **argv`和`char *argv[]`是等价的，无非是同一种东西两种写法。

`argc`是`int`型的，我们可以做一个测试，我们尝试打印一下这个值，看看它是什么?

我们可以测试一下以下程序:

```cpp
#include <stdio.h>
int main(int argc, char **argv)
{
    printf("%d\n", argc);
    return 0;
}
```

编译:`g++ main.cpp -o main`,运行`./main`，然后做一个测试，如图:

![](https://i.loli.net/2018/12/23/5c1f486962d9e.png)

从图中一共进行了三次测试:

- 第一次：输入`./main`，然后程序运行结果是1
- 第二次：输入`./main 1 2`，程序执行结果是3
- 第三次：输入`./main 1 2 3 4 5 6`，程序执行结果是7

答案已经很明显了，`argc`获取到了这个可执行程序**一共接受到了多少个参数**，第一个只有`./main`这一个参数，后面的`./main 1 2 3 4 5 6`一共有7个。



那么还有第二个问题，`char **argv`这个里面是什么，我们继续探究，写一个程序把这里面的东西打印出来：

```cpp
#include <stdio.h>
int main(int argc, char **argv)
{
    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; i++)
        printf("%s\n", argv[i]);
    return 0;
}
```

然后进行测试：

![](https://i.loli.net/2018/12/23/5c1f4b105cf9f.png)



我们可以看出，**argv实际上保存的就是我们输入进去的参数**，它保存在了一个字符串里。

## 总结

因为有了这两个参数，所以我们平时用的很多命令行的工具都是通过这两个参数的传值来知道你的意图。

如果不需要进行命令行参数的获取，当然也可以不打`int argc, char **argv`这一句。

参考资料：

- [What does int argc, char *argv[] mean?](https://stackoverflow.com/questions/3024197/what-does-int-argc-char-argv-mean)(来自 Stack Overflow)




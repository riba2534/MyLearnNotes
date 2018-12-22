# Makefile的简单教程

## 概述

> 什么是makefile？或许很多Winodws的程序员都不知道这个东西，因为那些Windows的集成开发环境 （integrated development environment，IDE）都为你做了这个工作，但我觉得要作一个好的和专 业的程序员，makefile还是要懂。这就好像现在有这么多的HTML编辑器，但如果你想成为一个专业人士， 你还是要了解HTML的标签的含义。特别在Unix下的软件编译，你就不能不自己写makefile了，会不会 写makefile，从一个侧面说明了一个人是否具备完成大型工程的能力。

说起makefile，很多人可能不知道是干什么的，当一个工程可能包含很多个文件的时候，编译起来可能会很复杂，这个时候我们可以写一个`makefile`编译脚本来定义一下编译的规则，然后只需要执行`make`命令，就会自动的执行编译脚本里面定义好的编译规则来编译项目，来得到希望得到的可执行文件。

在这里我简述一下几种简单的`makefile`文件的写法，已经可以解决很大一部分问题了。

提起`makefile`就必须得说一下`make`，make是一个工具，它可以解释`makefile`这个脚本，然后根据规则来编译一个项目。`makefile`定义规则，make来编译它。

关于makefile的更多知识，可以参考一下[跟我一起写Makefile](https://seisman.github.io/how-to-write-makefile/index.html)

## 语法规范

大概就长这个样子：

```makefile
target ... : prerequisites ...
    command
    ...
    ...
```

- target：你要生成的可执行文件
- prerequisites：你要生成的可执行文件的依赖
- command：通过什么命令来生成它

## 示例

概念说得再多，不如几个例子来的亲切。

以下包含几个例子以及如何写`makefile`文件来进行编译。

### 例子一：

现在我们随便写一个代码，用来找出一个数组中的最大值。

```cpp
// main.cpp
#include <bits/stdc++.h>
using namespace std;
int find_max(int arr[], int n)
{
    int maxx = 0;
    for (int i = 0; i < n; i++)
        maxx = max(maxx, arr[i]);
    return maxx;
}
int main()
{
    int arr[5] = {1, 8, 6, 3, 4};
    printf("max is %d.\n", find_max(arr, 5));
    return 0;
}
```

这个时候，是最简单的情况，我们只需要一句命令：

```shell
g++ main.cpp -o main
```

然后使用`./main`就可以执行了，但是今天我们学习的是`makefile`，所以要使用`makefile`应该来怎么写呢。

我们在本目录下新建一个文件`makefile`,没有后缀名，然后写上:

```makefile
main: main.cpp
	g++ main.cpp -o main
```

这样，就写好了一个最简单的`makefile`文件，这个时候，我们在终端中输入`make`，然后`make`命令就会自动解析`makefile`这个文件，然后生成一个可执行文件,然后执行，即可

![](https://i.loli.net/2018/12/22/5c1deaa59f72b.png)

那么我们来解析一个这个文件，首先写下你要生成的目标文件是啥，这里是`main`,然后写下依赖于哪个文件，这里依赖`main.cpp`,然后换一行打一个`Tab`键，输入你要执行的命令，这时候写下`make`就是自动执行.

## 例子二:

我们还是要求数组中的最大值，我们把刚才的文件改造一下，改造成几个文件.

```cpp
//main.cpp
#include "tool.h"
#include <stdio.h>
int main()
{
    int arr[5] = {1, 8, 6, 3, 4};
    printf("max is %d.\n", find_max(arr, 5));
    return 0;
}
```

```cpp
//tool.h
int find_max(int arr[], int n);
```

```cpp
//tool.cpp
int find_max(int arr[], int n)
{
    int maxx = 0;
    for (int i = 0; i < n; i++)
        maxx = maxx > arr[i] ? maxx : arr[i];
    return maxx;
}
```

这三个文件，`main.cpp`调用了`tool.h`中的`find_max`函数，而这个函数的实现再`tool.cpp`中。我们也可以用一个命令来编译：

```shell
g++ main.cpp tool.cpp -o main
```

然后执行`./main`同样可以执行，那么如果写成`makefile`的写法应该怎么写呢.

```makefile
main: main.cpp tool.o
	g++ main.cpp tool.o -o main
tool.o: tool.cpp
	g++ -c tool.cpp
```

那这些代表什么意思呢，`main`这个可执行文件的来源是`main.cpp`和`tool.o`,但是其中一个已经有了，那么另一个`tool.o`怎么来的呢，我们继续在下面写，`tool.o`怎么来的，他的来源是`tool.cpp`，生成他的命令是`g++ -c tool.cpp`,这时候运行结果如下图:

![](https://i.loli.net/2018/12/22/5c1df2b730a81.png)

这样也可以打到目的。

### 例子三:

假设文件特别多呢,我们对上面的例子进行改造:

```cpp
//main.cpp
#include "foo.h"
#include "bar.h"
#include <stdio.h>
int main()
{
    int arr[5] = {1, 8, 6, 3, 4};
    printf("max is %d.\n", find_max(arr, 5));
    printf("min is %d.\n", find_min(arr, 5));
    return 0;
}
```

```cpp
//foo.h
int find_max(int arr[], int n);
```

```cpp
//foo.cpp
int find_max(int arr[], int n)
{
    int maxx = 0;
    for (int i = 0; i < n; i++)
        maxx = maxx > arr[i] ? maxx : arr[i];
    return maxx;
}
```

```cpp
//bar.h
int find_min(int arr[], int n);
```

```cpp
//bar.cpp
int find_min(int arr[], int n)
{
    int minn = arr[0];
    for (int i = 0; i < n; i++)
        minn = minn < arr[i] ? minn : arr[i];
    return minn;
}
```

这一次我们有5个文件，怎么编译呢，当然，直接写编译命令是可以的，模仿上面的写法：

```shell
g++ bar.cpp foo.cpp main.cpp -o main
```

这样可以编译出来,同样也可以运行。

那么`makefile`怎么写呢.

```makefile
main: main.cpp bar.o foo.o
	g++ main.cpp bar.o foo.o -o main
bar.o:
	g++ -c bar.cpp
foo.o:
	g++ -c foo.cpp
clean:
	rm *.o main
```

很容易的就可以看出，main依赖谁，先写上，然后把依赖是怎么编译来的也写上，最后就生成了可执行文件。

但是这一次多了一个`clean`，这是什么意思呢，我们再生成可执行文件的过程中出现了很多`.o`文件，我们通过清理命令可以把它清理掉，先执行`make`生成可执行文件，等用完之后，执行`make clean`就会发现，现在已经回复原状，演示如下:

![](https://i.loli.net/2018/12/22/5c1df62138db9.png)

假设，我们现在要用其他的编译器编译这个项目，一个一个改编译器会很麻烦，所以我们可以类似于定义变量的方式，改的时候只要改一个地方，就可以完成编译,`makefile`定义变量也很简单，直接写即可，假设我现在要把编译器换成`clang`，可以这么写:

```makefile
cc = clang++ 
main: main.cpp bar.o foo.o
	$(cc) main.cpp bar.o foo.o -o main
bar.o:
	$(cc) -c bar.cpp
foo.o:
	$(cc) -c foo.cpp
clean:
	rm *.o main
```

修改的时候只需要修改一个地方即可,假设我们要加很多不同的编译参数呢，可以同样采用这种定义变量的方式，比如:

```makefile
cc = clang++ 
CFLAGS = -O2 -std=c++14 -Wall
main: main.cpp bar.o foo.o
	$(cc) $(CFLAGS) main.cpp bar.o foo.o -o main
bar.o:
	$(cc) $(CFLAGS) -c bar.cpp
foo.o:
	$(cc) $(CFLAGS) -c foo.cpp
clean:
	rm *.o main
```

### 例子四:

我们还是根据上面的例子来修改，这次头文件不变，我们定义两个主函数:

```cpp
//max_main.cpp
#include "foo.h"
#include "bar.h"
#include <stdio.h>
int main()
{
    int arr[5] = {1, 8, 6, 3, 4};
    printf("max is %d.\n", find_max(arr, 5));
    return 0;
}
```

```cpp
//min_main.cpp
#include "foo.h"
#include "bar.h"
#include <stdio.h>
int main()
{
    int arr[5] = {1, 8, 6, 3, 4};
    printf("min is %d.\n", find_min(arr, 5));
    return 0;
}
```

我们想要达成的目的是，最终编译出来两个可执行文件，那么我们怎么写呢，可以根据上面的例子模仿一下:

```makefile
cc = g++ 
CFLAGS = -O2 -std=c++14 -Wall
max_main: max_main.cpp bar.o foo.o
	$(cc) $(CFLAGS) max_main.cpp bar.o foo.o -o max_main
min_main: min_main.cpp bar.o foo.o
	$(cc) $(CFLAGS) min_main.cpp bar.o foo.o -o min_main
bar.o:
	$(cc) $(CFLAGS) -c bar.cpp
foo.o:
	$(cc) $(CFLAGS) -c foo.cpp
clean:
	rm *.o
```

这样对不对呢?我们来测试一下

![](https://i.loli.net/2018/12/22/5c1dfc0d4afde.png)

从图上可以发现，为啥编译出来的只有`max_main`这一个可执行文件呢，另一个去哪了。

**makefile**在执行的时候只根据第一条语句，第一条语句已经可以生成可执行文件了，那么他就**不会**执行生成下一个可执行文件，那么我们怎样可以打到我们的需求呢，我们需要进行一小部分的修改:

```makefile
cc = g++ 
CFLAGS = -O2 -std=c++14 -Wall

all:max_main min_main

max_main: max_main.cpp bar.o foo.o
	$(cc) $(CFLAGS) max_main.cpp bar.o foo.o -o max_main

min_main: min_main.cpp bar.o foo.o
	$(cc) $(CFLAGS) min_main.cpp bar.o foo.o -o min_main

bar.o:
	$(cc) $(CFLAGS) -c bar.cpp
foo.o:
	$(cc) $(CFLAGS) -c foo.cpp
clean:
	rm *.o

```

我们只需要加这一条即可`all:max_main min_main`,我们告诉他，需要生成两个文件,再试试:

![](https://i.loli.net/2018/12/22/5c1dfb7622152.png)

这个时候已经成功了，生成了两个可执行文件。



## 结语

这是`makefile`的一些简单的操作，如果要学习更详细的，推荐几个地址:

- [跟我一起写Makefile](https://seisman.github.io/how-to-write-makefile/introduction.html)
- [how-to-write-makefile](https://github.com/seisman/how-to-write-makefile)


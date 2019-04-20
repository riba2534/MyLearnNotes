# GoLang Notes

学习资源：[the-way-to-go_ZH_CN](https://github.com/Unknwon/the-way-to-go_ZH_CN)

学习资源是上面的链接，下面只记载一些比较有用的东西。

## 1 入门例子

```go
package main //每一个go 程序都必须包含 main 包

import (
	"fmt"    //其中的一个包
	fm "fmt" //给包定义别名
)
const c = "C" //定义常量
var v int = 5 //显式定义变量类型
type T struct{}
func init() { //一个函数的左花括号必须在紧接着函数右边
}
func main() {
	var a int
	Func1()
	//...
	fm.Println(a)
	fmt.Println(a)
}
func (t T) Method1() {
	//...
}
func func1() {
	//...
}
```

上面的代码并没有实际的任何意义，但是可以说明一些语法.

运行的时候是：

```shell
go run 文件名 # 执行
go build 文件名 # 编译成可执行的二进制文件
```


## GoLang 自定义包总结

大概总结一下 GoLang 自定义包，并不是需要发布的包，只是一次在代码工作路径下调用自定义包的一个尝试。

> 今天踩坑就是，以为 import 的时候要填写文件夹里面的 package 的名称，比如之前我是这么写的：
>
> ```go
> import "./pack/p"
> ```
>
> 这样无法通过编译，这就是本文的由来了

首先，工作的目录结构是这样的：

```
.
├── main.go
└── pack
    └── test.go

1 directory, 2 files
```

其中，各个文件代码为：

```go
// main.go
package main
import (
        "fmt"
       "./pack"
)
func main() {
        s := p.Sayhello()
        fmt.Println(s)
}
```

``` go
// pack/test.go
package p

func Sayhello() string {
        return "Hello!"
}
```

在主目录下通过 `go run` 或者 `go build` 的方法运行或编译，都是可以通过的。

证明了以下几点：

- import 语句使用的是文件夹的名称
  - 上面的文件夹名字为 `pack`
- 文件夹名称和 package 名称不一定一样
  - 比如 `pack` 和 `p`
- 调用自定义包使用 `package.函数名` 的方式
  - `p.xxx`
- 自定义包的调用和文件名没有关系。
  - `test.go` 和 `p` 没有关系



### 推荐博客

1. [理解Golang包导入](https://tonybai.com/2015/03/09/understanding-import-packages/)
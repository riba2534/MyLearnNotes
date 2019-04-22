# GoLang Notes

学习资源：[the-way-to-go_ZH_CN](https://github.com/Unknwon/the-way-to-go_ZH_CN)

学习资源是上面的链接，下面只记载一些比较有用的东西。

## 1 GoLang入门

### 1.1 入门例子

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

### 1.2 可见性规则

当标识符（包括常量、变量、类型、函数名、结构字段等等）以一个大写字母开头，如：Group1，那么使用这种形式的标识符的对象就可以被外部包的代码所使用（客户端程序需要先导入这个包），这被称为导出（像面向对象语言中的 public）；标识符如果以小写字母开头，则对包外是不可见的，但是他们在整个包的内部是可见并且可用的（像面向对象语言中的 private ）。

（大写字母可以使用任何 Unicode 编码的字符，比如希腊文，不仅仅是 ASCII 码中的大写字母）。

因此，在导入一个外部包后，能够且只能够访问该包中导出的对象。

假设在包 pack1 中我们有一个变量或函数叫做 Thing（以 T 开头，所以它能够被导出），那么在当前包中导入 pack1 包，Thing 就可以像面向对象语言那样使用点标记来调用：`pack1.Thing`（pack1 在这里是不可以省略的）。

因此包也可以作为命名空间使用，帮助避免命名冲突（名称冲突）：两个包中的同名变量的区别在于他们的包名，例如 `pack1.Thing` 和 `pack2.Thing`。

## 2 基本结构和基本数据类型

### 2.1 常量

常量使用关键字 `const` 定义，用于存储不会改变的数据。

存储在常量中的数据类型只可以是布尔型、数字型（整数型、浮点型和复数）和字符串型。

常量的定义格式：`const identifier [type] = value`，例如：

```go
const Pi = 3.14159
```

在 Go 语言中，你可以省略类型说明符 `[type]`，因为编译器可以根据变量的值来推断其类型。

- 显式类型定义： `const b string = "abc"`
- 隐式类型定义： `const b = "abc"`

一个没有指定类型的常量被使用时，会根据其使用环境而推断出它所需要具备的类型。换句话说，未定义类型的常量会在必要时刻根据上下文来获得相关类型。

```go
var n int
f(n + 5) // 无类型的数字型常量 “5” 它的类型在这里变成了 int
```

常量的值必须是能够在编译时就能够确定的；你可以在其赋值表达式中涉及计算过程，但是所有用于计算的值必须在编译期间就能获得。

- 正确的做法：`const c1 = 2/3`
- 错误的做法：`const c2 = getNumber()` // 引发构建错误: `getNumber() used as value`

**因为在编译期间自定义函数均属于未知，因此无法用于常量的赋值，但内置函数可以使用，如：len()。**

数字型的常量是没有大小和符号的，并且可以使用任何精度而不会导致溢出：

```go
const Ln2= 0.693147180559945309417232121458\
			176568075500134360255254120680009
const Log2E= 1/Ln2 // this is a precise reciprocal
const Billion = 1e9 // float constant
const hardEight = (1 << 100) >> 97
```

根据上面的例子我们可以看到，反斜杠 `\` 可以在常量表达式中作为多行的连接符使用。

与各种类型的数字型变量相比，你无需担心常量之间的类型转换问题，因为它们都是非常理想的数字。

不过需要注意的是，当常量赋值给一个精度过小的数字型变量时，可能会因为无法正确表达常量所代表的数值而导致溢出，这会在编译期间就引发错误。另外，常量也允许使用并行赋值的形式：

```go
const beef, two, c = "eat", 2, "veg"
const Monday, Tuesday, Wednesday, Thursday, Friday, Saturday = 1, 2, 3, 4, 5, 6
const (
	Monday, Tuesday, Wednesday = 1, 2, 3
	Thursday, Friday, Saturday = 4, 5, 6
)
```

常量还可以用作枚举：

```go
const (
	Unknown = 0
	Female = 1
	Male = 2
)
```

现在，数字 0、1 和 2 分别代表未知性别、女性和男性。这些枚举值可以用于测试某个变量或常量的实际值，比如使用 switch/case 结构 (第 5.3 节).

在这个例子中，`iota` 可以被用作枚举值：

```go
const (
	a = iota
	b = iota
	c = iota
)
```

第一个 `iota` 等于 0，每当 `iota` 在新的一行被使用时，它的值都会自动加 1；所以 `a=0, b=1, c=2` 可以简写为如下形式：

```go
const (
	a = iota
	b
	c
)
```

（ **译者注：关于 iota 的使用涉及到非常复杂多样的情况，这里作者解释的并不清晰，因为很难对 iota 的用法进行直观的文字描述。如希望进一步了解，请观看视频教程 《Go编程基础》 第四课：常量与运算符** ）

`iota` 也可以用在表达式中，如：`iota + 50`。在每遇到一个新的常量块或单个常量声明时， `iota` 都会重置为 0（ **简单地讲，每遇到一次 const 关键字，iota 就重置为 0** ）。

当然，常量之所以为常量就是恒定不变的量，因此我们无法在程序运行过程中修改它的值；如果你在代码中试图修改常量的值则会引发编译错误。

引用 `time` 包中的一段代码作为示例：一周中每天的名称。

```go
const (
	Sunday = iota
	Monday
	Tuesday
	Wednesday
	Thursday
	Friday
	Saturday
)
```

你也可以使用某个类型作为枚举常量的类型：

```go
type Color int

const (
	RED Color = iota // 0
	ORANGE // 1
	YELLOW // 2
	GREEN // ..
	BLUE
	INDIGO
	VIOLET // 6
)
```

### 2.2 变量

声明变量的几种方法：

```go
//比较完整
var identifier [type] = value
var a int = 15
var i = 5
var b bool = false
var str string = "Go says hello to the world!"
//go 语言编译器可以自行推导类型
var a = 15
var b = false
var str = "Go says hello to the world!"
//或
var (
	a = 15
	b = false
	str = "Go says hello to the world!"
	numShips = 50
	city string
)
//最简短的声明方式
a := 1
```

注意：

1. `:=` 符号是用来定义变量
2. `=` 符号是用来给变量赋值
3. `_` 实际上是一个只写变量，你不能得到它的值。这样做是因为 Go 语言中你必须使用所有被声明的变量，但有时你并不需要使用从一个函数得到的所有返回值。

### 2.3 init 函数

变量除了可以在全局声明中初始化，也可以在 init 函数中初始化。这是一类非常特殊的函数，它不能够被人为调用，而是在每个包完成初始化后自动执行，并且执行优先级比 main 函数高。

每个源文件都只能包含一个 init 函数。初始化总是以单线程执行，并且按照包的依赖关系顺序执行。

一个可能的用途是在开始执行程序之前对数据进行检验或修复，以保证程序状态的正确性。

```go
package main
import (
	"fmt"
	"math"
)
var Pi float64
func init() {
	Pi = 4 * math.Atan(1) // init() function computes Pi
}
func main() {
	fmt.Println(Pi)
}
```

比如上面的代码。

### 2.4 基本类型和运算符

在这个部分讲解有关布尔型、数字型和字符型的相关知识。

表达式是一种特定的类型的值，它可以由其它的值以及运算符组合而成。每个类型都定义了可以和自己结合的运算符集合，如果你使用了不在这个集合中的运算符，则会在编译时获得编译错误。

一元运算符只可以用于一个值的操作（作为后缀），而二元运算符则可以和两个值或者操作数结合（作为中缀）。

只有两个类型相同的值才可以和二元运算符结合，另外要注意的是，Go 是强类型语言，因此不会进行隐式转换，任何不同类型之间的转换都必须显式说明。Go 不存在像 C 和 Java 那样的运算符重载，表达式的解析顺序是从左至右。

#### 2.4.1 `bool` 类型

- 有两个值 `true` 和 `false`

- 可以使用 `==` 或者 `!=` 来比较两个 `bool` 值是不是相等

#### 2.4.2 数字类型

一共有两种，分别是整形数字和浮点型数字

整数：

- Int (基于架构的范围)

- int8（-128 -> 127）
- int16（-32768 -> 32767）
- int32（-2,147,483,648 -> 2,147,483,647）
- int64（-9,223,372,036,854,775,808 -> 9,223,372,036,854,775,807）

无符号整数：

- uint8（0 -> 255）
- uint16（0 -> 65,535）
- uint32（0 -> 4,294,967,295）
- uint64（0 -> 18,446,744,073,709,551,615）

浮点型（IEEE-754 标准）：

- float32（+- 1e-45 -> +- 3.4 * 1e38）
- float64（+- 5 * 1e-324 -> 107 * 1e308）

注意：

- `int` 型的计算速度最快
- `float32` 精确到小数点后 7 位
- `float64` 精确到小数点后15 位
- 尽可能使用 `float64` ，在数学函数库 `math` 中都需要接受 `float64` 的参数
- 前缀加上 `0` 可以表示8进制，前缀加上 `0x` 可以表示16进制

如过这些类型需要混用，那么需要对这些类型进行转换，比如下面的例子：

```go
package main

import "fmt"

func main() {
	var n int16 = 34
	var m int32
	// compiler error: cannot use n (type int16) as type int32 in assignment
	//m = n
	m = int32(n)

	fmt.Printf("32 bit int is: %d\n", m)
	fmt.Printf("16 bit int is: %d\n", n)
}
```

可以显式的把 `int16` 类型转换成 `int32` 类型，在 GoLang 中不存在隐式转换。

#### 2.4.3 格式化说明符

在格式化字符串里，`%d` 用于格式化整数（`%x` 和 `%X` 用于格式化 16 进制表示的数字），`%g` 用于格式化浮点型（`%f` 输出浮点数，`%e` 输出科学计数表示法），`%0d` 用于规定输出定长的整数，其中开头的数字 0 是必须的。

`%n.mg` 用于表示数字 n 并精确到小数点后 m 位，除了使用 g 之外，还可以使用 e 或者 f，例如：使用格式化字符串 `%5.2e` 来输出 3.4 的结果为 `3.40e+00`。

**类型的安全转换**

对于大的数据类型转换到小的数据类型上面时，可能会引发一些精度的问题，所以需要自己写一个函数进行转换。比如下面的例子：

```go
package main
import (
	"fmt"
	"math"
)
// 把 int 型整数转换为 uint8 型
func Uint8FromInt(n int) (uint8, error) {
	if 0 <= n && n <= math.MaxUint8 { // conversion is safe
		return uint8(n), nil
	}
	return 0, fmt.Errorf("%d is out of the uint8 range", n)
}
//把 float64 位整数转换为 int 型
func IntFromFloat64(x float64) int {
	if math.MinInt32 <= x && x <= math.MaxInt32 { // x lies in the integer range
		whole, fraction := math.Modf(x)
		if fraction >= 0.5 {
			whole++
		}
		return int(whole)
	}
	panic(fmt.Sprintf("%g is out of the int32 range", x))
}
func main() {
	var a int = 1230000
	b, c := Uint8FromInt(a)
	fmt.Println(b, c)
}
```

#### 2.4.4 复数

Go 拥有以下复数类型：

```go
complex64 (32 位实数和虚数)
complex128 (64 位实数和虚数)
```

复数使用 `re+imI` 来表示，其中 `re` 代表实数部分，`im` 代表虚数部分，I 代表根号负 1。

示例：

```go
var c1 complex64 = 5 + 10i
fmt.Printf("The value is: %v", c1)
// 输出： 5 + 10i
```

如果 `re` 和 `im` 的类型均为 float32，那么类型为 complex64 的复数 c 可以通过以下方式来获得：

```go
c = complex(re, im)
```

函数 `real(c)` 和 `imag(c)` 可以分别获得相应的实数和虚数部分。

在使用格式化说明符时，可以使用 `%v` 来表示复数，但当你希望只表示其中的一个部分的时候需要使用 `%f`。

复数支持和其它数字类型一样的运算。当你使用等号 `==` 或者不等号 `!=` 对复数进行比较运算时，注意对精确度的把握。`cmath` 包中包含了一些操作复数的公共方法。如果你对内存的要求不是特别高，最好使用 complex128 作为计算类型，因为相关函数都使用这个类型的参数。

#### 2.4.5 位运算

看了一下，大概和 `C/CPP` 中的位运算一样，所以略过。

#### 2.4.6 逻辑运算符

Go 中拥有以下逻辑运算符：`==`、`!=`（第 4.5.1 节）、`<`、`<=`、`>`、`>=`。

值的类型是 `bool` 型。

#### 2.4.7 算数运算符

大部分运算符和 `C/CPP` 中的算数运算符一致，比如：

```
+ - * / %
```

值得注意的是，`++` 和 `—` 运算符的使用与 `C/CPP` 中不一样

对于整数和浮点数，你可以使用一元运算符 `++`（递增）和 `--`（递减），但只能用于后缀：

```go
i++ -> i += 1 -> i = i + 1
i-- -> i -= 1 -> i = i - 1
```

同时，带有 `++` 和 `--` 的只能作为语句，而非表达式，因此 `n = i++` 这种写法是无效的，其它像 `f(i++)` 或者 `a[i]=b[i++]` 这些可以用于 C、C++ 和 Java 中的写法在 Go 中也是不允许的。

#### 2.4.8 随机数

有时候需要用到随机数，以下例子可以提供随机数：

```go
package main
import (
	"fmt"
	"math/rand"
	"time"
)
func main() {
	for i := 0; i < 10; i++ {
		a := rand.Int()
		fmt.Printf("%d\n", a)
	}
	for i := 0; i < 5; i++ {
		r := rand.Intn(8)
		fmt.Printf("%d\n", r)
	}
	fmt.Println()
	timens := int64(time.Now().Nanosecond())
	rand.Seed(timens)
	for i := 0; i < 10; i++ {
		fmt.Printf("%2.2f\n", 100*rand.Float32())
	}
}
```

函数 `rand.Float32` 和 `rand.Float64` 返回介于 [0.0, 1.0) 之间的伪随机数，其中包括 0.0 但不包括 1.0。函数 `rand.Intn` 返回介于 [0, n) 之间的伪随机数。

你可以使用 `Seed(value)` 函数来提供伪随机数的生成种子，一般情况下都会使用当前时间的纳秒级数字。

#### 2.4.9 运算符优先级

```
优先级 	运算符
 7 		^ !
 6 		* / % << >> & &^
 5 		+ - | ^
 4 		== != < <= >= >
 3 		<-
 2 		&&
 1 		||
```

当不知道优先级的时候最好用括号

#### 2.4.10 类型别名

当需要给一个类型起一个别名的时候，可以使用 `type` 关键字。

比如：`type TZ int` 用法类似于 `C/CPP` 的 `typedef`

例子：

```go
package main
import "fmt"
type TZ int
func main() {
	var a, b TZ = 3, 4
	c := a + b
	fmt.Printf("c has the value: %d\n", c) // 输出：c has the value: 7
}
```

#### 2.4.11 字符类型

严格来说，这并不是 Go 语言的一个类型，字符只是整数的特殊用例。`byte` 类型是 `uint8` 的别名，对于只占用 1 个字节的传统 ASCII 编码的字符来说，完全没有问题。例如：`var ch byte = 'A'`；字符使用单引号括起来。

在 ASCII 码表中，A 的值是 65，而使用 16 进制表示则为 41，所以下面的写法是等效的：

```go
var ch byte = 65 或 var ch byte = '\x41'
```

（`\x` 总是紧跟着长度为 2 的 16 进制数）

另外一种可能的写法是 `\` 后面紧跟着长度为 3 的八进制数，例如：`\377`。

不过 Go 同样支持 Unicode（UTF-8），因此字符同样称为 Unicode 代码点或者 runes，并在内存中使用 int 来表示。在文档中，一般使用格式 U+hhhh 来表示，其中 h 表示一个 16 进制数。其实 `rune` 也是 Go 当中的一个类型，并且是 `int32`的别名。

在书写 Unicode 字符时，需要在 16 进制数之前加上前缀 `\u` 或者 `\U`。

因为 Unicode 至少占用 2 个字节，所以我们使用 `int16` 或者 `int` 类型来表示。如果需要使用到 4 字节，则会加上 `\U`前缀；前缀 `\u` 则总是紧跟着长度为 4 的 16 进制数，前缀 `\U` 紧跟着长度为 8 的 16 进制数。



```go
var ch int = '\u0041'
var ch2 int = '\u03B2'
var ch3 int = '\U00101234'
fmt.Printf("%d - %d - %d\n", ch, ch2, ch3) // integer
fmt.Printf("%c - %c - %c\n", ch, ch2, ch3) // character
fmt.Printf("%X - %X - %X\n", ch, ch2, ch3) // UTF-8 bytes
fmt.Printf("%U - %U - %U", ch, ch2, ch3) // UTF-8 code point
```

输出：

```
65 - 946 - 1053236
A - β - r
41 - 3B2 - 101234
U+0041 - U+03B2 - U+101234
```

格式化说明符 `%c` 用于表示字符；当和字符配合使用时，`%v` 或 `%d` 会输出用于表示该字符的整数；`%U` 输出格式为 U+hhhh 的字符串（另一个示例见第 5.4.4 节）。

包 `unicode` 包含了一些针对测试字符的非常有用的函数（其中 `ch` 代表字符）：

- 判断是否为字母：`unicode.IsLetter(ch)`
- 判断是否为数字：`unicode.IsDigit(ch)`
- 判断是否为空白符号：`unicode.IsSpace(ch)`

这些函数返回一个布尔值。包 `utf8` 拥有更多与 rune 相关的函数。

### 2.5 字符串

字符串是 UTF-8 字符的一个序列（当字符为 ASCII 码时则占用 1 个字节，其它字符根据需要占用 2-4 个字节）。UTF-8 是被广泛使用的编码格式，是文本文件的标准编码，其它包括 XML 和 JSON 在内，也都使用该编码。由于该编码对占用字节长度的不定性，Go 中的字符串里面的字符也可能根据需要占用 1 至 4 个字节（示例见第 4.6 节），这与其它语言如 C++、Java 或者 Python 不同（Java 始终使用 2 个字节）。Go 这样做的好处是不仅减少了内存和硬盘空间占用，同时也不用像其它语言那样需要对使用 UTF-8 字符集的文本进行编码和解码。

Go 支持以下 2 种形式的字面值：

- 解释字符串：

  该类字符串使用双引号括起来，其中的相关的转义字符将被替换，这些转义字符包括：

  - `\n`：换行符
  - `\r`：回车符
  - `\t`：tab 键
  - `\u` 或 `\U`：Unicode 字符
  - `\\`：反斜杠自身

- 非解释字符串：

  该类字符串使用反引号括起来，支持换行，例如：

  ```go
    `This is a raw string \n` 中的 `\n\` 会被原样输出。
  ```

和 C/C++不一样，Go 中的字符串是根据长度限定，而非特殊字符`\0`。

字符串拼接符和其他语言一样，都是：`+`

### 2.6 `strings` 和 `strconv` 包相关

`strings` 是用来进行一系列字符串操作的包。

| 函数名                   | 功能                                                         | 例子                                           |
| ------------------------ | ------------------------------------------------------------ | ---------------------------------------------- |
| `strings.HasPrefix`      | `HasPrefix` 判断字符串 `s` 是否以 `prefix` 开头              | `strings.HasPrefix(s, prefix string) bool`     |
| `strings.HasSuffix`      | `HasSuffix` 判断字符串 `s` 是否以 `suffix` 结尾              | `strings.HasSuffix(s, suffix string) bool`     |
| `strings.Contains`       | 判断字符串 `s` 是否包含 `substr`                             | `strings.Contains(s, substr string) bool`      |
| `strings.Index`          | Index` 返回字符串 `str` 在字符串 `s` 中的索引（`str` 的第一个字符的索引），-1 表示字符串 `s` 不包含字符串 ` str | `strings.Index(s, str string) int`             |
| `strings.LastIndex`      | LastIndex` 返回字符串 `str` 在字符串 `s` 中最后出现位置的索引（`str` 的第一个字符的索引），-1 表示字符串 `s` 不包含字符串 `str | `strings.LastIndex(s, str string) int`         |
| `strings.IndexRune`      | 查询非 ASCII 编码的字符在父字符串中的位置                    | `strings.IndexRune(s string, r rune) int`      |
| `strings.Replace`        | 字符串替换，Replace` 用于将字符串 `str` 中的前 `n` 个字符串 `old` 替换为字符串 `new`，并返回一个新的字符串，如果 `n = -1` 则替换所有字符串 `old` 为字符串 `new | `strings.Replace(str, old, new, n) string`     |
| `strings.Count`          | `Count` 用于计算字符串 `str` 在字符串 `s` 中出现的非重叠次数 | `strings.Count(s, str string) int`             |
| `strings.Repeat`         | `Repeat` 用于重复 `count` 次字符串 `s` 并返回一个新的字符串  | `strings.Repeat(s, count int) string`          |
| `strings.ToLower`        | 大小写转换，`ToLower` 将字符串中的 Unicode 字符全部转换为相应的小写字符 | `strings.ToLower(s) string`                    |
| `strings.ToUpper`        | ``ToUpper` 将字符串中的 Unicode 字符全部转换为相应的大写字符` | `strings.ToUpper(s) string`                    |
| `strings.TrimSpace(s)`   | 修剪字符串，剔除字符串开头和结尾的空白符号                   |                                                |
| `strings.Trim(s, "cut")` | 剔除指定字符，将开头和结尾的 `cut` 去掉                      |                                                |
| `TrimLeft 或 TrimRight`  | 剔除开头或者结尾的字符串                                     |                                                |
| `strings.Fields(s)`      | 分割字符串,利用 1 个或多个空白符号来作为动态长度的分隔符将字符串分割成若干小块，并返回一个 slice，如果字符串只包含空白符号，则返回一个长度为 0 的 slice |                                                |
| `strings.Split(s, sep)`  | 自定义分割符号来对指定字符串进行分割，同样返回 slice         |                                                |
| `strings.Join`           | `Join` 用于将元素类型为 string 的 slice 使用分割符号来拼接组成一个字符串 | `strings.Join(sl []string, sep string) string` |
|                          | 从字符串中读取内容，函数 `strings.NewReader(str)` 用于生成一个 `Reader` 并读取字符串中的内容，然后返回指向该 `Reader` 的指针，从其它类型读取内容的函数还有 |                                                |

 一些例子：

```go
//分割字符串例子
package main
import (
	"fmt"
	"strings"
)
func main() {
	str := "The quick brown fox jumps over the lazy dog"
	sl := strings.Fields(str) //分割字符串
	fmt.Printf("Splitted in slice: %v\n", sl)
	for _, val := range sl {
		fmt.Printf("%s - ", val)
	}
	fmt.Println()
	str2 := "GO1|The ABC of Go|25"
	sl2 := strings.Split(str2, "|")
	fmt.Printf("Splitted in slice: %v\n", sl2)
	for _, val := range sl2 {
		fmt.Printf("%s - ", val)
	}
	fmt.Println()
	str3 := strings.Join(sl2, ";")
	fmt.Printf("sl2 joined by ;: %s\n", str3)
}
```

`strconv` 是用来，进行与字符串相关的类型转换

| 函数名                | 功能                                                         | 例子                                                         |
| --------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| `strconv.Itoa`        | int 型转 string                                              | `strconv.Itoa(i int) string`                                 |
| `strconv.FormatFloat` | 64 位浮点型的数字转换为字符串，其中 `fmt` 表示格式（其值可以是 `'b'`、`'e'`、`'f'` 或 `'g'`），`prec` 表示精度，`bitSize` 则使用 32 表示 float32，用 64 表示 float64 | `strconv.FormatFloat(f float64, fmt byte, prec int, bitSize int) string` |
| `strconv.Atoi`        | 字符串转 int                                                 | `strconv.Atoi(s string) (i int, err error)`                  |
| `strconv.ParseFloat`  | 字符串转成 `float64` 类型                                    | `strconv.ParseFloat(s string, bitSize int) (f float64, err error)` |

字符串转换例子：

```go
package main
import (
	"fmt"
	"strconv"
)
func main() {
	var orig string = "666"
	var an int
	var newS string
	//获取操作系统的int型所占位数
	fmt.Printf("The size of ints is: %d\n", strconv.IntSize)

	an, _ = strconv.Atoi(orig)
	fmt.Printf("The integer is: %d\n", an)
	an = an + 5
	newS = strconv.Itoa(an)
	fmt.Printf("The new string is: %s\n", newS)
}
```

### 2.7 时间与日期相关

`time` 包为我们提供了一个数据类型 `time.Time`（作为值使用）以及显示和测量时间和日期的功能函数。

当前时间可以使用 `time.Now()` 获取，或者使用 `t.Day()`、`t.Minute()` 等等来获取时间的一部分；你甚至可以自定义时间格式化字符串，例如： `fmt.Printf("%02d.%02d.%4d\n", t.Day(), t.Month(), t.Year())` 将会输出 `21.07.2011`。

Duration 类型表示两个连续时刻所相差的纳秒数，类型为 int64。Location 类型映射某个时区的时间，UTC 表示通用协调世界时间。

包中的一个预定义函数 `func (t Time) Format(layout string) string` 可以根据一个格式化字符串来将一个时间 t 转换为相应格式的字符串，你可以使用一些预定义的格式，如：`time.ANSIC` 或 `time.RFC822`。

一般的格式化设计是通过对于一个标准时间的格式化描述来展现的，这听起来很奇怪，但看下面这个例子你就会一目了然：

```go
fmt.Println(t.Format("02 Jan 2006 15:04")) 
```

输出：

```
21 Jul 2011 10:31
```

示例代码：

```go
package main

import (
	"fmt"
	"time"
)

var week time.Duration

func main() {
	t := time.Now()
	fmt.Println(t) // e.g. Wed Dec 21 09:52:14 +0100 RST 2011
	fmt.Printf("%02d.%02d.%4d\n", t.Day(), t.Month(), t.Year())
	// 21.12.2011
	t = time.Now().UTC()
	fmt.Println(t)          // Wed Dec 21 08:52:14 +0000 UTC 2011
	fmt.Println(time.Now()) // Wed Dec 21 09:52:14 +0100 RST 2011
	// calculating times:
	week = 60 * 60 * 24 * 7 * 1e9 // must be in nanosec
	week_from_now := t.Add(time.Duration(week))
	fmt.Println(week_from_now) // Wed Dec 28 08:52:14 +0000 UTC 2011
	// formatting times:
	fmt.Println(t.Format(time.RFC822))         // 21 Dec 11 0852 UTC
	fmt.Println(t.Format(time.ANSIC))          // Wed Dec 21 08:56:34 2011
	fmt.Println(t.Format("02 Jan 2006 15:04")) // 21 Dec 2011 08:52
	s := t.Format("20060102")
	fmt.Println(t, "=>", s)
	// Wed Dec 21 08:52:14 +0000 UTC 2011 => 20111221
}
```

### 2.8 指针

Go 语言也提供了指针，但是不能对指针进行运算。

Go 语言的取地址符是 `&` ，放在一个变量前面就是这个变量在内存中的地址。

比如这样：

```go
var i1 = 5
fmt.Printf("An integer: %d, it's location in memory: %p\n", i1, &i1)
```

指针也拥有对应的数据类型：

```go
var intP *int
```


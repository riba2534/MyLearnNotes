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

一个指针可以指向任何一个值得内存地址，在 32 位机器占 4 个字节，在 64 位机器占 8 个字节，并且与他所指向的值得大小无关。可以在前面加上 `*` 来获取其指向的内容。

注意事项：

1. 在书写表达式 `var p *type` 时一定要爱在 `*` 号和指针类型之间留一个空格，要不然由于表达式变得复杂，更容易被误认为是一个乘法表达式。
2. 符号 `*` 可以用来获取值，在任何情况下，这个表达式都是正确的 `var == *(&var)`

下面是一个例子：

```go
package main
import "fmt"
func main() {
	var i1 = 5
	fmt.Printf("%d %p\n", i1, &i1)
	var intP *int
	intP = &i1
	fmt.Printf("%p %d\n", intP, *intP)
}
// 运行结果：
// 5 0xc000090000
// 0xc000090000 5

```

从上面可以看出指针的用法。

```go
package main
import "fmt"
func main() {
	s := "good bye"
	var p *string = &s
	*p = "ciao"
	fmt.Printf("指针地址为：%p\n", p)
	fmt.Printf("指针所指向的内容为：%s\n", *p)
	fmt.Printf("s 串的内容为：%s\n", s)
}
// 运行结果：
// 指针地址为：0xc0000101e0
// 指针所指向的内容为：ciao
// s 串的内容为：ciao
```

以上代码证明了可以根据指针来改变目标的内容。

注意事项：

1. 在 GoLang 中，不允许像 `C` 语言中一样的指针运算，比如 `pointer+2` ,Go 语言的指针保证了内存安全，因此 `c = *p++` 在 Go 语言中的代码中是不合法的

2. 指针的一个高级应用是传递变量的引用(如函数的参数)，这样不会传递变量的拷贝，比较省内存

3. 指针也可以指向指针，跟 `C` 语言中的多级指针一样，但是往往这样会使代码结构不清晰。

4. 一个空指针的反向引用是不合法的，会报错，比如:

   ```go
   package main
   func main() {
   	var p *int = nil
   	*p = 0
   }
   ```

   这样就是不合法的，不能对一个空指针进行反向引用。

## 3 控制结构

GoLang 提供了以下的分支与结构语句：

- If-else 语句
- switch 语句
- select 结构，用于 channel 的选择

可以使用迭代循环结构来重复执行一次或者多次的代码：

- for (range) 结构

用 `break` 与 `continue` 这样的关键字可以用于中途改变循环的状态，可以用 `return`来结束某个函数的执行，或者使用 `goto` 和标签调整程序的执行位置。

GoLang  中完全省略了 `if` `switch` `for` 语句中的两侧括号，减少了很多视觉混乱因素，使得代码更加简洁。 

### 3.1 `if-else` 结构

写法类似于 `C/CPP` ，一般有以下几种写法：

```go
//方法一：
if 条件{
	//语句
}
//方法二：
if 条件1{
	//语句
}else{
	//语句
}
//方法三：
if 条件1{
	//
}else if 条件2{
	//
}else{
	//
}
```

从用法上来说，就比 `C` 语言少了括号，其余完全一样

注意：左大括号一定要跟在条件右边

例子：

```go
// 判断系统是否是 windows
if runtime.GOOS == "windows"	 {
 	.	..
 } else { // Unix-like
 	.	..
 }
```

if 中还可以包含一个初始化语句，不过要必须用 `;` 来进行分割，可以这样写：

```go
if val := 10; val > 1 {
	//语句
}
```

### 3.2 测试多返回值函数的错误

Go 语言的函数经常使用两个返回值来表示一个函数是否执行成功，返回某个值以及 `true` 来表示执行成功，返回零值或者 `false` 表示返回失败。当不使用 `true` 或 `false` 的时候，也可以使用一个`error` 类型的变量来代替作为第二个返回值，成功执行的话 `error` 的值为 `nil`  ,从下面的例子可以看出：

```go
package main

import (
	"fmt"
	"strconv"
)

func main() {
	var orig string = "ABC"
	// var an int
	var newS string
	// var err error

	fmt.Printf("The size of ints is: %d\n", strconv.IntSize)
	// anInt, err = strconv.Atoi(origStr)
	an, err := strconv.Atoi(orig)
	if err != nil {
		fmt.Printf("orig %s is not an integer - exiting with error\n", orig)
		return
	}
	fmt.Printf("The integer is %d\n", an)
	an = an + 5
	newS = strconv.Itoa(an)
	fmt.Printf("The new string is: %s\n", newS)
}
```

需要注意的是，每一次最好先获取错误信息，当确保没有错误时，再执行相应的动作。

### 3.3 `switch` 结构

相比较 C 和 Java 等其它语言而言，Go 语言中的 switch 结构使用上更加灵活。它接受任意形式的表达式：

```go
switch var1 {
	case val1:
		...
	case val2:
		...
	default:
		...
}
```

变量 var1 可以是任何类型，而 val1 和 val2 则可以是同类型的任意值。类型不被局限于常量或整数，但必须是相同的类型；或者最终结果为相同类型的表达式。前花括号 `{` 必须和 switch 关键字在同一行。

您可以同时测试多个可能符合条件的值，使用逗号分割它们，例如：`case val1, val2, val3`。

每一个 `case` 分支都是唯一的，从上至下逐一测试，直到匹配为止。（ Go 语言使用快速的查找算法来测试 switch 条件与 case 分支的匹配情况，直到算法匹配到某个 case 或者进入 default 条件为止。）

一旦成功地匹配到某个分支，在执行完相应代码后就会退出整个 switch 代码块，也就是说您**不需要特别使用 `break` 语句来表示结束**。**程序也不会自动地去执行下一个分支的代码。如果在执行完每个分支的代码后，还希望继续执行后续分支的代码，可以使用 `fallthrough` 关键字来达到目的**

因此，程序也不会自动地去执行下一个分支的代码。如果在执行完每个分支的代码后，还希望继续执行后续分支的代码，可以使用 `fallthrough` 关键字来达到目的。

因此：

```go
switch i {
	case 0: // 空分支，只有当 i == 0 时才会进入分支
	case 1:
		f() // 当 i == 0 时函数不会被调用
}
```

并且：

```go
switch i {
	case 0: fallthrough
	case 1:
		f() // 当 i == 0 时函数也会被调用
}
```

switch 语句的第三种形式是包含一个初始化语句：

```go
switch initialization {
	case val1:
		...
	case val2:
		...
	default:
		...
}
```

这种形式可以非常优雅地进行条件判断：

```go
switch result := calculate(); {
	case result < 0:
		...
	case result > 0:
		...
	default:
		// 0
}
```

例子：

```go
package main
import "fmt"
func main() {
	var num1 int = 100
	switch num1 {
	case 98, 99:
		fmt.Println("It's equal to 98")
	case 100:
		fmt.Println("It's equal to 100")
	default:
		fmt.Println("It's not equal to 98 or 100")
	}
}
```

任何支持进行相等判断的类型都可以作为测试表达式的条件，包括 int、string、指针等。

### 3.4 `for` 结构

#### 3.4.1 普通用法

直接用几个例子来演示 `for` 的用法，基本形式为：

```go
for 初始化语句; 条件语句; 修饰语句 {}
```

您还可以在循环中同时使用多个计数器：

```go
for i, j := 0, N; i < j; i, j = i+1, j-1 {}
```

如过对于一个字符串循环打印 `ASCII` 是可以的，但是对于一些占用 2 到 4 个字节的语言，比如中文日文这种，打印的就会有问题。

```go
package main
import "fmt"
func main() {
	str := "Go is a beautiful language!"
	fmt.Printf("The length of str is: %d\n", len(str))
	for ix := 0; ix < len(str); ix++ {
		fmt.Printf("Character on position %d is: %c \n", ix, str[ix])
	}
	str2 := "日本語"
	fmt.Printf("The length of str2 is: %d\n", len(str2))
	for ix := 0; ix < len(str2); ix++ {
		fmt.Printf("Character on position %d is: %c \n", ix, str2[ix])
	}
}
// 运行结果:
// The length of str is: 27
// Character on position 0 is: G
// Character on position 1 is: o
// Character on position 2 is:
// Character on position 3 is: i
// Character on position 4 is: s
// Character on position 5 is:
// Character on position 6 is: a
// Character on position 7 is:
// Character on position 8 is: b
// Character on position 9 is: e
// Character on position 10 is: a
// Character on position 11 is: u
// Character on position 12 is: t
// Character on position 13 is: i
// Character on position 14 is: f
// Character on position 15 is: u
// Character on position 16 is: l
// Character on position 17 is:
// Character on position 18 is: l
// Character on position 19 is: a
// Character on position 20 is: n
// Character on position 21 is: g
// Character on position 22 is: u
// Character on position 23 is: a
// Character on position 24 is: g
// Character on position 25 is: e
// Character on position 26 is: !
// The length of str2 is: 9
// Character on position 0 is: æ
// Character on position 1 is:
// Character on position 2 is: ¥
// Character on position 3 is: æ
// Character on position 4 is:
// Character on position 5 is: ¬
// Character on position 6 is: è
// Character on position 7 is: ª
// Character on position 8 is: %
```

#### 3.4.2 基于条件的迭代

for 结构的第二种形式是没有头部的条件判断迭代（类似其它语言中的 while 循环），基本形式为：`for 条件语句 {}`。

您也可以认为这是没有初始化语句和修饰语句的 for 结构，因此 `;;` 便是多余的了。

比如这样：

```go
package main
import "fmt"
func main() {
	var i int = 5
	for i >= 0 {
		i = i - 1
		fmt.Printf("The variable i is now: %d\n", i)
	}
}
```

#### 3.4.3 无限循环

条件语句是可以被省略的，如 `i:=0; ; i++` 或 `for { }` 或 `for ;; { }`（`;;` 会在使用 gofmt 时被移除）：这些循环的本质就是无限循环。最后一个形式也可以被改写为 `for true { }`，但一般情况下都会直接写 `for { }`。

#### 3.4.4 `for-range` 结构

这是 Go 特有的一种的迭代结构，您会发现它在许多情况下都非常有用。它可以迭代任何一个集合。语法上很类似其它语言中 foreach 语句，依旧可以获得每次迭代所对应的索引。一般形式为：`for ix, val := range coll { }`。

每个 rune 字符和索引在 for-range 循环中是一一对应的。它能够自动根据 UTF-8 规则识别 Unicode 编码的字符。

举个例子：

```go
package main

import "fmt"

func main() {
	str := "Go is a beautiful language!"
	fmt.Printf("长度为：%d\n", len(str))
	for pos, char := range str {
		fmt.Printf("下标是：%d,字符是: %c \n", pos, char)
	}
	fmt.Println()
	str2 := "Chinese: 日本語"
	fmt.Printf("The length of str2 is: %d\n", len(str2))
	for pos, char := range str2 {
		fmt.Printf("character %c starts at byte position %d\n", char, pos)
	}
	fmt.Println()
	fmt.Println("index int(rune) rune    char bytes")
	for index, s := range str2 {
		fmt.Printf("%-2d      %d      %U '%c' % X\n", index, s, s, s, []byte(string(s)))
	}
}

```

#### 3.4.5 `break` 和 `continue` 

这两个关键字的用法和 `C/CPP` 一样，就不说了。

#### 3.4.6 标签与 `goto`

for、switch 或 select 语句都可以配合标签（label）形式的标识符使用，即某一行第一个以冒号（`:`）结尾的单词（gofmt 会将后续代码自动移至下一行）。

比如以下代码：

```go
package main
import "fmt"
func main() {
LABEL1:
	for i := 0; i <= 5; i++ {
		for j := 0; j <= 5; j++ {
			if j == 4 {
				continue LABEL1
			}
			fmt.Printf("i  is : %d ,and j is :%d\n", i, j)
		}
	}
}
// 运行结果:
// i  is : 0 ,and j is :0
// i  is : 0 ,and j is :1
// i  is : 0 ,and j is :2
// i  is : 0 ,and j is :3
// i  is : 1 ,and j is :0
// i  is : 1 ,and j is :1
// i  is : 1 ,and j is :2
// i  is : 1 ,and j is :3
// i  is : 2 ,and j is :0
// i  is : 2 ,and j is :1
// i  is : 2 ,and j is :2
// i  is : 2 ,and j is :3
// i  is : 3 ,and j is :0
// i  is : 3 ,and j is :1
// i  is : 3 ,and j is :2
// i  is : 3 ,and j is :3
// i  is : 4 ,and j is :0
// i  is : 4 ,and j is :1
// i  is : 4 ,and j is :2
// i  is : 4 ,and j is :3
// i  is : 5 ,and j is :0
// i  is : 5 ,and j is :1
// i  is : 5 ,and j is :2
// i  is : 5 ,and j is :3
```

上面每当到 `j==4` 的时候，就会重新去前面执行循环，所以 j 永远也不会输出 4 和 5。

## 4 函数

### 4.1 介绍

每一个程序都包含很多的函数：函数是基本的代码块。

Go是编译型语言，所以函数编写的顺序是无关紧要的；鉴于可读性的需求，最好把 `main()` 函数写在文件的前面，其他函数按照一定逻辑顺序进行编写（例如函数被调用的顺序）。

编写多个函数的主要目的是将一个需要很多行代码的复杂问题分解为一系列简单的任务（那就是函数）来解决。而且，同一个任务（函数）可以被调用多次，有助于代码重用。

Go 里面有三种类型的函数：

- 普通的带有名字的函数
- 匿名函数或者lambda函数
- 方法（Methods)

除了main()、init()函数外，其它所有类型的函数都可以有参数与返回值。函数参数、返回值以及它们的类型被统称为函数签名。

函数被调用的基本格式如下：

```go
pack1.Function(arg1, arg2, …, argn)
```

函数可以将其他函数调用作为它的参数，只要这个被调用函数的返回值个数、返回值类型和返回值的顺序与调用函数所需求的实参是一致的，例如：

假设 f1 需要 3 个参数 `f1(a, b, c int)`，同时 f2 返回 3 个参数 `f2(a, b int) (int, int, int)`，就可以这样调用 f1：`f1(f2(a, b))`。

**Go语言不支持函数重载**

如果需要申明一个在外部定义的函数，你只需要给出函数名与函数签名，不需要给出函数体：

```go
func flushICache(begin, end uintptr) // implemented externally
```

**函数也可以以申明的方式被使用，作为一个函数类型**，就像：

```go
type binOp func(int, int) int
```

在这里，不需要函数体 `{}`。

函数是一等值（first-class value）：它们可以赋值给变量，就像 `add := binOp` 一样。

这个变量知道自己指向的函数的签名，所以给它赋一个具有不同签名的函数值是不可能的。

> 函数值（functions value）之间可以相互比较：如果它们引用的是相同的函数或者都是 nil 的话，则认为它们是相同的函数。函数不能在其它函数里面声明（不能嵌套），不过我们可以通过使用匿名函数来破除这个限制。
>
> 目前 Go 没有泛型（generic）的概念，也就是说它不支持那种支持多种类型的函数。不过在大部分情况下可以通过接口（interface），特别是空接口与类型选择（type switch）与/或者通过使用反射（reflection，参考）来实现相似的功能。使用这些技术将导致代码更为复杂、性能更为低下，所以在非常注意性能的的场合，最好是为每一个类型单独创建一个函数，而且代码可读性更强。
>

### 4.2 函数参数与返回值

在函数块里面，`return` 之后的语句都不会执行。如果一个函数需要返回值，那么这个函数里面的每一个代码分支（code-path）都要有 `return` 语句。

函数定义时，它的形参一般是有名字的，不过我们也可以定义没有形参名的函数，只有相应的形参类型，就像这样：`func f(int, int, float64)`。

没有参数的函数通常被称为 **niladic** 函数（niladic function），就像 `main.main()`。

### 4.3 按值传递和引用传递

Go 默认使用按值传递来传递参数，也就是传递参数的副本。函数接收参数副本之后，在使用变量的过程中可能对副本的值进行更改，但不会影响到原来的变量，比如 `Function(arg1)`。

如果希望函数可以直接修改参数的值，而不是对参数的副本进行操作，你需要将参数的地址（变量名前面添加&符号，比如 &variable）传递给函数，这就是按引用传递，比如 `Function(&arg1)`，此时传递给函数的是一个指针。如果传递给函数的是一个指针，指针的值（一个地址）会被复制，但指针的值所指向的地址上的值不会被复制；我们可以通过这个指针的值来修改这个值所指向的地址上的值。

### 4.4 命名的返回值

multiple_return.go 里的函数带有一个 `int` 参数，返回两个 `int` 值；其中一个函数的返回值在函数调用时就已经被赋予了一个初始零值。

`getX2AndX3` 与 `getX2AndX3_2` 两个函数演示了如何使用非命名返回值与命名返回值的特性。当需要返回多个非命名返回值时，需要使用 `()` 把它们括起来，比如 `(int, int)`。

命名返回值作为结果形参（result parameters）被初始化为相应类型的零值，当需要返回的时候，我们只需要一条简单的不带参数的return语句。需要注意的是，即使只有一个命名返回值，也需要使用 `()` 括起来

```go
package main
import "fmt"
var num int = 10
var numx2, numx3 int
func main() {
	numx2, numx3 = getX2AndX3(num)
	PrintValues()
	numx2, numx3 = getX2AndX3_2(num)
	PrintValues()
}
func PrintValues() {
	fmt.Printf("num = %d, 2x num = %d, 3x num = %d\n", num, numx2, numx3)
}
func getX2AndX3(input int) (int, int) {
	return 2 * input, 3 * input
}
func getX2AndX3_2(input int) (x2 int, x3 int) {
	x2 = 2 * input
	x3 = 3 * input
	return
}
```

空白符用来匹配一些不需要的值，`_`

### 4.5 改变外部变量

给函数传递指针可以修改外部变量，还节省内存，比如。

```go
package main
import (
    "fmt"
)
// this function changes reply:
func Multiply(a, b int, reply *int) {
    *reply = a * b
}
func main() {
    n := 0
    reply := &n
    Multiply(10, 5, reply)
    fmt.Println("Multiply:", *reply) // Multiply: 50
}
```

### 4.6 传递变长参数

**注意：可变长参数应该是函数定义的最右边的参数，即最后一个参数**

如果函数的最后一个参数是采用 `...type` 的形式，那么这个函数就可以处理一个变长的参数，这个长度可以为 0，这样的函数称为变参函数。

```go
func myFunc(a, b, arg ...int) {}
```

这个函数接受一个类似某个类型的 slice 的参数，该参数可以通过for 循环结构迭代。

示例函数和调用：x

```go
func Greeting(prefix string, who ...string)
Greeting("hello:", "Joe", "Anna", "Eileen")
```

在 Greeting 函数中，变量 `who` 的值为 `[]string{"Joe", "Anna", "Eileen"}`。

如果参数被存储在一个 slice 类型的变量 `slice` 中，则可以通过 `slice...` 的形式来传递参数，调用变参函数。

比如下面的例子：

```go
package main
import "fmt"
func main() {
	x := min(1, 3, 2, 0)
	fmt.Printf("The minimum is: %d\n", x)
	slice := []int{7, 9, 3, 5, 1}
	x = min(slice...)
	fmt.Printf("The minimum in the slice is: %d\n", x)
}
func min(s ...int) int {
	if len(s) == 0 {
		return 0
	}
	min := s[0]
	for _, v := range s {
		if v < min {
			min = v
		}
	}
	return min
}
```

上面的代码可以传递一个变长参数，然后求出一个参数列表中的最小值。下面这种调用方法也可以。

```go
package main
import "fmt"
func run(s ...int) {
	for _, v := range s {
		fmt.Printf("%d\n", v)
	}
}
func main() {
	run(1, 2, 3, 4, 5, 6, 7)
	run([]int{9, 8, 7, 6, 5, 4, 3, 2, 1}...)
}
```

### 4.7 `defer` 和追踪

关键字 defer 允许我们推迟到函数返回之前（或任意位置执行 `return` 语句之后）一刻才执行某个语句或函数（为什么要在返回之后才执行这些语句？因为 `return` 语句同样可以包含一些操作，而不是单纯地返回某个值）。

关键字 defer 的用法类似于面向对象编程语言 Java 和 C# 的 `finally` 语句块，它一般用于释放某些已分配的资源。

```go
package main
import "fmt"
func fun1() {
	fmt.Printf("fun1 start\n")
	defer fun2()
	fmt.Printf("fun1 end\n")
}
func fun2() {
	fmt.Printf("fun2\n")
}
func a() {
	i := 0
	defer fmt.Println(i)
	i++
	fmt.Printf("i=%d\n", i)
	return
}
func f() {
	for i := 0; i < 5; i++ {
		defer fmt.Printf("%d ", i)
	}
}
func main() {
	fun1()
	a()
	f()
}
```

当有多个 `defer` 时，最后的一个最先执行，类似于栈

### 4.8 内置函数

以下是一个简单的列表。

| 名称               | 说明                                                         |
| ------------------ | ------------------------------------------------------------ |
| close              | 用于管道通信                                                 |
| len、cap           | len 用于返回某个类型的长度或数量（字符串、数组、切片、map 和管道）；cap 是容量的意思，用于返回某个类型的最大容量（只能用于切片和 map） |
| new、make          | new 和 make 均是用于分配内存：new 用于值类型和用户定义的类型，如自定义结构，make 用于内置引用类型（切片、map 和管道）。它们的用法就像是函数，但是将类型作为参数：new(type)、make(type)。new(T) 分配类型 T 的零值并返回其地址，也就是指向类型 T 的指针（详见第 10.1 节）。它也可以被用于基本类型：`v := new(int)`。make(T) 返回类型 T 的初始化之后的值，因此它比 new 进行更多的工作（详见第 7.2.3/4 节、第 8.1.1 节和第 14.2.1 节）**new() 是一个函数，不要忘记它的括号** |
| copy、append       | 用于复制和连接切片                                           |
| panic、recover     | 两者均用于错误处理机制                                       |
| print、println     | 底层打印函数（详见第 4.2 节），在部署环境中建议使用 fmt 包   |
| complex、real imag | 用于创建和操作复数（详见第 4.5.2.2 节）                      |

### 4.9 递归函数

用递归写一个斐波那契如下：

```go
package main

import "fmt"

func fib(n int) (res int) {
	if n <= 1 {
		res = 1
	} else {
		res = fib(n-1) + fib(n-2)
	}
	return
}
func main() {
	res := 0
	for i := 0; i < 10; i++ {
		res = fib(i)
		fmt.Println("第", i, "个斐波那契数是", res)
	}
}
```

还可以支持过个递归函数相互调用。

### 4.10 将函数作为参数

函数可以作为其他函数的参数进行传递，然后在其他函数内调用执行，一般称之为回调，下面是一个例子：

```go
package main

import "fmt"

func ADD(a, b int) {
	fmt.Printf("和是 %d\n", a+b)
}
func callback(y int, f func(int, int)) {
	f(y, 2)
}
func main() {
	callback(1, ADD)
}

```

### 4.11 闭包

不希望给函数起名字的时候可以使用匿名函数，例如：`func(x, y int) int { return x + y }`。可以被赋值于某个变量，即保存函数的地址到变量中：`fplus := func(x, y int) int { return x + y }`，然后通过变量名对函数进行调用：`fplus(3,4)`。

```go
package main

import "fmt"

func main() {
	f()
}
func f() {
	for i := 0; i < 4; i++ {
		g := func(i int) { fmt.Printf("%d ", i) } //此例子中只是为了演示匿名函数可分配不同的内存地址，在现实开发中，不应该把该部分信息放置到循环中。
		g(i)
		fmt.Printf(" - g is of type %T and has value %v\n", g, g)
	}
}
```

**闭包的应用：将函数作为返回值**

```go
package main
import "fmt"
func main() {
	// make an Add2 function, give it a name p2, and call it:
	p2 := Add2()
	fmt.Printf("Call Add2 for 3 gives: %v\n", p2(3))
	// make a special Adder function, a gets value 2:
	TwoAdder := Adder(2)
	fmt.Printf("The result is: %v\n", TwoAdder(3))
}
func Add2() func(b int) int {
	return func(b int) int {
		return b + 2
	}
}
func Adder(a int) func(b int) int {
	return func(b int) int {
		return a + b
	}
}
```

工厂函数：

一个返回值为另一个函数的函数可以被称之为工厂函数，这在您需要创建一系列相似的函数的时候非常有用：书写一个工厂函数而不是针对每种情况都书写一个函数。下面的函数演示了如何动态返回追加后缀的函数：

```go
func MakeAddSuffix(suffix string) func(string) string {
	return func(name string) string {
		if !strings.HasSuffix(name, suffix) {
			return name + suffix
		}
		return name
	}
}
```

现在，我们可以生成如下函数：

```go
addBmp := MakeAddSuffix(".bmp")
addJpeg := MakeAddSuffix(".jpeg")
```

然后调用它们：

```go
addBmp("file") // returns: file.bmp
addJpeg("file") // returns: file.jpeg
```

## 5 数组与切片

### 5.1 声明和初始化

#### 5.1.1 概念

声明的格式是:

```go
var identifier [len]type
```

例如：

```go
var arr[5] int
```

- 获得数组中元素数量，使用 `len()`
- 可以使用 for 的各种操作来访问数组元素

比如：

```go
package main
import "fmt"

func main() {
	var arr1 [5]int

	for i:=0; i < len(arr1); i++ {
		arr1[i] = i * 2
	}

	for i:=0; i < len(arr1); i++ {
		fmt.Printf("Array at index %d is %d\n", i, arr1[i])
	}
}
```

Go 语言中的数组是一种 **值类型**（不像 C/C++ 中是指向首元素的指针），所以可以通过 `new()` 来创建： `var arr1 = new([5]int)`。

那么这种方式和 `var arr2 [5]int` 的区别是什么呢？arr1 的类型是 `*[5]int`，而 arr2的类型是 `[5]int`。

这样的结果就是当把一个数组赋值给另一个时，需要在做一次数组内存的拷贝操作。例如：

```go
arr2 := *arr1
arr2[2] = 100
```

这样两个数组就有了不同的值，在赋值后修改 arr2 不会对 arr1 生效。

所以在函数中数组作为参数传入时，如 `func1(arr2)`，会产生一次数组拷贝，func1 方法不会修改原始的数组 arr2。

如果你想修改原数组，那么 arr2 必须通过&操作符以引用方式传过来，例如 func1(&arr2），下面是一个例子

#### 5.1.2 数组常量

如果数组的值已经提前知道了，可以通过数组常量的方法来初始化数组，而不用依次使用 `[]=` 方法。

1. `var arrAge = [5]int{18, 20, 15, 22, 16}` 直接初始化
2. `var arrLazy = [...]int{5, 6, 7, 8, 22}` 化成了切片
3. `var arrKeyValue = [5]string{3: "Chris", 4: "Ron"}` 给具体的下标赋具体的值

几何点（或者数学向量）是一个使用数组的经典例子。为了简化代码通常使用一个别名：

```go
type Vector3D [3]float32
var vec Vector3D
```

#### 5.1.3 多维数组

通过类似于：

```go
var [3][5] int
```

可以定义多维数组。

#### 5.1.4 将数组传递给函数

把一个大数组传递给函数会消耗很多内存，有两种方法可以避免这个现象

- 传递数组的指针
- 传递数组的切片

```go
package main
import "fmt"
func main() {
	array := [3]float64{7.0, 8.5, 9.1}
	x := Sum(&array) // Note the explicit address-of operator
	// to pass a pointer to the array
	fmt.Printf("The sum of the array is: %f\n", x)
}
func Sum(a *[3]float64) (sum float64) {
	for _, v := range a { // derefencing *a to get back to the array is not necessary!
		sum += v
	}
	return
}
```

这是一个传递指针的例子。

### 5.2 切片

#### 5.2.1 概念

切片是对数组的一个连续片段的引用，所以切片是一个引用类型，这个片段可以是整个数组也可以是由起始和终止索引标识项的子集。需要注意的是，终止索引标识符的项不在切片内。切片提供了一个相关数组的动态窗口。

切片可以被索引，可以用 `len()` 函数来获取其长度。

切片的长度可以在运行时修改，最小为 0 ，最大为相关数组的长度，**切片是一个长度可变的数组**。

切片提供了计算容量的函数 `cap()` 可以测量切片长度最大可以到达多少：它等于切片长度 + 数组除切片之外的长度。如果 `s` 是一个切片，那么 `cap(s)` 就是从 `s[0]` 到数组末尾的长度。切片的长度永远不会超过它的容量，所以对于切片，一个不等式永远成立：`0 <= len(s) <= cap(s)`

多个切片如果表示同一个数组的片段，它们可以共享数据，因此一个切片和相关数组的其他切片是共享存储的，相反，不同数组总是代表不同存储。数组实际上是切片的构建块。

优点：

- 切片是引用，所以不需要使用额外内存并且比使用数组更有效率，所以在 Go 代码中切片比数组更常用。

- 声明切片的格式是：`var identifier []type` (不需要说明长度)

- 一个切片在初始化之前默认为 nil ,长度为 0

- 切片的初始化格式是：`var slice1 []type = arr1[start:end]`

- 如果某个人写：`var slice1 []type = arr1[:]` 那么 slice1 就等于完整的 arr1 数组（所以这种表示方式是 `arr1[0:len(arr1)]` 的一种缩写）。另外一种表述方式是：`slice1 = &arr1`。

- `arr1[2:]` 和 `arr1[2:len(arr1)]` 相同，都包含了数组从第三个到最后的所有元素

- `arr1[:3]` 和 `arr1[0:3]` 相同，包含了从第一个到第三个元素（不包括第四个）。

- 如果你想去掉 slice1 的最后一个元素，只要 `slice1 = slice1[:len(slice1)-1]`。

- `s2 := s[:]` 是用切片组成的切片，拥有相同的元素，但是仍然指向相同的相关数组。

- 一个切片 s 可以这样扩展到它的大小上限：`s = s[:cap(s)]`，如果再扩大的话就会导致运行时错误

- 对于每一个切片，下面的状态总是成立的：

  ```go
  s == s[:i] + s[i:] // i是一个整数且: 0 <= i <= len(s)
  len(s) <= cap(s)
  ```

- 切片也可以用类似数组的方式初始化：`var x = []int{2, 3, 5, 7, 11}`。这样就创建了一个长度为 5 的数组并且创建了一个相关切片。

```go
package main
import "fmt"
func main() {
	var arr1 [6]int
	var slice1 []int = arr1[2:5]
	for i := 0; i < len(arr1); i++ {
		arr1[i] = i
	} //标号填入 0 1 2 3 4 5
	for i := 0; i < len(slice1); i++ {
		fmt.Printf("%d is %d\n", i, slice1[i])
	} //输出 2 3 4
	fmt.Printf("The length of arr1 is %d\n", len(arr1))       //数组大小
	fmt.Printf("The length of slice1 is %d\n", len(slice1))   //切片大小
	fmt.Printf("The capacity of slice1 is %d\n", cap(slice1)) //切片的最大扩展大小

	slice1 = slice1[0:4]
	for i := 0; i < len(slice1); i++ {
		fmt.Printf("Slice at %d is %d\n", i, slice1[i])
	}
	fmt.Printf("The length of slice1 is %d\n", len(slice1))
	fmt.Printf("The capacity of slice1 is %d\n", cap(slice1))
}
```

- 不要用指针指向 sclice 。切片本身就是一个引用类型，所以他本身就是一个指针

#### 5.2.2 将切片传递给函数

可以看下面的代码：

```go
package main
func sum(a []int) int {
	s := 0
	for i := 0; i < len(a); i++ {
		s += a[i]
	}
	return s
}
func main() {
	var arr = [5]int{0, 1, 2, 3, 4}
	println(sum(arr[:]))
}
```

把数组切片传成一个引用给函数。

#### 5.2.3 用 `make()` 创建一个切片

相关数组还没有定义时，可以用 make() 函数创建一个切片，同时创建好数组：

```go
var slice1 []type = make([]type,len)
```

也可以简写为 `slice1 := make([]type, len)`，这里 `len` 是数组的长度并且也是 `slice` 的初始长度。

所以定义 `s2 := make([]int, 10)`，那么 `cap(s2) == len(s2) == 10`。

make 接受 2 个参数：元素的类型以及切片的元素个数。

如果你想创建一个 slice1，它不占用整个数组，而只是占用以 len 为个数个项，那么只要：`slice1 := make([]type, len, cap)`。

make 的使用方式是：`func make([]T, len, cap)`，其中 cap 是可选参数。

所以下面两种方法可以生成相同的切片:

```go
make([]int, 50, 100)
new([100]int)[0:50]
```

例子：

```go
package main

import "fmt"

func main() {
	var slice1 []int = make([]int, 10)
	for i := 0; i < len(slice1); i++ {
		slice1[i] = 5 * i
	}
	for i := 0; i < len(slice1); i++ {
		fmt.Printf("%d is %d\n", i, slice1[i])
	}
	fmt.Printf("\nThe length of slice1 is %d\n", len(slice1))
	fmt.Printf("The capacity of slice1 is %d\n", cap(slice1))
}

```

#### 5.2.4 new() 和 make() 的区别

看起来二者没区别，都是在堆上分配内存，但是他们的行为不同，适用于的类型不同：

- new(T) 为每个新的类型T分配一片内存，初始化为 0 并且返回类型为*T的内存地址：这种方法 **返回一个指向类型为 T，值为 0 的地址的指针**，它适用于值类型如数组和结构体（参见第 10 章）；它相当于 `&T{}`。
- make(T) **返回一个类型为 T 的初始值**，它只适用于3种内建的引用类型：切片、map 和 channel。
- new 函数分配内存，make 函数初始化

下面的方法：

```go
var v []int = make([]int, 10, 50)
```

或者

```go
v := make([]int, 10, 50)
```

这样分配一个有 50 个 int 值的数组，并且创建了一个长度为 10，容量为 50 的 切片 v，该 切片 指向数组的前 10 个元素。

#### 5.2.5 多维切片

和数组一样，切片通常也是一维的，但是也可以由一维组合成高维。通过分片的分片（或者切片的数组），长度可以任意动态变化，所以 Go 语言的多维切片可以任意切分。而且，内层的切片必须单独分配（通过 make 函数）

#### 5.2.6 byte 包

类型 `[]byte` 的切片十分常见，Go 语言有一个 bytes 包专门用来解决这种类型的操作方法。

bytes 包和字符串包十分类似。而且它还包含一个十分有用的类型 Buffer:

```go
import "bytes"

type Buffer struct {
	...
}
```

这是一个长度可变的 bytes 的 buffer，提供 Read 和 Write 方法，因为读写长度未知的 bytes 最好使用 buffer。

Buffer 可以这样定义：`var buffer bytes.Buffer`。

或者使用 new 获得一个指针：`var r *bytes.Buffer = new(bytes.Buffer)`。

或者通过函数：`func NewBuffer(buf []byte) *Buffer`，创建一个 Buffer 对象并且用 buf 初始化好；NewBuffer 最好用在从 buf 读取的时候使用。

**通过 buffer 串联字符串**

类似于 Java 的 StringBuilder 类。

在下面的代码段中，我们创建一个 buffer，通过 `buffer.WriteString(s)` 方法将字符串 s 追加到后面，最后再通过 `buffer.String()` 方法转换为 string：

```go
var buffer bytes.Buffer
for {
	if s, ok := getNextString(); ok { //method getNextString() not shown here
		buffer.WriteString(s)
	} else {
		break
	}
}
fmt.Print(buffer.String(), "\n")
```

这种实现方式比使用 `+=` 要更节省内存和 CPU，尤其是要串联的字符串数目特别多的时候。

类似于数组一样，切片也可以使用 `For-range` 结构。

### 5.3 切片重组

我们已经知道切片创建的时候通常比相关数组小，例如：

```go
slice1 := make([]type, start_length, capacity)
```

其中 `start_length` 作为切片初始长度而 `capacity` 作为相关数组的长度。

这么做的好处是我们的切片在达到容量上限后可以扩容。改变切片长度的过程称之为切片重组 **reslicing**，做法如下：`slice1 = slice1[0:end]`，其中 end 是新的末尾索引（即长度）。

将切片扩展 1 位可以这么做：

```go
sl = sl[0:len(sl)+1]
```

切片可以反复扩展直到占据整个相关数组。

### 5.4 切片的复制与追加

```go
package main

import "fmt"

func main() {
	sl_from := []int{1, 2, 3}
	sl_to := make([]int, 10)
	n := copy(sl_to, sl_from)
	fmt.Println(sl_to)
	fmt.Printf("Copied %d elements\n", n) // n == 3

	sl3 := []int{1, 2, 3}
	sl3 = append(sl3, 4, 5, 6)
	fmt.Println(sl3)
}

```

如上述代码，使用 copy 与 append 可以完成切片的复制与追加

如果想自己掌控 append 的过程，可以手动实现一下:

```go
func AppendByte(slice []byte, data ...byte) []byte {
	m := len(slice)
	n := m + len(data)
	if n > cap(slice) {
		newSlice := make([]byte, (n+1)*2)
		copy(newSlice, slice)
		slice = newSlice
	}
	slice = slice[0:n]
	copy(slice[m:n], data)
	return slice
}
```

### 5.5 字符串、数组和切片的应用

1. 从字符串生成字节切片

   ```go
   package main
   import "fmt"
   func main() {
       s := "\u00ff\u754c"
       for i, c := range s {
           fmt.Printf("%d:%c ", i, c)
       }
   }
   ```

2. 获取字符串的某一部分：使用 `substr := str[start:end]` 可以从字符串 str 获取到从索引 start 开始到 `end-1` 位置的子字符串。同样的，`str[start:]` 则表示获取从 start 开始到 `len(str)-1` 位置的子字符串。而 `str[:end]` 表示获取从 0 开始到 `end-1` 的子字符串。

3. 修改字符串中的某一个字符:

   Go 语言中的字符串是不可变的，也就是说 `str[index]` 这样的表达式是不可以被放在等号左侧的。如果尝试运行 `str[i] = 'D'` 会得到错误：`cannot assign to str[i]`。

   因此，您必须先将字符串转换成字节数组，然后再通过修改数组中的元素值来达到修改字符串的目的，最后将字节数组转换回字符串格式。

   例如，将字符串 "hello" 转换为 "cello"：

   ```go
   s := "hello"
   c := []byte(s)
   c[0] = 'c'
   s2 := string(c) // s2 == "cello"
   ```

   所以，您可以通过操作切片来完成对字符串的操作

4. 字节数组对比函数

   下面的函数可以比较两个数组的字典序大小：

   ```go
   func Compare(a, b []byte) int {
   	for i := 0; i < len(a) && i < len(b); i++ {
   		switch {
   		case a[i] > b[i]:
   			return 1
   		case a[i] < b[i]:
   			return -1
   		}
   	}
   	// 数组的长度可能不同
   	switch {
   	case len(a) < len(b):
   		return -1
   	case len(a) > len(b):
   		return 1
   	}
   	return 0 // 数组相等
   }
   ```

5. 搜索及排序切片和数组

   标准库提供了 `sort` 包来实现常见的搜索和排序操作。您可以使用 `sort` 包中的函数 `func Ints(a []int)` 来实现对 int 类型的切片排序。例如 `sort.Ints(arri)`，其中变量 arri 就是需要被升序排序的数组或切片。为了检查某个数组是否已经被排序，可以通过函数 `IntsAreSorted(a []int) bool` 来检查，如果返回 true 则表示已经被排序。

   类似的，可以使用函数 `func Float64s(a []float64)` 来排序 float64 的元素，或使用函数 `func Strings(a []string)`排序字符串元素。

   想要在数组或切片中搜索一个元素，该数组或切片必须先被排序（因为标准库的搜索算法使用的是二分法）。然后，您就可以使用函数 `func SearchInts(a []int, n int) int` 进行搜索，并返回对应结果的索引值。

6. append 函数常见操作

   - 将切片 b 的元素追加到切片 a 之后：`a = append(a,b...)`

   - 复制切片 a 的元素到新的切片 b 上：

     ```go
     b = make([]T,len(a))
     copy(b,a)
     ```

   - 删除位于索引 i 的元素：`a = append(a[:i],a[i+1:]...)`

     ```go
     package main
     import "fmt"
     func main() {
     	a := []int{1, 2, 3, 4}
     	a = append(a[:2], a[3:]...) //append第二个参数需要接受一个可变长参数，使用a[3:]...把切片打散
     	fmt.Println(a)
     }
     ```

   - 切除切片 a 中从索引 i 至 j 位置的元素：`a = append(a[:i], a[j:]...)`

   - 为切片 a 扩展 j 个元素长度：`a = append(a, make([]T, j)...)`

   - 在索引 i 的位置插入元素 x：`a = append(a[:i], append([]T{x}, a[i:]...)...)`

   - 在索引 i 的位置插入长度为 j 的新切片：`a = append(a[:i], append(make([]T, j), a[i:]...)...)`

   - 在索引 i 的位置插入切片 b 的所有元素：`a = append(a[:i], append(b, a[i:]...)...)`

   - 取出位于切片 a 最末尾的元素 x：`x, a = a[len(a)-1], a[:len(a)-1]`

   - 将元素 x 追加到切片 a：`a = append(a, x)`

## 6 Map

map 是一种特殊的数据结构：一种元素对（pair）的无序集合，pair 的一个元素是 key，对应的另一个元素是 value，所以这个结构也称为关联数组或字典。这是一种快速寻找值的理想结构：给定 key，对应的 value 可以迅速定位。

map 这种数据结构在其他编程语言中也称为字典（Python）、hash 和 HashTable 等。

### 6.1 概念

未初始化的 map 的值是 nil

map 是引用类型，可以使用如下声明：

```go
var map1 map[keytype]valuetype
var map1 map[string]int
```

（`[keytype]` 和 `valuetype` 之间允许有空格，但是 gofmt 移除了空格）

在声明的时候不需要知道 map 的长度，map 是可以动态增长的。

未初始化的 map 的值是 nil。

key 可以是任意可以用 == 或者 != 操作符比较的类型，比如 string、int、float。所以数组、切片和结构体不能作为 key (译者注：含有数组切片的结构体不能作为 key，只包含内建类型的 struct 是可以作为 key 的），但是指针和接口类型可以。如果要用结构体作为 key 可以提供 `Key()` 和 `Hash()` 方法，这样可以通过结构体的域计算出唯一的数字或者字符串的 key。

- Value 可以是任意类型的
- 使用 map 比线性查找快，但是比数组和切片索引慢得多

令 `v := map1[key1]` 可以将 key1 对应的值赋值给 v；如果 map 中没有 key1 存在，那么 v 将被赋值为 map1 的值类型的空值。

常用的 `len(map1)` 方法可以获得 map 中的 pair 数目，这个数目是可以伸缩的，因为 map-pairs 在运行时可以动态添加和删除。

```go
package main

import "fmt"

func main() {
	var mapLit map[string]int
	var mapAssigned map[string]int

	mapLit = map[string]int{"one": 1, "two": 2}
	mapCreated := make(map[string]float32)
	mapAssigned = mapLit

	mapCreated["key1"] = 4.5
	mapCreated["key2"] = 3.14159
	mapAssigned["two"] = 3

	fmt.Printf("Map literal at \"one\" is: %d\n", mapLit["one"])
	fmt.Printf("Map created at \"key2\" is: %f\n", mapCreated["key2"])
	fmt.Printf("Map assigned at \"two\" is: %d\n", mapLit["two"])
	fmt.Printf("Map literal at \"ten\" is: %d\n", mapLit["ten"])
}

```

上面是一个 map 的使用例子。

**不要使用 new，永远用 make 来构造 map**

map 的 value 甚至可以是函数，比如：

```go
package main
import "fmt"
func main() {
	mf := map[int]func() int{
		1: func() int { return 10 },
		2: func() int { return 20 },
		5: func() int { return 50 },
	}
	fmt.Println(mf)
	fmt.Printf("%d", mf[2]())
}
```

### 6.2 map 容量

和数组不同，map 可以根据新增的 key-value 对动态的伸缩，因此它不存在固定长度或者最大限制。但是你也可以选择标明 map 的初始容量 `capacity`，就像这样：`make(map[keytype]valuetype, cap)`。例如：

```go
map2 := make(map[string]float32, 100)
```

当 map 增长到容量上限的时候，如果再增加新的 key-value 对，map 的大小会自动加 1。所以出于性能的考虑，对于大的 map 或者会快速扩张的 map，即使只是大概知道容量，也最好先标明。

这里有一个 map 的具体例子，即将音阶和对应的音频映射起来：

```go
noteFrequency := map[string]float32 {
	"C0": 16.35, "D0": 18.35, "E0": 20.60, "F0": 21.83,
	"G0": 24.50, "A0": 27.50, "B0": 30.87, "A4": 440}
```

### 6.3 用切片作为 map 的值

既然一个 key 只能对应一个 value，而 value 又是一个原始类型，那么如果一个 key 要对应多个值怎么办？例如，当我们要处理unix机器上的所有进程，以父进程（pid 为整形）作为 key，所有的子进程（以所有子进程的 pid 组成的切片）作为 value。通过将 value 定义为 `[]int` 类型或者其他类型的切片，就可以优雅的解决这个问题。

这里有一些定义这种 map 的例子：

```go
mp1 := make(map[int][]int)
mp2 := make(map[int]*[]int)
```

### 6.4 测试键值对是否存在以及删除元素

为了解决这个问题，我们可以这么用：`val1, isPresent = map1[key1]`

isPresent 返回一个 bool 值：如果 key1 存在于 map1，val1 就是 key1 对应的 value 值，并且 isPresent为true；如果 key1 不存在，val1 就是一个空值，并且 isPresent 会返回 false。

如果你只是想判断某个 key 是否存在而不关心它对应的值到底是多少，你可以这么做：

```go
_, ok := map1[key1] // 如果key1存在则ok == true，否则ok为false
```

或者和 if 混合使用：

```go
if _, ok := map1[key1]; ok {
	// ...
}
```

从 map1 中删除 key1：

直接 `delete(map1, key1)` 就可以。

如果 key1 不存在，该操作不会产生错误。

```go
package main
import "fmt"

func main() {
	var value int
	var isPresent bool
	map1 := make(map[string]int)
	map1["New Delhi"] = 55
	map1["Beijing"] = 20
	map1["Washington"] = 25
	value, isPresent = map1["Beijing"]
	if isPresent {
		fmt.Printf("The value of \"Beijing\" in map1 is: %d\n", value)
	} else {
		fmt.Printf("map1 does not contain Beijing")
	}
	value, isPresent = map1["Paris"]
	fmt.Printf("Is \"Paris\" in map1 ?: %t\n", isPresent)
	fmt.Printf("Value is: %d\n", value)
	// delete an item:
	delete(map1, "Washington")
	value, isPresent = map1["Washington"]
	if isPresent {
		fmt.Printf("The value of \"Washington\" in map1 is: %d\n", value)
	} else {
		fmt.Println("map1 does not contain Washington")
	}
}
```

### 6.5 for-range 的配套用法

可以使用 for 循环构造 map：

```go
for key, value := range map1 {
	...
}
```

第一个返回值 key 是 map 中的 key 值，第二个返回值则是该 key 对应的 value 值；这两个都是仅 for 循环内部可见的局部变量。其中第一个返回值key值是一个可选元素。如果你只关心值，可以这么使用：

```go
for _, value := range map1 {
	...
}
```

如果只想获取 key，你可以这么使用：

```go
for key := range map1 {
	fmt.Printf("key is: %d\n", key)
}
```

### 6.7 map 类型的切片

假设我们想获取一个 map 类型的切片，我们必须使用两次 `make()` 函数，第一次分配切片，第二次分配 切片中每个 map 元素

```go
package main

import "fmt"

func main() {
	// Version A:
	items := make([]map[int]int, 5)
	for i := range items {
		items[i] = make(map[int]int, 1)
		items[i][1] = 2
	}
	fmt.Printf("Version A: Value of items: %v\n", items)

	// Version B: NOT GOOD!
	items2 := make([]map[int]int, 5)
	for _, item := range items2 {
		item = make(map[int]int, 1) // item is only a copy of the slice element.
		item[1] = 2                 // This 'item' will be lost on the next iteration.
	}
	fmt.Printf("Version B: Value of items: %v\n", items2)
}

// 运行结果：
// Version A: Value of items: [map[1:2] map[1:2] map[1:2] map[1:2] map[1:2]]
// Version B: Value of items: [map[] map[] map[] map[] map[]]
```

应当像 A 版本那样通过索引使用切片的 map 元素。在 B 版本中获得的项只是 map 值的一个拷贝而已，所以真正的 map 元素没有得到初始化。

### 6.8 map 的排序

map 默认是无序的，不管是按照 key 还是按照 value 默认都不排序（详见第 8.3 节）。

如果你想为 map 排序，需要将 key（或者 value）拷贝到一个切片，再对切片排序（使用 sort 包，详见第 7.6.6 节），然后可以使用切片的 for-range 方法打印出所有的 key 和 value。

比如以下的例子：

```go
package main

import (
	"fmt"
	"sort"
)

var (
	barVal = map[string]int{"alpha": 34, "bravo": 56, "charlie": 23,
		"delta": 87, "echo": 56, "foxtrot": 12,
		"golf": 34, "hotel": 16, "indio": 87,
		"juliet": 65, "kili": 43, "lima": 98}
)
func main() {
	fmt.Println("unsorted:")
	for k, v := range barVal {
		fmt.Printf("Key: %v, Value: %v / ", k, v)
	}
	keys := make([]string, len(barVal))
	i := 0
	for k, _ := range barVal {
		keys[i] = k
		i++
	}
	sort.Strings(keys)
	fmt.Println()
	fmt.Println("sorted:")
	for _, k := range keys {
		fmt.Printf("Key: %v, Value: %v / ", k, barVal[k])
	}
}
```

## 7 包

### 7.1 基本介绍

像 `fmt`、`os` 等这样具有常用功能的内置包在 Go 语言中有 150 个以上，它们被称为标准库，大部分(一些底层的除外)内置于 Go 本身。完整列表可以在 [Go Walker](https://gowalker.org/search?q=gorepos) 查看。

- `unsafe`: 包含了一些打破 Go 语言“类型安全”的命令，一般的程序中不会被使用，可用在 C/C++ 程序的调用中。

- `syscall`-`os`-`os/exec`:

  - `os`: 提供给我们一个平台无关性的操作系统功能接口，采用类Unix设计，隐藏了不同操作系统间差异，让不同的文件系统和操作系统对象表现一致。
  - `os/exec`: 提供我们运行外部操作系统命令和程序的方式。
  - `syscall`: 底层的外部包，提供了操作系统底层调用的基本接口。

- `archive/tar` 和 `/zip-compress`：压缩(解压缩)文件功能。

- `fmt`-`io`-`bufio`-`path/filepath`-`flag`:

  - `fmt`: 提供了格式化输入输出功能。
  - `io`: 提供了基本输入输出功能，大多数是围绕系统功能的封装。
  - `bufio`: 缓冲输入输出功能的封装。
  - `path/filepath`: 用来操作在当前系统中的目标文件名路径。
  - `flag`: 对命令行参数的操作。

- `strings`-`strconv`-`unicode`-`regexp`-`bytes`:

  - `strings`: 提供对字符串的操作。
  - `strconv`: 提供将字符串转换为基础类型的功能。
  - `unicode`: 为 unicode 型的字符串提供特殊的功能。
  - `regexp`: 正则表达式功能。
  - `bytes`: 提供对字符型分片的操作。
  - `index/suffixarray`: 子字符串快速查询

- `math`-`math/cmath`-`math/big`-`math/rand`-`sort`:

  - `math`: 基本的数学函数。
  - `math/cmath`: 对复数的操作。
  - `math/rand`: 伪随机数生成。
  - `sort`: 为数组排序和自定义集合。
  - `math/big`: 大数的实现和计算

- `container`-`/list-ring-heap`: 实现对集合的操作

  - `list`: 双链表。
  - `ring`: 环形链表。

  下面代码演示了如何遍历一个链表(当 l 是 `*List`)：

  ```go
  for e := l.Front(); e != nil; e = e.Next() {
  	//do something with e.Value
  }
  ```

- `time`-`log`:

  - `time`: 日期和时间的基本操作。
  - `log`: 记录程序运行时产生的日志,我们将在后面的章节使用它

- `encoding/json`-`encoding/xml`-`text/template`:

  - `encoding/json`: 读取并解码和写入并编码 JSON 数据。
  - `encoding/xml`:简单的 XML1.0 解析器,有关 JSON 和 XML 的实例请查阅第 12.9/10 章节。
  - `text/template`:生成像 HTML 一样的数据与文本混合的数据驱动模板

- `net`-`net/http`-`html`:

  - `net`: 网络数据的基本操作。
  - `http`: 提供了一个可扩展的 HTTP 服务器和基础客户端，解析 HTTP 请求和回复。
  - `html`: HTML5 解析器。

- `runtime`: Go 程序运行时的交互操作，例如垃圾回收和协程创建。

- `reflect`: 实现通过程序运行时反射，让程序操作任意类型的变量

### 7.2 regexp 包

在下面的程序里，我们将在字符串中对正则表达式进行匹配。

如果是简单模式，使用 `Match` 方法便可：

```go
ok, _ := regexp.Match(pat, []byte(searchIn))
```

变量 ok 将返回 true 或者 false,我们也可以使用 `MatchString`：

```go
ok, _ := regexp.MatchString(pat, searchIn)
```

更多方法中，必须先将正则通过 `Compile` 方法返回一个 Regexp 对象。然后我们将掌握一些匹配，查找，替换相关的功能。

```go
package main

import (
	"fmt"
	"regexp"
	"strconv"
)

func main() {
	sea := "John: 2578.34 William: 4567.23 Steve: 5632.18"
	pat := "[0-9]+.[0-9]+"
	f := func(s string) string {
		v, _ := strconv.ParseFloat(s, 32)
		return strconv.FormatFloat(v*2, 'f', 2, 32)
	}
	if ok, _ := regexp.Match(pat, []byte(sea)); ok {
		fmt.Println("Match Found")
	}
	re, _ := regexp.Compile(pat)
	str := re.ReplaceAllString(sea, "##.#")
	fmt.Println(str)
	str2 := re.ReplaceAllStringFunc(sea, f)
	fmt.Println(str2)
}

// 运行结果：
// Match Found
// John: ##.# William: ##.# Steve: ##.#
// John: 5156.68 William: 9134.46 Steve: 11264.36

```

以上是使用的例子

### 7.3 锁和 sync 包

在一些复杂的程序中，通常通过不同线程执行不同应用来实现程序的并发。当不同线程要使用同一个变量时，经常会出现一个问题：无法预知变量被不同线程修改的顺序！(这通常被称为资源竞争,指不同线程对同一变量使用的竞争)显然这无法让人容忍，那我们该如何解决这个问题呢？

经典的做法是一次只能让一个线程对共享变量进行操作。当变量被一个线程改变时(临界区)，我们为它上锁，直到这个线程执行完成并解锁后，其他线程才能访问它。

特别是我们之前章节学习的 map 类型是不存在锁的机制来实现这种效果(出于对性能的考虑)，所以 map 类型是非线程安全的。当并行访问一个共享的 map 类型的数据，map 数据将会出错。

在 Go 语言中这种锁的机制是通过 sync 包中 Mutex 来实现的。sync 来源于 "synchronized" 一词，这意味着线程将有序的对同一变量进行访问。

`sync.Mutex` 是一个互斥锁，它的作用是守护在临界区入口来确保同一时间只能有一个线程进入临界区。

假设 info 是一个需要上锁的放在共享内存中的变量。通过包含 `Mutex` 来实现的一个典型例子如下：

```go
import  "sync"

type Info struct {
	mu sync.Mutex
	// ... other fields, e.g.: Str string
}
```

如果一个函数想要改变这个变量可以这样写:

```go
func Update(info *Info) {
	info.mu.Lock()
    // critical section:
    info.Str = // new value
    // end critical section
    info.mu.Unlock()
}
```

还有一个很有用的例子是通过 Mutex 来实现一个可以上锁的共享缓冲器:

```go
type SyncedBuffer struct {
	lock 	sync.Mutex
	buffer  bytes.Buffer
}
```

在 sync 包中还有一个 `RWMutex` 锁：他能通过 `RLock()` 来允许同一时间多个线程对变量进行读操作，但是只能一个线程进行写操作。如果使用 `Lock()` 将和普通的 `Mutex` 作用相同。包中还有一个方便的 `Once` 类型变量的方法 `once.Do(call)`，这个方法确保被调用函数只能被调用一次。

### 7.4 精密计算和 big 包

我们知道有些时候通过编程的方式去进行计算是不精确的。如果你使用 Go 语言中的 float64 类型进行浮点运算，返回结果将精确到 15 位，足以满足大多数的任务。当对超出 int64 或者 uint64 类型这样的大数进行计算时，如果对精度没有要求，float32 或者 float64 可以胜任，但如果对精度有严格要求的时候，我们不能使用浮点数，在内存中它们只能被近似的表示。

对于整数的高精度计算 Go 语言中提供了 big 包。其中包含了 math 包：有用来表示大整数的 `big.Int` 和表示大有理数的 `big.Rat` 类型（可以表示为 2/5 或 3.1416 这样的分数，而不是无理数或 π）。这些类型可以实现任意位类型的数字，只要内存足够大。缺点是更大的内存和处理开销使它们使用起来要比内置的数字类型慢很多。

大的整型数字是通过 `big.NewInt(n)` 来构造的，其中 n 为 int64 类型整数。而大有理数是通过 `big.NewRat(N,D)` 方法构造。N（分子）和 D（分母）都是 int64 型整数。因为 Go 语言不支持运算符重载，所以所有大数字类型都有像是 `Add()`和 `Mul()` 这样的方法。它们作用于作为 receiver 的整数和有理数，大多数情况下它们修改 receiver 并以 receiver 作为返回结果。因为没有必要创建 `big.Int` 类型的临时变量来存放中间结果，所以这样的运算可通过内存链式存储。

```go
package main

import (
	"fmt"
	"math"
	"math/big"
)

func main() {
	// Here are some calculations with bigInts:
	im := big.NewInt(math.MaxInt64)
	in := im
	io := big.NewInt(1956)
	ip := big.NewInt(1)
	ip.Mul(im, in).Add(ip, im).Div(ip, io) //a.ADD(b,c)的意思是，把b+c的值赋值给a
	fmt.Printf("Big Int: %v\n", ip)
	// Here are some calculations with bigInts:
	rm := big.NewRat(math.MaxInt64, 1956)
	rn := big.NewRat(-1956, math.MaxInt64)
	ro := big.NewRat(19, 56)
	rp := big.NewRat(1111, 2222)
	rq := big.NewRat(1, 1)
	rq.Mul(rm, rn).Add(rq, ro).Mul(rq, rp)
	fmt.Printf("Big Rat: %v\n", rq)
}

```

### 7.5 自定义包及可见性

包是 Go 语言中代码组织和代码编译的主要方式。当写自己包的时候，要使用短小的不含有 `_`(下划线)的小写单词来为文件命名。

import 的一般格式如下:

```go
import "包的路径或 URL 地址" 
```

例如：

```go
import "github.com/org1/pack1”
```

路径是指当前目录的相对路径。

大概总结一下 GoLang 自定义包，并不是需要发布的包，只是一次在代码工作路径下调用自定义包的一个尝试。

> 以为 import 的时候要填写文件夹里面的 package 的名称，比如之前我是这么写的：
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

``` go
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

## 8 结构（struct）与方法（method）


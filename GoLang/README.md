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


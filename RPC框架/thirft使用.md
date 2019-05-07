## Thrift IDL 使用

### 介绍

首先看一下维基百科额定义：

> **Thrift**是一种[接口描述语言](https://zh.wikipedia.org/wiki/接口描述语言)和二进制通讯协议，[[1\]](https://zh.wikipedia.org/wiki/Thrift#cite_note-1)它被用来定义和创建跨语言的服务。[[2\]](https://zh.wikipedia.org/wiki/Thrift#cite_note-2)它被当作一个[远程过程调用](https://zh.wikipedia.org/wiki/远程过程调用)（RPC）框架来使用，是由[Facebook](https://zh.wikipedia.org/wiki/Facebook)为“大规模跨语言服务开发”而开发的。它通过一个代码生成引擎联合了一个软件栈，来创建不同程度的、无缝的[跨平台](https://zh.wikipedia.org/wiki/跨平台)高效服务，可以使用[C#](https://zh.wikipedia.org/wiki/C♯)、[C++](https://zh.wikipedia.org/wiki/C%2B%2B)（基于[POSIX](https://zh.wikipedia.org/wiki/POSIX)兼容系统[[3\]](https://zh.wikipedia.org/wiki/Thrift#cite_note-3)）、Cappuccino、[[4\]](https://zh.wikipedia.org/wiki/Thrift#cite_note-4)[Cocoa](https://zh.wikipedia.org/wiki/Cocoa)、[Delphi](https://zh.wikipedia.org/wiki/Delphi)、[Erlang](https://zh.wikipedia.org/wiki/Erlang)、[Go](https://zh.wikipedia.org/wiki/Go)、[Haskell](https://zh.wikipedia.org/wiki/Haskell)、[Java](https://zh.wikipedia.org/wiki/Java)、[Node.js](https://zh.wikipedia.org/wiki/Node.js)、[OCaml](https://zh.wikipedia.org/wiki/OCaml)、[Perl](https://zh.wikipedia.org/wiki/Perl)、[PHP](https://zh.wikipedia.org/wiki/PHP)、[Python](https://zh.wikipedia.org/wiki/Python)、[Ruby](https://zh.wikipedia.org/wiki/Ruby)和[Smalltalk](https://zh.wikipedia.org/wiki/Smalltalk)。[[5\]](https://zh.wikipedia.org/wiki/Thrift#cite_note-5)虽然它以前是由Facebook开发的，但它现在是[Apache软件基金会](https://zh.wikipedia.org/wiki/Apache软件基金会)的[开源](https://zh.wikipedia.org/wiki/开源)项目了。该实现被描述在2007年4月的一篇由Facebook发表的技术论文中，该论文现由Apache掌管。[[6\]](https://zh.wikipedia.org/wiki/Thrift#cite_note-6)

简单的来说就是一个跨语言的 RPC 框架，Thrift是一个典型的CS（客户端/服务端）结构，客户端和服务端可以使用不同的语言开发。既然客户端和服务端能使用不同的语言开发，那么一定就要有一种中间语言来关联客户端和服务端的语言，没错，这种语言就是IDL（Interface Description Language）。

所以这篇博客就主要介绍一下 Thrift IDL 语言。

### 类型

#### 基本类型

| 类型名称 |  类型说明   |               备注                |
| :------: | :---------: | :-------------------------------: |
| `string` | 字符串类型  |                                   |
|  `i16`   |  16位整型   |                                   |
|  `i32`   |  32位整型   |                                   |
|  `i64`   |  64位整型   |                                   |
|  `byte`  | 8位字符类型 |                                   |
|  `bool`  |   布尔值    |                                   |
| `double` | 双精度浮点  |                                   |
|  `void`  |   空类型    |                                   |
|  `map`   |   map类型   |   类似cpp,如：`map<i32,i32>mp`    |
|  `set`   |  集合类型   |     类似cpp ,如： `set<i32>s`     |
|  `list`  |  列表类型   | 定义一个 list 对象：`list<i32>li` |

#### 结构体(struct)：

定义形式如下：

```idl
struct People {
     1: string name;
     2: i32 age;
     3: string sex;
}
```

约束:

> 1.struct不能继承，但是可以嵌套，不能嵌套自己。
> 2.其成员都是有明确类型
> 3.成员是被正整数编号过的，其中的编号使不能重复的，这个是为了在传输过程中编码使用。
> 4.成员分割符可以是逗号（,）或是分号（;），而且可以混用
> 5.字段会有optional和required之分和protobuf一样，但是如果不指定则为无类型–可以不填充该值，但是在序列化传输的时候也会序列化进去，optional是不填充则部序列化，required是必须填充也必须序列化。
> 6.每个字段可以设置默认值
> 7.同一文件可以定义多个struct，也可以定义在不同的文件，进行include引入。

#### 枚举类型(enum):

定义形式如下：

```idl
enum Sex {
    MALE,
    FEMALE
}
```

#### 异常(exception)

thrift支持自定义exception，规则和struct一样，如下：

```idl
exception RequestException {
    1: i32 code;
    2: string reason;
}
```

#### 服务(service)

thrift定义服务相当于Java中创建Interface一样，创建的service经过代码生成命令之后就会生成客户端和服务端的框架代码。定义形式如下：

```idl
service HelloWordService {
     // service中定义的函数，相当于Java interface中定义的函数
     string doAction(1: string name, 2: i32 age);
 }
```

### 类型定义

Thrift的IDL支持C/C++中类似typedef的功能，例如：

```idl
typedef i32 Integer
typedef i64 Long
```

### 命名空间

命名空间就是 package 的意思，主要目的是组织代码，例如：

```idl
namespace go content.xxx
namespace py content.xxx
```

### 文件包含

thrift也支持文件包含，相当于C/C++中的include，Java中的import。使用关键字include定义，例 如：

```idl
include "global.thrift"
```

### 可选与必选

thrift提供两个关键字required，optional，分别用于表示对应的字段时必填的还是可选的。例如：

```idl
struct People {
    1: required string name;
    2: optional i32 age;
}
```

表示name是必填的，age是可选的。



### 参考资料

1. Thrift的官方网站：[http://thrift.apache.org/](http://thrift.apache.org/)

2. Thrift官方下载地址：[http://thrift.apache.org/download](http://thrift.apache.org/download)

3. Thrift官方的IDL示例文件（自己写IDL文件时可以此为参考）：

   https://git-wip-us.apache.org/repos/asf?p=thrift.git;a=blob_plain;f=test/ThriftTest.thrift;hb=HEAD

4. 各种环境下搭建Thrift的方法：[http://thrift.apache.org/docs/install/](http://thrift.apache.org/docs/install/)